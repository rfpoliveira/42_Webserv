#pragma once

#include "../core/Common.hpp"
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
		bool isMethodallowed(std::string method) const;
		void applyServerBlockDefaults(const ServerBlock& ServerBlock);

		Location();
		Location(std::string locationStr);
		Location(const Location& other);
		Location& operator=(const Location& other);
		~Location();
};
