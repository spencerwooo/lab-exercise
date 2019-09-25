#include <stdio.h>
#include <Winsock2.h>

#pragma comment( lib, "ws2_32.lib" ) 

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
	for (int index = 0;; index++)
	{
		SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

		SOCKADDR_IN addrClt;//需要包含服务端IP信息
		addrClt.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");// inet_addr将IP地址从点数格式转换成网络字节格式整型。
		addrClt.sin_family = AF_INET;
		addrClt.sin_port = htons(8888);

		connect(sockClient, (SOCKADDR*)& addrClt, sizeof(SOCKADDR));//客户机向服务器发出连接请求
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