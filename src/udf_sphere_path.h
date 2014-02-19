#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_PATH_H__
#define __SQLITE_UDF_SPHERE_PATH_H__

void spathFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_equal_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_overlap_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_overlap_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_contains_pointFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_contains_point_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_contains_point_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_contains_point_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_path_inverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_overlap_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_overlap_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_overlap_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_overlap_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_overlap_line(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_overlap_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_overlap_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_overlap_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_overlap_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_overlap_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_overlap_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_overlap_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlap_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlap_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlap_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlap_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_add_point_aggrStep(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_add_point_aggrFinalize(sqlite3_context *context);
void spath_aggrStep(sqlite3_context *context, int argc, sqlite3_value **argv);
void spath_aggrFinalize(sqlite3_context *context);

#endif