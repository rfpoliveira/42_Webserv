#include "../../includes/server/Server.hpp"
#include "core/common.hpp"
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <csignal>
#include <set>

Server::Server(const Config& config) : _config(config) {};

Server::Server(const Server& other)
: _config(other._config), _clients(other._clients), _listenFds(other._listenFds), _pollFds(other._pollFds) {}

Server& Server::operator=(const Server& other)
{
	if (this != &other)
	{
		_clients = other._clients;
		_listenFds = other._listenFds;
		_pollFds = other._pollFds;
	}
	return *this;
}

Server::~Server() {
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		close(it->first);
	}
	for (std::map<int, int>::iterator it = _listenFds.begin(); it != _listenFds.end(); ++it) {
		close(it->first);
	}
}

void Server::closeAll()
{
	for (std::map<int, int>::iterator it = _listenFds.begin(); it != _listenFds.end(); ++it)
		close(it->first);
	_listenFds.clear();
}

void Server::setup()
{
	std::set <std::string> seen;
	for (size_t i = 0; i < _config.serverBlocks.size(); i ++)
	{
		std::string host = _config.serverBlocks[i].host;
		int port = _config.serverBlocks[i].port;
		std::string key = host + ":" + intToString(port);
		if (seen.count(key))
			continue;
		seen.insert(key);
		int fd = createListenSocket(host, port);
		if (fd < 0)
		{
			closeAll();
			throw ServerException("Failed to create socket of " + key);
		}
		_listenFds[fd] = port;
	}
	if (_listenFds.empty())
		throw ServerException("No listening sockets configured");
}

int Server::createListenSocket(const std::string& host, int port)
{
	struct addrinfo hints;
	struct addrinfo* res = NULL;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	std::string portStr = intToString(port);
	int gai = getaddrinfo(host.c_str(), portStr.c_str(), &hints, &res); // I send hints to get res as a result.
	if (gai != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(gai) << "\n";
		return -1;
	}
	int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (fd < 0) {freeaddrinfo(res); return (-1);}
	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		{ close(fd); freeaddrinfo(res); return (-1);}
	if (bind(fd, res->ai_addr, res->ai_addrlen) < 0)
		{ close (fd); freeaddrinfo(res); return (-1);}
	freeaddrinfo(res);
	if (listen(fd, 128) < 0)
		{ close(fd); return -1; }
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		{ close(fd); return -1; }
	return (fd);
}

// Rebuild the poll() array every iteration: all listening fds + all client
// fds. POLLOUT is requested ONLY when a client still has bytes to send,
// otherwise the loop would spin at 100% CPU.
void Server::buildPollFds()
{
	_pollFds.clear();

	for (std::map<int, int>::iterator it = _listenFds.begin(); it != _listenFds.end(); ++it)
	{
		struct pollfd pfd;
		pfd.fd = it->first;
		pfd.events = POLLIN;
		pfd.revents = 0;
		_pollFds.push_back(pfd);
	}
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		struct pollfd pfd;
		pfd.fd = it->first;
		pfd.events = POLLIN;
		if (!it->second.getWriteBuffer().empty())
			pfd.events |= POLLOUT;
		pfd.revents = 0;
		_pollFds.push_back(pfd);
	}
}

bool Server::isListenFd(int fd) const
{
	return (_listenFds.count(fd) != 0);
}

void Server::acceptNewClient(int listenFd)
{
	int clientFd = accept(listenFd, NULL, NULL);
	if (clientFd < 0)
		return;
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0)
	{
		close(clientFd);
		return;
	}
	_clients[clientFd] = Client(clientFd, _listenFds[listenFd]);
}

void Server::readFromClient(int fd)
{
	char buf[4096];
	ssize_t n = recv(fd, buf, sizeof(buf), 0);
	if (n <= 0) // 0 = peer closed; <0 = treat as done, close
	{
		closeClient(fd);
		return;
	}
	// #3 temporary: echo the received bytes straight back (replaced in #8).
	_clients[fd].getWriteBuffer().append(buf, n);
}

void Server::writeToClient(int fd)
{
	Client& c = _clients[fd];
	std::string& out = c.getWriteBuffer();
	size_t sent = c.getBytesSent();

	ssize_t n = send(fd, out.c_str() + sent, out.size() - sent, 0);
	if (n <= 0)
	{
		closeClient(fd);
		return;
	}
	sent += n;
	if (sent >= out.size()) // whole buffer flushed
	{
		out.clear();
		c.setBytesSent(0);
	}
	else
		c.setBytesSent(sent);
}

void Server::closeClient(int fd)
{
	close(fd);
	_clients.erase(fd);
}

void Server::run()
{
	std::signal(SIGPIPE, SIG_IGN); // writing to a dead client must not kill us

	while (true)
	{
		buildPollFds();
		int ready = poll(&_pollFds[0], _pollFds.size(), -1);
		if (ready < 0)
			continue;

		for (size_t i = 0; i < _pollFds.size(); i ++)
		{
			short re = _pollFds[i].revents;
			if (re == 0)
				continue;
			int fd = _pollFds[i].fd;

			if (re & (POLLHUP | POLLERR | POLLNVAL))
			{
				if (!isListenFd(fd))
					closeClient(fd);
				continue;
			}
			if (re & POLLIN)
			{
				if (isListenFd(fd))
					acceptNewClient(fd);
				else
					readFromClient(fd);
			}
			if ((re & POLLOUT) && _clients.count(fd))
				writeToClient(fd);
		}
	}
}

const std::map<int, int>& Server::getListenFds() const
{
	return (_listenFds);
}e
