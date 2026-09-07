#pragma once

#include <Common.hpp>

class Client;

class CgiHandler
{
	private:
		int _pipeIn[2];
		int _pipeOut[2];
		pid_t _pid;
		std::string _scriptPath;
		bool _isValid;
		std::string _cgiExten;
		std::map<std::string, std::string> _envMap;


		void setupEnv(const Client &client, const Request &request, const Config &config);
		char** convertEnvToCstyle();

	public:
		CgiHandler();
		CgiHandler(std::string &_scriptPath, const Client& client, const Config& config, const Request& request);
		~CgiHandler();

		bool execute();
		bool isValid() const;
		int getReadFd() const;
		int getWriteFd() const;
		pid_t getPid() const;
};
