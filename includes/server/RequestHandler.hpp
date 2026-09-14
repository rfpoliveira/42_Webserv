#pragma once

#include <Common.hpp>

class HandlerOutcome;

class RequestHandler
{
	private:
		static std::string handleGet(const Request &request, const Location &location);
		static std::string handlePost(const Request &request, const Location &location);
		static std::string handleDelete(const Request &request, const Location &location);

	public:
		static HandlerOutcome handler(const Client &client, const Config &config);
		static std::string buildFullPath(const std::string& root, const std::string& urlPath);
};
