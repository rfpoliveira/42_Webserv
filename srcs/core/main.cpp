#include <Common.hpp>

int main(int argc, char** argv)
{
	try
	{
		if (argc != 2)
			throw ConfigException("Please enter valid arguments:\n./webserv <config_file>");
		parseConfigFile(argv[1]);
		Config configs(argv[1]);
		parseConfigInfo(configs);
		//printConfigs(configs);
		Server server(configs);
		server.setup(); // #2 open listening sockets

		std::signal(SIGINT, handleShutDownSignal);
		std::signal(SIGTERM, handleShutDownSignal);
		std::signal(SIGPIPE, SIG_IGN);

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
