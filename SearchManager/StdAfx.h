#ifndef _RSS_GPSP_STDAFX_H_
#define _RSS_GPSP_STDAFX_H_

#define GPSP_VERSION "0.1.2"
/*
	Version 0.1.2
		database
	Version 0.1.1
		nicks handling
	Version 0.1
		valid handling
*/

#include "../Common/Common.h"
#include "../Common/Socket.h"
#include "../Common/Database.h"

#include <string>

#define GPSP_PORT 29901

extern int server_running;
extern void console_pause();

#define GP_EMAIL_LEN				51
#define GP_PASSWORD_LEN             31
#define GP_PASSWORDENC_LEN          ((((GP_PASSWORD_LEN+2)*4)/3)+1)
#define GP_NICK_LEN                 31
#define GP_UNIQUENICK_LEN           21

#endif