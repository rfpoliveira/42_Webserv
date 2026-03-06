/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:35:19 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/03/06 10:38:55 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/main.hpp"

//check if the file exist of can be opened (valid_file_check)
//checks if the brackets are okay

int parse_config_file(std::string config_file)
{
    if (valid_file_check(config_file) < 0)
        return (-1);
    if (brackets_count(config_file) < 0)
        return(-2);
    return (0);
}


