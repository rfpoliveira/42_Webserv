/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 09:26:09 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/02/11 13:11:33 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server()
{
    host = nullptr;
    port = -1;
    server_name = nullptr;
    max_body_size = -1;
};

int Server::check_line_server(std::string line)
{
    std::vector<std::string> tokens = ft_split(line, ' ');
    
    if(tokens.at(0) == "listen")
        port = atoi(tokens.at(1).c_str());
    else if(tokens.at(0) == "host")
        host = tokens.at(1);
    else if(tokens.at(0) == "client_max_body_size")
        max_body_size = get_body_size(tokens.at(1));
    else if (tokens.at(0) == "erro_page")
        add_error_page(error_pages, tokens);
    else if (tokens.at(0) == "location")
        return (1);
    //TODO: ERROR IF NO KEYWORD IS NOT FOUND
    return (0);
};


Server::Server(int server_pos, std::string config_file)
{
    host = nullptr;
    port = -1;
    server_name = nullptr;
    max_body_size = -1;
    
    std::string line;
    std::string location_string;
    std::ifstream file(config_file);

    while(std::getline(file, line))
    {
        if (line == "server {")
            server_pos--;
        if (server_pos == 0)
            break;
    }
    while(std::getline(file, line, ';'))
    {
        //TODO: IFGORE COMMENTS
        if(check_line_server(line))
        {
            std::getline(file, location_string, '}');
            Locations.push_back(Location(line + location_string));
        }
    }

    file.close();
};

Server::Server(const Server &other)
{
    //TODO
};

Server& Server::operator=(const Server &other)
{
    //TODO
};
 
Server::~Server(){};