#include <Client.hpp>

Client::Client()
{
	_fd = -1;
	_port = -1;
	_bytesSent = 0;
	_lastActivity = std::time(NULL);
	_state = READING;
};

Client::Client(int fd, int port)
{
	_fd = fd;
	_port = port;
	_bytesSent = 0;
	_lastActivity = std::time(NULL);
	_state = READING;
};

Client::Client(const Client& other)
{
	this->_fd = other._fd;
	this->_port = other._port;
	this->_readBuffer = other._readBuffer;
	this->_writeBuffer = other._writeBuffer;
	this->_bytesSent = other._bytesSent;
	this->_request = other._request;
	this->_lastActivity = other._lastActivity;
	this->_state = other._state;
};

Client& Client::operator=(const Client& other)
{
	if (this != &other)
	{
		this->_fd = other._fd;
		this->_port = other._port;
		this->_readBuffer = other._readBuffer;
		this->_writeBuffer = other._writeBuffer;
		this->_bytesSent = other._bytesSent;
		this->_request = other._request;
		this->_lastActivity = other._lastActivity;
		this->_state = other._state;
	}
	return (*this);
};

int Client::getFd() const
{
	return (_fd);
}

int Client::getPort() const
{
	return (_port);
}

std::string& Client::getReadBuffer()
{
	return (_readBuffer);
}

const std::string& Client::getReadBuffer() const
{
	return (_readBuffer);
}

std::string& Client::getWriteBuffer()
{
	return (_writeBuffer);
}

const std::string& Client::getWriteBuffer() const
{
	return (_writeBuffer);
}

Request& Client::getRequest()
{
	return (_request);
}

const Request& Client::getRequest() const
{
	return (_request);
}

size_t Client::getBytesSent() const
{
	return (_bytesSent);
}

void Client::setBytesSent(size_t bytesSent)
{
	_bytesSent = bytesSent;
}

time_t Client::getLastActivity() const
{
	return (_lastActivity);
}

void Client::setLastActivity(time_t lastActivity)
{
	_lastActivity = lastActivity;
}

Client::State Client::getState() const
{
	return (_state);
}

void Client::setState(State state)
{
	_state = state;
}

Client::~Client(){};
