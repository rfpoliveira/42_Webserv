#include "../../includes/config/Config.hpp"
#include "../../includes/config/Server.hpp"

Config::Config(): number_servers(0) {};

//counts the number of servers and as it finds 1
//calls the constructor of the Server class as it hads it to the Servers vector in this calss

Config::Config(std::string config_file)
{
	std::string line;
	std::ifstream file(config_file.c_str());
	number_servers = 0;

	while(std::getline(file, line))
	{
		if (line == "server {")
		{
			this->number_servers++;
			this->servers.push_back(Server(this->number_servers, config_file));
		}
	}
	file.close();
};

Config::Config(const Config &other)
{
	this->number_servers = other.number_servers;
	this->servers = other.servers;
};

Config& Config::operator=(const Config &other)
{
	if (this != &other)
	{
		this->number_servers = other.number_servers;
		this->servers = other.servers;
	}
	return (*this);
};

Location* Config::getLocation(int port, std::string& path)
{
	Location*	best = NULL;
	size_t		best_len = 0;

	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i].port != port)
			continue;
		for (size_t j = 0; j < servers[i].Locations.size(); j++)
		{
			const std::string& lp = servers[i].Locations[j].path;
			if (path.compare(0, lp.size(), lp) == 0 && lp.size() >= best_len)
			{
				best = &servers[i].Locations[j];
				best_len = lp.size();
			}
		}
	}
	return (best);
}

Config::~Config(){};
