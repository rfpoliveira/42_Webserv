#pragma once

#include <string>
#include <map>

class MimeTypes
{
	private:
		static std::map<std::string, std::string> _types;
		static void _init();
	public:
		static std::string getType(const std::string &path);
};
