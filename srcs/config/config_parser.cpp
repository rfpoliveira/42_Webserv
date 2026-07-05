#include "../../includes/core/main.hpp"

//check if the file exist of can be opened (valid_file_check)
//checks if the brackets are okay

int parse_config_file(std::string config_file)
{
    if (valid_file_check(config_file) < 0)
        return (-1);
    if (brackets_count(config_file) < 0)
        return(-2);
    return (0);
}


