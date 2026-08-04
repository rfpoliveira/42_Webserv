#include "../../includes/cgi/CgiHandler.hpp"

CgiHandler::CgiHandler(std::string _scriptPath)
{
	_pid = -1;
	this->_scriptPath = _scriptPath;
	_pipeIn[0] = -1;
	_pipeIn[1] = -1;
	_pipeOut[0] = -1;
	_pipeOut[1] = -1;

    if(pipe(_pipeIn) < 0)
    {
        throw HttpException(1, "Error opening pipe");
    }

    if(pipe(_pipeOut) < 0)
    {
        close(_pipeIn[0]);
        close(_pipeIn[1]);
        throw HttpException(1, "Error opening pipe");
    } 
}

void CgiHandler::setupEnv(Request request)
{
	_envMap["REQUEST_METHOD"] = request.requestMethod;
	_envMap["SCRIPT_NAME"] = request.resourcePath;
	_envMap["PATH_TRANSLATED"] = request.resourcePath;
	_envMap["QUERY_STRING"] = request.queryString;
	_envMap["CONTENT_LENGTH"] = intToString(request.body.size());
	_envMap["CONTENT_TYPE"] = request.getHeader("Content-Type");
	_envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
	_envMap["ServerBlock_PROTOCOL"] = "HTTP/1.1";
	_envMap["ServerBlock_SOFTWARE"] = "Webserv42/1.0";
	_envMap["HTTP_COOKIE"] = request.getHeader("Cookie");
}

char** CgiHandler::convertEnvToCstyle()
{
	char** envp = new char*[_envMap.size() + 1];

	size_t i = 0;
	std::map<std::string, std::string>::iterator it;

	for (it = _envMap.begin(); it != _envMap.end(); it++)
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
	this->setupEnv(request);
	char** envp = this->convertEnvToCstyle();

	char* args[3];
	args[0] = const_cast<char*>("/usr/bin/python3");
	args[1] = const_cast<char*>(_scriptPath.c_str());
	args[2] = NULL;

	_pid = fork();
	if(_pid < 0)
	{
		freeEnvp(envp);
		return (false);
	}

	//child process
	if(_pid == 0)
	{
		if(dup2(_pipeIn[0], STDIN_FILENO) < 0)
		{
			std::exit(1);
		}

		if(dup2(_pipeOut[1], STDOUT_FILENO) < 0)
		{
			std::exit(1);
		}

		close(_pipeIn[0]);
		close(_pipeIn[1]);
		close(_pipeOut[1]);
		close(_pipeOut[1]);//CHECK THIS

		execve(args[0], args, envp);

		std::exit(1);
	}

	//parent process

	freeEnvp(envp);
	close(_pipeIn[0]);
	_pipeIn[0] = -1;
	close(_pipeOut[1]);
	_pipeOut[1] = -1;

	//transforming the pipe in nonblock so select/poll can write a couple bytes at a time
	if (fcntl(_pipeOut[0], F_SETFL, O_NONBLOCK) < 0)
	{
		return false;
	}

	if (fcntl(_pipeIn[1], F_SETFL, O_NONBLOCK) < 0)
	{
		return false;
	}

	return true;
}


int CgiHandler::getReadFd() const
{
	return(_pipeOut[0]);
}
int CgiHandler::getWriteFd() const
{
	return(_pipeIn[1]);
}

pid_t CgiHandler::getPid() const
{
	return(_pid);
}

CgiHandler::~CgiHandler()
{
	if(_pipeIn[0] != -1)
		close(_pipeIn[0]);
	if(_pipeIn[1] != -1)
		close(_pipeIn[1]);
	if(_pipeOut[0] != -1)
		close(_pipeOut[0]);
	if(_pipeOut[1] != -1)
		close(_pipeOut[1]);
}
