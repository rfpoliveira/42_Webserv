#include "../../includes/core/main.hpp"
#include "../../includes/exceptions/ConfigException.hpp"

int hostParse(std::string& host)
{
	if (host == "localhost")
	{
		host = "127.0.0.1";
		return (0);
	}

	std::vector<std::string> tokens = ftSplit(host, '.');
	if (tokens.size() != 4)
		return (1);

	std::vector<std::string>::iterator it;
	std::string::iterator strIt;
	int tmp = 0;

	for(it = tokens.begin(); it != tokens.end(); it++)
	{
		for(strIt = (*it).begin(); strIt != (*it).end(); strIt++)
		{
			if (!std::isdigit((*strIt)))
				return (1);
		}
		tmp = std::atoi((*it).c_str());
		if (tmp < 0 || tmp > 255)
			return (1);
	}
	return (0);
}

int errorPageParse(int code, std::string path)
{
	if (code < 300 || code > 599 || path[0] != '/')
		return (1);
	return (0);
}

int validPathCheck(std::string path)
{
	struct stat sb;

	if (stat(path.c_str(), &sb))
		return (1);
	return (0);
}

void parse_config_info(Config& configs) //TODO: CHECK IF ITS WORKING PROPERLY (WORK IN PROGRESS)
{
	std::vector<Server>::iterator itVec;
	std::vector<Location>::iterator itLoc;
	std::map<int, std::string>::iterator itMap;
	int i = 0;


    for(it_vec = configs.servers.begin(); it_vec != configs.servers.end(); it_vec++)
    {
        if ((*it_vec).port <= 0)
            throw ConfigException("Invalid port.");
        if (host_parse((*it_vec).host))
            throw ConfigException("Invalid host.");
        if ((*it_vec).max_body_size <= 0)
            throw ConfigException("Invalid max_body_size.");
        for (it_map = (*it_vec).error_pages.begin(); it_map != (*it_vec).error_pages.end();it_map++)
        {
            if (error_page_parse((*it_map).first, (*it_map).second))
                throw ConfigException("Invalid error pages.");
        }
        for(it_loc = (*it_vec).Locations.begin(); it_loc != (*it_vec).Locations.end(); it_loc++)
        {
            if (valid_path_check((*it_loc).path) || valid_path_check((*it_loc).root))
                throw ConfigException("Invalid paths.");
        }
            i++;
    }
}
