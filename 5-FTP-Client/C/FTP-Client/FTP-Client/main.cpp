#include "Winsock.h"
#include "windows.h" 
#include <stdio.h> 
#include "time.h" 
#include <WinInet.h>
#include <iostream>
using namespace std;
#pragma comment(lib, "wsock32.lib")

SOCKET connectSock(char * IP, int port)
{
	WSADATA WSAData;
	SOCKET sock;
	sockaddr_in ipAddr;                 //服务器地址 

	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)//加载winsock版本
	{
		exit(1);
	}
	//设置地址结构
	ipAddr.sin_family = AF_INET;//AF_INET表示使用IP地址族 
	ipAddr.sin_addr.s_addr = inet_addr(IP);//指定服务器IP 
	ipAddr.sin_port = htons(port);//设置端口号 
	
	//创建套接字
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sock == SOCKET_ERROR)
	{
		printf("sockclient create fail! \n");
		WSACleanup();
		exit(1);
	}
	
	//连接服务器
	if (connect(sock, (struct sockaddr*)&ipAddr, sizeof(ipAddr)) == SOCKET_ERROR)
	{								//connect函数创建与指定外部端口的连接
		printf("Connect fail \n");
		exit(1);
	}
	return sock;
}

bool Login(SOCKET sock, char* user, char* pass)
{
	char rbuff[BUFSIZ] = { 0 };    //接收缓冲区  
	char sbuff[BUFSIZ];            //发送缓冲区 
	
	sprintf(sbuff, "USER %s", user);   
	strcat(sbuff, "\r\n");
	send(sock, sbuff, strlen(sbuff), 0);
	memset(rbuff, 0, BUFSIZ);
	recv(sock, rbuff, BUFSIZ, 0);
	printf(rbuff);
	
	memset(rbuff, 0, BUFSIZ);
	memset(sbuff, 0, BUFSIZ);
	sprintf(sbuff, "PASS %s", pass);
	strcat(sbuff, "\r\n");
	send(sock, sbuff, strlen(sbuff), 0);
	recv(sock, rbuff, BUFSIZ, 0);
	printf(rbuff);

	return 1;
}

SOCKET PasvConnect(SOCKET sock)
{
	char rbuff[BUFSIZ] = { 0 };
	char sbuff[BUFSIZ];
	char pasvIP[20];
	int addr[10];
	int port, a, b;
	SOCKET pasv;

	sprintf(sbuff, "PASV\r\n");
	send(sock, sbuff, sizeof(sbuff), 0);
	memset(rbuff, 0, BUFSIZ);
	recv(sock, rbuff, BUFSIZ, 0);
	sscanf(rbuff, "%*[^(](%d,%d,%d,%d,%d,%d)",
				& addr[0], &addr[1], &addr[2], &addr[3],
				& addr[4], &addr[5]);
	sprintf(pasvIP, "%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
	pasv = connectSock(pasvIP, addr[4] * 256 + addr[5]);

	return pasv;

}

DWORD readList(SOCKET client)
{
	char sbuff[BUFSIZ];
	char rbuff[BUFSIZ] = { 0 };
	sprintf(sbuff, "LIST\r\n");		
	SOCKET ps = PasvConnect(client);
	send(client, sbuff, strlen(sbuff), 0);//发送LIST命令给服务器
	recv(client, rbuff, BUFSIZ, 0);
	recv(ps, rbuff, BUFSIZ, 0);			//从数据端口接收列表信息
	printf("%s", rbuff);
	closesocket(ps);
	return 0;
}

/**
* 作用: 设置FTP传输类型 A:ascii I:Binary
* 参数: SOCkET，类型
* 返回值: 0 表示成功   -1 表示失败
* */
int setType(SOCKET sock, char mode)
{
	char buf[BUFSIZ];
	sprintf(buf, "TYPE %c\r\n", mode);
	send(sock, buf, sizeof(buf), 0);
	recv(sock, buf, strlen(buf), 0);
	return 1;
}

/**
* 作用: 从服务器复制文件到本地 RETR
* 参数: SOCKET，源地址，目的地址，文件大小
* 返回值: 0 表示列表成功  result>0 表示其他错误响应码
*         -1:文件创建失败  -2 pasv接口错误
* */
int Download(SOCKET sock, char *s, char *d, int * size)
{
	SOCKET d_sock;
	int len, write_len;
	char buf[BUFSIZ];
	int result;
	*size = 0;
	
	FILE * fp = fopen(d, "wb");//打开本地文件
	if (NULL == fp)
	{
		printf("Can't Open the file.\n");
		return -1;
	}
	
	setType(sock, 'I');			//设置传输模式
	
	d_sock = PasvConnect(sock);	//连接到PASV接口 用于传输文件
	if (-1 == d_sock)
	{
		fclose(fp);				//关闭文件
		return -2;
	}
					
	memset(buf, sizeof(buf), 0);
	sprintf(buf, "RETR %s\r\n", s);	//发送RETR命令
	result = send(sock, buf, sizeof(buf), 0);
	// 150 Opening data channel for file download from server of "xxxx"
	if (result == 0) //失败可能是没有权限什么的，具体看响应码
	{
		fclose(fp);
		return result;
	}

	//开始向PASV读取数据(下载)
	memset(buf, sizeof(buf), 0);
	while ((len = recv(d_sock, buf, BUFSIZ, 0)) > 0)
	{
		write_len = fwrite(&buf, len, 1, fp);
		if (write_len != 1) //写入文件不完整
		{
			closesocket(d_sock); //关闭套接字
			fclose(fp);			//关闭文件
			return -1;
		}
		if (NULL != size)
		{
			*size += write_len;
		}
	}
	//下载完成
	closesocket(d_sock);
	fclose(fp);

	//向服务器接收返回值
	memset(buf, sizeof(buf), 0);
	len = recv(sock, buf, BUFSIZ, 0);
	buf[len] = 0;
	printf("%s\n", buf);
	sscanf(buf, "%d", &result);
	if (result >= 300)
	{
		return result;
	}
	//226 Successfully transferred "xxxx"
	return 0;
}

/**
* 作用: 从本地复制文件到服务器 STOR
* 参数: SOCKET，源地址，目的地址，文件大小
* 返回值: 0 表示列表成功  result>0 表示其他错误响应码
*         -1:文件创建失败  -2 pasv接口错误
* */
int Update(SOCKET c_sock, char *s, char *d, int * size)
{
	SOCKET d_sock;
	int len, send_len;
	char buf[BUFSIZ];
	FILE * fp;
	int send_re;
	int result;
	//打开本地文件
	fp = fopen(s, "rb");
	if (NULL == fp)
	{
		printf("Can't Not Open the file.\n");
		return -1;
	}
	//设置传输模式
	setType(c_sock, 'I');
	//连接到PASV接口
	d_sock = PasvConnect(c_sock);
	if (d_sock == -1)
	{
		fclose(fp);
		return -1;
	}

	//发送STOR命令
	memset(buf, sizeof(buf), 0);
	sprintf(buf, "STOR %s\r\n", d);
	send_re = send(c_sock, buf, sizeof(buf), 0);
	if (send_re == 0)
	{
		fclose(fp);
		return send_re;
	}

	//开始向PASV通道写数据
	memset(buf, sizeof(buf), 0);
	while ((len = fread(buf, 1, BUFSIZ, fp)) > 0)
	{
		send_len = send(d_sock, buf, len, 0);
		if (send_len != len)
		{
			closesocket(d_sock);
			fclose(fp);
			return -1;
		}
		if (NULL != size)
		{
			*size += send_len;
		}
	}
	//完成上传
	closesocket(d_sock);
	fclose(fp);

	//向服务器接收响应码
	memset(buf, sizeof(buf), 0);
	len = recv(c_sock, buf, BUFSIZ, 0);
	buf[len] = 0;
	printf("%s\n", buf);
	sscanf(buf, "%d", &result);
	if (result >= 300)
	{
		return result;
	}
	return 0;
}

int main()
{
	SOCKET sockclient;
	int port;								//端口
	int size;
	
	char rbuff[BUFSIZ];                        //接收缓冲区  
	char sbuff[BUFSIZ];                       //发送缓冲区  
	char InputIP[20];                       //存储输入的服务器IP
	char message1[10];				//定义输入要处理的文件名  
	char message2[20];				//定义输入要处理的文件名 

	printf("请输入FTP服务器IP：");
	scanf("%s", InputIP);
	//printf("请输入端口号：");
	//scanf("%d", &port);
	sockclient = connectSock(InputIP, 21);
	memset(rbuff, 0, BUFSIZ);
	recv(sockclient, rbuff, BUFSIZ, 0); //连接成功提示
	printf(rbuff);

	int timeout = 0;
	setsockopt(sockclient, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

	printf("请输入用户名：");
	scanf("%s", message1);
	printf("请输入密码：");
	scanf("%s", message2);
	gets_s(message2);
	Login(sockclient, message1, message2);

	printf("命令：\n	\
		LIST 查看服务器中的文件和文件夹列表\n \
		DOWN 源文件 目标文件 从服务器下载文件到本地\n \
		UP 源文件 目标文件 从本地上传文件到服务器\n \
		QUIT 关闭连接\n");

	while (1) {
		printf("\n请输入命令:\n");           
		memset(message2, 0, 20);	//清空数组
		memset(message1, 0, 10);
		memset(rbuff, 0, BUFSIZ);
		memset(sbuff, 0, BUFSIZ);

		scanf("%s", message1);	//输入字符串
		sprintf(sbuff, message1);
		
		if (strncmp(message1, "QUIT", 4) == 0)
		{
			goto end;
		}
		 
		strcat(sbuff, "\r\n\0");	//把调整buff格式

		//send(sockclient, buff, strlen(buff), 0);
		if (strncmp(message1, "LIST", 4) == 0)
		{
			readList(sockclient);
			continue;
		}
		if (strncmp(message1, "DOWN", 4) == 0)
		{
			scanf("%s", message1);
			scanf("%s", message2);
			Download(sockclient, message1, message2, &size);
		}
		if (strncmp(message1, "UP", 2) == 0)
		{
			scanf("%s", message1);
			scanf("%s", message2);
			Update(sockclient, message1, message2, &size);
		}
		//recv(sockclient, rbuff, BUFSIZ, 0);
		//printf(rbuff);
	}

end:
	closesocket(sockclient);            //关闭连接
	WSACleanup();                       //释放Winsock    
	return 0;
}