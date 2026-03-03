/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 09:26:09 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/03/03 12:18:17 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"

Server::Server()
{
    host = nullptr;
    port = -1;
    server_name = nullptr;
    max_body_size = -1;
};

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
        add_error_page(error_pages, tokens);
    else if (tokens.at(0) == "location")
        return (1);
    //TODO: ERROR IF NO KEYWORD IS NOT FOUND
    return (0);
};


Server::Server(int server_pos, std::string config_file)
{
    std::cout << "starting server construction\n";
    host = "";
    port = -1;
    server_name = "";
    max_body_size = -1;
    
    std::string line;
    std::string location_string;
    std::ifstream file(config_file);

    std::cout << "file opened in server contrctuion\n";

    while(std::getline(file, line))
    {
        std::cout << "getting to the position of the " << server_pos << " server\n";
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

/* Server::Server(const Server &other)
{
    //TODO
};

Server& Server::operator=(const Server &other)
{
    //TODO
}; */
 
Server::~Server(){};