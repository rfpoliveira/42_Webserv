/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 09:26:09 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/03/06 11:04:59 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"

Server::Server()
{
    host = "127.0.0.1";
    port = 80;
    server_name = "server1.com";
    max_body_size = 1000000;
};

//check for a keyword and stores the information associated in the right place in the class

int Server::check_line_server(std::string line)
{
    ignore_comments(line);

    std::vector<std::string> tokens = ft_split(line, ' ');

    if(tokens.at(0) == "server")
        return (2);
    
    if(tokens.at(0) == "listen")
        port = atoi(tokens.at(1).c_str());
    else if (tokens.at(0) == "server_name")
        server_name = tokens.at(1);
    else if(tokens.at(0) == "host")
        host = tokens.at(1);
    else if(tokens.at(0) == "client_max_body_size")
        max_body_size = get_body_size(tokens.at(1));
    else if (tokens.at(0) == "error_page")
    {
        if (add_error_page(error_pages, tokens) != 0)
            throw (ServerErrorExeption());
    }
    else if (tokens.at(0) == "location")
        return (1);
    return (0);
};

//searches for the right server information for each one in the config file
//goes line by line checking for the keyword
//if a location is found, calls its construtor and saves it in the Locations vector

Server::Server(int server_pos, std::string config_file)
{
    host = "127.0.0.1";
    port = 80;
    server_name = "server1.com";
    max_body_size = 1000000;
    
    std::string line;
    std::string location_string;
    std::ifstream file(config_file.c_str());

    while(std::getline(file, line))
    {
        if (line == "server {")
            server_pos--;
        if (server_pos == 0)
            break;
        line.clear();
    }

    int ret = 0;

    while(std::getline(file, line, ';'))
    {
        ret = check_line_server(line);
        if (ret == 1)
        {
            std::getline(file, location_string, '}');
            Locations.push_back(Location(line + location_string));
        }
        else if (ret == 2)
            break ;
        line.clear();
    }

    file.close();
};

Server::Server(const Server &other)
{
    this->host = other.host;
    this->port = other.port;
    this->server_name = other.server_name;
    this->max_body_size = other.max_body_size;
    this->error_pages = other.error_pages;
    this->Locations = other.Locations;
};

Server& Server::operator=(const Server &other)
{
    Server copy(other);

    Server & ret = copy;
    return(ret);
};
 
Server::~Server(){};