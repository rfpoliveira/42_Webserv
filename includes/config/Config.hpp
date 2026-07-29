#pragma once

#include "ServerBlock.hpp"

class Config
{
	public:
		int numberServerBlocks;
		std::vector<ServerBlock> serverBlocks;

		Location* getLocation(int port, const std::string& path) const;

		Config();
		Config(std::string configFile);
		Config(const Config& other);
		Config& operator=(const Config& other);
		~Config();
};
