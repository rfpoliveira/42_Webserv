#include <CgiHandler.hpp>

CgiHandler::CgiHandler(std::string &_scriptPath, const Client& client, const Config& config, const Request& request): 
	_scriptPath(_scriptPath), _client(client), _config(config), _request(request), _isValid(false)
{
	_pid = -1;
	_pipeIn[0] = -1;
	_pipeIn[1] = -1;
	_pipeOut[0] = -1;
	_pipeOut[1] = -1;

	if(pipe(_pipeIn) < 0)
		return ;

	if(pipe(_pipeOut) < 0)
	{
		close(_pipeIn[0]);
		close(_pipeIn[1]);
		_pipeIn[0] = -1;
        _pipeIn[1] = -1;
		return ;
	}
	_isValid = true;
}

void CgiHandler::setupEnv()
{
	_envMap["REQUEST_METHOD"] = _request.requestMethod;
	_envMap["SCRIPT_NAME"] = _request.resourcePath;
	_envMap["PATH_TRANSLATED"] = _request.resourcePath;
	_envMap["QUERY_STRING"] = _request.queryString;
	if (_request.body.size() == 0)
		_envMap["CONTENT_LENGTH"] = "";
	else
		_envMap["CONTENT_LENGTH"] = intToString(_request.body.size());
	_envMap["CONTENT_TYPE"] = _request.getHeader("Content-Type");
	_envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
	_envMap["SERVER_PROTOCOL"] = "HTTP/1.1";
	_envMap["SERVER_SOFTWARE"] = "Webserv42/1.0";
	_envMap["HTTP_COOKIE"] = _request.getHeader("Cookie");
	_envMap["SERVER_PORT"] = _client.getPort();

	const ServerBlock* targetServerBlock = _config.getServerBlock(_client.getPort());

	_envMap["SERVER_NAME"] = targetServerBlock->serverBlockName;
	//_envMAP["REMOTE_ADDR"] = //TODO: ask for the client ip?
	size_t pos = _scriptPath.find(".py");
	_envMap["PATH_INFO"] = _scriptPath.substr(pos);
	_envMap["REQUEST_URI"] = _request.queryString;
}

char** CgiHandler::convertEnvToCstyle()
{
	char** envp = new char*[_envMap.size() + 1];

	size_t i = 0;
	std::map<std::string, std::string>::iterator it;

	for (it = _envMap.begin(); it != _envMap.end(); it++)
	{
		std::string envLine = "HTTP_" + it->first + "=" + it->second;
		envp[i] = new char[envLine.size() + 1];
		std::strcpy(envp[i], envLine.c_str());
		i++;
	}

	envp[i] = NULL;

	return(envp);
}

bool CgiHandler::execute()
{
	if(!_isValid)
		return (false); //pipe failed, will send a 500 error response

	this->setupEnv();
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
			freeEnvp(envp);
			std::exit(1);
		}

		if(dup2(_pipeOut[1], STDOUT_FILENO) < 0)
		{
			freeEnvp(envp);
			std::exit(1);
		}

		close(_pipeIn[0]);
		close(_pipeIn[1]);
		close(_pipeOut[0]);
		close(_pipeOut[1]);

		execve(args[0], args, envp);

		freeEnvp(envp);
		_exit(1);
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
		kill(_pid, SIGKILL);
		waitpid(_pid, NULL, 0);
		return false;
	}

	if (fcntl(_pipeIn[1], F_SETFL, O_NONBLOCK) < 0)
	{
		kill(_pid, SIGKILL);
		waitpid(_pid, NULL, 0);
		return false;
	}
	return true;
}

bool CgiHandler::isValid() const 
{ 
	return this->_isValid;
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
