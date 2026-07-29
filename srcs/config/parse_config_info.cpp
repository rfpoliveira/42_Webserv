#include "../../includes/core/main.hpp"

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

int parseConfigInfo(Config& configs) //TODO: CHECK IF ITS WORKING PROPERLY (WORK IN PROGRESS)
{
	std::vector<ServerBlock>::iterator itVec;
	std::vector<Location>::iterator itLoc;
	std::map<int, std::string>::iterator itMap;
	int i = 0;


	for(itVec = configs.serverBlocks.begin(); itVec != configs.serverBlocks.end(); itVec++)
	{
		if ((*itVec).port <= 0)
			return (-3);
		if (hostParse((*itVec).host))
			return (-4);
		if ((*itVec).maxBodySize <= 0)
			return (-5);
		for (itMap = (*itVec).errorPages.begin(); itMap != (*itVec).errorPages.end();itMap++)
		{
			if (errorPageParse((*itMap).first, (*itMap).second))
				return (-6);
		}
		for(itLoc = (*itVec).Locations.begin(); itLoc != (*itVec).Locations.end(); itLoc++)
		{
			if (validPathCheck((*itLoc).path) || validPathCheck((*itLoc).root))
				return (-7);
		}
			i++;
	}
	(void)i;
	//TODO:FUNCTION THAT CHECK IF THERE ARE 2 SERVERS WITH THE SAME PORT

	return (0);

}
