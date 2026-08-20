#include <Response.hpp>
#include <RequestHandler.hpp>
#include <CgiHandler.hpp>
#include <sstream>
#include <cstdlib>

std::string RequestHandler::handler(const Client& client, const Config& config)
{
	const Request& request = client.getRequest();
	std::string path = request.resourcePath;

	const Location* location = config.getLocation(client.getPort(), path);
	if (!location)
		return (Response::fromError(404).serialize());
	if (!location->isMethodallowed(request.requestMethod))
		return (Response::fromError(405).serialize());
	if (path.find("..") != std::string::npos) // reject traversal escaping the root (before CGI!)
		return (Response::fromError(403).serialize());
	if (path.find(".py") != std::string::npos) //TODO BETTER CGI CHECK
	{
		CgiHandler cgiHandler(path, client, config, request);
		if (!cgiHandler.execute())
			return (Response::fromError(500, NULL, location).serialize());
		return (Response::fromError(501, NULL, location).serialize());
	}
	// Until here I have general checks. From now and on I can handle the request based on the method
	if (request.requestMethod == "GET")
		return (handleGet(request, *location));
	else if (request.requestMethod == "POST")
		return (handlePost(request, *location));
	else if (request.requestMethod == "DELETE")
		return (handleDelete(request, *location));
	return (Response::fromError(501, NULL, location).serialize());
}

std::string RequestHandler::handleGet(const Request &request, const Location &location)
{
	std::string fullPath = location.root + request.resourcePath;

	if (fullPath[fullPath.size() - 1] == '/')
		fullPath += location.index.empty() ? "index.html" : location.index;
	Response res = Response::fromStaticFile(fullPath);
	res.setHeader("Connection", "close"); // keep-alive is out of scope (issue #10)
	return (res.serialize());
}

std::string RequestHandler::handlePost(const Request &request, const Location &location)
{
	std::string contentLengthHeader = request.getHeader("Content-Length");
	unsigned long bodySize = contentLengthHeader.empty() ? 0 : std::strtoul(contentLengthHeader.c_str(), NULL, 10);
	if (bodySize > location.maxBodySize)
		return (Response::fromError(413, NULL, &location).serialize());
	return Response::fromError(501, NULL, &location).serialize();
}

std::string RequestHandler::handleDelete(const Request &request, const Location &location)
{
	std::string root = location.root;
	std::string fullPath = root + request.resourcePath;

	struct stat fileStats;

	if(stat(fullPath.c_str(), &fileStats) != 0) //file does not exist
		return (Response::fromError(404).serialize());

	if (S_ISDIR(fileStats.st_mode)) //cant delete diretories
		return (Response::fromError(403, "Forbidden: Directory deletion not allowed").serialize());

	if(access(fullPath.c_str(), W_OK) != 0) //no writing permission
		return (Response::fromError(403, "Forbidden: No write permission").serialize());
	
	if(std::remove(fullPath.c_str()) == 0)
		return ("HTTP/1.1 204 No Content\r\nServer: webserv\r\nConnection: close\r\n\r\n"); //sucess, 204 and no cotent in the body is default
	else
		return (Response::fromError(500, "Internal Server Error").serialize()); //unexpeted errro on delete
}
