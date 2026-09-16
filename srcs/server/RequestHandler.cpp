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

std::string RequestHandler::buildFullPath(const std::string& root, const std::string& urlPath)
{
	std::string fullPath = root;
	if (!fullPath.empty() && fullPath[fullPath.size() - 1] == '/')
		fullPath.erase(fullPath.size() - 1);

	if (!urlPath.empty() && urlPath[0] != '/')
		fullPath += "/";

	fullPath += urlPath;
	return fullPath;
}

HandlerOutcome RequestHandler::handler(const Client& client, const Config& config)
{
	const Request& request = client.getRequest();
	std::string path = request.resourcePath;

	const Location* location = config.getLocation(client.getPort(), path);

	std::cerr << "[DEBUG] location match for path=" << path
          << " found=" << (location != NULL) << "\n";
	if (location)
    	std::cerr << "[DEBUG] location.root=" << location->root << "\n";	

	if (location && path.find("..") != std::string::npos) // reject traversal escaping the root (before CGI!)
		return (HandlerOutcome(CGI_COMPLETE, Response::fromError(403, NULL, location).serialize(), NULL));

	if (!location)
		return (HandlerOutcome(CGI_COMPLETE, Response::fromError(404, NULL, location).serialize(), NULL));

	path = RequestHandler::buildFullPath(location->root, path);

	if (!location->isMethodallowed(request.requestMethod))
		return (HandlerOutcome(CGI_COMPLETE, Response::fromError(405, NULL, location).serialize(), NULL));

	if (isCgiRequest(path))
	{

		std::cerr << "[DEBUG] Detected CGI\n";

		struct stat sb;
		if (stat(path.c_str(), &sb))
			return (HandlerOutcome(CGI_COMPLETE, Response::fromError(404, NULL, location).serialize(), NULL));

		CgiSession *session = new CgiSession(path, client, config, request);
		if (!session->handler.execute())
			return (HandlerOutcome(CGI_COMPLETE, Response::fromError(500, NULL, location).serialize(), NULL));
        return (HandlerOutcome(CGI_PENDING, "", session));
	}
	// Until here I have general checks. From now and on I can handle the request based on the method
	if (request.requestMethod == "GET")
		return (HandlerOutcome(CGI_COMPLETE, handleGet(request, *location), NULL));
	else if (request.requestMethod == "POST")
		return (HandlerOutcome(CGI_COMPLETE, handlePost(request, *location), NULL));
	else if (request.requestMethod == "DELETE")
		return (HandlerOutcome(CGI_COMPLETE, handleDelete(request, *location), NULL));
	return (HandlerOutcome(CGI_COMPLETE, Response::fromError(501, NULL, location).serialize(), NULL));
}

std::string RequestHandler::handleGet(const Request &request, const Location &location)
{
	std::string fullPath = RequestHandler::buildFullPath(location.root, request.resourcePath);
	std::cerr << "[DEBUG] handleGet trying fullPath=[" << fullPath << "]\n";

	Response res;
	struct stat fileStats;
	if (stat(fullPath.c_str(), &fileStats) != 0)
	{
		std::cerr << "[DEBUG] stat failed errno=" << errno << " (" << strerror(errno) << ")\n";
    	return (Response::fromError(404, NULL, &location).serialize());
	}

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
				return (Response::fromError(403, "Forbidden: Index file not found", &location).serialize());
		}
		res = Response::fromStaticFile(fullPath, &location); // If index file exists, serve it
	}
	else
	{
		res = Response::fromStaticFile(fullPath, &location); // If not a directory, serve the file
	}
	res.setHeader("Connection", "close"); // keep-alive is out of scope (issue #10)
	return (res.serialize());
}

std::string RequestHandler::handlePost(const Request &request, const Location &location)
{
	// 1. Reject bodies larger than this route allows.
	std::string contentLengthHeader = request.getHeader("Content-Length");
	unsigned long bodySize = contentLengthHeader.empty() ? 0 : std::strtoul(contentLengthHeader.c_str(), NULL, 10);
	if (bodySize > location.maxBodySize)
		return (Response::fromError(413, NULL, &location).serialize());

	// 2. POST is an upload here: the route must define where uploads are stored.
	if (location.uploadPath.empty())
		return (Response::fromError(403, "Upload not allowed on this route", &location).serialize());

	// 3. Derive the target filename from the last path segment and sanitize it,
	//    so a client cannot escape uploadPath (e.g. /upload/../../etc/passwd).
	std::string name = request.resourcePath;
	size_t slash = name.find_last_of('/');
	if (slash != std::string::npos)
		name = name.substr(slash + 1);
	if (name.empty() || name.find("..") != std::string::npos)
		return (Response::fromError(400, "Invalid upload filename", &location).serialize());

	// 4. Build the full destination path: uploadPath + "/" + name.
	std::string dest = location.uploadPath;
	if (dest.empty() || dest[dest.size() - 1] != '/')
		dest += '/';
	dest += name;

	// 5. Write the request body to disk. Regular files are exempt from poll(),
	//    so a direct write is allowed by the subject.
	std::ofstream out(dest.c_str(), std::ios::binary | std::ios::trunc);
	if (!out.is_open())
		return (Response::fromError(500, "Cannot open upload target", &location).serialize());
	out.write(request.body.data(), request.body.size());
	out.close();
	if (out.fail())
		return (Response::fromError(500, "Failed to write upload", &location).serialize());

	// 6. Success: 201 Created, pointing at the newly created resource.
	Response res;
	res.setStatus(201, Response::reasonPhrase(201));
	res.setHeader("Location", request.resourcePath);
	res.setHeader("Connection", "close");
	res.setBody("File uploaded\n");
	return (res.serialize());
}

std::string RequestHandler::handleDelete(const Request &request, const Location &location)
{
	std::string fullPath = RequestHandler::buildFullPath(location.root, request.resourcePath);

	struct stat fileStats;

	std::cerr << "[DEBUG] handle delete path: " << fullPath << "\n";

	if(stat(fullPath.c_str(), &fileStats) != 0) //file does not exist
		return (Response::fromError(404, NULL, &location).serialize());

	if (S_ISDIR(fileStats.st_mode)) //cant delete diretories
		return (Response::fromError(403, "Forbidden: Directory deletion not allowed", &location).serialize());

	if(access(fullPath.c_str(), W_OK) != 0) //no writing permission
		return (Response::fromError(403, "Forbidden: No write permission", &location).serialize());
	
	if(std::remove(fullPath.c_str()) == 0)
		return ("HTTP/1.1 204 No Content\r\nServer: webserv\r\nConnection: close\r\n\r\n"); //sucess, 204 and no cotent in the body is default
	else
		return (Response::fromError(500, "Internal Server Error", &location).serialize()); //unexpeted errro on delete
}
