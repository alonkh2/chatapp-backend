#include "Server.h"
#include "Helper.h"
#include <exception>
#include <iostream>
#include <string>
#include <fstream>


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

void Server::serve(unsigned short port)
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


	std::thread t([=] { clientHandler(client_socket); });
	t.detach();

	
	// the function that handle the conversation with the client
}


void Server::clientHandler(SOCKET client_socket)
{
	std::string name;
	try
	{
		name = connect(client_socket);
		if (sockets_.size() == 2)
		{
			std::thread msg_handling([=] { handle_message(); });
			msg_handling.detach();
		}
		while (client_socket != INVALID_SOCKET)
		{
			auto msg = Helper::getStringPartFromSocket(client_socket, 200);
			auto new_msg = Helper::read_message(msg);
			auto to = Helper::get_second_user(msg);
			if (!new_msg.empty() && !to.empty())
			{
				add_message(new_msg, name, to);
			}
			Helper::send_update_message_to_client(client_socket, get_file(name, to), to, get_users());
		}
		std::cout << "here" << std::endl;
		std::string err = "Error while receiving from socket: ";
		err += std::to_string(client_socket);
		throw std::exception(err.c_str());
	}
	catch (const std::exception& e)
	{
		closesocket(client_socket);
		mx_.lock();
		sockets_.erase(sockets_.find(name));
		mx_.unlock();
	}
}

std::string Server::connect(SOCKET client_socket)
{
	char m[105];
	recv(client_socket, m, 104, 0);
	m[104] = 0;

	const std::string str(m);

	const auto len = atoi(str.substr(3, 2).c_str());

	auto name = str.substr(5, len);
	// std::cout << "Client name is: " << name << std::endl;

	mx_.lock();
	sockets_.insert(std::pair<std::string, SOCKET>(name, client_socket));
	mx_.unlock();

	Helper::send_update_message_to_client(client_socket, "", "", get_users());
	return name;
}

std::string Server::get_users()
{
	std::string user_list;
	mx_.lock();
	for (auto user : sockets_)
	{
		user_list.append(user.first);
		user_list.append("&");
	}
	mx_.unlock();
	return user_list.substr(0, user_list.length() - 1);
}

void Server::add_message(const std::string& msg, const std::string& from, const std::string& to)
{
	// std::lock_guard<std::mutex> lg(messages_mutex_);
	messages_mutex_.lock();
	messages_.push(message(msg, from, to));
	messages_mutex_.unlock();
	cv_.notify_all();
}

void Server::handle_message()
{
	while (true)
	{
		auto i = 0;
		std::unique_lock<std::mutex> ul(messages_mutex_);
		cv_.wait(ul);
		auto* const messages = new std::string[messages_.size()];
		auto* const file_names = new std::string[messages_.size()];
		while (!messages_.empty())
		{
			messages[i] = messages_.front().to_string();
			file_names[i] = messages_.front().get_file_name();
			messages_.pop();
			i++;
		}
		ul.unlock();

		for (int j = 0; j < i; ++j)
		{
			add_to_file(messages[j], file_names[j]);
		}
		delete[] messages;
		delete[] file_names;
	}
}

void Server::add_to_file(const std::string& msg, const std::string& file)
{
	std::string message;
	std::string temp;
	std::string path = R"(E:\Magshimim\Coding\assignment_13\assignment13\Debug\)";
	path.append(file);
	std::ifstream f1(path);
	if (f1)
	{
		while (f1 >> temp)
		{
			message.append(temp + " ");
		}
		f1.close();
	}
	std::ofstream f2(path);
	message.append(msg);
	f2 << message << std::endl;
	f2.close();
}

std::string Server::get_file(const std::string& from, const std::string& to)
{
	std::string path = R"(E:\Magshimim\Coding\assignment_13\assignment13\Debug\)";
	path.append(Helper::get_file_name(from, to));
	std::ifstream f1(path);
	std::string msg;
	std::string temp;
	if (f1)
	{
		while (f1 >> temp)
		{
			msg.append(temp + " ");
		}
		f1.close();
	}
	return msg;
}
