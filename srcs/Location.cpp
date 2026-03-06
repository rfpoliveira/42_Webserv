/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 11:45:01 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/03/06 11:23:33 by rpedrosa         ###   ########.fr       */
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

//check for a keyword and stores the information associated in the right place in the class

int Location::check_line_location(std::string line)
{
    ignore_comments(line);

    std::vector<std::string> tokens = ft_split(line, ' ');
    int i = 0;

    int size = tokens.size();

    if (size == 0)
        return (0);
    if (size < 2)
        return (0);

    if(tokens.at(i) == "location")
        path = tokens.at(i + 1);
    else if(tokens.at(i) == "root")
        root = tokens.at(i + 1);
    else if(tokens.at(i) == "allow_methods")
    {
        while(size > 0)
        {
            if (tokens.at(i) == "GET")
                GET = true;
            if (tokens.at(i) == "POST")
                POST = true;
            if (tokens.at(i) == "DELETE")
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
            return (3);
    }
    else if (tokens.at(i) == "return")
    {
        if (tokens.size() < 3)
            return (4);
        redirection = tokens.at(i + 2);
    }
    else if (tokens.at(i) == "upload_pass")
        upload_path = tokens.at(i + 1);
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

    //TODO: check for defaults

    std::istringstream iss(location_str);
    std::string line;
    int error = 0;

    while(getline(iss, line))
    {
        error = check_line_location(line);
        if (error != 0)
        {
            std::cout << "Error: " << error << "\n";
            throw (LocationErrorExeption());
        }
    }
};

Location::Location(const Location &other)
{
    this->path = other.path;
    this->root = other.root;
    this->GET = other.GET;
    this->POST = other.POST;
    this->DELETE = other.DELETE;
    this->autoindex = other.autoindex;
    this->index = other.index;
    this->redirection = other.redirection;
    this->upload_path = other.upload_path;
};

Location& Location::operator=(const Location &other)
{
    Location copy(other);

    Location & ret = copy;
    return(ret);
};
 
Location::~Location(){};