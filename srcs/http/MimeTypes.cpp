#include "../../includes/http/MimeTypes.hpp"
#include <algorithm>
#include <cctype>

std::map<std::string, std::string> MimeTypes::_types;

void MimeTypes::_init()
{
	if (!_types.empty())
		return;

	_types[".html"] = "text/html";
	_types[".htm"]  = "text/html";
	_types[".css"]  = "text/css";
	_types[".js"]   = "text/javascript";
	_types[".json"] = "application/json";
	_types[".png"]  = "image/png";
	_types[".jpg"]  = "image/jpeg";
	_types[".jpeg"] = "image/jpeg";
	_types[".gif"]  = "image/gif";
	_types[".svg"]  = "image/svg+xml";
	_types[".ico"]  = "image/x-icon";
	_types[".txt"]  = "text/plain";
	_types[".pdf"]  = "application/pdf";
}

std::string MimeTypes::getType(const std::string& path)
{
	_init();

	size_t dotPosition = path.find_last_of('.'); //finds the position of the "dot": image.png <-
	if (dotPosition == std::string::npos) //did not find dot -> no extension -> default
		return ("application/octet-stream");

	std::string extension = path.substr(dotPosition); //extrat just the extension: .png

	std::string::iterator it;
	for(it = extension.begin(); it != extension.end(); it++) //convert to lowercase to be case insensitive
		(*it) = std::tolower(*it);

	std::map<std::string, std::string>::iterator it2 = _types.find(extension); //finds extension in the map

	if(it2 != _types.end())
		return(it2->second);
	return("application/octet-stream"); //did not find the extension in the map, revertes to default
}
