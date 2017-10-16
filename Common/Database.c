#include "Common.h"
#include "Database.h"
//#include "../Common/sqlite3.h"

sqlite3 *database_sqlite = 0;

static int database_exec_callback(void *unused, int argc, char **argv, char **azColName)
{
	int i;

	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}

int database_prepare(const char *sql, int sql_length, sqlite3_stmt **ppStmt, char **pzTail)
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
