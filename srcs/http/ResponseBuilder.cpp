#include "../../includes/http/ResponseBuilder.hpp"

std::string ResponseBuilder::build_static_file(const std::string &file_path)
{
	(void)file_path;
	return ("HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: 13\r\n"
			"\r\n"
			"Hello, World!");
}
