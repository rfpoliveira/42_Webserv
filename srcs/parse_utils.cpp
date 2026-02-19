/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 09:50:27 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/02/19 12:58:25 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/main.hpp"

void ignore_comments(std::string& line)
{
    std::string::iterator it_first;
    std::string::iterator it_last;

    while (42)
    {
        for(it_first = line.begin(); it_first != line.end(); it_first++)
        {
            if(*it_first == '#')
                break ;
        }
        if (it_first == line.end())
            return ;
        for(it_last = it_first; it_last != line.end(); it_last++)
        {
            if (*it_last == '\n')
                break ;
        }
        if (it_last == line.end())
            it_last--;
        line.erase(it_first, it_last);
    }
}

//takes out tabs, spaces and the end of line delimiter from the lines

void clean_strings(std::vector<std::string>& buff)
{
    std::vector<std::string>::iterator it_vec;
    std::string::iterator it_str;

    for(it_vec = buff.begin(); it_vec != buff.end(); it_vec++)
    {
        while ((*it_vec).length() == 0)
        {
            buff.erase(it_vec);
            if (buff.empty())
                return ; //TODO BUG
            it_vec = buff.begin();
        }
        for(it_str = (*it_vec).begin(); it_str != (*it_vec).end(); it_str++)
        {
            if(*it_str == '\v' || *it_str == '\t' || *it_str == ';' || *it_str == '\n' || *it_str == ' ' || \
            *it_str == '{' || *it_str == '}')
            {
                if ((*it_vec).length() == 1)
                {
                    buff.erase(it_vec);
                    it_vec = buff.begin();
                    break ;
                }
                (*it_vec).erase(it_str);
                it_str = (*it_vec).begin();
            }
        }
    }
}


//splits the line into strings using a custom delimiter(usually ' ')

std::vector<std::string> ft_split(std::string s, char delimiter)
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
    clean_strings(buff);

    std::cout << "printing splitted: \n";
    std::vector<std::string>::iterator it;
    for(it = buff.begin(); it != buff.end(); it++)
        std::cout << *it << "\n";

    return (buff);
}

//takes the max_body_size syntax and puts in in decimal (k or K for kb, m or M for mb and nothing for bytes)
unsigned long get_body_size(std::string s)
{
    long long numb = 0;

    //TODO: ERROR IF THE NUMBER IS NOT WELL FORMATED

    if(s.back() == 'm' || s.back() == 'M')
        numb = atoi(s.c_str()) * 1000000;
    else if(s.back() == 'k' || s.back() == 'K')
        numb = atoi(s.c_str()) * 1000;
    else
        numb = atoi(s.c_str());
    return (numb);
}

void add_error_page(std::map<int, std::string>& error_pages, std::vector<std::string> tokens)
{
    //TODO IF INFORMATION IS MISSING

    int error_code = atoi(tokens.at(1).c_str());
    std::string path = tokens.at(2);
    error_pages[error_code] = path;
}

int valid_file_check(std::string config_file)
{
    std::fstream file;

    file.open(config_file);
    if(!file.is_open())
    {
        std::cout << "Error opening file\n";
        return (-1);
    }
    file.close();
    return (0);
}
int brackets_count(std::string config_file)
{
    std::string line;
    std::ifstream file(config_file);
    int count = 0;

    while(getline(file, line))
    {
        if (line.find('}', 0) != std::string::npos)
            count--;
        if (count < 0)
            break ;
        if(line.find('{', 0) != std::string::npos)
            count++;
    }
    if (count != 0)
    {
        std::cout << "Error: Config file format invalid: invalid number of brackets\n";
        return (-1);
    }
    return (0);
}