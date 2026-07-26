#pragma once

#include "../core/common.hpp"

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

		int checkLineLocation(std::string line);
		bool isMethodallowed(std::string method);

		Location();
		Location(std::string locationStr, unsigned long maxBodySize);
		Location(const Location& other);
		Location& operator=(const Location& other);
		~Location();

	class LocationErrorExeption: public std::exception
	{
		public:
			virtual const char* what() const throw()
			{
				return ("Error: Config file error at Location context");
			}
	};
};
