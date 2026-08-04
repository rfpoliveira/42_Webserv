#pragma once

#include "Location.hpp"

class Location;

class ServerBlock
{
	public:
		std::string root;
		std::string host;
		int port;
		std::string ServerBlockName;
		unsigned long maxBodySize;
		std::map<int, std::string> errorPages;
		std::vector<Location> Locations;

		int checkLineServerBlock(std::string line);

		ServerBlock();
		ServerBlock(int ServerBlockPos, std::string configFile);
		ServerBlock(const ServerBlock& other);
		ServerBlock& operator=(const ServerBlock& other);
		~ServerBlock();
};
