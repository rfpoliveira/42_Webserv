#include "../../includes/server/Server.hpp"
#include "../../includes/core/Common.hpp"
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <csignal>
#include <set>
#include <Request.hpp>
#include <RequestHandler.hpp>
#include <Response.hpp>
#include <MimeTypes.hpp>

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

			if (isCgiFd(fd))
			{
				if (re & (POLLHUP | POLLERR | POLLNVAL))
				{
					// pipe broke unexpectedly — treat like EOF/error, let reap sweep clean up
					CgiSession* session = _fdToCgi[fd];
					if (fd == session->handler.getReadFd())
						session->readDone = true;
					else
						session->writeDone = true;
					continue;
				}
				if (re & POLLIN)
					readFromCgi(fd);
				if (re & POLLOUT)
					writeToCgi(fd);
				continue;
			}
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
		reapAndTimeoutCgiSessions(); // non-blocking sweep, every iteration
	}
}

int Server::createListenSocket(const std::string& host, int port)
{
	struct addrinfo hints;
	struct addrinfo* res = NULL;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4 only
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = AI_PASSIVE; // for wildcard IP address if host is empty

	std::string portStr = intToString(port);
	int gai = getaddrinfo(host.c_str(), portStr.c_str(), &hints, &res); // I send hints to get res as a result.
	if (gai != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(gai) << "\n";
		return -1;
	}
	// on the macOS, SOCK_CLOEXEC is not defined, so we need to set it manually after socket creation
	// int fd = socket(res->ai_family, res->ai_socktype | SOCK_CLOEXEC, res->ai_protocol); //setting sock_cloexec so execve closes the sockets in the child automaticly
	int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (fd < 0) {freeaddrinfo(res); return (-1);}
	int flags = fcntl(fd, F_GETFD);
	if (flags < 0) {close(fd); freeaddrinfo(res); return (-1);}
	if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) < 0) {close(fd); freeaddrinfo(res); return (-1);}
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

	for (std::map<int, CgiSession*>::iterator it = _fdToCgi.begin(); it != _fdToCgi.end(); ++it)
	{
		CgiSession* session = it->second;
		struct pollfd pfd;
		pfd.fd = it->first;
		pfd.events = 0;
		pfd.revents = 0;

		if (it->first == session->handler.getReadFd() && !session->readDone)
			pfd.events |= POLLIN;
		if (it->first == session->handler.getWriteFd() && !session->writeDone)
			pfd.events |= POLLOUT;

		if (pfd.events != 0)
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
	Client &c = _clients[fd];
	c.getReadBuffer().append(buf, n);                 // 1. accumulate
	c.getRequest().parseHeaders(c.getReadBuffer());   // 2. parse request line + headers

	if (c.getRequest().isMalformed)                   // 3a. bad request -> 400
	{
		c.getWriteBuffer() = Response::fromError(400).serialize();
		c.setState(Client::WRITING);
	}
	else if (c.getRequest().isOversized)              // 3b. body too large -> 413
	{
		c.getWriteBuffer() = Response::fromError(413).serialize();
		c.setState(Client::WRITING);
	}
	else if (c.getRequest().isComplete)               // 3c. full request -> handle
	{
		c.setState(Client::PROCESSING);

		HandlerOutcome outcome = RequestHandler::handler(c, _config);

		if (outcome.type == CGI_COMPLETE)
		{
			c.getWriteBuffer() = outcome.response;
			c.setState(Client::WRITING);
		}
		else //CGI_PENDING
		{
			registerCgiSession(c, outcome.cgiSession);
			c.setState(Client::CGI_WAITING);
		}
	}
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
		c.setState(Client::DONE);
		closeClient(fd);        // Connection: close model (keep-alive out of scope)
	}
	else
		c.setBytesSent(sent);
}

void Server::closeClient(int fd)
{
	close(fd);
	_clients.erase(fd);
}

const std::map<int, int>& Server::getListenFds() const
{
	return (_listenFds);
}

bool Server::isCgiFd(int fd) const
{
	return (_fdToCgi.count(fd) != 0);
}

void Server::registerCgiSession(Client& client, CgiSession* session)
{
	session->clientFd = client.getFd();
	session->startTime = time(NULL);

	_cgiSessions[session->handler.getPid()] = session;
	_fdToCgi[session->handler.getReadFd()] = session;
	if (!session->requestBody.empty())
		_fdToCgi[session->handler.getWriteFd()] = session;
	else
	{
		close(session->handler.getWriteFd());
		session->writeDone = true;
	}
}

void Server::readFromCgi(int fd)
{
	CgiSession* session = _fdToCgi[fd];
	char buf[4096];
	ssize_t n = read(fd, buf, sizeof(buf));

	if (n > 0)
	{
		session->responseBuf.append(buf, n);
	}
	else if (n == 0)
	{
		session->readDone = true;
	}
	else if (errno != EAGAIN && errno != EWOULDBLOCK)
	{
		session->readDone = true;
	}
}

void Server::writeToCgi(int fd)
{
	CgiSession* session = _fdToCgi[fd];
	const char* data = session->requestBody.c_str() + session->writeOffset;
	size_t remaining = session->requestBody.size() - session->writeOffset;

	ssize_t n = write(fd, data, remaining);
	if (n > 0)
	{
		session->writeOffset += n;
		if (session->writeOffset == session->requestBody.size())
		{
			close(fd);
			session->writeDone = true;
			_fdToCgi.erase(fd);
		}
	}
	else if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
	{
		close(fd);
		session->writeDone = true;
		_fdToCgi.erase(fd);
	}
}

void Server::reapAndTimeoutCgiSessions()
{
	time_t now = time(NULL);

	for (std::map<pid_t, CgiSession*>::iterator it = _cgiSessions.begin();
	     it != _cgiSessions.end(); )
	{
		CgiSession* session = it->second;
		pid_t pid = it->first;
		int status = 0;

		pid_t reaped = waitpid(pid, &status, WNOHANG);
		if (reaped == pid)
		{
			finalizeCgiSession(session, status);
			cleanupCgiSession(session);
			_cgiSessions.erase(it++);
			continue;
		}

		if (!session->killSent && (now - session->startTime) >= CGI_TIMEOUT_SECONDS)
		{
			kill(pid, SIGKILL);
			session->killSent = true;
		}

		if (session->killSent)
		{
			reaped = waitpid(pid, &status, WNOHANG);
			if (reaped == pid)
			{
				if (_clients.count(session->clientFd))
				{
					Client& c = _clients[session->clientFd];
					c.getWriteBuffer() = Response::fromError(504).serialize();
					c.setState(Client::WRITING);
				}
				cleanupCgiSession(session);
				_cgiSessions.erase(it++);
				continue;
			}
		}
		++it;
	}
}

void Server::finalizeCgiSession(CgiSession* session, int status)
{
	if (!_clients.count(session->clientFd))
		return; // client already is off

	Client& c = _clients[session->clientFd];

	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
	{
		c.getWriteBuffer() = Response::fromError(502).serialize();
	}
	else
	{
		c.getWriteBuffer() = Response::fromCGI(session->responseBuf); // mounts http response
	}
	c.setState(Client::WRITING);
}

void Server::cleanupCgiSession(CgiSession* session)
{
	int readFd = session->handler.getReadFd();
	int writeFd = session->handler.getWriteFd();

	if (readFd != -1) 
	{ 
		_fdToCgi.erase(readFd);
		close(readFd); 
	}
	if (writeFd != -1) 
	{ 
		_fdToCgi.erase(writeFd);
		close(writeFd);
	}

	delete session;
}

void Server::abortCgiForClient(int clientFd) //client off mid cgi
{
	for (std::map<pid_t, CgiSession*>::iterator it = _cgiSessions.begin();
	     it != _cgiSessions.end(); ++it)
	{
		if (it->second->clientFd == clientFd)
		{
			kill(it->first, SIGKILL);
			it->second->clientFd = -1; //marks it to take it out later
		}
	}
}