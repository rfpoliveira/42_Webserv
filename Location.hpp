/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:49:16 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/02/11 12:58:37 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <main.hpp>

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

        int check_line_location(std::string line);

        Location();
        Location(std::string location_str);
        Location(const Location &other);
        Location &operator=(const Location &other); 
        ~Location();
};