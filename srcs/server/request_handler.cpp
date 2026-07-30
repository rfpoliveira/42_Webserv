#include "../../includes/server/request_handler.hpp"
#include "../../includes/http/ResponseBuilder.hpp"
#include "../../includes/cgi/CgiHandler.hpp"
#include <sstream>
#include <cstdlib>

static std::string buildMinimalResponse(int code, const std::string& reason, const std::string& body)
{
	std::ostringstream oss;

	oss << "HTTP/1.1 " << code << " " << reason << "\r\n"
		<< "Content-Type: text/plain\r\n"
		<< "Content-Length: " << body.size() << "\r\n"
		<< "Connection: close\r\n"
		<< "\r\n"
		<< body;
	return (oss.str());
}

std::string requestHandler(const Client& client, Config& config)
{
	const Request& request = client.getRequest();
	std::string path = request.resourcePath;

	Location* location = config.getLocation(client.getPort(), path);
	if (!location)
		return (buildMinimalResponse(404, "Not Found", "404 Not Found"));

	if (!location->isMethodallowed(request.requestMethod))
		return (buildMinimalResponse(405, "Method Not Allowed", "405 Method Not Allowed"));

	std::string contentLengthHeader = request.getHeader("Content-Length");
	unsigned long bodySize = contentLengthHeader.empty() ? 0 : std::strtoul(contentLengthHeader.c_str(), NULL, 10);
	if (bodySize > location->maxBodySize)
		return (buildMinimalResponse(413, "Payload Too Large", "413 Payload Too Large"));

	if (path.find(".py") != std::string::npos) //TODO BETTER CGI CHECK
	{
		CgiHandler cgiHandler(path);
		if (!cgiHandler.execute(request))
			return (buildMinimalResponse(500, "Internal ServerBlock Error", "500 CGI failed to start"));
		return (buildMinimalResponse(501, "Not Implemented", "501 CGI output streaming not wired to the event loop yet"));
	}
	
	std::string fullPath = location->root + path;

	if (fullPath[fullPath.lenght() - 1] == '/')
		fullPath += location->index.empty() ? "index.html" : location->index;
	return (ResponseBuilder::buildStaticFile(fullPath));
}