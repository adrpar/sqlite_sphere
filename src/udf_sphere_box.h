#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_BOX_H__
#define __SQLITE_UDF_SPHERE_BOX_H__

void sboxFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_swFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_seFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_nwFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_neFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_equal_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_boxFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_box_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_box_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_box_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_boxFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_box_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_pointFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_point_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_point_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_point_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_boxFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_box_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_box_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_box_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_ellipse_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_ellipse_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_ellipse_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_boxFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_box_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_box_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_box_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_ellipse_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_ellipse_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_ellipse_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_polyFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_poly_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_poly_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_poly_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_boxFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_box_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_box_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_box_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_polyFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_poly_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_poly_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_poly_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_contains_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sbox_overlap_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

#endif