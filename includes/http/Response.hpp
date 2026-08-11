#pragma once

#include <map>
#include <string>
#include <Location.hpp>

class Response
{
	private:
		int _code;
		std::string _reason;
		std::map<std::string, std::string> _headers;
		std::string _body;
	public:
		Response();
		Response(const Response &copy);
		Response& operator=(const Response &copy);
		~Response();
		void setStatus(int code, const std::string &reason);
		void setHeader(const std::string &k, const std::string &v);
		void setBody(const std::string &body);

		static std::string reasonPhrase(int code);
		static Response fromStaticFile(const std::string &fullPath);
		static Response fromError(int code, const std::string &reason="", const Location *loc=NULL);
		std::string serialize() const;
};
