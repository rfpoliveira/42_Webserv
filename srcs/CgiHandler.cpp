/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 11:54:26 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/06/16 17:22:49 by rpedrosa         ###   ########.fr       */
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
        //ERRO LIBERTAR MEMORIA DO ENVP TODO
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
    
    //LIBERTAR MEMORIA DO ENVP TODO
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