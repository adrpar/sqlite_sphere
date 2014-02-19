#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_POLYGON_H__
#define __SQLITE_UDF_SPHERE_POLYGON_H__

void spolyFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_equal_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_polygonFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_polygon_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_polygon_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_polygon_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlap_polygonFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlap_polygon_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_pointFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_point_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_point_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_point_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_polyFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_poly_inverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_polygonFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_polygon_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_polygon_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void scircle_contains_polygon_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlap_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlap_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlap_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlap_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlaps_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlaps_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlaps_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlaps_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_ellipse_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_ellipse_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_contains_ellipse_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_polygonFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_polygon_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_polygon_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void sellipse_contains_polygon_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlap_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlap_ellipse_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlap_ellipse_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_overlap_ellipse_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

//aggregates
void spoly_add_point_aggrStep(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_add_point_aggrFinalize(sqlite3_context *context);
void spoly_aggrStep(sqlite3_context *context, int argc, sqlite3_value **argv);
void spoly_aggrFinalize(sqlite3_context *context);

#endif