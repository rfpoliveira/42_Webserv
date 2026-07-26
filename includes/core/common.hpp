#pragma once

// Standard library — everything the project needs, in one place.
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <cstring>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

// Free functions that do NOT depend on any project type.
// (Anything needing Config lives in main.hpp instead.)

// main parse
int parseConfigFile(std::string configFile);

// utils
std::vector<std::string> ftSplit (std::string s, char delimiter);
void cleanStrings(std::vector<std::string>& buff);
unsigned long getBodySize(std::string s);
int addErrorPage(std::map<int, std::string>& errorPages, std::vector<std::string> tokens);
int validFileCheck(std::string configFile);
int bracketsCount(std::string configFile);
void ignoreComments(std::string& line);
std::string intToString(int value);
void freeEnvp(char** envp);

// debug
void debugVectorPrint(std::vector<std::string> buff);
void debug(int value);
