#ifndef _RSS_GPSP_DATABASE_H_
#define _RSS_GPSP_DATABASE_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int database_open(const char *name);
extern void database_close();
extern int database_exec(const char *query);

extern void database_finalize(sqlite3_stmt *stmt);
extern int database_prepare(const char *sql, int sql_length, sqlite3_stmt **ppStmt, char **pzTail);
extern int database_step(sqlite3_stmt *stmt);

#ifdef __cplusplus
}
#endif

#endif