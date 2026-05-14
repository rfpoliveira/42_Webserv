/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 11:09:34 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/05/14 11:46:00 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"

class Request
{
    private:
        std::string request_method; //get, post or delete
        std::string query_string;
        std::string content_length;
        std::string content_type;
        std::string resource_path;
        std::string html_version;
        std::string body;

    public:
        Request();
        Request(std::string Request_line);
        Request(const Request &other);
        Request &operator=(const Request &other); 
        ~Request();
};