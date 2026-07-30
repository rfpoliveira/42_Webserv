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

	if (path.find(".py") != std::string::npos)
	{
		CgiHandler cgiHandler(path);
		if (!cgiHandler.execute(request))
			return (buildMinimalResponse(500, "Internal ServerBlock Error", "500 CGI failed to start"));
		return (buildMinimalResponse(501, "Not Implemented", "501 CGI output streaming not wired to the event loop yet"));
	}

	return (ResponseBuilder::buildStaticFile(location->root + path));
}
// ============================================================================
// PARKED until the Client class exists (build step 1).
//
// The sketch below is kept verbatim as the design intent. It does not compile
// yet because `Client` is not defined, and it has known defects to fix when
// reviving it:
//   - typo in the name: request_hanlder -> request_handler
//   - client.getPort  -> missing parentheses: client.getPort()
//   - getLocation now returns Location* (NULL = no match) -> must null-check
//   - getHeader("Content-Length") is a std::string; convert to a number before
//     comparing against location.maxBodySize
//   - the empty `return(/*some error*/)` statements need real error responses
//   - take (const Client&, const Config&) instead of copying both by value
//   - no return on the final path; every branch must produce a std::string
//
// std::string request_handler(const Client& client, const Config& config)
// {
//     Location* location = config.getLocation(client.getPort(),
//                                             client.request.resource_path);
//     if (!location)
//         return (/* 404 */);
//     if (!location->isMethodallowed(client.request.request_method))
//         return (/* 405 */);
//     if (body_size > location->maxBodySize)
//         return (/* 413 */);
//     if (client.request.resource_path.find(".py") != std::string::npos)
//     {
//         CgiHandler CGI_handler(client.request.resource_path);
//         CGI_handler.execute(client.request);
//     }
//     else
//         return (ResponseBuilder::build_static_file(location->root +
//                                                    client.request.resource_path));
// }
// ============================================================================