#pragma once
#include <string>

class message
{
public:
	message(std::string msg, std::string from, std::string to);
	~message();

	std::string get_msg() const;
	std::string get_from() const;
	std::string get_to() const;

	void set_msg(const std::string&);
	void set_from(const std::string&);
	void set_to(const std::string&);

	std::string to_string() const;
	std::string get_file_name() const;
private:
	std::string msg_;
	std::string from_;
	std::string to_;
};
