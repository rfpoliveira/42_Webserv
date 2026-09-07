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
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>      // std::remove
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
#include <exception>
#include <ctime>
#include <poll.h>
#include <cstddef>
#include <algorithm>
#include <cctype>
#include <dirent.h>
#include <sys/socket.h>
#include <netdb.h>
#include <set>

#include <ServerBlock.hpp>
#include <Location.hpp>
#include <Config.hpp>
#include <Request.hpp>
#include <Client.hpp>
#include <HttpException.hpp>
#include <ConfigException.hpp>
#include <ServerException.hpp>
#include <Response.hpp>
#include <MimeTypes.hpp>
#include <CgiHandler.hpp>
#include <CgiSession.hpp>
#include <HandlerOutcome.hpp>
#include <RequestHandler.hpp>
#include <Server.hpp>

// Free functions that do NOT depend on any project type.
// (Anything needing Config lives in main.hpp instead.)

//shutdown handling
extern volatile sig_atomic_t g_shutdownRequested;
void handleShutDownSignal(int);

// main parse
void parseConfigFile(std::string config_file);

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
void parseConfigInfo(Config& configs);

// debug
void printConfigs(Config& configs);
void debugVectorPrint(std::vector<std::string> buff);
void debug(int value);

