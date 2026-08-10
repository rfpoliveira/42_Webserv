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

int validPathCheck(std::string path)
{
	struct stat sb;

	if (stat(path.c_str(), &sb) != 0) //does not exist
		return (1);

	if (S_ISREG(sb.st_mode)) //its a file
		return (2);
	return (0);
}

int errorPageParse(int code)
{
	if (code < 300 || code > 599)
		return (1);
	return (0);
}


void parseConfigInfo(Config& configs)
{
	std::vector<ServerBlock>::iterator itVec;
	std::vector<Location>::iterator itLoc;
	std::map<int, std::string>::iterator itMap;
	int i = 0;


    for(itVec = configs.serverBlocks.begin(); itVec != configs.serverBlocks.end(); itVec++)
    {
        if ((*itVec).port <= 0)
            throw ConfigException("Invalid port.");
        if (hostParse((*itVec).host))
            throw ConfigException("Invalid host.");
        if ((*itVec).maxBodySize <= 0)
            throw ConfigException("Invalid maxBodySize.");
        for (itMap = (*itVec).errorPages.begin(); itMap != (*itVec).errorPages.end();itMap++)
        {
            if (errorPageParse((*itMap).first))
                throw ConfigException("Invalid error pages.");
        }
        for(itLoc = (*itVec).Locations.begin(); itLoc != (*itVec).Locations.end(); itLoc++)
        {
            if ((*itLoc).POST && validPathCheck((*itLoc).uploadPath))
                throw ConfigException("Needs a valid uploadPath");
			if (validPathCheck((*itLoc).root))
				throw ConfigException("Needs a valid root path");
        }
            i++;
    }
}
