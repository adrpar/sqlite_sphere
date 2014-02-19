#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_POINT_H__
#define __SQLITE_UDF_SPHERE_POINT_H__

void spointFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoint_longFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoint_latFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoint_xFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoint_yFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoint_zFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoint_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

#endif