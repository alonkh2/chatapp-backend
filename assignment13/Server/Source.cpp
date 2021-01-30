#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "Server.h"
#include <iostream>
#include <exception>
#include <fstream>

int main()
{
	std::ifstream f1;
	f1.open("config.txt");
	unsigned short port;
	
	if (f1)
	{
		std::string port_str;

		std::getline(f1, port_str);
		std::getline(f1, port_str);
		port_str = port_str.substr(port_str.length() - 4, 4);
		port = atoi(port_str.c_str());
	}

	port = 8826;

	
	
	try
	{
		WSAInitializer wsaInit;
		Server myServer;

		myServer.serve(port);
	}
	catch (std::exception& e)
	{
		std::cout << "Error occurred: " << e.what() << std::endl;
	}
	system("PAUSE");
	return 0;
}