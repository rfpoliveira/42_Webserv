#pragma once

#include "Server.hpp"

class Config
{
	public:
		int numberServers;
		std::vector<Server> servers;

		Location* getLocation(int port, std::string& path);

		Config();
		Config(std::string configFile);
		Config(const Config& other);
		Config& operator=(const Config& other);
		~Config();
};
