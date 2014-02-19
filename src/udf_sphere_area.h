#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_AREA_H__
#define __SQLITE_UDF_SPHERE_AREA_H__

void sareaFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

#endif