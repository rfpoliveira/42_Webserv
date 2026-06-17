/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 11:54:26 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/06/17 16:35:08 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/CgiHandler.hpp"

CgiHandler::CgiHandler(std::string script_path)
{
    pid = -1;
    this->script_path = script_path;
    pipeIn[0] = -1;
    pipeIn[1] = -1;
    pipeOut[0] = -1;
    pipeOut[1] = -1;

    if(pipe(pipeIn) < 0)
    {
        //TODO ERROR NAO CONSEGUIR ABRIR PIPE
    }

    if(pipe(pipeOut) < 0)
    {
        close(pipeIn[0]);
        close(pipeIn[1]);
        //TODO ERROR NAO CONSEGUIR ABRIR PIPE
    }
    
}

void    CgiHandler::setup_env(Request request)
{
    envMap["REQUEST_METHOD"] = request.request_method;
    envMap["SCRIPT_NAME"] = request.resource_path;
    envMap["PATH_TRANSLATED"] = request.resource_path;
    envMap["QUERY_STRING"] = request.query_string;
    envMap["CONTENT_LENGTH"] = intToString(request.body.size());
    envMap["CONTENT_TYPE"] = request.getHeader("Content-Type");
    envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
    envMap["SERVER_PROTOCOL"] = "HTTP/1.1";
    envMap["SERVER_SOFTWARE"] = "Webserv42/1.0";
    envMap["HTTP_COOKIE"] = request.getHeader("Cookie");
}

char**  CgiHandler::convert_env_to_cstyle()
{
    char** envp = new char*[envMap.size() + 1];

    size_t i = 0;
    std::map<std::string, std::string>::iterator it;

    for (it = envMap.begin(); it != envMap.end(); it++)
    {
        std::string envLine = it->first + "=" + it->second;
        envp[i] = new char[envLine.size() + 1];
        std::strcpy(envp[i], envLine.c_str());
        i++;
    }
    
    envp[i] = NULL;
    
    return(envp);
}

bool CgiHandler::execute(Request request)
{
    this->setup_env(request); //TODO
    char **envp = this->convert_env_to_cstyle(); //TODO

    char *args[3];
    args[0] = const_cast<char*>("/usr/bin/python3");
    args[1] = const_cast<char*>(script_path.c_str());
    args[2] = NULL;

    pid = fork();
    if(pid < 0)
    {
        freeEnvp(envp);        
        return (false);
    }
    
    //child process
    if(pid == 0)
    {
        if(dup2(pipeIn[0], STDIN_FILENO) < 0)
        {
            std::exit(1);
        }

        if(dup2(pipeOut[1], STDOUT_FILENO) < 0)
        {
            std::exit(1);
        }

        close(pipeIn[0]);
        close(pipeIn[1]);
        close(pipeOut[1]);
        close(pipeOut[1]);//CHECK THIS

        execve(args[0], args, envp);
        
        std::exit(1);
    }

    //parent process
    
    freeEnvp(envp);        
    close(pipeIn[0]);
    pipeIn[0] = -1;
    close(pipeOut[1]);
    pipeOut[1] = -1;

    //transforming the pipe in nonblock so select/poll can write a couple bytes at a time
    if (fcntl(pipeOut[0], F_SETFL, O_NONBLOCK) < 0) 
    {
        return false;
    }
    
    if (fcntl(pipeIn[1], F_SETFL, O_NONBLOCK) < 0) 
    {
        return false;
    }

    return true;
}


int CgiHandler::getReadFd() const
{
    return(pipeOut[0]);
}
int CgiHandler::getWriteFd() const
{
    return(pipeIn[1]);
}

pid_t CgiHandler::getPid() const
{
    return(pid);
}

CgiHandler::~CgiHandler()
{
    if(pipeIn[0] != -1)
        close(pipeIn[0]);
    if(pipeIn[1] != -1)
        close(pipeIn[1]);
    if(pipeOut[0] != -1)
        close(pipeOut[0]);
    if(pipeOut[1] != -1)
        close(pipeOut[1]);
}