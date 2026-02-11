/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:53:23 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/02/11 11:44:44 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Config.hpp>

Config::Config(): number_servers(0) {};

Config::Config(std::string config_file)
{
    std::string line;
    std::ifstream file(config_file);
    number_servers = 0;

    //TODO CHECK NUMBER OF BRACKETS
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
    std::vector<Server>::iterator it;

    for(it = other.servers.begin(); it != other.servers.end(); it++)
        this->servers.push_back(Server(*it));
};

Config& Config::operator=(const Config &other)
{
    Config copy(other);
    return(copy);
}; 

Config::~Config(){};