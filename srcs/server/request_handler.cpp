#include "../../includes/server/request_handler.hpp"
#include "../../includes/http/Response.hpp"
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

std::string requestHandler(const Client& client, Config& config) //TODO check duplicated resquesthanlder
{
	const Request& request = client.getRequest();
	std::string path = request.resourcePath;

	const Location* location = config.getLocation(client.getPort(), path);
	if (!location)
		return (buildMinimalResponse(404, "Not Found", "404 Not Found"));

	int methodChecker = location->isMethodallowed(request.requestMethod);
	if (methodChecker == 0)
		return (buildMinimalResponse(405, "Method Not Allowed", "405 Method Not Allowed"));
	else if (methodChecker == 2)
		return (buildMinimalResponse(501, "Unknown verb", "501 Unknown verb"));

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

	if (fullPath[fullPath.length() - 1] == '/')
		fullPath += location->index.empty() ? "index.html" : location->index;

	Response res = Response::fromStaticFile(fullPath);
	return (res.serialize());
}

std::string handleDelete(const Request& req, std::string root)
{
	std::string fullPath = root + req.resourcePath;

	struct stat fileStats;

	if(stat(fullPath.c_str(), &fileStats) != 0) //file does not exist
		return (buildMinimalResponse(404, "Not Found", "404 Not Found"));

	if (S_ISDIR(fileStats.st_mode)) //cant delete diretories
		return (buildMinimalResponse(403, "Forbidden: Directory deletion not allowed", "403 Forbidden: Directory deletion not allowed"));

	if(access(fullPath.c_str(), W_OK) != 0) //no writing permission
		return (buildMinimalResponse(403, "Forbidden: No write permission", "403 Forbidden: No write permission"));
	
	if(std::remove(fullPath.c_str()) == 0)
		return ("HTTP/1.1 204 No Content\r\nServer: webserv\r\nConnection: keep-alive\r\n\r\n"); //sucess, 204 and no cotent in the body is default
	else
		return (buildMinimalResponse(500, "Internal Server Error", "500 Internal Server Error")); //unexpeted errro on delete
}