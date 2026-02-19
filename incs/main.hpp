/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:38:45 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/02/19 12:45:44 by rpedrosa         ###   ########.fr       */
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
void ignore_comments(std::string& line);

//debug
void debug_vector_print(std::vector<std::string> buff);
void debug(int value);
