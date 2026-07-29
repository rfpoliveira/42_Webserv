#include "../../includes/config/Config.hpp"
#include "../../includes/config/ServerBlock.hpp"

Config::Config(): numberServerBlocks(0) {};

//counts the number of servers and as it finds 1
//calls the constructor of the Server class as it hads it to the Servers vector in this calss

Config::Config(std::string configFile)
{
	std::string line;
	std::ifstream file(configFile.c_str());
	numberServerBlocks = 0;

	while(std::getline(file, line))
	{
		if (line == "server {")
		{
			this->numberServerBlocks++;
			this->serverBlocks.push_back(ServerBlock(this->numberServerBlocks, configFile));
		}
	}
	file.close();
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

Location* Config::getLocation(int port, const std::string& path) const
{
	Location*	best = NULL;
	size_t		bestLen = 0;

	for (size_t i = 0; i < serverBlocks.size(); i++)
	{
		if (serverBlocks[i].port != port)
			continue;
		for (size_t j = 0; j < serverBlocks[i].Locations.size(); j++)
		{
			const std::string& lp = serverBlocks[i].Locations[j].path;
			if (path.compare(0, lp.size(), lp) == 0 && lp.size() >= bestLen)
			{
				best = const_cast<Location*>(&serverBlocks[i].Locations[j]);
				bestLen = lp.size();
			}
		}
	}
	return (best);
}

Config::~Config(){};
