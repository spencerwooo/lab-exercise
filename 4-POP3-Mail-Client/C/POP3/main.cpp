#include <stdio.h>
#include <Winsock2.h>
#include <iostream>
using namespace std;

#pragma comment( lib, "ws2_32.lib" ) 


SOCKET sockClient;

void sendMsg(const char* sendBuf)
{
	cout << "send: " << endl << sendBuf << endl;
	send(sockClient, sendBuf, strlen(sendBuf), 0);
}

void receiveMsg()
{
	char recvBuf[5000];
	memset(recvBuf, 0, sizeof(char) * 5000);
	int num = recv(sockClient, recvBuf, 5000, 0);
	cout << "recv:" << endl;
	printf("%s\n", recvBuf);
}

int main()
{
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

	sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrClt;//需要包含服务端IP信息
	addrClt.sin_addr.S_un.S_addr = inet_addr("3.88.75.43");// 测试用 pop3服务器
	addrClt.sin_family = AF_INET;
	addrClt.sin_port = htons(1100);//测试pop3服务器的pop3端口

	connect(sockClient, (SOCKADDR*)& addrClt, sizeof(SOCKADDR));//连接服务器
	receiveMsg();// 接受服务器发送的信息

	sendMsg("user 0f24017c4bf5b0\r\n"); // 
	receiveMsg();

	sendMsg("pass 3b5c4c5470b52b\r\n"); // 
	receiveMsg();

	sendMsg("list\r\n"); // 
	receiveMsg();
	receiveMsg();

	sendMsg("retr 1\r\n"); // 
	receiveMsg();

	sendMsg("quit\r\n"); // 发送 
	receiveMsg();

	closesocket(sockClient);

	WSACleanup();

	return 0;
}