#pragma once

#include "Client.hpp"
#include <CgiSession.hpp>
#include "../exceptions/ServerException.hpp"
#include <poll.h>

class Server {
	private:
		const Config& _config;
		std::map<int, Client> _clients;
		std::map<int, int> _listenFds; // listening fd -> port it serves
		std::vector<struct pollfd> _pollFds;

		std::map<pid_t, CgiSession*> _cgiSessions;      // pid -> session (reap/timeout sweep)
		std::map<int, CgiSession*>   _fdToCgi;          // pipe fd -> session (poll dispatch)

		static const time_t CGI_TIMEOUT_SECONDS = 5;
	
		int createListenSocket(const std::string& host, int port);
		void closeAll();
		void buildPollFds();
		void acceptNewClient(int listenFd);
		void readFromClient(int fd);
		void writeToClient(int fd);
		void closeClient(int fd);
		bool isListenFd(int fd) const;

		// CGI
		bool isCgiFd(int fd) const;
		void registerCgiSession(Client& client, CgiSession* session);
		void readFromCgi(int fd);
		void writeToCgi(int fd);
		void reapAndTimeoutCgiSessions();
		void finalizeCgiSession(CgiSession* session, int status);
		void cleanupCgiSession(CgiSession* session);
		void abortCgiForClient(int clientFd);

	public:
		Server(const Config& config);
		Server(const Server& other);
		Server& operator=(const Server& other);
		~Server();
		void setup(); // open one socket per unique host:port
		void run(); // the single poll() loop
		const std::map<int, int>& getListenFds() const; // the loop needs this to poll the listening sockets
};
