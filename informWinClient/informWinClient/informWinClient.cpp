#include "Client.h"
#include <iostream>
#include <vector>

void split(string& s, string& delim, vector<string> &ret)
{
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != std::string::npos)
	{
		ret.push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last > 0)
	{
		ret.push_back(s.substr(last, index - last));
	}
}

void print_usage()
{
	cout << "Unrecognized Command Usage:" << endl;
	cout << "conn <ip> <port>: connect server ip:port" << endl;
	cout << "disconn：disconnect from server " << endl;
	cout << "time: request time from server " << endl;
	cout << "name：request machine name from server" << endl;
	cout << "list：request connection info(ID, IP, PORT) of all client from server" << endl;
	cout << "send <id>：send message to the clients specified by id" << endl;
	cout << "quit：quit client program" << endl;
}

int main(int argc, char *argv[])
{
	vector<string> cmd;
	Client client;
	cout << "Socket Client V1.0 By Lu Kuan" << endl;
	print_usage();
	while (1)
	{
		cout << ">>";
		string cmdStr;
		getline(cin, cmdStr);
		split(cmdStr, string(" "), cmd);
		if (cmd[0] == "conn")
		{
			if (cmd.size() < 3)
			{
				print_usage();
				continue;
			}
			client.connectServer(cmd[1], cmd[2]);
		}
		else if (cmd[0] == "disconn")
		{
			client.disconnectServer();
		}
		else if (cmd[0] == "time")
		{
			client.req_time();
		}
		else if (cmd[0] == "name")
		{
			client.req_name();
		}
		else if (cmd[0] == "list")
		{
			client.req_list();
		}
		else if (cmd[0] == "send")
		{
			if (cmd.size() < 3)
			{
				print_usage();
				continue;
			}
			client.req_send(cmd[1], cmd[2]);
		}
		else if (cmd[0] == "quit")
		{
			client.disconnectServer();
			break;
		}
		else
		{
			print_usage();
		}
		cmd.clear();
	}
	return 0;
}