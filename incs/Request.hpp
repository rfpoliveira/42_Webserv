/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 11:09:34 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/06/17 16:38:08 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

class Request
{
    public:
        std::string request_method; //get, post or delete
        std::string query_string;
        std::string resource_path;
        std::string html_version;
        std::string body;

        std::map<std::string, std::string> headers;
        std::string getHeader(std::string key);

        bool is_complete;

        void parse_headers(std::string data);

        Request();
        Request(std::string Request_line);
        Request(const Request &other);
        Request &operator=(const Request &other); 
        ~Request();
};