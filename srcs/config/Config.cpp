#include "../../includes/config/Config.hpp"
#include "../../includes/config/Server.hpp"

Config::Config(): numberServers(0) {};

//counts the number of servers and as it finds 1
//calls the constructor of the Server class as it hads it to the Servers vector in this calss

Config::Config(std::string configFile)
{
	std::string line;
	std::ifstream file(configFile.c_str());
	numberServers = 0;

	while(std::getline(file, line))
	{
		if (line == "server {")
		{
			this->numberServers++;
			this->servers.push_back(Server(this->numberServers, configFile));
		}
	}
	file.close();
};

Config::Config(const Config& other)
{
	this->numberServers = other.numberServers;
	this->servers = other.servers;
};

Config& Config::operator=(const Config& other)
{
	if (this != &other)
	{
		this->numberServers = other.numberServers;
		this->servers = other.servers;
	}
	return (*this);
};

Location* Config::getLocation(int port, const std::string& path) const
{
	Location*	best = NULL;
	size_t		bestLen = 0;

	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i].port != port)
			continue;
		for (size_t j = 0; j < servers[i].Locations.size(); j++)
		{
			const std::string& lp = servers[i].Locations[j].path;
			if (path.compare(0, lp.size(), lp) == 0 && lp.size() >= bestLen)
			{
				best = const_cast<Location*>(&servers[i].Locations[j]);
				bestLen = lp.size();
			}
		}
	}
	return (best);
}

Config::~Config(){};
