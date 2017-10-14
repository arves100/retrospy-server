#ifndef _RSS_GPSP_DATABASE_H_
#define _RSS_GPSP_DATABASE_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int database_open(const char *name);
extern void database_close();
extern int database_exec(const char *query);

#ifdef __cplusplus
}
#endif

#endif