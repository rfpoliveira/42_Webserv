#pragma once

#include <exception>
#include <string>

class HttpException : public std::exception
{
    private:
        int         _error_code;
        std::string _error_msg;
    public:
        HttpException(int code, const std::string& msg);
        virtual ~HttpException() throw();
        int getCode() const;
        virtual const char* what() const throw();
};