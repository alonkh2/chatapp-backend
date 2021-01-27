#pragma once
#include <string>

class message
{
public:
	// C'tor and d'tor.
	
	message(std::string msg, std::string from, std::string to);
	~message();

	// Getters.
	std::string get_msg() const;
	std::string get_from() const;
	std::string get_to() const;

	// Setters.
	void set_msg(const std::string&);
	void set_from(const std::string&);
	void set_to(const std::string&);

	// Stringing.
	std::string to_string() const;
	std::string get_file_name() const;
private:
	std::string msg_;
	std::string from_;
	std::string to_;
};
