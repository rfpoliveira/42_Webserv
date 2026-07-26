#include "../../includes/config/Server.hpp"

Server::Server()
{
	host = "127.0.0.1";
	port = 80;
	serverName = "server1.com";
	maxBodySize = 1000000;
};

//check for a keyword and stores the information associated in the right place in the class

int Server::checkLineServer(std::string line)
{
	ignoreComments(line);

	std::vector<std::string> tokens = ftSplit(line, ' ');
	cleanStrings(tokens);

	if(tokens.at(0) == "server")
		return (2);

	if(tokens.at(0) == "listen")
		port = atoi(tokens.at(1).c_str());
	else if (tokens.at(0) == "server_name")
		serverName = tokens.at(1);
	else if(tokens.at(0) == "host")
		host = tokens.at(1);
	else if(tokens.at(0) == "client_max_body_size")
		maxBodySize = getBodySize(tokens.at(1));
	else if (tokens.at(0) == "error_page")
	{
		if (addErrorPage(errorPages, tokens) != 0)
			throw (ServerErrorExeption());
	}
	else if (tokens.at(0) == "location")
		return (1);
	else
		return (2);
	return (0);
};

//searches for the right server information for each one in the config file
//goes line by line checking for the keyword
//if a location is found, calls its construtor and saves it in the Locations vector

Server::Server(int serverPos, std::string configFile)
{
	host = "127.0.0.1";
	port = 80;
	serverName = "server1.com";
	maxBodySize = 1000000;

	std::string line;
	std::string locationString;
	std::ifstream file(configFile.c_str());

	while(std::getline(file, line))
	{
		if (line == "server {")
			serverPos--;
		if (serverPos == 0)
			break;
		line.clear();
	}

	int ret = 0;

	while(std::getline(file, line, ';'))
	{
		ret = checkLineServer(line);
		if (ret == 1)
		{
			std::getline(file, locationString, '}');
			Locations.push_back(Location(line + locationString, this->maxBodySize));
		}
		else if (ret == 2)
			break ;
		line.clear();
	}

	file.close();
};

Server::Server(const Server& other)
{
	this->host = other.host;
	this->port = other.port;
	this->serverName = other.serverName;
	this->maxBodySize = other.maxBodySize;
	this->errorPages = other.errorPages;
	this->Locations = other.Locations;
};

Server& Server::operator=(const Server& other)
{
	if (this != &other)
	{
		this->host = other.host;
		this->port = other.port;
		this->serverName = other.serverName;
		this->maxBodySize = other.maxBodySize;
		this->errorPages = other.errorPages;
		this->Locations = other.Locations;
	}
	return (*this);
};

Server::~Server(){};
