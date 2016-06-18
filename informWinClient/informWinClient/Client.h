#pragma once
#include <stdio.h>
#include <string>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#define MAX_BUF_SIZE 512

using namespace std;

class Client
{
private:
	WORD wVersionRequested;
	WSADATA wsaData;
	SOCKET sClient; //连接套接字
	struct sockaddr_in saServer;//地址信息
	BOOL isConnected;

	void sendToServer(string mesg);
	void receiveFromServer();
public:
	Client() {
		isConnected = false;
	}
	void connectServer(string ip,string port);
	void disconnectServer();
	void req_time();
	void req_name();
	void req_list();
	void req_send(string id, string mesg);
};

