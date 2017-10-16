#include "StdAfx.h"
#include "GameSpy.h"

void gamespy_valid(SOCKET clientsock, std::string& buffer)
{
	char email[GP_EMAIL_LEN] = { 0 };
	char partnerid[sizeof(int)] = { 0 };
	char gamename[GAMESPY_GAMENAME_MAX] = { 0 };
	
	std::string check = "";
	size_t find = 0;

	char query[QUERY_MAX_LEN] = { 0 }, *tail = 0;
	sqlite3_stmt *stmt = 0;

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

	sprintf_s(query, QUERY_MAX_LEN, "SELECT id FROM users WHERE email=\"%s\"", email);

	if (!database_prepare(query, strlen(query) + 1, &stmt, &tail))
	{
#ifdef _DEBUG
		printf("= Query preparation failed!\n");
#endif
		socket_send(clientsock, "\\error", 7);
		return;
	}

	if (database_step(stmt) == SQLITE_ROW)
	{
		if (sqlite3_column_type(stmt, 0) != SQLITE_NULL)
		{
#ifdef _DEBUG
			printf("= Email: %s found in our database!\n", email);
#endif
			socket_send(clientsock, "\\vr\\1\\final\\", 13);
			database_finalize(stmt);
			return;
		}
	}

#ifdef _DEBUG
	printf("= Email: %s not found in our database!\n", email);
#endif

	database_finalize(stmt);
	socket_send(clientsock, "\\vr\\0\\final\\", 13);
}

void gamespy_nicks(SOCKET clientsock, std::string& buffer)
{
	char email[GP_EMAIL_LEN] = { 0 };
	char partnerid[sizeof(int)] = { 0 };
	char gamename[GAMESPY_GAMENAME_MAX] = { 0 };
	char namespaceid[sizeof(int)] = { 0 };
	char passenc[GP_PASSWORDENC_LEN] = { 0 };

	char query[QUERY_MAX_LEN] = { 0 }, *tail = 0;
	sqlite3_stmt *stmt = 0;
	char *nickname = 0, *uniquenick = 0;
	int his_id = -1;

	std::string check = "";
	size_t find = 0;

	while (TRUE)
	{
		find = buffer.find("\\");
		if (find == std::string::npos)
		{
			if (email[0] == 0 || partnerid[0] == 0 || gamename[0] == 0 || namespaceid[0] == 0 /*|| passenc[0] == 0*/)
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

	sprintf_s(query, QUERY_MAX_LEN, "SELECT id FROM users WHERE email=\"%s\"", email);

	if (passenc)
	{
		strcat_s(query, QUERY_MAX_LEN, " AND password=\"");
		strcat_s(query, QUERY_MAX_LEN, passenc);
		strcat_s(query, QUERY_MAX_LEN, "\"");
	}

	if (!database_prepare(query, strlen(query) + 1, &stmt, &tail))
	{
#ifdef _DEBUG
		printf("= Query preparation failed!\n");
#endif
		socket_send(clientsock, "\\error", 7);
		return;
	}

	if (database_step(stmt) == SQLITE_ROW)
	{
		if (sqlite3_column_type(stmt, 0) != SQLITE_NULL)
		{
			his_id = sqlite3_column_int(stmt, 0);
#ifdef _DEBUG
			printf("= Got ID: %d\n", his_id);
#endif
		}
	}

	database_finalize(stmt); // Always finalize

	if (his_id == -1)
	{
		socket_send(clientsock, "\\nr\\0\\ndone\\final\\", 19); // Send ERROR if no id exists (wrong account/check?)
		return;
	}

	memset(query, '\0', sizeof(query)); // Reset query array
	sprintf_s(query, QUERY_MAX_LEN, "SELECT nickname, unique_nickname FROM nicks WHERE acc_id=\"%d\"", his_id);

	if (!database_prepare(query, strlen(query) + 1, &stmt, &tail))
	{
#ifdef _DEBUG
		printf("= Query preparation failed!\n");
#endif
		socket_send(clientsock, "\\error", 7);
		return;
	}

	memset(query, '\0', sizeof(query)); // Reset query array
	strcpy_s(query, QUERY_MAX_LEN, "\\nr\\1\\");
	
	his_id = 0; // Use this as I++

	while (database_step(stmt) == SQLITE_ROW)
	{
		if ((sqlite3_column_type(stmt, 0) != SQLITE_NULL) || (sqlite3_column_type(stmt, 1) != SQLITE_NULL))
		{
			nickname = (char *)sqlite3_column_text(stmt, 0);
			uniquenick = (char *)sqlite3_column_text(stmt, 1);

#ifdef _DEBUG
			printf("= Got %s and %s for row %d\n", nickname, uniquenick, his_id);
#endif

			strcat_s(query, QUERY_MAX_LEN, "nick\\");
			strcat_s(query, QUERY_MAX_LEN, nickname);
			strcat_s(query, QUERY_MAX_LEN, "\\uniquenick\\");
			strcat_s(query, QUERY_MAX_LEN, uniquenick);
			strcat_s(query, QUERY_MAX_LEN, "\\");
		}
		else
		{
			break; // Reached the end
		}

		his_id++;
	}

	database_finalize(stmt); // Always finalize

	if (his_id == 0)
	{
#ifdef _DEBUG
		printf("= No profile exists\n");
#endif
		socket_send(clientsock, "\\nr\\0\\ndone\\final\\", 19); // No profile exists
		return;
	}


	strcat_s(query, QUERY_MAX_LEN, "ndone\\final\\"); // All done

	socket_send(clientsock, query, strlen(query) + 1);
}