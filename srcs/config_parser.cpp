/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:35:19 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/02/19 10:58:22 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/main.hpp"

int parse_config_file(std::string config_file)
{
    if (valid_file_check(config_file) < 0)
        return (-1);
    if (brackets_count(config_file) < 0)
        return(-2);
    return (0);
}


