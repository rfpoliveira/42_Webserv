#include "../../includes/exceptions/ConfigException.hpp"

explicit ConfigException::ConfigException(const std::string& msg)
{
    _error_msg = "Config error: " + msg;
}

ConfigException::~ConfigException() throw() {}

const char* ConfigException::what() const throw()
{
    return (_error_msg.c_str());
}