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
			printf("Error: this VALID request is invalid!\n");
			return;
		}

		check = buffer.substr(0, find);
		buffer = buffer.substr(find + 1);

		if (check.compare("email") == 0)
		{
			find = buffer.find("\\");
			if (find == std::string::npos)
			{
				printf("Error while getting Email!\n");
				return;
			}

			strcpy_s(email, GP_EMAIL_LEN, buffer.substr(0, find).c_str());
		}
		else if (check.compare("partnerid") == 0)
		{
			find = buffer.find("\\");
			if (find == std::string::npos)
			{
				printf("Error while getting PartnerID!\n");
				return;
			}

			strcpy_s(partnerid, sizeof(int), buffer.substr(0, find).c_str());
		}
		else if (check.compare("gamename") == 0)
		{
			find = buffer.find("\\");
			if (find == std::string::npos)
			{
				printf("Error while getting GameName!\n");
				return;
			}

			strcpy_s(gamename, GAMESPY_GAMENAME_MAX, buffer.substr(0, find).c_str());
		}
	}

	printf("=> GOT EMAIL: %s\n=> GOT PARTNERID: %s\n=> GOT GAMENAME: %s\n", email, partnerid, gamename);

	socket_send(clientsock, "\\vr\\1\\final\\", 13); // Send OK!
}