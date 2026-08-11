#include "../../includes/core/main.hpp"

//takes the commented line of the config file out of the line  of the config file (#)

void ignoreComments(std::string& line)
{
	std::string::iterator itFirst;
	std::string::iterator itLast;

	while (42)
	{
		for(itFirst = line.begin(); itFirst != line.end(); itFirst++)
		{
			if(*itFirst == '#')
				break ;
		}
		if (itFirst == line.end())
			return ;
		for(itLast = itFirst; itLast != line.end(); itLast++)
		{
			if (*itLast == '\n')
				break ;
		}
		if (itLast == line.end())
			itLast--;
		line.erase(itFirst, itLast);
	}
}

//takes out tabs, spaces and the end of line delimiter from the lines

void cleanStrings(std::vector<std::string>& buff)
{
	std::vector<std::string>::iterator itVec;
	std::string::iterator itStr;

	for(itVec = buff.begin(); itVec != buff.end(); itVec++)
	{
		while ((*itVec).length() == 0)
		{
			buff.erase(itVec);
			if (buff.size() == 0)
				return ;
			itVec = buff.begin();
		}
		for(itStr = (*itVec).begin(); itStr != (*itVec).end(); itStr++)
		{
			if(*itStr == '\v' || *itStr == '\t' || *itStr == ';' || *itStr == '\n' || *itStr == ' ' || \
			*itStr == '{' || *itStr == '}' || *itStr == '\r')
			{
				if ((*itVec).length() == 1)
				{
					buff.erase(itVec);
					itVec = buff.begin();
					break ;
				}
				(*itVec).erase(itStr);
				itStr = (*itVec).begin();
			}
		}
	}
}


//splits the line into strings using a custom delimiter(usually ' ' and clean the line of unwated chars(clean strings))

std::vector<std::string> ftSplit(std::string s, char delimiter)
{
	size_t last = 0;
	size_t next = 0;
	std::vector<std::string> buff;

	while ((next = s.find(delimiter, last)) != std::string::npos)
	{
		buff.push_back(s.substr(last, next-last));
		last = next + 1;
	}
	buff.push_back(s.substr(last));

/*     std::cout << "printing splitted: \n";
	std::vector<std::string>::iterator it;
	for(it = buff.begin(); it != buff.end(); it++)
		std::cout << *it << "\n";
	std::cout << "stop splitted \n"; */
	return (buff);
}

//takes the maxBodySize syntax and puts in in decimal (k or K for kb, m or M for mb and nothing for bytes)
unsigned long getBodySize(std::string s)
{
	long long numb = 0;

	if(s[s.size() - 1] == 'm' || s[s.size() - 1] == 'M')
		numb = atoi(s.c_str()) * 1000000;
	else if(s[s.size() - 1] == 'k' || s[s.size() - 1] == 'K')
		numb = atoi(s.c_str()) * 1000;
	else
		numb = atoi(s.c_str());
	return (numb);
}

//separates the information about the error pages, puts them in a map, the key is the code corresponding to the path

int addErrorPage(std::map<int, std::string>& errorPages, std::vector<std::string> tokens)
{
	if (tokens.size() < 3)
		return (1);
	int errorCode = atoi(tokens.at(1).c_str());
	std::string path = tokens.at(2);
	errorPages[errorCode] = path;
	return (0);
}

//checks if the file exists and if it can be openned

int validFileCheck(std::string configFile)
{
	std::fstream file;

	file.open(configFile.c_str());
	if(!file.is_open())
	{
		std::cout << "Error opening file\n";
		return (-1);
	}
	file.close();
	return (0);
}

// counts and brackets in all the file, checks if its all well formatted

int bracketsCount(std::string configFile)
{
	std::string line;
	std::ifstream file(configFile.c_str());
	int count = 0;

	while(getline(file, line))
	{
		if (line.find('}', 0) != std::string::npos)
			count--;
		if (count < 0 || count > 2)
			break ;
		if(line.find('{', 0) != std::string::npos)
			count++;
	}
	if (count != 0)
	{
		std::cout << "Error: Config file format invalid: brackets\n";
		return (-1);
	}
	return (0);
}