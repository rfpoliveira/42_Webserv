/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 12:44:34 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/02/19 12:46:13 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/main.hpp"

void debug_vector_print(std::vector<std::string> buff)
{
    std::vector<std::string>::iterator it;
    for(it = buff.begin(); it != buff.end(); it++)
        std::cout << *it << "\n";
}

void debug(int value)
{
    std::cout << "debug " << value << "\n";
}