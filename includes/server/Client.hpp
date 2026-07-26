#pragma once

#include "../http/Request.hpp"
#include <ctime>

class Client
{
	public:
		enum State { READING, PROCESSING, WRITING, DONE };

	private:
		int _fd;
		int _port;
		std::string _readBuffer;
		std::string _writeBuffer;
		size_t _bytesSent;
		Request _request;
		time_t _lastActivity;
		State _state;

	public:
		int getFd() const;
		int getPort() const;

		std::string& getReadBuffer();
		const std::string& getReadBuffer() const;

		std::string& getWriteBuffer();
		const std::string& getWriteBuffer() const;

		Request& getRequest();
		const Request& getRequest() const;

		size_t getBytesSent() const;
		void setBytesSent(size_t bytesSent);

		time_t getLastActivity() const;
		void setLastActivity(time_t lastActivity);

		State getState() const;
		void setState(State state);

		Client();
		Client(int fd, int port);
		Client(const Client& other);
		Client& operator=(const Client& other);
		~Client();
};
