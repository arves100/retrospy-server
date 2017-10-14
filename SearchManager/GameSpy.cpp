#include "StdAfx.h"
#include "GameSpy.h"

void gamespy_valid(SOCKET clientsock, std::string& buffer)
{
	char email[GP_EMAIL_LEN] = { 0 };
	char partnerid[sizeof(int)] = { 0 };
	char gamename[GAMESPY_GAMENAME_MAX] = { 0 };
	
	std::string check = "";
	size_t find = 0;

	while (TRUE)
	{
		find = buffer.find("\\");
		if (find == std::string::npos)
		{
			if (email[0] == 0 || partnerid[0] == 0 || gamename[0] == 0)
			{
#ifdef _DEBUG
				printf("= Error: valid request is invalid!\n");
#endif
				return;
			}
			else
				break;
		}

		check = buffer.substr(0, find);
		buffer = buffer.substr(find + 1);

		if (check.compare("email") == 0)
		{
			find = buffer.find("\\");
			if (find == std::string::npos)
			{
#ifdef _DEBUG
				printf("= Error: Cannot get Email!\n");
#endif
				return;
			}

			strcpy_s(email, GP_EMAIL_LEN, buffer.substr(0, find).c_str());
		}
		else if (check.compare("partnerid") == 0)
		{
			find = buffer.find("\\");
			if (find == std::string::npos)
			{
#ifdef _DEBUG
				printf("= Error: Cannot get PartnerID!\n");
#endif
				return;
			}

			strcpy_s(partnerid, sizeof(int), buffer.substr(0, find).c_str());
		}
		else if (check.compare("gamename") == 0)
		{
			find = buffer.find("\\");
			if (find == std::string::npos)
			{
#ifdef _DEBUG
				printf("= Error: Cannot get GameName!\n");
#endif
				return;
			}

			strcpy_s(gamename, GAMESPY_GAMENAME_MAX, buffer.substr(0, find).c_str());
		}
	}

#ifdef _DEBUG
	printf("= Successfully received valid request:\n\tEmail: %s\n\tPartnerID: %s\n\tGameName: %s\n", email, partnerid, gamename);
#endif

	socket_send(clientsock, "\\vr\\1\\final\\", 13); // Send OK!
}

void gamespy_nicks(SOCKET clientsock, std::string& buffer)
{
	char email[GP_EMAIL_LEN] = { 0 };
	char partnerid[sizeof(int)] = { 0 };
	char gamename[GAMESPY_GAMENAME_MAX] = { 0 };
	char namespaceid[sizeof(int)] = { 0 };
	char passenc[GP_PASSWORDENC_LEN] = { 0 };

	std::string check = "";
	size_t find = 0;

	while (TRUE)
	{
		find = buffer.find("\\");
		if (find == std::string::npos)
		{
			if (email[0] == 0 || partnerid[0] == 0 || gamename[0] == 0 || namespaceid[0] == 0 || passenc[0] == 0)
			{
#ifdef _DEBUG
				printf("= Error: nicks request is invalid!\n");
#endif
				return;
			}
			else
				break;
		}

		check = buffer.substr(0, find);
		buffer = buffer.substr(find + 1);

		if (check.compare("email") == 0)
		{
			find = buffer.find("\\");
			if (find == std::string::npos)
			{
#ifdef _DEBUG
				printf("= Error: Cannot get Email!\n");
#endif
				return;
			}

			strcpy_s(email, GP_EMAIL_LEN, buffer.substr(0, find).c_str());
		}
		else if (check.compare("partnerid") == 0)
		{
			find = buffer.find("\\");
			if (find == std::string::npos)
			{
#ifdef _DEBUG
				printf("= Error: Cannot get PartnerID!\n");
#endif
				return;
			}

			strcpy_s(partnerid, sizeof(int), buffer.substr(0, find).c_str());
		}
		else if (check.compare("gamename") == 0)
		{
			find = buffer.find("\\");
			if (find == std::string::npos)
			{
#ifdef _DEBUG
				printf("= Error: Cannot get GameName!\n");
#endif
				return;
			}

			strcpy_s(gamename, GAMESPY_GAMENAME_MAX, buffer.substr(0, find).c_str());
		}
		else if (check.compare("namespaceid") == 0)
		{
			find = buffer.find("\\");
			if (find == std::string::npos)
			{
#ifdef _DEBUG
				printf("= Error: Cannot get NameSpaceID!\n");
#endif
				return;
			}

			strcpy_s(namespaceid, sizeof(int), buffer.substr(0, find).c_str());
		}
		else if (check.compare("passenc") == 0)
		{
			find = buffer.find("\\");
			if (find == std::string::npos)
			{
#ifdef _DEBUG
				printf("= Error: Cannot get PassEnc!\n");
#endif
				return;
			}

			strcpy_s(passenc, GP_PASSWORDENC_LEN, buffer.substr(0, find).c_str());
		}
	}

#ifdef _DEBUG
	printf("= Successfully received nicks request:\n\tEmail: %s\n\tPassEnc: %s\n\tNameSpaceID: %s\n\tPartnerID: %s\n\tGameName: %s\n", email, passenc, namespaceid, partnerid, gamename);
#endif

	socket_send(clientsock, "\\error\\", 8);
}