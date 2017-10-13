#include "../Common/Common.h"
#include <stdio.h>

#define GPSP_PORT 29901
#define GPSP_VERSION "0.l-INDEV"

int server_running = FALSE;

void console_pause();
void server_handle_receive(char *buffer, int buflen);

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

	while (server_running)
	{
		char *buffer;
		int buflen = 0;

		if (!socket_accept())
			continue;

		buffer = socket_recv(buflen);

		server_handle_receive(buffer, buflen);
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

void server_handle_receive(char *buffer, int buflen)
{
	char *new_buffer = NULL, *found = NULL;

	if (buflen < 1)
		return;
	if (buffer == NULL)
		return;

	if (buffer[0] != '\\')
	{
		printf("Request %s is not valid!\n", buffer);
		return;
	}

	new_buffer = buffer + 1;

	found = strstr(new_buffer, "\\\\");
}