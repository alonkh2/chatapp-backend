#include "message.h"

#include "Helper.h"

#include <utility>

#include <iomanip>

#include <sstream>

message::message(std::string msg, std::string from, std::string to):
	msg_(std::move(msg)), from_(std::move(from)), to_(std::move(to))
{
}

message::~message()
= default;

std::string message::get_msg() const
{
	return msg_;
}

std::string message::get_from() const
{
	return from_;
}

std::string message::get_to() const
{
	return to_;
}

void message::set_msg(const std::string& msg)
{
	msg_ = msg;
}

void message::set_from(const std::string& from)
{
	from_ = from;
}

void message::set_to(const std::string& to)
{
	to_ = to;
}

std::string message::to_string() const
{
	std::ostringstream ostr;
	ostr << "&MAGSH_MESSAGE&&Author&" << from_ << "&DATA&" << msg_;
	return ostr.str();
}

std::string message::get_file_name() const
{
	return Helper::get_file_name(from_, to_);
	/*std::ostringstream ostr;
	auto const first = from_.compare(to_) > 0 ? to_ : from_, second = first == to_? from_ : to_;

	ostr << first << "&" << second << ".txt";
	return ostr.str();*/
}
