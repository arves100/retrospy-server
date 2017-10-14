#include "StdAfx.h"
#include "Server.h"
#include "GameSpy.h"

void server_handle_receive(SOCKET clientsock, std::string& buffer)
{
	size_t found = 0;
	std::string req = "";

	if (buffer.empty())
		return;

	if ((buffer[0] != '\\') || (buffer.find("final\\") == std::string::npos))
	{
		printf("Request %s is not valid!\n", buffer.c_str());
		return;
	}

	buffer = buffer.substr(1); // Remove \\ from the buffer

	found = buffer.find("\\\\");

	if (found == std::string::npos)
	{
		printf("Cannot retrive request!\n");
		return;
	}

	req = buffer.substr(0, found);

	if (req.compare("valid") == 0)
	{
		gamespy_valid(clientsock, buffer.substr(found + 1));
	}
	else
	{
		printf("Unknown request: %s\n", req.c_str());
	}
}