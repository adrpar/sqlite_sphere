#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_CENTER_H__
#define __SQLITE_UDF_SPHERE_CENTER_H__

void scenterFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

#endif