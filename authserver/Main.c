/**
	RetroSpy Server
*/
// Windows/WinSock2 imports
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // Tell linker to link ws2_32.dll
// C Standard Imports
#include <stdio.h>

// Defines
#define GAMESPY_BUFLEN 1024
#define GAMESPY_SEARCHMANAGER

#ifdef GAMESPY_CONNECTMANAGER
	#define PORT 29900
#elif defined(GAMESPY_SEARCHMANAGER)
	#define PORT 29901
#else
	#error "Unsupported Server!"
#endif

// Variables
SOCKET ListenSocket = INVALID_SOCKET; // Listen Socket
SOCKET ClientSocket = INVALID_SOCKET; // Client Socket

// Function Prototypes
void consolepause();
int ws2_init();
void ws2_destroy();
char* socket_recv(int *lenout);
int socket_accept();
int socket_send(char *input, int length);
void server_do();

// Entrypoint
int main(int argc, char** argv)
{
	if (!ws2_init()) // Initialize Winsock2
	{
		consolepause();
		return 1;
	}

	printf("Successfully initialized WinSock2\n");

	server_do();

	printf("Bye bye\n");
	ws2_destroy(); // Deinitialize Winsock2
	consolepause(); // Wait for user import
	return 0;
}

void server_do()
{
	printf("RetroSpy Auth Server (GPCM)\n");
	printf("Waiting for client...\n");

//	while (socket_accept());

	socket_accept();

#ifdef GAMESPY_CONNECTMANAGER
	printf("Sending lc challenge...\n");
	char *lc_challenge = "\\lc\\1\\challenge\\FPHZLVNZLH\\id\\1\\final\\";
	socket_send(lc_challenge, strlen(lc_challenge));
#endif

	printf("Activated \"logger\" mode\n");
	while (1)
	{
		char *recv;
		int recvl = 0;

		recv = socket_recv(&recvl);

		if (recvl <= 0)
			break;

		printf("Recv %s (len: %d)\n", recv, recvl);
	}
}

void consolepause()
{
	printf("Press ENTER to continue...");
	char ch = 0;
	scanf_s("%1c", &ch, 1); // Windows :/
}

int ws2_init()
{
	WSADATA wsaData;
	int iResult = 0;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	char real_port[6] = { 0 };
	sprintf_s(real_port, 6, "%d", PORT);
	
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // Initalize WS2
	if (iResult != 0)
	{
		printf("Error in ws2_init: WSAStartup returned %d\n", iResult);
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
		printf("Error in ws2_init: getaddrinfo returned %d\n", iResult);
		WSACleanup();
		return 0;
	}

	// Create the listen socket
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("Error in ws2_init: socket returned %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 0;
	}

	// Bind the socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("Error in ws2_init: bind returned %d\n", iResult);
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 0;
	}

	freeaddrinfo(result);

	// Start listening on the socket
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("Error in ws2_init: listen returned %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 0;
	}

	return 1;
}

void ws2_destroy()
{
	closesocket(ClientSocket);
	closesocket(ListenSocket); // Close the listensocket
	WSACleanup(); // Destroy WS2
}

int socket_accept()
{
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
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

	*lenout = recv(ClientSocket, recvbuf, recvbuflen, 0);
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
	iResult = send(ClientSocket, input, length, 0);
	if (iResult == SOCKET_ERROR)
	{
		printf("Error in socket_send: send error: %d\n", WSAGetLastError());
		return 1;
	}

	printf("socket_recv: sended %d bytes\n", length);
	return 0;
}