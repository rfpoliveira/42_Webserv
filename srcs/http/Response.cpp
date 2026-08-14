#include <Response.hpp>
#include <MimeTypes.hpp>
#include <Server.hpp>
#include <ctime>
#include <Request.hpp>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

std::string Response::reasonPhrase(int code)
{
	switch (code)
	{
		case 200: return ("OK");
		case 201: return ("Created");
		case 204: return ("No Content");
		case 301: return ("Moved Permanently");
		case 400: return ("Bad Request");
		case 403: return ("Forbidden");
		case 404: return ("Not Found");
		case 405: return ("Method Not Allowed");
		case 413: return ("Payload Too Large");
		case 500: return ("Internal Server Error");
		case 501: return ("Not Implemented");
		case 504: return ("Gateway Timeout");
		default:  return ("Unknown");
	}
}

Response Response::fromError(int code, const char *detail, const Location *loc)
{
	(void)loc; // reserved for custom error_page lookup. Implement later.
	Response res;
	std::string detailStr = detail ? detail : "";
	std::string phrase = reasonPhrase(code);

	std::ostringstream body;
	body << "<html><head><title>" << code << " " << phrase << "</title></head>"
		 << "<body><center><h1>" << code << " " << phrase << "</h1></center>";
	if (!detailStr.empty())
		body << "<p>" << detail << "</p>";
	body << "<hr><center>webserver</center></body></html>";
	res.setStatus(code, phrase);
	res.setBody(body.str());
	std::ostringstream len;
	len << res._body.size();
	res.setHeader("Content-Type", "text/html");
	res.setHeader("Content-Length", len.str());
	res.setHeader("Connection", "close");
	return (res);
}

Response::Response() : _code(200), _reason("OK"), _body("")
{

}

Response::~Response()
{

}

Response::Response(const Response &copy) : _code(copy._code), _reason(copy._reason),
	_headers(copy._headers), _body(copy._body)
{

}

Response& Response::operator=(const Response &copy)
{
	if (this != &copy)
	{
		_code = copy._code;
		_reason = copy._reason;
		_headers = copy._headers;
		_body = copy._body;
	}
	return *this;
}

void Response::setStatus(int code, const std::string &reason)
{
	_code = code;
	_reason = reason;
}

void Response::setHeader(const std::string &k, const std::string &v)
{
	_headers[k] = v;
}

void Response::setBody(const std::string &body)
{
	_body = body;
}

Response Response::fromStaticFile(const std::string &fullPath)
{
	Response res;
	char buf[64];
	time_t now = time(NULL);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
	struct stat st;
	if (stat(fullPath.c_str(), &st) != 0)   // does not exist
		return (fromError(404));
	if (S_ISDIR(st.st_mode))                // a directory (listing is issue #14)
		return (fromError(403));
	if (access(fullPath.c_str(), R_OK) != 0) // exists but not readable
		return (fromError(403));
	std::ifstream file(fullPath.c_str(), std::ios::in | std::ios::binary);
	if (!file)
		return (fromError(404));
	std::ostringstream oss;
	oss << file.rdbuf();
	res.setBody(oss.str());
	res.setStatus(200, "OK");

	std::ostringstream len;
	len << res._body.size();
	res.setHeader("Server", "webserver");
	res.setHeader("Date", buf);
	res.setHeader("Content-Type", MimeTypes::getType(fullPath));
	res.setHeader("Content-Length", len.str());
	return (res);
}

std::string Response::serialize() const
{
	std::ostringstream oss;

	oss << "HTTP/1.1 " << _code << " " << _reason << "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
	{
		oss << it->first << ": " << it->second << "\r\n";
	}
	oss << "\r\n" << _body;
	return (oss.str());
}
