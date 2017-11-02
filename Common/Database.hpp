#ifndef _RSS_COMMON_DATABASE_HPP_
#define _RSS_COMMON_DATABASE_HPP_

#include <string>
#include <vector>
#include "sqlite3.h"

// Use ResultQuery if you want to execute "SELECT" queries
class ResultQuery
{
public:
	ResultQuery(std::string query);
	~ResultQuery();

	bool next();

	bool isFirst() { return nCurrRow == 1 ? true : false; }
	bool isLast() { return nCurrRow == nRows ? true : false; }

	int getRow() { return nCurrRow; }

	void close();
	bool isClose() { return isClosed; }

	bool wasNull() { return nRows > 0 ? false : true; }

	std::string getString(unsigned int columnIndex) { return std::string((char *)sqlite3_value_text(get(columnIndex))); }
	bool getBoolean(unsigned int columnIndex) { return getInt(columnIndex) > 0 ? true : false; }
	int getInt(unsigned int columnIndex) { return sqlite3_value_int(get(columnIndex)); }
	void *getPointer(unsigned int columnIndex, const char* type) { return sqlite3_value_pointer(get(columnIndex), type); }
	double getDouble(unsigned int columnIndex) { return sqlite3_value_double(get(columnIndex)); }

	int getAffectedColumns() { return nColumns; }
	int getAffectedRows() { return nRows; }

	std::string getQuery() { return query; }
private:
	int nRows;
	int nColumns;
	int nCurrRow;

	std::vector<sqlite3_value *> values;
	bool isClosed;

	std::string query;
	sqlite3_stmt *stmt;

	void vector_free();
	bool exec();

	sqlite3_value *get(unsigned int columnIndex);
};

extern int database_exec_count(const char *query, int *nOut);
extern int database_exec_count_from_select(std::string &query, int *nOut);

extern int database_is_column_null(sqlite3_stmt *stmt, int column);

extern int database_open(const char *name);
extern void database_close();
extern int database_exec(const char *query);

extern void database_finalize(sqlite3_stmt *stmt);
extern int database_prepare(const char *sql, int sql_length, sqlite3_stmt **ppStmt, const char **pzTail);
extern int database_step(sqlite3_stmt *stmt);

#endif
