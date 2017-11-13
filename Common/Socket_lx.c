#include "Common.h"
#include "Socket.h"

#ifdef PLATFORM_LINUX

int socket_init(void)
{
	// Linux dosen't have a Socket initialization like WinSock2
	return 1;
}

int socket_bind(int port, SOCKET* dest)
{
	struct sockaddr_in serv_addr;
	int iResult;
	
	*dest = socket(AF_INET, SOCK_STREAM, 0);
	if (*dest < 0)
	{
#ifdef _DEBUG
		printf("Error in socket_bind: socket returned %d\n", *dest);
#endif
		return 0;
	}

    bzero((char *) &serv_addr, sizeof(serv_addr)); //ZeroMemory
	
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
	iResult = bind(*dest, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if (iResult < 0)
	{
#ifdef _DEBUG
		printf("Error in socket_bind: bind returned %d\n", iResult);
#endif
		socket_close(*dest);
		return 0;
	}
    
	iResult = listen(*dest, 5);
	
    if (iResult != 0)
	{
#ifdef _DEBUG
		printf("Error in socket_bind: listen returned %d\n", iResult);
#endif
		socket_close(*dest);
		return 0;		
	}
	
	return 1;
}

void socket_destroy(void) {} //Useless function since linux dosen't have a socket library destroy

void socket_close(SOCKET sock)
{
	close(sock);
	sock = 0;
}

int socket_recv(SOCKET sock, char* dst, int *dstlen)
{
	*dstlen = read(sock, dst, GAMESPY_BUFLEN);
	if (*dstlen < 0)
	{
#ifdef _DEBUG
		printf("Error in socket_recv: read returned %d\n", *dstlen);
#endif
		return 0;
	}
	
	return 1;
}

int socket_accept(SOCKET listen, SOCKET* dest)
{
	socklen_t clilen;
	struct sockaddr_in cli_addr;
	
	clilen = sizeof(cli_addr);
	
    *dest = accept(listen, (struct sockaddr *) &cli_addr, &clilen);
    if (*dest < 0)
	{
#ifdef _DEBUG
		printf("Error in socket_accept: accept returned %d\n", *dest);
#endif
		return 0;
	}
	
	return 1;
}

int socket_send(SOCKET sock, char *input, int length)
{
	int n;
	
	n = write(sock, input, length);
	if (n < 0)
	{
#ifdef _DEBUG
		printf("Error in socket_send: write returned %d\n", n);
#endif
		return 0;
	}

	return 1;
}

int socket_connect(const char *ip, int port, SOCKET* dest)
{
    struct hostent *server;
	struct sockaddr_in serv_addr;
	int n;
	
    *dest = socket(AF_INET, SOCK_STREAM, 0);
    if (*dest < 0)
	{
#ifdef _DEBUG
		printf("Error in socket_connect: socket returned %d\n", *dest);
#endif
		return 0;
	}

    server = gethostbyname(ip);
    if (server == NULL) {
#ifdef _DEBUG
		printf("Error in socket_connect: no such host %s\n", ip);
#endif
		socket_close(*dest);
		return 0;
    }
	
    bzero((char *) &serv_addr, sizeof(serv_addr)); //ZeroMemory
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); //memcpy
    serv_addr.sin_port = htons(port);
    
	n = connect(*dest,(struct sockaddr *) &serv_addr,sizeof(serv_addr));
	if (n < 0)
	{
#ifdef _DEBUG
		printf("Error in socket_connect: connect returned %d\n", n);
#endif
		return 0;
	}
	
	return 1;
}

#endif
