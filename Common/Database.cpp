#include "Common.h"
#include "Database.h"

#include <vector>

sqlite3 *database_sqlite = 0;

void sqlresult_free(SQLResult sql)
{
	int i = 0;

//	sql.nAffectedColumns
//	sql.nAffectedRows

	for (; i < sql.nAffectedRows; i++)
	{
		if (sql.pResult[i])
			delete[] sql.pResult[i];
	}

	delete[] sql.pResult;

	sql.nAffectedColumns = 0;
	sql.nAffectedRows = 0;
	sql.pResult = 0;
}

static int database_exec_callback(void *unused, int argc, char **argv, char **azColName)
{
	return 0;
}

int database_prepare(const char *sql, int sql_length, sqlite3_stmt **ppStmt, const char **pzTail)
{
	int result = 0;

	result = sqlite3_prepare_v2(database_sqlite, sql, sql_length, ppStmt, pzTail);
	if (result != SQLITE_OK)
	{
#ifdef _DEBUG
		printf("Error in database_prepare: sqlite3_prepare_v2 returned error: %s\n", sqlite3_errstr(result));
#endif
		return 0;
	}

	return 1;
}

int database_step(sqlite3_stmt *stmt)
{
	int result = 0;

	result = sqlite3_step(stmt);
	return result;
}

int database_open(const char *name)
{
	int rc = 0;
	char *zErrMsg = 0;

	rc = sqlite3_open(name, &database_sqlite);
	if (rc)
	{
#ifdef _DEBUG
		printf("Error in database_open: sqlite3_open returned error: %s\n", sqlite3_errmsg(database_sqlite));
#endif
		sqlite3_close(database_sqlite);
		return 0;
	}

	return 1;
}

void database_finalize(sqlite3_stmt *stmt)
{
	sqlite3_finalize(stmt);
}

void database_close()
{
	sqlite3_close(database_sqlite);
}

int database_exec(const char *query)
{
	char *zErrMsg = 0;
	int rc = 0;

	rc = sqlite3_exec(database_sqlite, query, database_exec_callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
#ifdef _DEBUG
		printf("Error in database_exec: SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return 0;
#endif
	}

//	sqlite3_free(zErrMsg);
	return 1;
}

int database_exec_count(const char *query, int *nOut)
{
	const char *tail = 0;
	sqlite3_stmt *stmt = 0;


	if (!database_prepare(query, strlen(query) + 1, &stmt, &tail))
	{
		return 0;
	}

	if (database_step(stmt) != SQLITE_ROW)
	{
		return 0;
	}

	*nOut = sqlite3_column_int(stmt, 0);

	database_finalize(stmt); // Free stmt
	return 1;
}

int database_exec_result(const char *query, SQLResult *pOut)
{
	const char *tail = 0;
	sqlite3_stmt *stmt = 0;
	int i = 0;

	std::vector<void **> vector;
	
	if (!database_prepare(query, strlen(query) + 1, &stmt, &tail))
	{
		return 0;
	}

	pOut->nAffectedColumns = sqlite3_column_count(stmt);

	while (database_step(stmt) == SQLITE_ROW)
	{
		void **pCurrent = new void *[pOut->nAffectedColumns];

		for (; i < pOut->nAffectedColumns; i++)
		{
			pCurrent[i] = (void **)sqlite3_column_blob(stmt, i);
		}
		
		vector.push_back(pCurrent);
	}

	database_finalize(stmt); // Free stmt
	i = 0;

	pOut->nAffectedRows = vector.size();
	pOut->pResult = new void **[pOut->nAffectedRows];

	for (; i < pOut->nAffectedRows; i++)
	{
		pOut->pResult[i] = vector.at(i);
	}

	vector.clear(); // Clear the vector memory

	return 1;
}

int database_is_column_null(sqlite3_stmt *stmt, int column)
{
	if (sqlite3_column_type(stmt, column) == SQLITE_NULL)
		return 1;
	
	return 0;
}

/*if (!database_prepare(query, strlen(query) + 1, &stmt, &tail))
{
#ifdef _DEBUG
printf("= Query preparation failed!\n");
#endif
socket_send(clientsock, "\\error", 7);
return;
}


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


}

*/