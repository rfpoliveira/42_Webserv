#include "../../includes/config/Config.hpp"
#include "../../includes/core/main.hpp"

void printConfis(Config& configs)
{
	std::vector<Server>::iterator itVec;
	std::vector<Location>::iterator itLoc;
	std::map<int, std::string>::iterator itMap;

	int i = 0;

	std::cout << "Number of Servers: " << configs.numberServers << "\n";
	for(itVec = configs.servers.begin(); itVec != configs.servers.end(); itVec++)
	{
		std::cout << "server " << i << ": \n";
		std::cout << "host: " << (*itVec).host << "\n";
		std::cout << "port: " << (*itVec).port << "\n";
		std::cout << "server_name: " << (*itVec).serverName << "\n";
		std::cout << "max_body_size: " << (*itVec).maxBodySize << "\n";
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
	if (argc != 2)
	{
		std::cout << "Please enter valid arguments:\n./webserv <config_file>\n";
		return (-1);
	}
	if (parseConfigFile(argv[1]) != 0)
		return (-2);

	Config configs(argv[1]);

	int error = parseConfigInfo(configs);
	if (error != 0)
		return (error);

	//TODO: PRINT ERROR FUNCTION

	//print_confis(configs);

	//SOCKETS
	//CHAMAR REQUEST PARSER

	//handle_request


	//RESPONSE BUILDER

	//CLIENT MACHINE
}