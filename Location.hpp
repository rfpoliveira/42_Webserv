/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:49:16 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/02/10 10:49:31 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <main.hpp>

class Location
{
    private:
        std::string path;
        std::string root;
        bool GET;
        bool POST;
        bool DELETE;
        bool autoindex;
        std::string redirection;
        std::string upload_path;

    public:
        Location();
        Location(const Location &other);
        Location &operator=(const Location &other); 
        ~Location();
};