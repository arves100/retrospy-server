#ifndef _RSS_GPSP_SERVER_H_
#define _RSS_GPSP_SERVER_H_

extern void server_handle_receive(SOCKET clientsock, std::string buffer);
extern void server_start();
extern int server_init_database();

#endif
