#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "Server.h"
#include <iostream>
#include <exception>
#include <fstream>

int main()
{
	std::ifstream f1;
	f1.open(R"(E:\Magshimim\Coding\assignment_13\assignment13\Debug\config.txt)");

	std::string port_str;

	std::getline(f1, port_str);
	std::getline(f1, port_str);

	port_str = port_str.substr(port_str.length() - 4, 4);
	const unsigned short port = atoi(port_str.c_str());
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