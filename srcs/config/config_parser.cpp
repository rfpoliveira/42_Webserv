#include "../../includes/core/main.hpp"

//check if the file exist of can be opened (valid_file_check)
//checks if the brackets are okay

int parseConfigFile(std::string configFile)
{
	if (validFileCheck(configFile) < 0)
		return (-1);
	if (bracketsCount(configFile) < 0)
		return(-2);
	return (0);
}
