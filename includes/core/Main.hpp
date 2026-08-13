#pragma once

#include "Common.hpp"
#include "../config/Config.hpp"

// main parse
void parseConfigFile(std::string config_file);

// utils
std::vector<std::string> ft_split (std::string s, char delimiter);
void cleanStrings(std::vector<std::string>& buff);
unsigned long getBodySize(std::string s);
int addErrorPage(std::map<int, std::string>& errorPages, std::vector<std::string> tokens);
int validFileCheck(std::string config_file);
int bracketsCount(std::string config_file);
void ignoreComments(std::string& line);
void parseConfigInfo(Config& configs);
std::string intToString(int value);
void freeEnvp(char** envp);

// debug
void debugVectorPrint(std::vector<std::string> buff);
void debug(int value);

