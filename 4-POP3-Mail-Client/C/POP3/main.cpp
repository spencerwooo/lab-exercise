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

	wVersionRequested = MAKEWORD(1, 1);//��һ������Ϊ��λ�ֽڣ��ڶ�������Ϊ��λ�ֽ�

	err = WSAStartup(wVersionRequested, &wsaData);//��winsock DLL����̬���ӿ��ļ������г�ʼ����Э��Winsock�İ汾֧�֣��������Ҫ����Դ��
	if (err != 0)
	{
		return 0;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)//LOBYTE����ȡ��16���������λ��HIBYTE����ȡ��16��������ߣ�����ߣ��Ǹ��ֽڵ�����		
	{
		WSACleanup();
		return 0;
	}

	sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrClt;//��Ҫ���������IP��Ϣ
	addrClt.sin_addr.S_un.S_addr = inet_addr("3.88.75.43");// ������ pop3������
	addrClt.sin_family = AF_INET;
	addrClt.sin_port = htons(1100);//����pop3��������pop3�˿�

	connect(sockClient, (SOCKADDR*)& addrClt, sizeof(SOCKADDR));//���ӷ�����
	receiveMsg();// ���ܷ��������͵���Ϣ

	sendMsg("user 0f24017c4bf5b0\r\n"); // 
	receiveMsg();

	sendMsg("pass 3b5c4c5470b52b\r\n"); // 
	receiveMsg();

	sendMsg("list\r\n"); // 
	receiveMsg();
	receiveMsg();

	sendMsg("retr 1\r\n"); // 
	receiveMsg();

	sendMsg("quit\r\n"); // ���� 
	receiveMsg();

	closesocket(sockClient);

	WSACleanup();

	return 0;
}