#ifndef _RSS_COMMON_COMMON_H_
#define _RSS_COMMON_COMMON_H_

#define COMMONSDK_VERSION "0.1.3"

#if defined(_WIN32) || defined(WIN32) || defined(WINDOWS) || defined(_WINNT) || defined(_WIN64) || defined(__WIN32__)
	#define PLATFORM_WINDOWS
#else
	#error "Unsupported platform!"
#endif

#if defined(PLATFORM_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <WinSock2.h>
	#include <ws2tcpip.h>
	#pragma comment(lib, "ws2_32.lib")
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlite3.h"

#define GAMESPY_BUFLEN 1024
#define GAMESPY_GAMENAME_MAX 64
#define QUERY_MAX_LEN 1024

#endif