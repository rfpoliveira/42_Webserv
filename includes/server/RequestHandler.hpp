#pragma once

#include <Client.hpp>
#include <Config.hpp>
#include <Location.hpp>
#include <CgiSession.hpp>
#include <Response.hpp>
#include <CgiHandler.hpp>
#include <sstream>
#include <cstdlib>
#include <sys/stat.h>
#include "HandlerOutcome.hpp"

class RequestHandler
{
	private:
		static std::string handleGet(const Request &request, const Location &location);
		static std::string handlePost(const Request &request, const Location &location);
		static std::string handleDelete(const Request &request, const Location &location);

	public:
		static HandlerOutcome handler(const Client &client, const Config &config);
};
