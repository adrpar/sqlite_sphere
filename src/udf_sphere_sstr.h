#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_SSTR_H__
#define __SQLITE_UDF_SPHERE_SSTR_H__

void sstrFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

#endif