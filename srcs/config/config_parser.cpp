#include "../../includes/core/main.hpp"
#include "../../includes/exceptions/ConfigException.hpp"

//check if the file exist of can be opened (valid_file_check)
//checks if the brackets are okay

void parse_config_file(std::string config_file)
{
    if (valid_file_check(config_file) < 0)
        throw ConfigException("Can't read config file");
    if (brackets_count(config_file) < 0)
        throw ConfigException("Invalid bracket format");
}
