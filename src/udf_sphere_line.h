#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_LINE_H__
#define __SQLITE_UDF_SPHERE_LINE_H__

void slineFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void smeridianFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_begFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_endFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_equal_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_turnFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_crossesFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_crosses_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_overlapFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_overlap_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_contains_pointFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_contains_point_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_contains_point_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_contains_point_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_line_inverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_overlap_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_overlap_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_overlap_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sline_overlap_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

#endif