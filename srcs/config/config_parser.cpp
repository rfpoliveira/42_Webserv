#include "../../includes/core/main.hpp"
#include "../../includes/exceptions/ConfigException.hpp"

//check if the file exist of can be opened (valid_file_check)
//checks if the brackets are okay

void parseConfigFile(std::string config_file)
{
    if (validFileCheck(config_file) < 0)
        throw ConfigException("Can't read config file");
    if (bracketsCount(config_file) < 0)
        throw ConfigException("Invalid bracket format");
}
