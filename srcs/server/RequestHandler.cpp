#include <RequestHandler.hpp>
#include <HandlerOutcome.hpp>


bool isCgiRequest(std::string path)
{
	if (path.find(".py") != std::string::npos ||
		path.find(".php") != std::string::npos ||
		path.find(".pl") != std::string::npos)
		return (true);
	return(false);
}

HandlerOutcome RequestHandler::handler(const Client& client, const Config& config)
{
	const Request& request = client.getRequest();
	std::string path = request.resourcePath;

	const Location* location = config.getLocation(client.getPort(), path);

	if (!location)
		return (HandlerOutcome(CGI_COMPLETE, Response::fromError(404).serialize(), NULL));
	if (!location->isMethodallowed(request.requestMethod))
		return (HandlerOutcome(CGI_COMPLETE, Response::fromError(405).serialize(), NULL));
	if (path.find("..") != std::string::npos) // reject traversal escaping the root (before CGI!)
		return (HandlerOutcome(CGI_COMPLETE, Response::fromError(403).serialize(), NULL));
	if (isCgiRequest(path))
	{
		CgiSession *session = new CgiSession(path, client, config, request);
		if (!session->handler.execute())
			return (HandlerOutcome(CGI_COMPLETE, Response::fromError(500).serialize(), NULL));
        return (HandlerOutcome(CGI_PENDING, "", session));
	}
	// Until here I have general checks. From now and on I can handle the request based on the method
	if (request.requestMethod == "GET")
		return (HandlerOutcome(CGI_COMPLETE, handleGet(request, *location), NULL));
	else if (request.requestMethod == "POST")
		return (HandlerOutcome(CGI_COMPLETE, handlePost(request, *location), NULL));
	else if (request.requestMethod == "DELETE")
		return (HandlerOutcome(CGI_COMPLETE, handleDelete(request, *location), NULL));
	return (HandlerOutcome(CGI_COMPLETE, Response::fromError(501).serialize(), NULL));
}

std::string RequestHandler::handleGet(const Request &request, const Location &location)
{
	std::string fullPath = location.root + request.resourcePath;
	Response res;
	struct stat fileStats;
	if (stat(fullPath.c_str(), &fileStats) != 0)
    	return (Response::fromError(404).serialize());

	if (S_ISDIR(fileStats.st_mode)) // Is a directory?
	{
		if (fullPath.empty() || fullPath[fullPath.size() - 1] != '/')
			fullPath += '/'; // Ensure the path ends with a slash
		fullPath += location.index.empty() ? "index.html" : location.index;
		if (stat(fullPath.c_str(), &fileStats) != 0 || !S_ISREG(fileStats.st_mode)) // Does index file exist and is a readible file?
		{
			if (location.autoindex)
				return (Response::fromAutoIndex(location, request.resourcePath).serialize());
			else
				return (Response::fromError(403, "Forbidden: Index file not found").serialize());
		}
		res = Response::fromStaticFile(fullPath); // If index file exists, serve it
	}
	else
	{
		res = Response::fromStaticFile(fullPath); // If not a directory, serve the file
	}
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
	std::string fullPath = location.root + request.resourcePath;

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
