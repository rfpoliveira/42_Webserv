#include "../../includes/config/Config.hpp"
#include "../../includes/core/main.hpp"

void print_confis(Config& configs)
{
    std::vector<Server>::iterator it_vec;
    std::vector<Location>::iterator it_loc;
    std::map<int, std::string>::iterator it_map;

    int i = 0;

    std::cout << "Number of Servers: " << configs.number_servers << "\n";
    for(it_vec = configs.servers.begin(); it_vec != configs.servers.end(); it_vec++)
    {
        std::cout << "server " << i << ": \n";
        std::cout << "host: " << (*it_vec).host << "\n";
        std::cout << "port: " << (*it_vec).port << "\n";
        std::cout << "server_name: " << (*it_vec).server_name << "\n";
        std::cout << "max_body_size: " << (*it_vec).max_body_size << "\n";
        for (it_map = (*it_vec).error_pages.begin(); it_map != (*it_vec).error_pages.end();it_map++)
            std::cout << "error page: " << (*it_map).first << ", " << (*it_map).second << "\n";
        for(it_loc = (*it_vec).Locations.begin(); it_loc != (*it_vec).Locations.end(); it_loc++)
        {
            std::cout << "location: \n";
            std::cout << "path: " << (*it_loc).path << "\n";
            std::cout << "root: " << (*it_loc).root << "\n";
            std::cout << "GET? " << (*it_loc).GET << "\n";
            std::cout << "POST? " << (*it_loc).POST << "\n";
            std::cout << "DELETE? " << (*it_loc).DELETE << "\n";
            std::cout << "autofixer: " << (*it_loc).autoindex << "\n";
            std::cout << "index: " << (*it_loc).index << "\n";
            std::cout << "redirection: " << (*it_loc).redirection<< "\n";
            std::cout << "upload_path: " << (*it_loc).upload_path << "\n";
        }
            i++;
    }
}


int main(int argc, char **argv)
{
    try 
    {
        if (argc != 2)
            throw ConfigException("Please enter valid arguments:\n./webserv <config_file>")
        parse_config_file(argv[1])
        Config configs(argv[1]);
        parse_config_info(configs);
    }
    catch (ConfigException& e)
    {
        std::cout << e.what() << "\n";
        return (-3);
    }
    
    //TODO: PRINT ERROR FUNCTION

    //print_confis(configs);

    //SOCKETS
    //CHAMAR REQUEST PARSER

    //handle_request
    

    //RESPONSE BUILDER

    //CLIENT MACHINE
}