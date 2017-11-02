#include "Common.h"
#include "Database.hpp"

// ResultQuery Class

ResultQuery::ResultQuery(std::string query)
{
	nRows = -1;
	nColumns = -1;
	nCurrRow = 0;

	stmt = 0;

	isClosed = true;

	ResultQuery::query = query;
}

ResultQuery::~ResultQuery()
{
	vector_free();
	close();
}

void ResultQuery::close()
{
	if (isClosed)
		return;

	database_finalize(stmt);

	isClosed = true;

	nCurrRow = 0;
	nRows = -1;
	nColumns = -1;
}

bool ResultQuery::exec()
{
	const char *tail = 0;
	int nTmp = 0;

	if (query.length() < 1)
		return false;

	if (!stmt)
	{
		if (!database_prepare(query.c_str(), query.length() + 1, &stmt, &tail))
			return false;

		isClosed = false;
	}

	if (nRows == -1)
	{
		if (!database_exec_count_from_select(query, &nRows))
			return false;
	}
	
	if (nColumns == -1)
	{
		nColumns = sqlite3_column_count(stmt);
		if (nColumns < 1)
			return false;
	}

	if (isLast())
		return false;

	if (wasNull())
		return true; //Don't continue if the column is null

	if (database_step(stmt) == SQLITE_ROW)
	{
		for (; nTmp < nColumns; nTmp++)
			values.push_back(sqlite3_column_value(stmt, nTmp));

		nCurrRow++;
	}
	else
		return false; // No row

	return true;
}

void ResultQuery::vector_free()
{
	unsigned int nTmp = 0;
	for (; nTmp < values.size(); nTmp++)
	{
		if (values.at(nTmp))
			sqlite3_value_free(values.at(nTmp));
	}

	values.clear();
}

bool ResultQuery::next()
{
	vector_free();
	return exec();
}

sqlite3_value *ResultQuery::get(unsigned int columnIndex)
{
	if (isClosed)
		return 0;

	if (wasNull())
		return 0;

	if (nCurrRow == 0)
		return 0;

	if (nCurrRow > nRows)
		return 0;

	if (columnIndex > (unsigned)nColumns)
		return 0;

	if (values.size() < columnIndex)
		return 0;

	return values.at(columnIndex);
}

// Normal Database API

sqlite3 *database_sqlite = 0;

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

int database_exec_count_from_select(std::string &query, int *nOut)
{
	std::string qr2 = query;

	size_t pos = query.find("SELECT ");
	if (pos == std::string::npos)
		return 0;

	qr2 = query.replace(pos, 7, "SELECT COUNT(");

	pos = qr2.find("FROM");
	if (pos == std::string::npos)
		return 0;

	qr2 = qr2.replace(pos, 4, ") AS NumQuery FROM");
	if (pos == std::string::npos)
		return 0;

	return database_exec_count(qr2.c_str(), nOut);
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

int database_is_column_null(sqlite3_stmt *stmt, int column)
{
	if (sqlite3_column_type(stmt, column) == SQLITE_NULL)
		return 1;
	
	return 0;
}
