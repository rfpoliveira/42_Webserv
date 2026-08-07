#pragma once

#include "../../includes/config/Location.hpp"
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

	class ServerBlockErrorExeption: public std::exception
	{
		public:
			virtual const char* what() const throw()
			{
				return ("Error: Config file error at Server context");
			}
	};
};
