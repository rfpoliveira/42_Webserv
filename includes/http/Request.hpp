#pragma once

#include "../core/Main.hpp"

class Request
{
	public:
		std::string requestMethod; //get, post or delete
		std::string queryString;
		std::string resourcePath;
		std::string htmlVersion;
		std::string body;
		unsigned long maxBodySize;
		size_t contentLen;

		std::map<std::string, std::string> headers;
		std::string getHeader(std::string key) const;

		bool isComplete;
		bool isMalformed;
		bool isOversized;

		void parseHeaders(std::string data);

		Request();
		Request(std::string Request_line, unsigned long maxBodySize);
		Request(const Request& other);
		Request& operator=(const Request& other);
		~Request();
};
