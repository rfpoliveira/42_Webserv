#pragma once

#include <exception>
#include <string>

class ConfigException: public std::exception
{
	private:
		std::string _error_msg;
	public:
		explicit ConfigException(const std::string& msg);
		virtual ~ConfigException() throw();
		virtual const char* what() const throw();
};
