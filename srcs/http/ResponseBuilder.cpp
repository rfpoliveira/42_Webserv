#include "../../includes/http/ResponseBuilder.hpp"

std::string ResponseBuilder::buildStaticFile(const std::string& filePath)
{
	(void)filePath;
	return ("HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: 13\r\n"
			"\r\n"
			"Hello, World!");
}
