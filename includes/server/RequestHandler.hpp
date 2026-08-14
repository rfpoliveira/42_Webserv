#pragma once

#include <Client.hpp>
#include <Config.hpp>
#include <Location.hpp>

class RequestHandler
{
	private:
		static std::string handleGet(const Request &request, const Location &location);
		static std::string handlePost(const Request &request, const Location &location);
		static std::string handleDelete(const Request &request, const Location &location);
	public:
		static std::string handler(const Client &client, const Config &config);
};
