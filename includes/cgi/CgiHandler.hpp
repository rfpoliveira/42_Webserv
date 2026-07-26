#pragma once

#include "../http/Request.hpp"

class CgiHandler
{
	private:
		int _pipeIn[2];
		int _pipeOut[2];
		pid_t _pid;
		std::string _scriptPath;
		std::map<std::string, std::string> _envMap;

		void setupEnv(Request request);
		char** convertEnvToCstyle();

	public:
		CgiHandler(const std::string _scriptPath);
		~CgiHandler();

		bool execute(Request request);

		int getReadFd() const;
		int getWriteFd() const;
		pid_t getPid() const;
};
