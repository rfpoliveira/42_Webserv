/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_utils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 15:24:50 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/06/17 16:44:44 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/main.hpp"

std::string intToString(int value) 
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void freeEnvp(char** envp) 
{
    if (!envp)
        return;
    for (size_t i = 0; envp[i] != NULL; i++)
        delete[] envp[i];
    delete[] envp;
}