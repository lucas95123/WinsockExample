/*
 *	本程序的主要目的在于说明socket编程的基本过程，所以服务器/客户端的交互过程非常简单，
 *  只是由客户端向服务器传送一个学生信息的结构。
 */
//informWinServer.cpp：运行时没有参数，使用端口6666进行侦听：
#include <stdio.h>
#include <string>
#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#define SERVER_PORT	6666 //侦听端口
#define MAX_BUF_SIZE 512

using namespace std;

//客户端向服务器传送的结构：
struct student
{
	char name[32];
	int age;
};

void main()
{
	while (1)
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		int ret, nLeft, length;
		SOCKET sListen, sServer; //侦听套接字，连接套接字
		struct sockaddr_in saServer, saClient;//地址信息
		struct student stu;
		char *ptr;

		//WinSock初始化：
		wVersionRequested = MAKEWORD(2, 2);//希望使用的WinSock DLL的版本
		ret = WSAStartup(wVersionRequested, &wsaData);
		if (ret != 0)
		{
			printf("WSAStartup() failed!\n");
			return;
		}
		//确认WinSock DLL支持版本2.2：
		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		{
			WSACleanup();
			printf("Invalid Winsock version!\n");
			return;
		}

		//创建socket，使用TCP协议：
		sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sListen == INVALID_SOCKET)
		{
			WSACleanup();
			printf("socket() failed!\n");
			return;
		}

		//构建本地地址信息：
		saServer.sin_family = AF_INET;//地址家族
		saServer.sin_port = htons(SERVER_PORT);//注意转化为网络字节序
		saServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//使用INADDR_ANY指示任意地址

		//绑定：
		ret = bind(sListen, (struct sockaddr *)&saServer, sizeof(saServer));
		if (ret == SOCKET_ERROR)
		{
			printf("bind() failed! code:%d\n", WSAGetLastError());
			closesocket(sListen);//关闭套接字
			WSACleanup();
			return;
		}

		//侦听连接请求：
		ret = listen(sListen, 5);
		if (ret == SOCKET_ERROR)
		{
			printf("listen() failed! code:%d\n", WSAGetLastError());
			closesocket(sListen);//关闭套接字
			WSACleanup();
			return;
		}

		printf("Waiting for client connecting!\n");
		printf("tips : Ctrl+c to quit!\n");
		//阻塞等待接受客户端连接：
		length = sizeof(saClient);
		sServer = accept(sListen, (struct sockaddr *)&saClient, &length);
		if (sServer == INVALID_SOCKET)
		{
			printf("accept() failed! code:%d\n", WSAGetLastError());
			closesocket(sListen);//关闭套接字
			WSACleanup();
			return;
		}
		printf("Accepted client: %s:%d\n",
			inet_ntoa(saClient.sin_addr), ntohs(saClient.sin_port));

		//按照预定协议，客户端将发来一个学生的信息：
		while (1)
		{
			char *buffer = new char[MAX_BUF_SIZE];
			ret = recv(sServer, buffer, MAX_BUF_SIZE, 0);
			if (ret == SOCKET_ERROR)
			{
				printf("receive from client failed!\n");
				break;
			}
			else if (ret == 0)
			{
				printf("connection closed by client\n");
				break;
			}
			else
			{
				buffer[ret] = 0;
				cout << string(buffer) << endl;
				send(sServer, "Message back", 12, 0);
			}
			delete[]buffer;
		}
		closesocket(sListen);//关闭套接字
		closesocket(sServer);
		WSACleanup();
	}
}