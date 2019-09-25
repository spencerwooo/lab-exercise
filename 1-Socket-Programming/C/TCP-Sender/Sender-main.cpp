#include <stdio.h>
#include <Winsock2.h>

#pragma comment( lib, "ws2_32.lib" ) 

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
	for (int index = 0;; index++)
	{
		SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

		SOCKADDR_IN addrClt;//��Ҫ���������IP��Ϣ
		addrClt.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");// inet_addr��IP��ַ�ӵ�����ʽת���������ֽڸ�ʽ���͡�
		addrClt.sin_family = AF_INET;
		addrClt.sin_port = htons(8888);

		connect(sockClient, (SOCKADDR*)& addrClt, sizeof(SOCKADDR));//�ͻ����������������������
		char recvBuf[50];
		recv(sockClient, recvBuf, 50, 0);
		printf("my reply is : %s\n", recvBuf);

		char sendBuf[50];
		sprintf(sendBuf, "%3d,", index);
		strcat(sendBuf, "client: aqi");
		send(sockClient, sendBuf, strlen(sendBuf) + 1, 0);

		closesocket(sockClient);
		Sleep(2000);
	}
	WSACleanup();

	return 0;
}