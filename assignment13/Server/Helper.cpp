#include "Helper.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using std::string;

// recieves the type code of the message from socket (3 bytes)
// and returns the code. if no message found in the socket returns 0 (which means the client disconnected)
int Helper::getMessageTypeCode(SOCKET sc)
{
	char* s = getPartFromSocket(sc, 3);
	std::string msg(s);

	if (msg.empty())
		return 0;

	int res = std::atoi(s);
	delete s;
	return  res;
}


void Helper::send_update_message_to_client(SOCKET sc, const string& file_content, const string& second_username, const string& all_users)
{
	//TRACE("all users: %s\n", all_users.c_str())
	const string code = std::to_string(MT_SERVER_UPDATE);
	const string current_file_size = getPaddedNumber(file_content.size(), 5);
	const string username_size = getPaddedNumber(second_username.size(), 2);
	const string all_users_size = getPaddedNumber(all_users.size(), 5);
	const string res = code + current_file_size + file_content + username_size + second_username + all_users_size + all_users;
	//TRACE("message: %s\n", res.c_str());
	std::cout << res << std::endl;
	std::cout << "socket: " << std::to_string(sc) << std::endl;
	sendData(sc, res);
}

// recieve data from socket according byteSize
// returns the data as int
int Helper::getIntPartFromSocket(SOCKET sc, int bytesNum)
{
	char* s = getPartFromSocket(sc, bytesNum, 0);
	return atoi(s);
}

// recieve data from socket according byteSize
// returns the data as string
string Helper::getStringPartFromSocket(SOCKET sc, int bytesNum)
{
	char* s = getPartFromSocket(sc, bytesNum, 0);
	string res(s);
	return res;
}

// return string after padding zeros if necessary
string Helper::getPaddedNumber(int num, int digits)
{
	std::ostringstream ostr;
	ostr << std::setw(digits) << std::setfill('0') << num;
	return ostr.str();

}

void Helper::read_message(const std::string& msg, const std::string& name, SOCKET sc, const std::string& users)
{
	std::string message;
	const auto len_of_user = parse_int(msg, 3, 2), len_of_content = parse_int(msg, 5 + len_of_user, 5);
	if (len_of_user > 0)
	{
		
		auto second_user = msg.substr(5, len_of_user);
		std::string full = R"(E:\Magshimim\Coding\assignment_13\assignment13\Debug\)";
		full.append(second_user.compare(name) > 0 ? name + "&" + second_user : second_user + "&" + name);
		std::cout << full << std::endl;
		if (len_of_content > 0)
		{
			full.append(".txt");
			std::fstream f1(full, std::ios::out);
			if (f1)
			{
				f1 >> message;
				f1.close();
				f1.open(full, std::ios::app);
			}
			else
			{
				f1.open(full, std::ios::in);
			}
			std::string new_message = msg.substr(5 + len_of_content + len_of_user, len_of_content);
			message.append(new_message);
			std::cout << f1.is_open() << std::endl;
			f1 << message;
			f1.close();
			send_update_message_to_client(sc, message, second_user, users);
		}
	}
}

// receive data from socket according byteSize
// this is private function
char* Helper::getPartFromSocket(SOCKET sc, int bytesNum)
{
	return getPartFromSocket(sc, bytesNum, 0);
}

char* Helper::getPartFromSocket(SOCKET sc, int bytesNum, int flags)
{
	if (bytesNum == 0)
	{
		return (char*)"";
	}

	char* data = new char[bytesNum + 1];
	int res = recv(sc, data, bytesNum, flags);

	if (res == INVALID_SOCKET)
	{
		std::string s = "Error while receiving from socket: ";
		s += std::to_string(sc);
		throw std::exception(s.c_str());
	}

	if (bytesNum < strlen(data))
	{
		data[bytesNum] = 0;
	}
	return data;
}

int Helper::parse_int(const std::string& msg, const int begin, const int len)
{
	return atoi(msg.substr(begin, len).c_str());
}

// send data to socket
// this is private function
void Helper::sendData(SOCKET sc, std::string message)
{
	const char* data = message.c_str();

	if (send(sc, data, message.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}
}