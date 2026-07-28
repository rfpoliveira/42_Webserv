#include "../../includes/config/Config.hpp"
#include "../../includes/config/Server.hpp"
#include "../../includes/exceptions/ConfigException.hpp"

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

	std::vector<Server>::iterator it;	
	std::vector<Server>::iterator it2;
	int port_to_compare;

	for(it = servers.begin(); it != servers.end(); it++)
	{
		port_to_compare = (it->port);
		for(it2 = it + 1; it2 != servers.end(); it2++)
		{
			if (port_to_compare == (it2->port))
				throw ConfigException("Multiple Servers on the same port");
		}
	}
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

Location* Config::getLocation(int port, std::string& path)
{
	std::vector<Server>::iterator it;
	std::vector<Location>::iterator it2;

    for(it = this->servers.begin(); it != this->servers.end(); it++)
    {
        if ((*it).port == port)
        {
            for (it2 = (*it).Locations.begin(); it2 != (*it).Locations.end(); it2++)
            {
                if ((*it2).path == path)
                    return(&(*it2));
            }
        }
    }

    throw ConfigException("Invalid Location Settings"); 
};

Config::~Config(){};
