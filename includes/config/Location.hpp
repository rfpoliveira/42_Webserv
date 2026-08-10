#pragma once

#include "../core/common.hpp"
#include "../../includes/config/ServerBlock.hpp"
#include "../../includes/exceptions/ConfigException.hpp"

class ServerBlock;

class Location
{
	public:
		std::string path;
		std::string root;
		bool GET;
		bool POST;
		bool DELETE;
		bool autoindex;
		std::string index;
		std::string redirection;
		std::string uploadPath;
		unsigned long maxBodySize;

		void checkLineLocation(std::string line);
		int isMethodallowed(std::string method);
		void applyServerBlockDefaults(const ServerBlock& ServerBlock);

		Location();
		Location(std::string locationStr);
		Location(const Location& other);
		Location& operator=(const Location& other);
		~Location();
};
