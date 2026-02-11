/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:35:19 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/02/11 12:44:56 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.hpp"

int parse_config_file(std::string config_file)
{
    if (valid_file_check(config_file) < 0)
        return (-1);
    if (brackets_count(config_file) < 0)
        return(-2);
};


