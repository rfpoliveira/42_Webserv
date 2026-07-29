#pragma once

#include "Location.hpp"

class ServerBlock
{
	public:
		std::string host;
		int port;
		std::string serverName;
		unsigned long maxBodySize;
		std::map<int, std::string> errorPages;
		std::vector<Location> Locations;

		int checkLineServer(std::string line);

		ServerBlock();
		ServerBlock(int serverPos, std::string configFile);
		ServerBlock(const ServerBlock& other);
		ServerBlock& operator=(const ServerBlock& other);
		~ServerBlock();

	class ServerBlockErrorExeption: public std::exception
	{
		public:
			virtual const char* what() const throw()
			{
				return ("Error: Config file error at Server context");
			}
	};
};
