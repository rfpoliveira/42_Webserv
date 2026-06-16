/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:48:19 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/06/16 16:43:05 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"
#include "Server.hpp"

class Config
{
    public:
        int number_servers;
        std::vector<Server> servers;
    
        Location getLocation(int port, std::string path);
        
        Config();
        Config(std::string config_file);
        Config(const Config &other);
        Config &operator=(const Config &other); 
        ~Config();
};