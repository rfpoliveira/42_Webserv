/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 11:45:01 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/02/19 12:25:40 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Location.hpp"

Location::Location()
{
    path = "";
    root = "";
    GET = false;
    POST = false;
    DELETE = false;
    autoindex = false;
    redirection = "";
    upload_path = "";
    index = "";
};

int Location::check_line_location(std::string line)
{
    ignore_comments(line);
    
    std::vector<std::string> tokens = ft_split(line, ' ');

    int i = 0;
    int size = tokens.size();
    if (size < 2)
        return (1); //TODO ERROR
    
    if(tokens.at(i) == "location")
    {
        path = tokens.at(i + 1);
        if (size < 4)
            return (1); //TODO ERROR
        i+= 2;
    }
    if(tokens.at(i) == "root")
        root = tokens.at(i + 1);
    else if(tokens.at(i) == "allow_methods")
    {
        while(size > 0)
        {
            if (tokens.at(i + 1) == "GET")
                GET = true;
            if (tokens.at(i + 1) == "POST")
                POST = true;
            if (tokens.at(i + 1) == "DELETE")
                DELETE = true;
            i++;
            size--;
        }
    }
    else if(tokens.at(i) == "index")
        index = tokens.at(i + 1);
    else if(tokens.at(i) == "autoindex")
    {
        if (tokens.at(i + 1) == "off")
            autoindex = false;
        else if (tokens.at(i + 1) == "on")
            autoindex = true;
        else
            //TODO ERROR
            return (1);
    }
    else if (tokens.at(i) == "return")
    {
        if (tokens.size() < 3)
            return (1); //TODO ERROR
        redirection = tokens.at(i + 2);
    }
    else if (tokens.at(i) == "upload_pass")
        upload_path = tokens.at(i + 1);

    //TODO: ERROR IF NO KEYWORD IS NOT FOUND
    return (0);
};
Location::Location(std::string location_str)
{
    path = "";
    root = "";
    GET = false;
    POST = false;
    DELETE = false;
    autoindex = false;
    redirection = "";
    upload_path = "";
    index = "";

    std::istringstream iss(location_str);
    std::string line;

    //TODO handle comments
    while(getline(iss, line, ';'))
    {
        check_line_location(line);
        //ERROR CHECKING
    }
};

/* Location::Location(const Location &other)
{
    //TODO
    other = nullptr;
};

Location& Location::operator=(const Location &other)
{
    //TODO
}; */
 
Location::~Location(){};