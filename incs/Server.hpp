/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:49:43 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/05/14 11:43:40 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"
#include "Location.hpp"

class Server
{
    private:
        std::string host;
        int port;
        std::string server_name;
        unsigned long max_body_size;
        std::map<int, std::string> error_pages;
        std::vector<Location> Locations;

        int check_line_server(std::string line);

    public:
        Server();
        Server(int server_pos, std::string config_file);
        Server(const Server &other);
        Server &operator=(const Server &other);
        ~Server();

	class ServerErrorExeption: public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Config file error at Server context");
			}
	};
};