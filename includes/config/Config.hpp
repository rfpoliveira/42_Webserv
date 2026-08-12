#pragma once

#include "ServerBlock.hpp"

class Config
{
	public:
		int numberServerBlocks;
		std::vector<ServerBlock> serverBlocks;

		const Location* getLocation(int port, std::string& path) const;

		Config();
		Config(std::string configFile);
		Config(const Config& other);
		Config& operator=(const Config& other);
		~Config();
};
