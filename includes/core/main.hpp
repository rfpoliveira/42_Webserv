#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

#include "../config/Config.hpp"

// main parse
int parse_config_file(std::string config_file);

// utils
std::vector<std::string> ft_split (std::string s, char delimiter);
void clean_strings(std::vector<std::string>& buff);
unsigned long get_body_size(std::string s);
int add_error_page(std::map<int, std::string>& error_pages, std::vector<std::string> tokens);
int valid_file_check(std::string config_file);
int brackets_count(std::string config_file);
void ignore_comments(std::string& line);
int parse_config_info(Config& configs);
std::string intToString(int value);
void freeEnvp(char** envp);

// debug
void debug_vector_print(std::vector<std::string> buff);
void debug(int value);
