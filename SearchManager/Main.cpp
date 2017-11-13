#include "StdAfx.h"
#include "Server.h"

int main()
{
	printf("RetroSpy Search Manager Server v%s (Common SDK v%s)\n\n", GPSP_VERSION, COMMONSDK_VERSION);

	server_start();
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
