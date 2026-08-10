#include "../../includes/config/Config.hpp"
#include "../../includes/core/main.hpp"
#include "../../includes/exceptions/ConfigException.hpp"
#include "../../includes/server/Server.hpp"
#include "../../includes/exceptions/ServerException.hpp"

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
		}
			i++;
	}
}

int main(int argc, char** argv)
{
	try
	{
		if (argc != 2)
			throw ConfigException("Please enter valid arguments:\n./webserv <config_file>");
		parse_config_file(argv[1]);
		Config configs(argv[1]);
		parse_config_info(configs);
		printConfigs(configs);
		Server server(configs);
		server.setup(); // #2 open listening sockets
		server.run(); // #3 single poll() loop (echoes for now)
	}
	catch (ConfigException& e)
	{
		std::cout << e.what() << "\n";
		return (-3);
	}
	catch (ServerException& e)
	{
		std::cerr << e.what() << "\n";
		return (1);
	}
	return (0);
}
