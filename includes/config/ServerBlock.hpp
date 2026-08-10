#pragma once

#include "Location.hpp"


class Location;

class ServerBlock
{
	public:
		std::string host;
		std::string root;
		int port;
		std::string serverBlockName;
		unsigned long maxBodySize;
		std::map<int, std::string> errorPages;
		std::vector<Location> Locations;

		int checkLineServerBlock(std::string line);

		ServerBlock();
		ServerBlock(int serverPos, std::string configFile);
		ServerBlock(const ServerBlock& other);
		ServerBlock& operator=(const ServerBlock& other);
		~ServerBlock();
};
