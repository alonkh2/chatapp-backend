#pragma once

#include <map>
#include <string>
#include <thread>
#include <vector>
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
	void clientHandler(SOCKET clientSocket);

	std::string connect(SOCKET clientSocket) const;

	SOCKET server_socket_;
	std::map<std::string, std::thread> sockets_;
	// std::vector<std::thread> sockets_;
};

