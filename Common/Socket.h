#ifndef _RSS_COMMON_SOCKET_H_
#define _RSS_COMMON_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int socket_init(void);
extern int socket_bind(int port, SOCKET* dest);
extern void socket_destroy(void);
extern int socket_recv(SOCKET sock, char* dst, int *dstlen);
extern int socket_accept(SOCKET listen, SOCKET* dest);
extern int socket_send(SOCKET sock, char *input, int length);
extern int socket_connect(const char *ip, int port, SOCKET* dest);

#ifdef __cplusplus
}
#endif

#endif