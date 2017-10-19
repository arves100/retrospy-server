#include "StdAfx.h"
#include "Server.h"
#include "GameSpy.h"

int server_running = FALSE;

int server_init_database()
{
	char buffer[QUERY_MAX_LEN] = { 0 };
	
	if (!database_exec("SELECT id FROM users"))
	{
		sprintf_s(buffer, QUERY_MAX_LEN, "CREATE TABLE IF NOT EXISTS users(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, email VARCHAR(%d) NOT NULL, password VARCHAR(%d) NOT NULL)", GP_EMAIL_LEN, GP_PASSWORDENC_LEN);

		if (!database_exec(buffer))
			return 0;

	}

	if (!database_exec("SELECT id FROM nicks"))
	{
		sprintf_s(buffer, QUERY_MAX_LEN, "CREATE TABLE IF NOT EXISTS nicks(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, acc_id INTEGER NOT NULL, nickname VARCHAR(%d) NOT NULL, unique_nickname VARCHAR(%d) NOT NULL)", GP_NICK_LEN, GP_UNIQUENICK_LEN);

		if (!database_exec(buffer))
			return 0;
	}

	return 1;
}

void server_start()
{
	SOCKET listensock = INVALID_SOCKET;

	printf("Creating Network...\n");

	if (!socket_init())
	{
		printf("Error: Cannot initialize Socket\n");
		return;
	}

	if (!socket_bind(GPSP_PORT, &listensock))
	{
		printf("Error: Cannot bind to 0.0.0.0:%d (Is the port busy?)\n", GPSP_PORT);
		socket_destroy();
		return;
	}

	printf("Opening Database...\n");
	if (!database_open(database_name))
	{
		printf("Error: Cannot open database \"gamespy.db\"\n");
		socket_destroy();
		return;
	}

	if (!server_init_database())
	{
		printf("Error: Cannot initalize database\n");
		database_close();
		socket_destroy();
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
		{
#ifdef _DEBUG
			printf("= Socket rejected by Server\n");
#endif
			continue;
		}

#ifdef _DEBUG
		printf("= Accpeted Socket\n");
#endif

		if (!socket_recv(clientsock, buffer, &buflen))
		{
#ifdef _DEBUG
			printf("= Could not receive from Socket\n");
#endif
			shutdown(clientsock, SB_BOTH);
			closesocket(clientsock);
			continue;
		}

		if (buflen > 0)
			server_handle_receive(clientsock, std::string(buffer));
#ifdef _DEBUG
		else
			printf("= Socket didn't do nothing\n");
#endif

		shutdown(clientsock, SB_BOTH);
		closesocket(clientsock);
	}

	shutdown(listensock, SB_BOTH);
	closesocket(listensock);

	database_close();

	socket_destroy();
}

void server_handle_receive(SOCKET clientsock, std::string& buffer)
{
	size_t found = 0;
	std::string req = "";

	if (buffer.empty())
		return;

#ifdef _DEBUG
	printf("= Received %s from Socket\n", buffer.c_str());
#endif

	if ((buffer[0] != '\\') || (buffer.find("final\\") == std::string::npos))
	{
#ifdef _DEBUG
		printf("= Request %s is not valid!\n", buffer.c_str());
#endif
		return;
	}

	buffer = buffer.substr(1); // Remove \\ from the buffer

	found = buffer.find("\\\\");

	if (found == std::string::npos)
	{
#ifdef _DEBUG
		printf("= Cannot retrive request!\n");
#endif
		return;
	}

	req = buffer.substr(0, found);

	if (req.compare("valid") == 0)
	{
		gamespy_valid(clientsock, buffer.substr(found + 1));
	}
	else if (req.compare("nicks") == 0)
	{
		gamespy_nicks(clientsock, buffer.substr(found + 1));
	}
	else if (req.compare("search") == 0)
	{
		gamespy_search(clientsock, buffer.substr(found + 1));
	}
	else
	{
#ifdef _DEBUG
		printf("= Unknown request: %s\n", req.c_str());
#endif
	}
}