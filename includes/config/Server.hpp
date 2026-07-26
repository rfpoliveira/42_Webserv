#pragma once

#include "Location.hpp"

class Server
{
	public:
		std::string host;
		int port;
		std::string serverName;
		unsigned long maxBodySize;
		std::map<int, std::string> errorPages;
		std::vector<Location> Locations;

		int checkLineServer(std::string line);

		Server();
		Server(int serverPos, std::string configFile);
		Server(const Server& other);
		Server& operator=(const Server& other);
		~Server();

	class ServerErrorExeption: public std::exception
	{
		public:
			virtual const char* what() const throw()
			{
				return ("Error: Config file error at Server context");
			}
	};
};
