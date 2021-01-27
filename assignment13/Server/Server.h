#pragma once

#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <WinSock2.h>
#include <Windows.h>

#include "message.h"


class Server
{
public:
	// C'tor and d'tor.
	
	Server();
	~Server();

	// Server.
	void serve(unsigned short port);

private:
	// Listening.
	void accept();

	// Talking.
	void client_handler(SOCKET client_socket);
	std::string connect(SOCKET client_socket);


	// Stringing.
	std::string get_users();

	// Filing.
	void add_message(const std::string&, const std::string&, const std::string&);
	void handle_message();
	static void add_to_file(const std::string&, const std::string&);
	static std::string get_file(const std::string&, const std::string&);

	SOCKET server_socket_;
	std::map<std::string, SOCKET> sockets_;

	std::queue<message> messages_;
	     
	std::mutex mx_;
	std::mutex messages_mutex_;

	std::condition_variable cv_;
};
