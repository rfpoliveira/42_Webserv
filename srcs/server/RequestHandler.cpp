#include <Response.hpp>
#include <RequestHandler.hpp>
#include <CgiHandler.hpp>
#include <sstream>
#include <cstdlib>

std::string requestHandler(const Client& client, const Config& config)
{
	const Request& request = client.getRequest();
	std::string path = request.resourcePath;

	const Location* location = config.getLocation(client.getPort(), path);
	if (!location)
		return (Response::fromError(404, "Not found").serialize());

	if (!location->isMethodallowed(request.requestMethod))
		return (Response::fromError(405, "Method Not Allowed", location).serialize());

	std::string contentLengthHeader = request.getHeader("Content-Length");
	unsigned long bodySize = contentLengthHeader.empty() ? 0 : std::strtoul(contentLengthHeader.c_str(), NULL, 10);
	if (bodySize > location->maxBodySize)
		return (Response::fromError(413, "Payload Too Large", location).serialize());

	if (path.find(".py") != std::string::npos) //TODO BETTER CGI CHECK
	{
		CgiHandler cgiHandler(path);
		if (!cgiHandler.execute(request))
			return (Response::fromError(500, "Internal ServerBlock Error", location).serialize());
		return (Response::fromError(501, "CGI output streaming not wired to the event loop yet", location).serialize());
	}
	
	if (path.find("..") != std::string::npos) // reject traversal escaping the root
		return (Response::fromError(403).serialize());

	std::string fullPath = location->root + path;

	if (fullPath[fullPath.size() - 1] == '/')
		fullPath += location->index.empty() ? "index.html" : location->index;
	Response res = Response::fromStaticFile(fullPath);
	res.setHeader("Connection", "close"); // keep-alive is out of scope (issue #10)
	return (res.serialize());
}
