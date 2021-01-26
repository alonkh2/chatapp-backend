#pragma once

#include <map>
#include <mutex>
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
	void clientHandler(SOCKET client_socket);

	std::string connect(SOCKET client_socket) const;

	std::string get_users() const;

	SOCKET server_socket_;
	std::map<std::string, SOCKET> sockets_;
	// std::vector<std::string> users_;
	
	std::mutex mx_;
	// std::vector<std::thread> sockets_;
};

