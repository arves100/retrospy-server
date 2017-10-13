#include "Common.h"

#ifdef PLATFORM_WINDOWS

SOCKET socket_listensocket = INVALID_SOCKET, socket_clientsocket = INVALID_SOCKET;

int socket_init(int port)
{
	WSADATA wsaData;
	int iResult = 0;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	char real_port[6] = { 0 };

	sprintf_s(real_port, 6, "%d", port);

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // Initalize WS2
	if (iResult != 0)
	{
		printf("Error in socket_init: WSAStartup returned %d\n", iResult);
		return 0;
	}

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
		printf("Error in socket_init: getaddrinfo returned %d\n", iResult);
		WSACleanup();
		return 0;
	}

	// Create the listen socket
	socket_listensocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (socket_listensocket == INVALID_SOCKET)
	{
		printf("Error in socket_init: socket returned %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 0;
	}

	// Bind the socket
	iResult = bind(socket_listensocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("Error in socket_init: bind returned %d\n", iResult);
		freeaddrinfo(result);
		closesocket(socket_listensocket);
		WSACleanup();
		return 0;
	}

	freeaddrinfo(result);

	// Start listening on the socket
	if (listen(socket_listensocket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("Error in socket_init: listen returned %ld\n", WSAGetLastError());
		closesocket(socket_listensocket);
		WSACleanup();
		return 0;
	}

	return 1;
}

void socket_destroy()
{
	closesocket(socket_clientsocket);
	closesocket(socket_listensocket); // Close the listensocket
	WSACleanup(); // Destroy WS2
}

int socket_accept()
{
	socket_clientsocket = accept(socket_listensocket, NULL, NULL);
	if (socket_clientsocket == INVALID_SOCKET)
	{
		printf("Error in socket_accept: accept returned %ld\n", WSAGetLastError());
		return 0;
	}

	return 1;
}

char* socket_recv(int *lenout)
{
	char recvbuf[GAMESPY_BUFLEN] = { 0 };
	int recvbuflen = GAMESPY_BUFLEN;

	*lenout = recv(socket_clientsocket, recvbuf, recvbuflen, 0);
	if (*lenout > 0)
	{
		printf("socket_recv: received %d bytes\n", *lenout);
		return recvbuf;
	}

	if (*lenout == 0)
		printf("socket_recv: client disconnected\n");
	else
		printf("Error in socket_recv: recv error: %d\n", WSAGetLastError());

	return NULL;
}

int socket_send(char *input, int length)
{
	int iResult;
	iResult = send(socket_clientsocket, input, length, 0);
	if (iResult == SOCKET_ERROR)
	{
		printf("Error in socket_send: send error: %d\n", WSAGetLastError());
		return 1;
	}

	printf("socket_recv: sended %d bytes\n", length);
	return 0;
}
#endif
