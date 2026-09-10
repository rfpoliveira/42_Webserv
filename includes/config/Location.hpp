#pragma once

#include <Common.hpp>

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
		std::string redirectionFolder;
		int redirectionCode;
		std::string uploadPath;
		unsigned long maxBodySize;
		std::map<int, std::string> errorPages;

		void checkLineLocation(std::string line);
		bool isMethodallowed(std::string method) const;
		void applyServerBlockDefaults(const ServerBlock& ServerBlock);

		Location();
		Location(std::string locationStr);
		Location(const Location& other);
		Location& operator=(const Location& other);
		~Location();
};
