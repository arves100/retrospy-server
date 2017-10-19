#include "StdAfx.h"
#include "GameSpy.h"

int gamespy_buffer_validate(std::string &buffer)
{
	size_t pos = buffer.find("\\final\\");
	if (pos == std::string::npos) // Dosen't exists
		return 0;

	return 1;
}

std::string gamespy_buffer_get(std::string &buffer, std::string what)
{
	size_t find = 0;
	std::string substr_buffer = "";

	find = buffer.find(what + "\\");
	if (find == std::string::npos)
		return "";

	substr_buffer = buffer.substr(find + what.size() + 1);

	find = substr_buffer.find("\\");
	if (find == std::string::npos)
		return "";

	return substr_buffer.substr(0, find);
}

void gamespy_valid(SOCKET clientsock, std::string& buffer)
{
	char email[GP_EMAIL_LEN] = { 0 };
	int partnerid = 0;
	char gamename[GAMESPY_GAMENAME_MAX] = { 0 };
	
	char query[QUERY_MAX_LEN] = { 0 };
	SQLResult sResult = { 0 };

	if (!gamespy_buffer_validate(buffer))
	{
#ifdef _DEBUG
		printf("= Error: request is invalid!\n");
#endif
	}

	strcpy_s(email, GP_EMAIL_LEN, gamespy_buffer_get(buffer, "email").c_str());
	partnerid = std::stoi(gamespy_buffer_get(buffer, "partnerid"));
	strcpy_s(gamename, GAMESPY_GAMENAME_MAX, gamespy_buffer_get(buffer, "gamename").c_str());

	if (strcmp(email, "") == 0)
		return; // Invalid email

#ifdef _DEBUG
	printf("= Successfully received valid request:\n\tEmail: %s\n\tPartnerID: %d\n\tGameName: %s\n", email, partnerid, gamename);
#endif

	sprintf_s(query, QUERY_MAX_LEN, "SELECT id FROM users WHERE email=\"%s\"", email);

	if (!database_exec_result(query, &sResult))
	{
		return;
	}

	if (sResult.nAffectedRows > 0)
		socket_send(clientsock, "\\vr\\1\\final\\", 13);
	else
		socket_send(clientsock, "\\vr\\0\\final\\", 13);

	sqlresult_free(sResult);
}

void gamespy_nicks(SOCKET clientsock, std::string& buffer)
{
	int userid = 0;
	char email[GP_EMAIL_LEN] = { 0 };
	int partnerid = 0;
	char gamename[GAMESPY_GAMENAME_MAX] = { 0 };
	int namespaceid = 0;
	char passenc[GP_PASSWORDENC_LEN] = { 0 };
	char nickname[GP_NICK_LEN] = { 0 };
	char uniquenick[GP_UNIQUENICK_LEN] = { 0 };

	char query[QUERY_MAX_LEN] = { 0 };
	SQLResult sResult = { 0 };
	
	strcpy_s(email, GP_EMAIL_LEN, gamespy_buffer_get(buffer, "email").c_str());
	strcpy_s(gamename, GAMESPY_GAMENAME_MAX, gamespy_buffer_get(buffer, "gamename").c_str());
	strcpy_s(passenc, GP_PASSWORDENC_LEN, gamespy_buffer_get(buffer, "passenc").c_str());
	partnerid = std::stoi(gamespy_buffer_get(buffer, "partnerid"));
	namespaceid = std::stoi(gamespy_buffer_get(buffer, "namespaceid"));

#ifdef _DEBUG
	printf("= Successfully received nicks request:\n\tEmail: %s\n\tPassEnc: %s\n\tNameSpaceID: %d\n\tPartnerID: %d\n\tGameName: %s\n", email, passenc, namespaceid, partnerid, gamename);
#endif

	sprintf_s(query, QUERY_MAX_LEN, "SELECT id FROM users WHERE email=\"%s\"", email);

	if (passenc)
	{
		strcat_s(query, QUERY_MAX_LEN, " AND password=\"");
		strcat_s(query, QUERY_MAX_LEN, passenc);
		strcat_s(query, QUERY_MAX_LEN, "\"");
	}

	if (!database_exec_result(query, &sResult))
	{
		return;
	}

	if (sResult.nAffectedRows < 1)
	{
#ifdef _DEBUG
		printf("= No nicknames found\n");
#endif
		socket_send(clientsock, "\\nr\\0\\ndone\\final\\", 19); // Send ERROR if no nick exists
		return;
	}

	printf("Affected Col: %d\n", sResult.nAffectedColumns);

	userid = (int)sResult.pResult[0][0]; // Row 0: Column 0

	if (userid == 0)
	{
		socket_send(clientsock, "\\nr\\0\\ndone\\final\\", 19); // Send ERROR if no id exists (wrong account/check?)
		return;
	}

	sqlresult_free(sResult);
	memset(query, '\0', sizeof(query)); // Reset query array

	printf("= ID Found: %d\n", userid);

	sprintf_s(query, QUERY_MAX_LEN, "SELECT nickname, unique_nickname FROM nicks WHERE acc_id=\"%d\"", userid);

	if (!database_exec_result(query, &sResult))
	{
		socket_send(clientsock, "\\error", 7);
		return;
	}

	memset(query, '\0', sizeof(query)); // Reset query array

	if (sResult.nAffectedRows < 1)
	{
#ifdef _DEBUG
		printf("= No profile exists\n");
#endif
		socket_send(clientsock, "\\nr\\0\\ndone\\final\\", 19); // No profile exists

		sqlresult_free(sResult); // Free sqlresult
		return;
	}

	strcpy_s(query, QUERY_MAX_LEN, "\\nr\\1\\");

	userid = 0; // Use this as count variable

	while (userid < sResult.nAffectedRows)
	{
		strcpy_s(nickname, GP_NICK_LEN, (char *)sResult.pResult[userid][0]);
		strcpy_s(uniquenick, GP_UNIQUENICK_LEN, (char *)sResult.pResult[userid][1]);


#ifdef _DEBUG
		printf("= Got %s and %s for row %d\n", nickname, uniquenick, userid);
#endif

		strcat_s(query, QUERY_MAX_LEN, "nick\\");
		strcat_s(query, QUERY_MAX_LEN, nickname);
		strcat_s(query, QUERY_MAX_LEN, "\\uniquenick\\");
		strcat_s(query, QUERY_MAX_LEN, uniquenick);
		strcat_s(query, QUERY_MAX_LEN, "\\");

		userid++;
	}

	sqlresult_free(sResult);

	strcat_s(query, QUERY_MAX_LEN, "ndone\\final\\"); // All done

	socket_send(clientsock, query, strlen(query) + 1);
}

void gamespy_search(SOCKET clientsock, std::string& buffer)
{
#if 0
	char sesskey[sizeof(int)] = { 0 };
	char profileid[sizeof(int)] = { 0 };
	char namespaceid[sizeof(int)] = { 0 };
	char partnerid[sizeof(int)] = { 0 };

	// Optional Parametra
	char nickname[GP_NICK_LEN] = { 0 }; //nick
	char uniqnick[GP_UNIQUENICK_LEN] = { 0 }; //uniquenick
	char email[GP_EMAIL_LEN] = { 0 };
	char firstname[GP_FIRSTNAME_LEN] = { 0 };
	char lastname[GP_LASTNAME_LEN] = { 0 };
	char icquin[sizeof(int)] = { 0 };
	char skip[sizeof(int)] = { 0 };
	
	// End Parametra
	char gamename[GAMESPY_GAMENAME_MAX] = { 0 };
	
	/*
		Request: \search\\
		Response: \bsr\nick\<nickname>\uniquenick\<unickname>\namespaceid\<same as req>\firstname\<fname>\lastname\<lname>\email\<email>
				  \bsr\.............
				  \bsrdone\final\

				  // WIP: Check what "more" is and handle it
				  // WIP2: Check what skip is and handle it
				  \bsrdone\more\0\final\
				         or
			       \bsrdone\more\1\final\
	
	*/
	
	

	char query[QUERY_MAX_LEN] = { 0 }, *tail = 0;
	sqlite3_stmt *stmt = 0;

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
#endif

}
