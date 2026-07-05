#pragma once

#include "../core/main.hpp"

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
        std::string upload_path;
        unsigned long max_body_size;

        int check_line_location(std::string line);
        bool isMethodallowed(std::string method);

        Location();
        Location(std::string location_str, unsigned long max_body_size);
        Location(const Location &other);
        Location &operator=(const Location &other);
        ~Location();

	class LocationErrorExeption: public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Config file error at Location context");
			}
	};
};