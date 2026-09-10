#include <Common.hpp>

void debugVectorPrint(std::vector<std::string> buff)
{
	std::vector<std::string>::iterator it;
	for(it = buff.begin(); it != buff.end(); it++)
		std::cout << *it << "\n";
}

void debug(int value)
{
	std::cout << "debug " << value << "\n";
}
volatile sig_atomic_t g_shutdownRequested = 0;

void handleShutDownSignal(int)
{
	g_shutdownRequested = 1;
}

void printConfigs(Config& configs)
{
	std::vector<ServerBlock>::iterator itVec;
	std::vector<Location>::iterator itLoc;
	std::map<int, std::string>::iterator itMap;

	int i = 0;

	std::cout << "Number of ServerBlocks: " << configs.numberServerBlocks << "\n";
	for(itVec = configs.serverBlocks.begin(); itVec != configs.serverBlocks.end(); itVec++)
	{
		std::cout << "ServerBlock " << i << ": \n";
		std::cout << "host: " << (*itVec).host << "\n";
		std::cout << "port: " << (*itVec).port << "\n";
		std::cout << "ServerBlock_name: " << (*itVec).serverBlockName << "\n";
		std::cout << "maxBodySize: " << (*itVec).maxBodySize << "\n";
		for (itMap = (*itVec).errorPages.begin(); itMap != (*itVec).errorPages.end();itMap++)
			std::cout << "error page: " << (*itMap).first << ", " << (*itMap).second << "\n";
		for(itLoc = (*itVec).Locations.begin(); itLoc != (*itVec).Locations.end(); itLoc++)
		{
			std::cout << "location: \n";
			std::cout << "path: " << (*itLoc).path << "\n";
			std::cout << "root: " << (*itLoc).root << "\n";
			std::cout << "GET? " << (*itLoc).GET << "\n";
			std::cout << "POST? " << (*itLoc).POST << "\n";
			std::cout << "DELETE? " << (*itLoc).DELETE << "\n";
			std::cout << "autofixer: " << (*itLoc).autoindex << "\n";
			std::cout << "index: " << (*itLoc).index << "\n";
			std::cout << "redirection: " << (*itLoc).redirection<< "\n";
			std::cout << "upload_path: " << (*itLoc).uploadPath << "\n";
			for (itMap = (*itLoc).errorPages.begin(); itMap != (*itLoc).errorPages.end();itMap++)
				std::cout << "error page: " << (*itMap).first << ", " << (*itMap).second << "\n";
		}
			i++;
	}
}

