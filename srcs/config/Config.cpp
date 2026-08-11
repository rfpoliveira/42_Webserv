#include "../../includes/config/Config.hpp"
#include "../../includes/config/ServerBlock.hpp"
#include "../../includes/exceptions/ConfigException.hpp"
#include <cstddef>

Config::Config(): numberServerBlocks(0) {};

//counts the number of ServerBlocks and as it finds 1
//calls the constructor of the ServerBlock class as it hads it to the ServerBlocks vector in this calss

Config::Config(std::string configFile)
{
	std::string line;
	std::ifstream file(configFile.c_str());
	numberServerBlocks = 0;

	while(std::getline(file, line))
	{
		if (line == "ServerBlock {")
		{
			this->numberServerBlocks++;
			this->serverBlocks.push_back(ServerBlock(this->numberServerBlocks, configFile));
		}
	}
	file.close();

	std::vector<ServerBlock>::iterator it;
	std::vector<ServerBlock>::iterator it2;
	int port_to_compare;

	for(it = serverBlocks.begin(); it != serverBlocks.end(); it++)
	{
		port_to_compare = (it->port);
		for(it2 = it + 1; it2 != serverBlocks.end(); it2++)
		{
			if (port_to_compare == (it2->port))
				throw ConfigException("Multiple ServerBlocks on the same port");
		}
	}
};

Config::Config(const Config& other)
{
	this->numberServerBlocks = other.numberServerBlocks;
	this->serverBlocks = other.serverBlocks;
};

Config& Config::operator=(const Config& other)
{
	if (this != &other)
	{
		this->numberServerBlocks = other.numberServerBlocks;
		this->serverBlocks = other.serverBlocks;
	}
	return (*this);
};

const Location* Config::getLocation(int port, std::string& path) const
{
	std::vector<ServerBlock>::const_iterator it;
	std::vector<Location>::const_iterator it2;

	const ServerBlock* targetServerBlock = NULL;

    for(it = this->serverBlocks.begin(); it != this->serverBlocks.end(); it++)
    {
        if ((*it).port == port)
		{
			targetServerBlock = &(*it);
			break ; //found the correct ServerBlock
		}
    }
	if (targetServerBlock == NULL) //DID NOT FOUND THE ServerBlock
		return(NULL);
	
	const Location *bestMatch = NULL;
	size_t longestMatch = 0;

	for (it2 = targetServerBlock->Locations.begin(); it2 != targetServerBlock->Locations.end(); it2++) //iterate the location of said server
	{
		std::string locationPath = it2->path;
		if(path.find(locationPath) == 0) //if found the path in the location path
		{
			if (locationPath.length() > longestMatch)
			{
				longestMatch = locationPath.length(); //how much of the path did it found
				bestMatch = &(*it2); //currentyl the best match
			}
		}
	}
	return (bestMatch); //will return the best match of null if none is found
};

Config::~Config(){};
