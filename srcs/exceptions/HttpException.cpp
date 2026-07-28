#include "../../includes/exceptions/HttpException.hpp"

HttpException::HttpException(int code, const std::string& msg)
{
    _error_code = code;
    _error_msg = msg;
}
HttpException::~HttpException() throw() {}

int HttpException::getCode() const
{
    return(_error_code);
}

const char* HttpException::what() const throw()
{
    return(_error_msg.c_str());
}

