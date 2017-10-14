#include "Common.h"
#include "Socket.h"

int socket_init()
{
	int iResult = 0;
	WSADATA wsaData;
	
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // Initalize WS2
	if (iResult != 0)
	{
		printf("Error in socket_init: WSAStartup returned %d\n", iResult);
		return 0;
	}
	
	return 1;
}

int socket_connect(const char *ip, int port, SOCKET* dest)
{
	char sport[6] = { 0 };
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	int ir = 0;
	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	
	sprintf_s(sport, 6, "%d", port);

	ir = getaddrinfo(ip, sport, &hints, &result);

	if (ir != 0) {
		return 0;
	}

	ptr = result;

	*dest = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (*dest == INVALID_SOCKET) {
		return 0;
	}

	ir = connect(*dest, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (ir == SOCKET_ERROR || *dest == INVALID_SOCKET) {
		closesocket(*dest);
		freeaddrinfo(result);
		return 0;
	}

	freeaddrinfo(result);

	return 1;
}

int socket_bind(int port, SOCKET* dest)
{
	int iResult = 0;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	char real_port[6] = { 0 };

	sprintf_s(real_port, 6, "%d", port);

	// Setup listensocket structure
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET; //IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP; //TCP
	hints.ai_flags = AI_PASSIVE;

	//Resolve address
	iResult = getaddrinfo(NULL, real_port, &hints, &result);
	if (iResult != 0)
	{
		printf("Error in socket_bind: getaddrinfo returned %d\n", iResult);
		WSACleanup();
		return 0;
	}

	// Create the listen socket
	*dest = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (*dest == INVALID_SOCKET)
	{
		printf("Error in socket_bind: socket returned %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		return 0;
	}

	// Bind the socket
	iResult = bind(*dest, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("Error in socket_bind: bind returned %d\n", iResult);
		freeaddrinfo(result);
		closesocket(*dest);
		return 0;
	}

	freeaddrinfo(result);

	// Start listening on the socket
	if (listen(*dest, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("Error in socket_bind: listen returned %ld\n", WSAGetLastError());
		closesocket(*dest);
		return 0;
	}

	return 1;
}

void socket_destroy()
{
	WSACleanup(); // Destroy WS2
}

int socket_accept(SOCKET listen, SOCKET *dest)
{
	*dest = accept(listen, NULL, NULL);
	if (*dest == INVALID_SOCKET)
	{
		printf("Error in socket_accept: accept returned %ld\n", WSAGetLastError());
		return 0;
	}

	return 1;
}

int socket_recv(SOCKET sock, char* dst, int *dstlen)
{
	*dstlen = recv(sock, dst, GAMESPY_BUFLEN, 0);
	if (*dstlen > 0)
	{
		printf("socket_recv: received %d bytes\n", *dstlen);
		return TRUE;
	}

	if (*dstlen == 0)
		printf("socket_recv: client disconnected\n");
	else
		printf("Error in socket_recv: recv error: %d\n", WSAGetLastError());

	return FALSE;
}

int socket_send(SOCKET sock, char *input, int length)
{
	int iResult;
	iResult = send(sock, input, length, 0);
	if (iResult == SOCKET_ERROR)
	{
		printf("Error in socket_send: send error: %d\n", WSAGetLastError());
		return 1;
	}

	printf("socket_send: sended %d bytes\n", length);
	return 0;
}
