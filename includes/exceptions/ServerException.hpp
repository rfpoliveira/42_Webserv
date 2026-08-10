#pragma once

#include <exception>
#include <string>

class ServerException: public std::exception
{
	private:
		std::string _error_msg;
	public:
		explicit ServerException(const std::string& msg);
		virtual ~ServerException() throw();
		virtual const char* what() const throw();
};
