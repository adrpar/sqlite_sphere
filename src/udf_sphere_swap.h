#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_SWAP_H__
#define __SQLITE_UDF_SPHERE_SWAP_H__

void sswapFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

#endif