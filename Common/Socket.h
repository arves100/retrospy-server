#ifndef _RSS_COMMON_SOCKET_H_
#define _RSS_COMMON_SOCKET_H_

extern C_LINKAGE int socket_init(port);
extern C_LINKAGE void socket_destroy();
extern C_LINKAGE char *socket_recv(int *lenout);
extern C_LINKAGE int socket_accept();
extern C_LINKAGE int socket_send(char *input, int length);

#endif