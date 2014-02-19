#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_OPERATORS_H__
#define __SQLITE_UDF_SPHERE_OPERATORS_H__

//pgsphere '@' operator left is contained by right (i.e. right contains left)
void srcontainslFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

//pgsphere '~' operator left contains right
void slcontainsrFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

//pgsphere '!@' operator left is not contained by right (i.e. right does not contain left)
void srnotcontainslFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

//pgsphere '~' operator left does not contain right
void slnotcontainsrFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

//pgsphere '&&' operator both participants overlap
void soverlapsFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

//pgsphere '!&&' operator both participants overlap
void snotoverlapsFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

//pgsphere '=' operator both objects are equal
void sequalFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

//pgsphere '<>' operator both objects are equal
void snotequalFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

//pgsphere '+' operator transform
void stransformFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

//pgsphere '-' operator inverse transform
void sinverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

#endif