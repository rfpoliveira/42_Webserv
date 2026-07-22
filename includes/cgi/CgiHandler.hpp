#pragma once

#include "../http/Request.hpp"
#include "../exceptions/HttpException.hpp"

class CgiHandler
{
	private:
		int pipeIn[2];
		int pipeOut[2];
		pid_t pid;
		std::string script_path;
		std::map<std::string, std::string> envMap;

		void    setup_env(Request request);
		char** convert_env_to_cstyle();

	public:
		CgiHandler(const std::string script_path);
		~CgiHandler();

		bool execute(Request request);

		int getReadFd() const;
		int getWriteFd() const;
		pid_t getPid() const;
};
