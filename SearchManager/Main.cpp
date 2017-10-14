#include "StdAfx.h"
#include "Server.h"

int server_running = FALSE;
SOCKET listensock = INVALID_SOCKET;

void console_pause();

int main(int argc, char **argv)
{
	printf("RetroSpy Search Manager Server v%s (Common SDK v%s)\n\n", GPSP_VERSION, COMMONSDK_VERSION);
	printf("Creating Network...\n");

	if (!socket_init())
	{
		console_pause();
		return 1;
	}

	if (!socket_bind(GPSP_PORT, &listensock))
	{
		console_pause();
		return 1;
	}

	server_running = TRUE;
	printf("Server Started!\n");

	while (server_running)
	{
		SOCKET clientsock = INVALID_SOCKET;
		char *buffer;
		int buflen = 0;

		if (!socket_accept(listensock, &clientsock))
			continue;

		buffer = socket_recv(clientsock, &buflen);

		if (buflen > 0)
			server_handle_receive(clientsock, std::string(buffer));

		shutdown(clientsock, SB_BOTH);
		closesocket(clientsock);
	}

	shutdown(listensock, SB_BOTH);
	closesocket(listensock);

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