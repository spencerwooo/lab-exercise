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
	//addrClt.sin_addr.S_un.S_addr = inet_addr("52.205.50.181");// inet_addr将IP地址从点数格式转换成网络字节格式整型。
	addrClt.sin_addr.S_un.S_addr = inet_addr("14.18.245.164");// 
	addrClt.sin_family = AF_INET;
	addrClt.sin_port = htons(25);//smtp端口

	connect(sockClient, (SOCKADDR*)& addrClt, sizeof(SOCKADDR));//客户机向smtp服务器发出连接请求
	receiveMsg();// 接受服务器发送的信息

	sendMsg("EHLO Archie'sPC\r\n"); // 发送EHLO指令
	receiveMsg();

	sendMsg("AUTH LOGIN\r\n\0"); // 请求登陆
	receiveMsg();

	//sendMsg("MGYyNDAxN2M0YmY1YjA=\r\n"); // 发送 用户名 (base64加密)
	sendMsg("MTc1MTE4NDgxOQ==\r\n"); // 发送 用户名 (base64加密)
	receiveMsg();

	//sendMsg("M2I1YzRjNTQ3MGI1MmI=\r\n"); // 发送 密码 (base64加密)
	sendMsg("d3FveWthZ3dod3BnYmlkaQ==\r\n"); // 发送 密码 (QQ是授权码 base64加密)
	receiveMsg();

	//sendMsg("MAIL FROM: <archie@WoTaMaTaiNiuBiLe.com>\r\n"); // 发送 
	sendMsg("MAIL FROM: <1751184819@qq.com>\r\n"); // 发送 
	receiveMsg();

	sendMsg("RCPT TO: <1135819725@qq.com>\r\n");
	receiveMsg();

	sendMsg("DATA\r\n");
	receiveMsg();

	sendMsg("From:\"LiMing\"<1751184819@qq.com>\r\n"
			"To:\"Spencer Woo\"<1135819725@qq.com>\r\n"
			"Subject: Hello\r\n\r\n"
			"今天是八月中秋，日近黄昏，月已东升，坐在一排石窑洞中的我，"
			"正好修写家书寄远人。今年此地年成不好，夏旱秋涝，直至前天还是阴雨连绵，"
			"昨天突然放晴，今天有了好月亮看，但是人民苦了，只能望收到二成左右。"
			"河东来电，亦说是淫雨不止，不知你们那里的情形怎样？山居过节，居然也吃到两块月饼，"
			"几串葡萄。对月怀人，不知滹沱河畔有无月色可览，有无人在感想？假使你正在作农村访问，"
			"那你一定是忙着和农家姑嫂姊妹谈心拉话；假使你正在准备下乡的材料，"
			"那你或有可能与中工委一起过一个农村秋节。不管怎样，一切话题总离不开土地改革和前线胜利。"
			"九个年头了，似乎我们都是在一起过中秋的，这次分开，反显得比抗战头两年的分开大有不同。"
			"不仅因为我们都大了十岁，主要是因为我们在为人民服务上得到了更真切的安慰。"
			"你来电提议在东边多留半年，我是衷心赞成。再多在农民中锻炼半年，我想，"
			"不仅你的思想、感情、生活会起更大的变化，就连你的身体想会更结实而年轻。"
			"农民的健美，不仅是外形，而且还有那纯朴的内心，这是一面。另一面，便是坚强，"
			"坚定的意志，勇敢的行为，这在被压迫的群众中，更是数见不鲜。你从他们中间自会学习很多，"
			"只要不太劳累。我想半年的熏陶，当准备刮目相看。\r\n.\r\n");
	receiveMsg();

	sendMsg("QUIT\r\n");
	receiveMsg();

	closesocket(sockClient);

	WSACleanup();

	return 0;
}