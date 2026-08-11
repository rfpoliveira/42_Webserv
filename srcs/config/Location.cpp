#include "../../includes/config/Location.hpp"
#include "../../includes/exceptions/HttpException.hpp"

Location::Location()
{
	path = "";
	GET = true;
	POST = false;
	DELETE = false;
	autoindex = false;
	redirection = "";
	uploadPath = "";
	index = "index.html";
	maxBodySize = 0;
};

//check for a keyword and stores the information associated in the right place in the class

void Location::checkLineLocation(std::string line)
{
	ignoreComments(line);

	std::vector<std::string> tokens = ftSplit(line, ' ');
	cleanStrings(tokens);
	int i = 0;

	int size = tokens.size();

	if (size == 0)
		return ;
	if (size < 2)
		return ;

	if(tokens.at(i) == "location")
		path = tokens.at(i + 1);
	else if(tokens.at(i) == "root")
	{
		if (!root.empty())
			throw ConfigException("Multiple root declaration at a single Location");
		root = tokens.at(i + 1);
	}
	else if(tokens.at(i) == "allow_methods")
	{
		i++;
		size--;
		while(size > 0)
		{
			if (tokens.at(i) == "GET")
				GET = true;
			if (tokens.at(i) == "POST")
				POST = true;
			if (tokens.at(i) == "DELETE")
				DELETE = true;
			if (tokens.at(i) != "GET" && tokens.at(i) != "POST" && tokens.at(i) != "DELETE")
				throw ConfigException("Invalid method found");
			i++;
			size--;
		}
	}
	else if(tokens.at(i) == "index")
		index = tokens.at(i + 1);
	else if(tokens.at(i) == "autoindex")
	{
		if (tokens.at(i + 1) == "off")
			autoindex = false;
		else if (tokens.at(i + 1) == "on")
			autoindex = true;
		else
			throw ConfigException("Invalid token after autoindex");
	}
	else if (tokens.at(i) == "return")
	{
		if (tokens.size() < 3)
			throw ConfigException("Invalid return in Location");
		redirection = tokens.at(i + 2);
	}
	else if (tokens.at(i) == "upload_path")
		uploadPath = tokens.at(i + 1);
	else
		throw ConfigException("Invalid config Location");
	return ;
};

Location::Location(std::string locationStr)
{
	root = "";
	path = "";
	GET = true;
	POST = false;
	DELETE = false;
	autoindex = false;
	redirection = "";
	uploadPath = "";
	index = "index.html";
	maxBodySize = 0;

	std::istringstream iss(locationStr);
	std::string line;

	while(getline(iss, line))
		checkLineLocation(line);
};

Location::Location(const Location& other)
{
	this->path = other.path;
	this->root = other.root;
	this->GET = other.GET;
	this->POST = other.POST;
	this->DELETE = other.DELETE;
	this->autoindex = other.autoindex;
	this->index = other.index;
	this->redirection = other.redirection;
	this->uploadPath = other.uploadPath;
	this->maxBodySize = other.maxBodySize;
};

Location& Location::operator=(const Location& other)
{
	if (this != &other)
	{
		this->path = other.path;
		this->root = other.root;
		this->GET = other.GET;
		this->POST = other.POST;
		this->DELETE = other.DELETE;
		this->autoindex = other.autoindex;
		this->index = other.index;
		this->redirection = other.redirection;
		this->uploadPath = other.uploadPath;
		this->maxBodySize = other.maxBodySize;
	}
	return (*this);
};

bool Location::isMethodallowed(std::string method) const
{
	if (method == "GET")
		return(this->GET);
	else if(method == "DELETE")
		return(this->DELETE);
	else if(method == "POST")
		return(this->POST);
	else
		throw HttpException(2, "Method not allowed");
}

void Location::applyServerBlockDefaults(const ServerBlock& ServerBlock)
{
	if (root.empty())
		root = ServerBlock.root;
	if (maxBodySize == 0)
		maxBodySize = ServerBlock.maxBodySize;
}

Location::~Location(){};
