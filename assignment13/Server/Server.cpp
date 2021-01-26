#include "Server.h"
#include "Helper.h"
#include <exception>
#include <iostream>
#include <string>


Server::Server()
{
	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	server_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (server_socket_ == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
}

Server::~Server()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(server_socket_);
	}
	catch (...)
	{
	}
}

void Server::serve(int port)
{
	struct sockaddr_in sa = {0};

	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET; // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY; // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// again stepping out to the global namespace
	// Connects between the socket and the configuration (port and etc..)
	if (bind(server_socket_, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (listen(server_socket_, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << port << std::endl;

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "Waiting for client connection request" << std::endl;

		// std::thread t1([=] {accept();  });

		accept();
	}
}


void Server::accept()
{
	// notice that we step out to the global namespace
	// for the resolution of the function accept

	// this accepts the client and create a specific socket from server to this client
	const SOCKET client_socket = ::accept(server_socket_, nullptr, nullptr);

	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	std::cout << "Client accepted. Server and client can speak" << std::endl;

	// const auto name = connect(client_socket);

	// std::pair < std::string, std::thread*> p1;
	// p1.first = name;
	// p1.second = new std::thread([=] { clientHandler(client_socket); });

	std::thread t([=] { clientHandler(client_socket); });
	t.detach();
	
	// sockets_.insert(p1);
	
	// p1.second->detach();

	
	// Helper::getStringPartFromSocket();

	// the function that handle the conversation with the client

	// clientHandler(client_socket);
}


void Server::clientHandler(SOCKET client_socket)
{
	try
	{
		const auto name = connect(client_socket);
		std::string s = "Welcome! What is your name (4 bytes)? ";
		// send(clientSocket, s.c_str(), s.size(), 0);  // last parameter: flag. for us will be 0.

		// mx_.lock();
		sockets_.insert(std::pair<std::string, SOCKET>(name, client_socket));
		// mx_.unlock();

		while (client_socket != INVALID_SOCKET)
		{
			std::string msg = Helper::getStringPartFromSocket(client_socket, 200);
			// std::cout << msg << std::endl;
			Helper::send_update_message_to_client(client_socket, "", "", get_users());
		}
		std::cout << "here" << std::endl;
		std::string err = "Error while receiving from socket: ";
		err += std::to_string(client_socket);
		throw std::exception(err.c_str());
		
		// send(clientSocket, s.c_str(), s.size(), 0);

		// Closing the socket (in the level of the TCP protocol)
		closesocket(client_socket);
	}
	catch (const std::exception& e)
	{
		closesocket(client_socket);
	}
}

std::string Server::connect(SOCKET client_socket) const
{
	char m[105];
	recv(client_socket, m, 104, 0);
	m[104] = 0;

	std::string str(m);

	const auto len = atoi(str.substr(3, 2).c_str());

	const auto name = str.substr(5, len);
	std::cout << "Client name is: " << name << std::endl;

	/*str = "1010000000000";
	if (len / 10 == 0)
	{
		str.append("0");
	}

	str.append(std::to_string(len));
	str.append(name);

	Helper::sendData(client_socket, str);*/
	Helper::send_update_message_to_client(client_socket, "", "", get_users());
	return name;
}

std::string Server::get_users() const
{
	std::string user_list;
	for (auto user : sockets_)
	{
		user_list.append(user.first);
		user_list.append("&");
	}
	return user_list.substr(0, user_list.length() - 1);
}
