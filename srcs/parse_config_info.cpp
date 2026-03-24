/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config_info.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 11:24:36 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/03/24 12:14:42 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/main.hpp"

int host_parse(std::string& host)
{
    if (host == "localhost")
    {
        host = "127.0.0.1";
        return (0);
    }
    
    std::vector<std::string> tokens = ft_split(host, '.');
    if (tokens.size() != 4)
        return (1);
    
    std::vector<std::string>::iterator it;
    std::string::iterator str_it;
    int tmp = 0;
    
    for(it = tokens.begin(); it != tokens.end(); it++)
    {
        for(str_it = (*it).begin(); str_it != (*it).end(); str_it++)
        {
            if (!std::isdigit((*str_it)))
                return (1);
        }
        tmp = std::atoi((*it).c_str());
        if (tmp < 0 || tmp > 255)
            return (1);
    }
    return (0);
}

int error_page_parse(int code, std::string path)
{
    if (code < 300 || code > 599 || path[0] != '/')
        return (1);
    return (0);
}

int valid_path_check(std::string path)
{
    struct stat sb;
    
    if (stat(path.c_str(), &sb))
        return (1);
    return (0);
}

int parse_config_info(Config& configs) //TODO: CHECK IF ITS WORKING PROPERLY (WORK IN PROGRESS)
{
    std::vector<Server>::iterator it_vec;
    std::vector<Location>::iterator it_loc;
    std::map<int, std::string>::iterator it_map;
    int i = 0;


    for(it_vec = configs.servers.begin(); it_vec != configs.servers.end(); it_vec++)
    {
        if ((*it_vec).port <= 0)
            return (-3);
        if (host_parse((*it_vec).host))
            return (-4);
        if ((*it_vec).max_body_size <= 0)
            return (-5);
        for (it_map = (*it_vec).error_pages.begin(); it_map != (*it_vec).error_pages.end();it_map++)
        {
            if (error_page_parse((*it_map).first, (*it_map).second))
                return (-6);
        }
        for(it_loc = (*it_vec).Locations.begin(); it_loc != (*it_vec).Locations.end(); it_loc++)
        {
            if (valid_path_check((*it_loc).path) || valid_path_check((*it_loc).root))
                return (-7);
        }
            i++;
    }

    //TODO:FUNCTION THAT CHECK IF THERE ARE 2 SERVERS WITH THE SAME PORT

    return (0);

}