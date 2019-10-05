#include <WinSock.h>
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<string>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

void geturl(char* request, char* url)
{ //��ȡURL(127.0.0.1:80/csbit.html)
	char* p;
	char* r;
	p = request;
	while (*p != ' ')     //�ҵ�html�ļ�
		p++;
	p += 2;
	r = p;
	while (*r != ' ')     //��ȡ�ļ���
		r++;
	*r = '\0';
	strcpy(url, p);
	p = url;
	while (*p != '\0')
	{
		if (*p == '/')
			* p = '.\\';
		p++;
	}
}

void reply(char* request, char* replybuf)
{ //��ȡ���յ����ļ���Ϣ
	char url[1024] = { 0 };
	geturl(request, url); //�õ�url
	cout << url << endl;
	FILE* fp;
	if ((fp = fopen(url, "r")) == NULL)
	{  //��html�ļ�
		strcpy(replybuf, "HTTP/1.1 404 Not Found\r\n\r\n");
	}
	else
	{
		char tempstring[1024];
		char ch;
		int i = 0;
		while ((ch = fgetc(fp)) != EOF)
		{
			tempstring[i] = ch;
			i++;
		}
		tempstring[i] = '\0';
		sprintf(replybuf, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n%s", tempstring);
	}
}

int main()
{
	WSAData wsaData;
	if (WSAStartup(0x101, &wsaData))//����winsock��
	{
		cout << "Start Error!" << endl;
	}
	else
	{
		cout << "Start OK!" << endl;
	}

	SOCKET srvSock, clientSock;
	struct sockaddr_in srvAddr, client;
	srvSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (srvSock == INVALID_SOCKET)
	{
		cout << "Listen socket create Error!" << endl;
	}
	else
	{
		cout << "Listen socket create Succeed!" << endl;
	}

	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");
	srvAddr.sin_port = htons(80);
	if (bind(srvSock, (sockaddr*)& srvAddr, sizeof(srvAddr)) == SOCKET_ERROR)
	{ //����
		cout << "Bind Error!" << endl;
	}
	else
	{
		cout << "Bind OK!" << endl;
	}

	if (listen(srvSock, 1) == SOCKET_ERROR)
	{ //�����˿�
		cout << "Listen Error!" << endl;
	}

	char request[1024] = { 0 };
	char replybuf[1024] = { 0 };
	while (1)
	{
		cout << "Listening..." << endl;
		int len = sizeof(client);
		//��������
		clientSock = accept(srvSock, (sockaddr*)& client, &len);
		if (clientSock == INVALID_SOCKET)
		{
			cout << "Accept Error" << endl;
		}
		else
		{
			cout << "Accept OK!" << endl << endl;
			int flag = 0;
			while (1)
			{
				flag = 0;
				flag = recv(clientSock, request, 1024, 0);//����html�ļ�
				if (flag == 0 || flag == SOCKET_ERROR)
				{
					cout << "Connection terminated." << endl;
					break;
				}
				cout << request << endl;
				reply(request, replybuf);
				cout << replybuf << endl;
				send(clientSock, replybuf, strlen(replybuf), 0);
			}
			closesocket(clientSock);
		}
	}
	closesocket(srvSock);
	WSACleanup();
	return 0;
}
