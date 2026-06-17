/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 11:12:06 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/06/17 16:41:36 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Request.hpp"

Request::Request(){};

/* requestline ex: GET /test.py?user=nuno HTTP/1.1 */

Request::Request(std::string Request_line)
{
    is_complete = false;
    std::vector<std::string> buffer1 = ft_split(Request_line, ' ');
    request_method = buffer1[0];
    if(buffer1[2].find('?') != buffer1[1].npos)
    {
        std::vector<std::string>buffer2 = ft_split(buffer1[1], '?');
        resource_path = buffer2[0];
        query_string = buffer2[1];
    }
    else
        resource_path = buffer1[1];
    html_version = buffer1[2];
};


//puts in the headers and body, to be called inside the event loop
void Request::parse_headers(std::string data)
{
    size_t separator = data.find("\r\n\r\n");
    if (separator == std::string::npos)
        return ;
    
    std::string headers_part = data.substr(0, separator);
    std::string body_part = data.substr(separator + 4);

    std::stringstream ss(headers_part);
    std::string line;
    
    while (std::getline(ss, line))
    {
        if(!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);

        if(line.empty())
            continue ;
        std::vector<std::string> buff = ft_split(line, ':');
        clean_strings(buff);
        if (buff.size() > 1)
            headers[buff[0]] = buff[1];
    }
    
    if (headers.find("Content-Length") != headers.end())
    {
        int len = std::atoi(headers["Content-Length"].c_str());
        if (body_part.size() >= static_cast<size_t>(len))
        {
            body = body_part.substr(0, len);
            is_complete = true;
        }
        else
            is_complete = false;
    }
    else
        is_complete = true;
} 


Request::Request(const Request &other)
{
        this->is_complete = other.is_complete;
        this->request_method = other.request_method;
        this->query_string = other.query_string;
        this->headers = other.headers;
        this->resource_path = other.resource_path;
        this->html_version = other.html_version;
        this->body = other.body;
        
};

std::string Request::getHeader(std::string key)
{
    std::map<std::string, std::string>::const_iterator it = headers.find(key);
    if (it != headers.end()) 
        return it->second;
    return (""); 
}

Request& Request::operator=(const Request &other)
{
    Request copy(other);

    Request & ret = copy;
    return(ret);
}; 

Request::~Request(){};