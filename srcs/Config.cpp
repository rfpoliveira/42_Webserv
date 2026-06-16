/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:53:23 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/06/16 16:45:42 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Config.hpp"
#include "../incs/Server.hpp"

Config::Config(): number_servers(0) {};

//counts the number of servers and as it finds 1
//calls the constructor of the Server class as it hads it to the Servers vector in this calss

Config::Config(std::string config_file)
{
    std::string line;
    std::ifstream file(config_file.c_str());
    number_servers = 0;

    while(std::getline(file, line))
    {
        if (line == "server {")
        {
            this->number_servers++;
            this->servers.push_back(Server(this->number_servers, config_file));
        }
    }
    file.close();
};

Config::Config(const Config &other)
{
    this->number_servers = other.number_servers;
    this->servers = other.servers;
};

Config& Config::operator=(const Config &other)
{
    Config copy(other);

    Config & ret = copy;
    return(ret);
};

Location Config::getLocation(int port, std::string path)
{
    std::vector<Server>::iterator it;
    std::vector<Location>::iterator it2;

    for(it = this->servers.begin(); it != this->servers.end(); it++)
    {
        if ((*it).port == port)
        {
            for (it2 = (*it).Locations.begin(); it2 != (*it).Locations.end(); it2++)
            {
                if ((*it2).path == path)
                    return((*it2));
            }
        }
    }
    
    //TODO ERROR NO WRONG INFO
};


Config::~Config(){};