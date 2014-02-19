#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_NPOINTS_H__
#define __SQLITE_UDF_SPHERE_NPOINTS_H__

void snpointsFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

#endif