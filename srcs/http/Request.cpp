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

static void to_lower(std::string& s)
{
	for (size_t i = 0; i < s.size(); i++)
		s[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(s[i])));
}

Request::Request(){};

/* requestline ex: GET /test.py?user=nuno HTTP/1.1 */

Request::Request(std::string Request_line)
{
	is_complete = false;
	is_malformed = false;
	std::vector<std::string> buffer1 = ft_split(Request_line, ' ');
	if (buffer1.size() != 3)
	{
		is_malformed = true;
		return ;
	}
	request_method = buffer1[0];
	if (buffer1[1].find('?') != std::string::npos)
	{
		std::vector<std::string> buffer2 = ft_split(buffer1[1], '?');
		resource_path = buffer2[0];
		if (buffer2.size() > 1)
			query_string = buffer2[1];
	}
	else
		resource_path = buffer1[1];
	html_version = buffer1[2];
};


//puts in the headers and body, to be called inside the event loop
void Request::parse_headers(std::string data)
{
	size_t separator = data.find("\r\n\r\n");
	if (separator == std::string::npos)
		return ;
	std::string headers_part = data.substr(0, separator);
	std::string body_part = data.substr(separator + 4);

	std::stringstream ss(headers_part);
	std::string line;
	while (std::getline(ss, line))
	{
		if(!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);

		if(line.empty())
			continue ;
		size_t colon = line.find(':');
		if (colon == std::string::npos)
			continue ;
		std::string key = line.substr(0, colon);
		std::string value = line.substr(colon + 1);
		trim(key);
		trim(value);
		to_lower(key);
		if (!key.empty())
			headers[key] = value;
	}
	if (headers.find("content-length") != headers.end())
	{
		int len = std::atoi(headers["content-length"].c_str());
		if (body_part.size() >= static_cast<size_t>(len))
		{
			body = body_part.substr(0, len);
			is_complete = true;
		}
		else
			is_complete = false;
	}
	else
		is_complete = true;
}


Request::Request(const Request &other)
{
		this->is_complete = other.is_complete;
		this->is_malformed = other.is_malformed;
		this->request_method = other.request_method;
		this->query_string = other.query_string;
		this->headers = other.headers;
		this->resource_path = other.resource_path;
		this->html_version = other.html_version;
		this->body = other.body;
};

std::string Request::getHeader(std::string key)
{
	to_lower(key);
	std::map<std::string, std::string>::const_iterator it = headers.find(key);
	if (it != headers.end())
		return it->second;
	return ("");
}

Request& Request::operator=(const Request &other)
{
	if (this != &other)
	{
		this->is_complete = other.is_complete;
		this->is_malformed = other.is_malformed;
		this->request_method = other.request_method;
		this->query_string = other.query_string;
		this->headers = other.headers;
		this->resource_path = other.resource_path;
		this->html_version = other.html_version;
		this->body = other.body;
	}
	return (*this);
};

Request::~Request(){};
