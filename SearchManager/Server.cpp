#include "StdAfx.h"
#include "Server.h"
#include "GameSpy.h"

int server_running = FALSE;

void server_start()
{
	SOCKET listensock = INVALID_SOCKET;

	printf("Creating Network...\n");

	if (!socket_init())
	{
		console_pause();
		return;
	}

	if (!socket_bind(GPSP_PORT, &listensock))
	{
		console_pause();
		return;
	}

	server_running = TRUE;
	printf("Server Started!\n");

	while (server_running)
	{
		SOCKET clientsock = INVALID_SOCKET;
		char buffer[GAMESPY_BUFLEN] = { 0 };
		int buflen = 0;

		if (!socket_accept(listensock, &clientsock))
			continue;

		if (!socket_recv(clientsock, buffer, &buflen))
			continue;

		if (buflen > 0)
			server_handle_receive(clientsock, std::string(buffer));

		shutdown(clientsock, SB_BOTH);
		closesocket(clientsock);
	}

	shutdown(listensock, SB_BOTH);
	closesocket(listensock);

	socket_destroy();
}

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