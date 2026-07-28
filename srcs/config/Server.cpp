#include "../../includes/config/Server.hpp"


Server::Server()
{
	root = "/var/www/html";
	host = "0.0.0.0";
	port = 8080;
	serverName = "";
	maxBodySize = 1048576;
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
	else if(tokens.at(0) == "root")
	{
		if (!root.empty())
			throw ConfigException("Multiple root declaration at a single Server");
		root = tokens.at(1);
	}
	else if(tokens.at(0) == "client_maxBodySize")
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
	root = "/var/www/html";
	host = "0.0.0.0";
	port = 8080;
	serverName = "";
	maxBodySize = 1048576;

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
			Locations.push_back(Location(line + locationString));
		}
		else if (ret == 2)
			break ;
		line.clear();
	}

	file.close();

	std::vector<Location>::iterator it;
	for (it = Locations.begin(); it!= Locations.end(); it++)
		(*it).applyServerDefaults(*this);
};

Server::Server(const Server& other)
{
	this->root = other.root;
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
		this->root = other.root;
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
