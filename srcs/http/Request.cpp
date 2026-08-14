#include "../../includes/http/Request.hpp"
#include <cctype>

static void trim(std::string& s)
{
	size_t start = s.find_first_not_of(" \t");
	if (start == std::string::npos)
	{
		s.clear();
		return ;
	}
	size_t end = s.find_last_not_of(" \t");
	s = s.substr(start, end - start + 1);
}

static void toLower(std::string& s)
{
	for (size_t i = 0; i < s.size(); i++)
		s[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(s[i])));
}

Request::Request()
{
	isComplete = false;
	isMalformed = false;
	isOversized = false;
	maxBodySize = 0;
	contentLen = 0;
};

/* requestline ex: GET /test.py?user=nuno HTTP/1.1 */

Request::Request(std::string Request_line, unsigned long maxBodySize)
{
	isComplete = false;
	isMalformed = false;
	isOversized = false;
	this->maxBodySize = maxBodySize;
	contentLen = 0;
	std::vector<std::string> buffer1 = ftSplit(Request_line, ' ');
	if (buffer1.size() != 3)
	{
		isMalformed = true;
		return ;
	}
	requestMethod = buffer1[0];
	if (buffer1[1].find('?') != std::string::npos)
	{
		std::vector<std::string> buffer2 = ftSplit(buffer1[1], '?');
		resourcePath = buffer2[0];
		if (buffer2.size() > 1)
			queryString = buffer2[1];
	}
	else
		resourcePath = buffer1[1];
	htmlVersion = buffer1[2];
};


//puts in the headers and body, to be called inside the event loop
void Request::parseHeaders(std::string data)
{
	size_t separator = data.find("\r\n\r\n");
	if (separator == std::string::npos)
		return ;
	std::string headersPart = data.substr(0, separator);
	std::string bodyPart = data.substr(separator + 4);

	std::stringstream ss(headersPart);
	std::string line;
	bool firstLine = true;
	while (std::getline(ss, line))
	{
		if(!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);

		if (firstLine)                       // the request line: METHOD PATH VERSION
		{
			firstLine = false;
			std::vector<std::string> parts = ftSplit(line, ' ');
			if (parts.size() != 3)
			{
				isMalformed = true;
				return ;
			}
			requestMethod = parts[0];
			if (parts[1].find('?') != std::string::npos)
			{
				std::vector<std::string> tgt = ftSplit(parts[1], '?');
				resourcePath = tgt[0];
				if (tgt.size() > 1)
					queryString = tgt[1];
			}
			else
				resourcePath = parts[1];
			htmlVersion = parts[2];
			continue ;
		}

		if(line.empty())
			continue ;
		size_t colon = line.find(':');
		if (colon == std::string::npos)
			continue ;
		std::string key = line.substr(0, colon);
		std::string value = line.substr(colon + 1);
		trim(key);
		trim(value);
		toLower(key);
		if (!key.empty())
			headers[key] = value;
	}

	if (headers.find("content-length") != headers.end())
	{
		char* endptr;
        errno = 0;
		unsigned long len = std::strtoul(headers["content-length"].c_str(), &endptr, 10);
		
		if (errno != 0 || *endptr != '\0')
		{
			isMalformed = true;
			return ;
		}

		contentLen = static_cast<size_t>(len);

		if (maxBodySize > 0 && contentLen > maxBodySize)
		{
			isOversized = true;
			return ;
		}

		if (bodyPart.size() >= contentLen)
		{
			body = bodyPart.substr(0, len);
			isComplete = true;
		}
		else
			isComplete = false;
	}
	else
		isComplete = true;
}


Request::Request(const Request& other)
{
		this->isComplete = other.isComplete;
		this->isMalformed = other.isMalformed;
		this->requestMethod = other.requestMethod;
		this->queryString = other.queryString;
		this->headers = other.headers;
		this->resourcePath = other.resourcePath;
		this->htmlVersion = other.htmlVersion;
		this->body = other.body;
		this->isOversized = other.isOversized;
		this->maxBodySize = other.maxBodySize;
		this->contentLen = other.contentLen;
};

std::string Request::getHeader(std::string key) const
{
	toLower(key);
	std::map<std::string, std::string>::const_iterator it = headers.find(key);
	if (it != headers.end())
		return it->second;
	return ("");
}

Request& Request::operator=(const Request& other)
{
	if (this != &other)
	{
		this->isComplete = other.isComplete;
		this->isMalformed = other.isMalformed;
		this->requestMethod = other.requestMethod;
		this->queryString = other.queryString;
		this->headers = other.headers;
		this->resourcePath = other.resourcePath;
		this->htmlVersion = other.htmlVersion;
		this->body = other.body;
		this->isOversized = other.isOversized;
		this->maxBodySize = other.maxBodySize;
		this->contentLen = other.contentLen;
	}
	return (*this);
};

Request::~Request(){};
