/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:38:45 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/02/11 12:43:24 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <fstream>


//main parse
int parse_config_file(std::string config_file);

//utils
std::vector<std::string> ft_split (std::string s, char delimiter);
unsigned long get_body_size(std::string s);
void add_error_page(std::map<int, std::string>& error_pages, std::vector<std::string> tokens);
int valid_file_check(std::string config_file);
int brackets_count(std::string config_file);