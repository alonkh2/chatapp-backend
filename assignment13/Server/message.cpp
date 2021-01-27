#include "message.h"

#include "Helper.h"

#include <utility>

#include <iomanip>

#include <sstream>

/**
 * \brief C'tor.
 * \param msg The message itself.
 * \param from The name of the person who sent the message.
 * \param to The name of the recipient of the message.
 */
message::message(std::string msg, std::string from, std::string to):
	msg_(std::move(msg)), from_(std::move(from)), to_(std::move(to))
{
}

/**
 * \brief D'tor.
 */
message::~message()
= default;

/**
 * \brief msg_ getter.
 * \return msg_ field.
 */
std::string message::get_msg() const
{
	return msg_;
}

/**
 * \brief from_ getter.
 * \return from_ field.
 */
std::string message::get_from() const
{
	return from_;
}

/**
 * \brief to_ getter.
 * \return to_ field.
 */
std::string message::get_to() const
{
	return to_;
}

/**
 * \brief msg_ setter.
 * \param msg New msg_.
 */
void message::set_msg(const std::string& msg)
{
	msg_ = msg;
}

/**
 * \brief from_ setter.
 * \param from New from_.
 */
void message::set_from(const std::string& from)
{
	from_ = from;
}

/**
 * \brief to_ setter.
 * \param to New to_.
 */
void message::set_to(const std::string& to)
{
	to_ = to;
}

/**
 * \brief Creates a string of the message.
 * \return A string.
 */
std::string message::to_string() const
{
	std::ostringstream ostr;
	ostr << "&MAGSH_MESSAGE&&Author&" << from_ << "&DATA&" << msg_;
	return ostr.str();
}

/**
 * \brief Creates the message's matching file's name.
 * \return The file name.
 */
std::string message::get_file_name() const
{
	return Helper::get_file_name(from_, to_);
}
