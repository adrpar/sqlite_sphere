#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_CIRCUM_H__
#define __SQLITE_UDF_SPHERE_CIRCUM_H__

void scircumFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

#endif