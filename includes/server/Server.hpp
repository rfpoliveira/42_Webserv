#pragma once

#include "Client.hpp"
#include <poll.h>

class Server {
	private:
		const Config& _config;
		std::map<int, Client> _clients;
		std::map<int, int> _listenFds; // listening fd -> port it serves
		std::vector<struct pollfd> _pollFds;
		int createListenSocket(const std::string& host, int port);
		void closeAll();
		void buildPollFds();
		void acceptNewClient(int listenFd);
		void readFromClient(int fd);
		void writeToClient(int fd);
		void closeClient(int fd);
		bool isListenFd(int fd) const;

	public:
		Server(const Config& config);
		Server(const Server& other);
		Server& operator=(const Server& other);
		~Server();
		void setup(); // open one socket per unique host:port
		void run(); // the single poll() loop
		const std::map<int, int>& getListenFds() const; // the loop needs this to poll the listening sockets
		class ServerException : public std::exception
		{
			private:
				std::string _message;
			public:
				ServerException(const std::string& message) : _message(message) {}
				virtual ~ServerException() throw() {}
				virtual const char* what() const throw(){
					return _message.c_str();
				}
		};
};
