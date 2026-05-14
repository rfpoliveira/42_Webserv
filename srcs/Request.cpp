/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 11:12:06 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/05/14 11:45:50 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Request.hpp"

Request::Request(){};

/* requestline ex: GET /test.py?user=nuno HTTP/1.1 */

Request::Request(std::string Request_line)
{
    std::vector<std::string> buffer1 = ft_split(Request_line, ' ');
    request_method = buffer1[0];
    if(buffer1[2].find('?') != buffer1[1].npos)
    {
        std::vector<std::string>buffer2 = ft_split(buffer1[1], '?');
        buffer2[0] = resource_path;
        buffer2[1] = query_string;
    }
    else
        buffer1[1] = resource_path;
    html_version = buffer1[2];
};

Request::Request(const Request &other)
{
        this->request_method = other.request_method;
        this->query_string = other.query_string;
        this->content_length = other.content_length;
        this->content_type = other.content_type;
        this->resource_path = other.resource_path;
        this->html_version = other.html_version;
        this->body = other.body;
        
};

Request& Request::operator=(const Request &other)
{
    Request copy(other);

    Request & ret = copy;
    return(ret);
}; 

Request::~Request(){};