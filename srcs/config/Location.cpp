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

int Location::checkLineLocation(std::string line)
{
	ignoreComments(line);

	std::vector<std::string> tokens = ftSplit(line, ' ');
	cleanStrings(tokens);
	int i = 0;

	int size = tokens.size();

	if (size == 0)
		return (0);
	if (size < 2)
		return (0);

	if(tokens.at(i) == "location")
		path = tokens.at(i + 1);
	else if(tokens.at(i) == "root") //TODO: CHECK FOR MULTIPLE ROOT PATHS
		root = tokens.at(i + 1);
	else if(tokens.at(i) == "allow_methods")
	{
		while(size > 0)
		{
			if (tokens.at(i) == "GET")
				GET = true;
			if (tokens.at(i) == "POST")
				POST = true;
			if (tokens.at(i) == "DELETE")
				DELETE = true;
			if (tokens.at(i) != "GET" && tokens.at(i) != "POST" && tokens.at(i) != "DELETE") //TODO:: CHECK
				return (5);
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
			return (3);
	}
	else if (tokens.at(i) == "return")
	{
		if (tokens.size() < 3)
			return (4);
		redirection = tokens.at(i + 2);
	}
	else if (tokens.at(i) == "upload_pass")
		uploadPath = tokens.at(i + 1);
	else
		return (6);
	return (0);
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

	//TODO: check for defaults

	std::istringstream iss(locationStr);
	std::string line;
	int error = 0;

	while(getline(iss, line))
	{
		error = checkLineLocation(line);
		if (error != 0)
		{
			std::cout << "Error: " << error << "\n";
			throw (LocationErrorExeption());
		}
	}
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

bool Location::isMethodallowed(std::string method)
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

void Location::applyServerDefaults(const Server& server)
{
    if (root.empty())
        root = server.root;
    if (maxBodySize == 0)
        maxBodySize = server.maxBodySize;
}

Location::~Location(){};
