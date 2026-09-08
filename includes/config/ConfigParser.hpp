#pragma once

#include <string>
#include <vector>
#include <map>
#include "Config.hpp"

void parseConfigFile(std::string configFile);
void parseConfigInfo(Config& configs);

int validFileCheck(std::string configFile);
int bracketsCount(std::string configFile);
bool isBlockHeader(std::string line, const std::string& blockName);
unsigned long getBodySize(std::string s);
int addErrorPage(std::map<int, std::string>& errorPages, std::vector<std::string> tokens);
