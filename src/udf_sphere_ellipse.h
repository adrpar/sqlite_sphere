#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_ELLIPSE_H__
#define __SQLITE_UDF_SPHERE_ELLIPSE_H__

void sellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_incFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_lradFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_sradFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_equal_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_ellipse_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_ellipse_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_ellipse_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_overlap_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_overlap_ellipse_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_pointFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_point_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_point_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_point_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_ellipse_inverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_ellipse_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_ellipse_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_ellipse_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_overlap_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_overlap_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_overlap_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_overlap_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_overlap_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_overlap_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_overlap_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_overlap_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

#endif