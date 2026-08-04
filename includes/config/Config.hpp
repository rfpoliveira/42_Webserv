#pragma once

#include "ServerBlock.hpp"

class Config
{
	public:
		int numberServerBlocks;
		std::vector<ServerBlock> ServerBlocks;

		Location* getLocation(int port, std::string& path);

		Config();
		Config(std::string configFile);
		Config(const Config& other);
		Config& operator=(const Config& other);
		~Config();
};
