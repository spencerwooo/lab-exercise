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
	//cout << "yeah" << endl;
	char recvBuf[5000];
	memset(recvBuf, 0, sizeof(char) * 5000);
	int num = recv(sockClient, recvBuf, 5000, 0);
	cout << "recv:" << endl;
	printf("%s\n", recvBuf);
	//cout << "fuck" << endl;
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
	//addrClt.sin_addr.S_un.S_addr = inet_addr("52.205.50.181");// inet_addr��IP��ַ�ӵ�����ʽת���������ֽڸ�ʽ���͡�
	addrClt.sin_addr.S_un.S_addr = inet_addr("14.18.245.164");// 
	addrClt.sin_family = AF_INET;
	addrClt.sin_port = htons(25);//smtp�˿�

	connect(sockClient, (SOCKADDR*)& addrClt, sizeof(SOCKADDR));//�ͻ�����smtp������������������
	receiveMsg();// ���ܷ��������͵���Ϣ

	sendMsg("EHLO Archie'sPC\r\n"); // ����EHLOָ��
	receiveMsg();

	sendMsg("AUTH LOGIN\r\n\0"); // �����½
	receiveMsg();

	//sendMsg("MGYyNDAxN2M0YmY1YjA=\r\n"); // ���� �û��� (base64����)
	sendMsg("MTc1MTE4NDgxOQ==\r\n"); // ���� �û��� (base64����)
	receiveMsg();

	//sendMsg("M2I1YzRjNTQ3MGI1MmI=\r\n"); // ���� ���� (base64����)
	sendMsg("d3FveWthZ3dod3BnYmlkaQ==\r\n"); // ���� ���� (QQ����Ȩ�� base64����)
	receiveMsg();

	//sendMsg("MAIL FROM: <archie@WoTaMaTaiNiuBiLe.com>\r\n"); // ���� 
	sendMsg("MAIL FROM: <1751184819@qq.com>\r\n"); // ���� 
	receiveMsg();

	sendMsg("RCPT TO: <1135819725@qq.com>\r\n");
	receiveMsg();

	sendMsg("DATA\r\n");
	receiveMsg();

	sendMsg("From:\"LiMing\"<1751184819@qq.com>\r\n"
			"To:\"Spencer Woo\"<1135819725@qq.com>\r\n"
			"Subject: Hello\r\n\r\n"
			"�����ǰ�������ս��ƻ裬���Ѷ���������һ��ʯҤ���е��ң�"
			"������д�����Զ�ˡ�����˵���ɲ��ã��ĺ����ԣ�ֱ��ǰ�컹���������࣬"
			"����ͻȻ���磬�������˺�������������������ˣ�ֻ�����յ��������ҡ�"
			"�Ӷ����磬��˵�����겻ֹ����֪�������������������ɽ�ӹ��ڣ���ȻҲ�Ե������±���"
			"�������ѡ����»��ˣ���֪��������������ɫ�������������ڸ��룿��ʹ��������ũ����ʣ�"
			"����һ����æ�ź�ũ�ҹ�ɩ���̸����������ʹ������׼������Ĳ��ϣ�"
			"������п������й�ίһ���һ��ũ����ڡ�����������һ�л������벻�����ظĸ��ǰ��ʤ����"
			"�Ÿ���ͷ�ˣ��ƺ����Ƕ�����һ�������ģ���ηֿ������Եñȿ�սͷ����ķֿ����в�ͬ��"
			"������Ϊ���Ƕ�����ʮ�꣬��Ҫ����Ϊ������Ϊ��������ϵõ��˸����еİ�ο��"
			"�����������ڶ��߶������꣬���������޳ɡ��ٶ���ũ���ж������꣬���룬"
			"�������˼�롢���顢����������ı仯�������������������ʵ�����ᡣ"
			"ũ��Ľ��������������Σ����һ����Ǵ��ӵ����ģ�����һ�档��һ�棬���Ǽ�ǿ��"
			"�ᶨ����־���¸ҵ���Ϊ�����ڱ�ѹ�ȵ�Ⱥ���У������������ʡ���������м��Ի�ѧϰ�ܶ࣬"
			"ֻҪ��̫���ۡ���������Ѭ�գ���׼����Ŀ�࿴��\r\n.\r\n");
	receiveMsg();

	sendMsg("QUIT\r\n");
	receiveMsg();

	closesocket(sockClient);

	WSACleanup();

	return 0;
}