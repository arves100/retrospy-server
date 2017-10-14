#ifndef _RSS_COMMON_SOCKET_H_
#define _RSS_COMMON_SOCKET_H_

#ifdef __cplusplus
	#define C_LINKAGE "C"
#else
	#define C_LINKAGE 
#endif

extern C_LINKAGE int socket_init(void);
extern C_LINKAGE int socket_bind(int port, SOCKET* dest);
extern C_LINKAGE void socket_destroy(void);
extern C_LINKAGE char *socket_recv(SOCKET sock, int *lenout);
extern C_LINKAGE int socket_accept(SOCKET listen, SOCKET* dest);
extern C_LINKAGE int socket_send(SOCKET sock, char *input, int length);
extern C_LINKAGE int socket_connect(const char *ip, int port, SOCKET* dest);

#endif