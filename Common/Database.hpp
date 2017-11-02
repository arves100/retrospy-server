#ifndef _RSS_COMMON_DATABASE_HPP_
#define _RSS_COMMON_DATABASE_HPP_

/*#ifdef __cplusplus
extern "C" {
#endif*/

/*typedef struct _rss_common_sqlvalue_
{
	sqlite3_value *lpValue;
} SQLValue;

typedef struct _rss_common_sqlresult_
{
	int nAffectedColumns;
	int nAffectedRows;
	sqlite3_value *value;
	SQLResult *_next;
} SQLResult;

extern void sqlresult_free(SQLResult sql);*/

//extern int database_exec_result(const char *query, SQLResult *pOut);
extern int database_exec_count(const char *query, int *nOut);

extern int database_is_column_null(sqlite3_stmt *stmt, int column);

extern int database_open(const char *name);
extern void database_close();
extern int database_exec(const char *query);

extern void database_finalize(sqlite3_stmt *stmt);
extern int database_prepare(const char *sql, int sql_length, sqlite3_stmt **ppStmt, const char **pzTail);
extern int database_step(sqlite3_stmt *stmt);

/*#ifdef __cplusplus
}
#endif*/

#endif
