#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_CIRCLE_H__
#define __SQLITE_UDF_SPHERE_CIRCLE_H__

void scircleFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_radiusFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_equal_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_overlapFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_overlap_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contained_by_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contained_by_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoint_contained_by_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoint_contained_by_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoint_contained_by_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoint_contained_by_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_circle_inverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

#endif