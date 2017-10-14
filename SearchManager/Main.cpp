#include "StdAfx.h"
#include "GameSpy.h"

int server_running = FALSE;

void console_pause();
void server_handle_receive(std::string& buffer);

int main(int argc, char **argv)
{
	printf("RetroSpy Search Manager Server v%s (Common SDK v%s)\n\n", GPSP_VERSION, COMMONSDK_VERSION);
	printf("Creating Network...\n");
	if (!socket_init(GPSP_PORT))
	{
		console_pause();
		return 1;
	}

	server_running = TRUE;
	printf("Server Started!\n");

	while (server_running)
	{
		char *buffer;
		int buflen = 0;

		if (!socket_accept())
			continue;

		buffer = socket_recv(&buflen);

		if (buflen > 0)
			server_handle_receive(std::string(buffer));
	}

	socket_destroy();
	console_pause();
	return 0;
}

void console_pause()
{
	printf("Press ENTER to continue...");
	char ch = 0;

#ifdef PLATFORM_WINDOWS
	scanf_s("%1c", &ch, 1);
#else
	scanf("%1c", &ch);
#endif
}

void server_handle_receive(std::string& buffer)
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
		gamespy_valid(buffer.substr(found + 1));
	}
	else
	{
		printf("Unknown request: %s\n", req.c_str());
	}
}