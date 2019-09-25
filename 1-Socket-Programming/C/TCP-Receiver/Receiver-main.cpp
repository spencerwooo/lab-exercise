#include <stdio.h>
#include <Winsock2.h>
#include <iostream>
using namespace std;

#pragma comment( lib, "ws2_32.lib" )// ����Winsock2.h�ľ�̬���ļ�

int main()
{
	//��ʼ��winsocket
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

	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);//����socket��AF_INET��ʾ��Internet��ͨ�ţ�SOCK_STREAM��ʾsocket�����׽��֣���Ӧtcp��0ָ������Э��ΪTCP/IP

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);	//htonl�����������ֽ�˳��ת��Ϊ�����ֽ�˳��(to network long)
	//INADDR_ANY����ָ����ַΪ0.0.0.0�ĵ�ַ,
	//��ʾ��ȷ����ַ,�������ַ������
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8888);//htons�����������ֽ�˳��ת��Ϊ�����ֽ�˳��(to network short)

	bind(sockSrv, (SOCKADDR*)& addrSrv, sizeof(SOCKADDR));//�����ص�ַ�󶨵���������socket�ϣ���ʹ�������ϱ�ʶ��socket

	listen(sockSrv, 5);//socket������׼��������������

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	while (1)
	{
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)& addrClient, &len);//Ϊһ�����������ṩ����addrClient�����˷�����������Ŀͻ���IP��ַ��Ϣ�����ص���socket������������ÿͻ���������
		
		char sendBuf[50];
		sprintf(sendBuf, "TEMPERATURE = 60 HUMIDITY = 0 LIGHT = 1");
		send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);

		char recvBuf[50];
		recv(sockConn, recvBuf, 50, 0);
		printf("%s\n", recvBuf);

		closesocket(sockConn);
		Sleep(2000);//2000����
	}
	WSACleanup();

	return 0;
}