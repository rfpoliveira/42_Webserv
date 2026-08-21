#pragma once

#include "../http/Request.hpp"
#include "../server/Client.hpp"
#include "../exceptions/HttpException.hpp"
#include "csignal"
#include <sys/types.h>
#include <sys/wait.h>

class CgiHandler
{
	private:
		int _pipeIn[2];
		int _pipeOut[2];
		pid_t _pid;
		std::string _scriptPath;
		const Client& _client;
		const Config& _config;
		const Request& _request;
		bool _isValid;
		std::string _cgiExten;
		std::map<std::string, std::string> _envMap;

		void setupEnv();
		char** convertEnvToCstyle();

	public:
		CgiHandler(std::string &_scriptPath, const Client& client, const Config& config, const Request& request);
		~CgiHandler();

		bool execute();
		bool isValid() const;
		int getReadFd() const;
		int getWriteFd() const;
		pid_t getPid() const;
};
