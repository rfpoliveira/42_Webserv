#pragma once

#include "../core/main.hpp"
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