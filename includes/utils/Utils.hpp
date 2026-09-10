#pragma once

#include <string>
#include <vector>

std::vector<std::string> ftSplit(std::string s, char delimiter);
void cleanStrings(std::vector<std::string>& buff);
void ignoreComments(std::string& line);
std::string intToString(int value);
