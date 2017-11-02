#include "Common.h"
#include "GSBuffer.hpp"

int gamespy_buffer_validate(std::string &buffer)
{
	size_t pos = buffer.find("\\final\\");
	if (pos == std::string::npos) // Dosen't exists
		return 0;

	return 1;
}

std::string gamespy_buffer_get(std::string &buffer, std::string what)
{
	size_t find = 0;
	std::string substr_buffer = "";

	find = buffer.find(what + "\\");
	if (find == std::string::npos)
		return "";

	substr_buffer = buffer.substr(find + what.size() + 1);

	find = substr_buffer.find("\\");
	if (find == std::string::npos)
		return "";

	return substr_buffer.substr(0, find);
}
