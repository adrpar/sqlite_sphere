#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_DIST_H__
#define __SQLITE_UDF_SPHERE_DIST_H__

void sdistFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

#endif