#include "../../includes/exceptions/ServerException.hpp"

ServerException::ServerException(const std::string& msg)
{
	_error_msg = "Server error: " + msg;
}

ServerException::~ServerException() throw() {}

const char* ServerException::what() const throw()
{
	return (_error_msg.c_str());
}
