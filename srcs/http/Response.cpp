#include <Response.hpp>
#include <MimeTypes.hpp>
#include <Server.hpp>
#include <ctime>
#include <Request.hpp>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

Response::Response() : _code(200), _reason("OK"), _body("")
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

Response::~Response()
{

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
		case 502: return ("Bad Gateaway");
		case 501: return ("Not Implemented");
		case 504: return ("Gateway Timeout");
		default:  return ("Unknown");
	}
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

std::string cgi_trim(const std::string& s)
{
	size_t start = s.find_first_not_of(" \t");
	size_t end = s.find_last_not_of(" \t\r");
	if (start == std::string::npos)
		return "";
	return s.substr(start, end - start + 1);
}

std::string Response::fromCGI(const std::string &rawCGIoutput)
{
	//locate header/body separator
	size_t sepPos = rawCGIoutput.find("\r\n\r\n");
	size_t sepLen = 4;
	size_t altPos = rawCGIoutput.find("\n\n");

	if (sepPos == std::string::npos || (altPos != std::string::npos && altPos < sepPos))
	{
		sepPos = altPos;
		sepLen = 2;
	}

	if (sepPos == std::string::npos) // did not send any of the separators, bad output
		return(Response::fromError(502).serialize());
	
	std::string headerBlock = rawCGIoutput.substr(0, sepPos);
	std::string body = rawCGIoutput.substr(sepPos + sepLen);

	//parsing of the cgi headers
	Response resp;

	std::istringstream headerStream(headerBlock);
	std::string line;

	while(std::getline(headerStream, line))
	{
		if(!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		if(line.empty())
			continue ;
		
		size_t colon = line.find(":");
		if (colon == std::string::npos) //if there is no colon the line is not well formatted, we ignore
			continue ;
		std::string key = cgi_trim(line.substr(0, colon));
		std::string value = cgi_trim(line.substr(colon + 1));
		if (!key.empty())
			resp._headers[key] = value;
		
	}

	//status line: default 200 OK

	resp.setStatus(200, "OK");

	std::map<std::string, std::string>::iterator statusIt = resp._headers.find("Status");
	if(statusIt != resp._headers.end())
	{
		std::istringstream ss(statusIt->second);
		ss >> resp._code;
		size_t spacePos = statusIt->second.find(' ');
		if(spacePos != std::string::npos)
			resp._reason = cgi_trim(statusIt->second.substr(spacePos + 1));
		else
			resp._reason = reasonPhrase(resp._code);
		resp._headers.erase(statusIt);
	}

	if (statusIt == resp._headers.end() && resp._headers.count("Location")) //redirect
		resp.setStatus(302, "Found");

	std::string contentType = "text/hmtl"; //default
	std::map<std::string, std::string>::iterator ctIt = resp._headers.find("Content-Type");
	if(ctIt != resp._headers.end())
	{
		contentType = ctIt->second;
		resp._headers.erase(ctIt);
	}

	std::ostringstream os;
	os << "HTTP/1.1" << resp._code << " " << resp._reason << "\r\n";
	os << "Content-Type: " << contentType << "\r\n";
	os << "Content-Lenght: " << body.size() << "\r\n";

	for (std::map<std::string, std::string>::iterator it = resp._headers.begin(); it != resp._headers.end(); ++it)
		os << it->first << ": " << it->second << "\r\n";

	os << "Connection: close\r\n";
	os << "\r\n";
	os << body;

	return(os.str());
}

// RAII guard: closedir() runs on EVERY exit path (normal return or exception),
// so the directory stream can never leak even if we return mid-loop.
namespace
{
	struct DirGuard
	{
		DIR *dir;
		DirGuard(DIR *d) : dir(d) {}
		~DirGuard() { if (dir) closedir(dir); }
	};
}

Response Response::fromAutoIndex(const Location &loc, const std::string &requestUri)
{
	std::string dirPath = loc.root + requestUri;   // filesystem path to iterate

	DIR *raw = opendir(dirPath.c_str());
	if (!raw)                                       // cannot open -> forbidden
		return (fromError(403, NULL, &loc));
	DirGuard guard(raw);                            // closedir() now guaranteed

	// URL base for hrefs: built from the REQUEST URI (never the filesystem root),
	// and forced to end in '/' so absolute links resolve into the directory.
	std::string base = requestUri;
	if (base.empty() || base[base.size() - 1] != '/')
		base += '/';

	std::ostringstream html;
	html << "<html><head><title>Index of " << requestUri << "</title></head>"
		 << "<body><h1>Index of " << requestUri << "</h1><hr><ul>";

	struct dirent *entry;
	while ((entry = readdir(raw)) != NULL)          // NULL = end of directory
	{
		std::string name = entry->d_name;           // copy out immediately (owned by DIR)
		if (name == ".")                            // skip self link
			continue;
		html << "<li><a href=\"" << base << name << "\">" << name << "</a></li>";
	}
	html << "</ul><hr><center>webserver</center></body></html>";

	Response res;
	res.setStatus(200, "OK");
	res.setBody(html.str());
	res.setHeader("Server", "webserver");
	res.setHeader("Content-Type", "text/html");
	std::ostringstream len;
	len << html.str().size();
	res.setHeader("Content-Length", len.str());
	res.setHeader("Connection", "close");
	return (res);
	// guard destructor runs here -> closedir(raw)
}

Response Response::fromError(int code, const char *detail, const Location *loc)
{
	Response res;
	std::string detailStr = detail ? detail : "";
	std::string phrase = reasonPhrase(code);

	// 1) Custom error page: if this location configured error_page for `code`
	//    and the file opens/reads, use its contents as the body.
	if (loc != NULL)
	{
		std::map<int, std::string>::const_iterator it = loc->errorPages.find(code);
		if (it != loc->errorPages.end())
		{
			std::string filePath = it->second;
			std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
			if (file)
			{
				std::ostringstream oss;
				oss << file.rdbuf();
				res.setStatus(code, phrase);
				res.setBody(oss.str());
				std::ostringstream clen;
				clen << oss.str().size();
				res.setHeader("Server", "webserver");
				res.setHeader("Content-Type", MimeTypes::getType(filePath));
				res.setHeader("Content-Length", clen.str());
				res.setHeader("Connection", "close");
				return (res);
			}
			// file missing/unreadable -> fall through to the generated page
		}
	}

	// 2) Fallback: generate a default error page.
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
