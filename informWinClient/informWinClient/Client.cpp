#include "Client.h"
#include <iostream>
using namespace std;

void Client::connectServer(string ip, string port)
{
	int ret;

	if (isConnected)
	{
		cout << "Connection already established" << endl;
		return;
	}
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
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET)
	{
		WSACleanup();
		printf("Socket() failed!\n");
		return;
	}

	//������������ַ��Ϣ��
	saServer.sin_family = AF_INET;//��ַ����
	saServer.sin_port = htons(atoi(port.c_str()));//ע��ת��Ϊ�����ֽ���
	saServer.sin_addr.S_un.S_addr = inet_addr(ip.c_str());

	//���ӷ�������
	ret = connect(sClient, (struct sockaddr *)&saServer, sizeof(saServer));
	if (ret == SOCKET_ERROR)
	{
		printf("Connection failed!\n");
		closesocket(sClient);//�ر��׽���
		WSACleanup();
		return;
	}

	string connStr = "Client connented\n";
	ret = send(sClient, connStr.c_str(), sizeof(char)*connStr.length(), 0);
	if (ret == SOCKET_ERROR)
	{
		printf("Connection failed!\n");
		isConnected = false;
	}
	else
	{
		printf("Connection Established!\n");
		isConnected = true;
	}
	receiveFromServer();
}

void Client::disconnectServer()
{
	closesocket(sClient);//�ر��׽���
	WSACleanup();
	isConnected = false;
}

void Client::req_time()
{
	sendToServer("T\n");
}

void Client::req_name()
{
	sendToServer("N\n");
}

void Client::req_list()
{
	sendToServer("L\n");
}

void Client::req_send(string id, string mesg)
{
	sendToServer("L "+id+" "+mesg+" \n");
}

void Client::sendToServer(string mesg)
{
	int ret;
	if (!isConnected)
	{
		cout << "Connection not established please use conn <ip> <port> first" << endl;
		return;
	}
	ret = send(sClient, mesg.c_str(), sizeof(char)*mesg.length(), 0);
	if (ret == SOCKET_ERROR)
	{
		printf("Connection failed!\n");
		isConnected = false;
	}
	receiveFromServer();
}

void Client::receiveFromServer()
{
	int ret;
	char *buffer = new char[MAX_BUF_SIZE];
	ret = recv(sClient, buffer, MAX_BUF_SIZE, 0);
	if (ret == SOCKET_ERROR)
	{
		printf("receive from server failed!\n");
	}
	else if (ret == 0)
	{
		printf("connection closed by server\n");
	}
	else
	{
		buffer[ret] = 0;
		cout << string(buffer) << endl;
	}
	delete[]buffer;
}
