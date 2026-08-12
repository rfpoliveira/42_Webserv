#include "../../includes/core/Main.hpp"

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
