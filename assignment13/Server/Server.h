#pragma once

#include <map>
#include <string>
#include <thread>
#include <WinSock2.h>
#include <Windows.h>


class Server
{
public:
	Server();
	~Server();
	void serve(int port);

private:

	void accept();
	static void clientHandler(SOCKET clientSocket);

	SOCKET server_socket_;
	std::map<std::string, std::thread> sockets_;
};

