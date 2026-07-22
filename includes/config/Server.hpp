#pragma once

#include "Location.hpp"

class Server
{
	public:
		std::string host;
		int port;
		std::string server_name;
		unsigned long max_body_size;
		std::map<int, std::string> error_pages;
		std::vector<Location> Locations;

		int check_line_server(std::string line);

		Server();
		Server(int server_pos, std::string config_file);
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server();

	class ServerErrorExeption: public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Config file error at Server context");
			}
	};
};
