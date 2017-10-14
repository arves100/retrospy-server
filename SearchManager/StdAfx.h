#ifndef _RSS_GPSP_STDAFX_H_
#define _RSS_GPSP_STDAFX_H_

#define GPSP_VERSION "0.1"
/*
	Version 0.1
		valid handling
*/

#include "../Common/Common.h"
#include "../Common/Socket.h"

#include <string>

#define GPSP_PORT 29901

extern int server_running;
extern void console_pause();

#define GP_EMAIL_LEN 51

#endif