#ifndef _RSS_COMMON_COMMON_H_
#define _RSS_COMMON_COMMON_H_

#define COMMONSDK_VERSION "0.1"

/*
	Version 0.1
		Initial WinSock2 Handling
*/

#ifdef __cplusplus
	#define C_LINKAGE "C"
#else
	#define C_LINKAGE
#endif

#if defined(_WIN32) || defined(WIN32) || defined(WINDOWS) || defined(_WINNT) || defined(_WIN64) || defined(__WIN32__)
	#define PLATFORM_WINDOWS
#endif

#if defined(PLATFORM_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <WinSock2.h>
	#include <ws2tcpip.h>
	#pragma comment(lib, "ws2_32.lib")
#endif

#define GAMESPY_BUFLEN 1024

#include "Socket.h"

#endif