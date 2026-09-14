#include "../../includes/config/Config.hpp"
#include "../../includes/utils/Utils.hpp"
#include "../../includes/config/ConfigParser.hpp"
#include "../../includes/config/ServerBlock.hpp"
#include "../../includes/exceptions/ConfigException.hpp"
#include <cstddef>

Config::Config(): numberServerBlocks(0) {};

//counts the number ofserverBlocks and as it finds 1
//calls the constructor of the ServerBlock class as it hads it to theserverBlocks vector in this calss

Config::Config(std::string configFile)
{
	std::string line;
	std::ifstream file(configFile.c_str());
	numberServerBlocks = 0;
	int depth = 0;
	bool insideHttp = false;
	bool httpSeen = false;
	int httpDepth = -1;
	std::string pendingHeader; // block keyword seen, waiting for its '{' (allows brace on next line)

	while(std::getline(file, line))
	{
		std::string probe = line;
		ignoreComments(probe);
		std::vector<std::string> tokens = ftSplit(probe, ' ');
		cleanStrings(tokens);

		// A block keyword may sit on its own line, with '{' on a following line.
		// Remember it as pending; it is validated when its '{' is actually consumed.
		if (!tokens.empty())
		{
			std::string first = tokens.at(0);
			if (first == "http" || first == "http{")
				pendingHeader = "http";
			else if (first == "events" || first == "events{")
				pendingHeader = "events";
			else if (first == "server" || first == "server{")
				pendingHeader = "server";
			else if (first == "{")
				; // pure brace line: keep whatever header is pending
			else if (probe.find('{') != std::string::npos && depth <= (insideHttp ? httpDepth + 1 : 0))
			{
				// an unknown block opening at top level or directly inside http
				if (depth == 0)
					throw ConfigException("Unknown top-level block: " + first);
				if (insideHttp && depth == httpDepth + 1)
					throw ConfigException("Unknown block inside 'http': " + first);
			}
		}

		// Consume braces on this line one at a time so a pending header is
		// validated exactly when its opening '{' appears.
		for (std::string::iterator it = line.begin(); it != line.end(); ++it)
		{
			if (*it == '{')
			{
				if (pendingHeader == "http")
				{
					if (depth != 0)
						throw ConfigException("'http' block must be at the top level");
					if (httpSeen)
						throw ConfigException("Only one 'http' block is allowed");
					httpSeen = true;
					insideHttp = true;
					httpDepth = depth;
				}
				else if (pendingHeader == "events")
				{
					if (depth != 0)
						throw ConfigException("'events' block must be at the top level");
					// events is optional and ignored; its contents are not parsed
				}
				else if (pendingHeader == "server")
				{
					if (!insideHttp || depth != httpDepth + 1)
						throw ConfigException("'server' block is only allowed directly inside the 'http' block");
					this->numberServerBlocks++;
					this->serverBlocks.push_back(ServerBlock(this->numberServerBlocks, configFile));
				}
				pendingHeader.clear();
				depth++;
			}
			else if (*it == '}' && depth > 0)
			{
				depth--;
				if (insideHttp && depth == httpDepth)
					insideHttp = false;
			}
		}
	}
	file.close();

	if (!httpSeen)
		throw ConfigException("No 'http' block found in config file");
	if (this->numberServerBlocks == 0)
		throw ConfigException("No server block found in config file");

	std::vector<ServerBlock>::iterator it;
	std::vector<ServerBlock>::iterator it2;
	int port_to_compare;

	for(it = serverBlocks.begin(); it != serverBlocks.end(); it++)
	{
		port_to_compare = (it->port);
		for(it2 = it + 1; it2 != serverBlocks.end(); it2++)
		{
			if (port_to_compare == (it2->port))
				throw ConfigException("Multiple Servers on the same port");
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

const ServerBlock* Config::getServerBlock(int port) const
{
	std::vector<ServerBlock>::const_iterator it;

    for(it = this->serverBlocks.begin(); it != this->serverBlocks.end(); it++)
	{
        if ((*it).port == port)
			return(&(*it));
	}
	return (NULL);
}

const Location* Config::getLocation(int port, std::string& path) const
{
	std::vector<Location>::const_iterator it2;

	const ServerBlock* targetServerBlock = getServerBlock(port);

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
