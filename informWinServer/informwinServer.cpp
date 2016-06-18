/*
 *	���������ҪĿ������˵��socket��̵Ļ������̣����Է�����/�ͻ��˵Ľ������̷ǳ��򵥣�
 *  ֻ���ɿͻ��������������һ��ѧ����Ϣ�Ľṹ��
 */
//informWinServer.cpp������ʱû�в�����ʹ�ö˿�6666����������
#include <stdio.h>
#include <string>
#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#define SERVER_PORT	6666 //�����˿�
#define MAX_BUF_SIZE 512

using namespace std;

//�ͻ�������������͵Ľṹ��
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
		SOCKET sListen, sServer; //�����׽��֣������׽���
		struct sockaddr_in saServer, saClient;//��ַ��Ϣ
		struct student stu;
		char *ptr;

		//WinSock��ʼ����
		wVersionRequested = MAKEWORD(2, 2);//ϣ��ʹ�õ�WinSock DLL�İ汾
		ret = WSAStartup(wVersionRequested, &wsaData);
		if (ret != 0)
		{
			printf("WSAStartup() failed!\n");
			return;
		}
		//ȷ��WinSock DLL֧�ְ汾2.2��
		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		{
			WSACleanup();
			printf("Invalid Winsock version!\n");
			return;
		}

		//����socket��ʹ��TCPЭ�飺
		sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sListen == INVALID_SOCKET)
		{
			WSACleanup();
			printf("socket() failed!\n");
			return;
		}

		//�������ص�ַ��Ϣ��
		saServer.sin_family = AF_INET;//��ַ����
		saServer.sin_port = htons(SERVER_PORT);//ע��ת��Ϊ�����ֽ���
		saServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//ʹ��INADDR_ANYָʾ�����ַ

		//�󶨣�
		ret = bind(sListen, (struct sockaddr *)&saServer, sizeof(saServer));
		if (ret == SOCKET_ERROR)
		{
			printf("bind() failed! code:%d\n", WSAGetLastError());
			closesocket(sListen);//�ر��׽���
			WSACleanup();
			return;
		}

		//������������
		ret = listen(sListen, 5);
		if (ret == SOCKET_ERROR)
		{
			printf("listen() failed! code:%d\n", WSAGetLastError());
			closesocket(sListen);//�ر��׽���
			WSACleanup();
			return;
		}

		printf("Waiting for client connecting!\n");
		printf("tips : Ctrl+c to quit!\n");
		//�����ȴ����ܿͻ������ӣ�
		length = sizeof(saClient);
		sServer = accept(sListen, (struct sockaddr *)&saClient, &length);
		if (sServer == INVALID_SOCKET)
		{
			printf("accept() failed! code:%d\n", WSAGetLastError());
			closesocket(sListen);//�ر��׽���
			WSACleanup();
			return;
		}
		printf("Accepted client: %s:%d\n",
			inet_ntoa(saClient.sin_addr), ntohs(saClient.sin_port));

		//����Ԥ��Э�飬�ͻ��˽�����һ��ѧ������Ϣ��
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
		closesocket(sListen);//�ر��׽���
		closesocket(sServer);
		WSACleanup();
	}
}