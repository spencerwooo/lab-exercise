#include <Winsock2.h>
#include <iostream>
#include <cstdio>
using namespace std;

#pragma comment( lib, "ws2_32.lib" )// 链接Winsock2.h的静态库文件

int main()
{
	//初始化winsocket
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);//第一个参数为低位字节；第二个参数为高位字节

	err = WSAStartup(wVersionRequested, &wsaData);//对winsock DLL（动态链接库文件）进行初始化，协商Winsock的版本支持，并分配必要的资源。
	if (err != 0)
	{
		return 0;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)//LOBYTE（）取得16进制数最低位；HIBYTE（）取得16进制数最高（最左边）那个字节的内容		
	{
		WSACleanup();
		return 0;
	}

	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);//创建socket。AF_INET表示在Internet中通信；SOCK_STREAM表示socket是流套接字，对应tcp；0指定网络协议为TCP/IP

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);	//htonl用来将主机字节顺序转换为网络字节顺序(to network long)
	//INADDR_ANY就是指定地址为0.0.0.0的地址,
	//表示不确定地址,或“任意地址”。”
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8888);//htons用来将主机字节顺序转换为网络字节顺序(to network short)

	bind(sockSrv, (SOCKADDR*)& addrSrv, sizeof(SOCKADDR));//将本地地址绑定到所创建的socket上，以使在网络上标识该socket

	listen(sockSrv, 5);//socket监听，准备接受连接请求。

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	while (1)
	{
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)& addrClient, &len);//为一个连接请求提供服务。addrClient包含了发出连接请求的客户机IP地址信息；返回的新socket描述服务器与该客户机的连接

		char sendBuf[5000];
		sprintf(sendBuf, "HTTP/1.1 200 OK\r\n"\
			"Server: Potato server v0.0.1\r\n"\
			"Date: \r\nContent-Type: text/html\r\n\r\n"\
			"<html><p><h3>Server Worker:</h3></p><p> 哦吼 </p></html>");
		send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);

		char recvBuf[5000];
		memset(recvBuf, 0, sizeof(recvBuf));
		recv(sockConn, recvBuf, 5000, 0);
		printf("%s\n", recvBuf);

		closesocket(sockConn);
		Sleep(2000);//2000毫秒
	}
	WSACleanup();

	return 0;
}