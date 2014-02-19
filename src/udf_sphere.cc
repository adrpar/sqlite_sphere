#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <stdint.h>
#include <sqlite3ext.h>
#include <string.h>
#include <math.h>
#include "serialiser.h"

#include "udf_sphere_point.h"
#include "udf_sphere_euler.h"
#include "udf_sphere_circle.h"
#include "udf_sphere_line.h"
#include "udf_sphere_ellipse.h"
#include "udf_sphere_polygon.h"
#include "udf_sphere_path.h"
#include "udf_sphere_box.h"
#include "udf_sphere_area.h"
#include "udf_sphere_center.h"
#include "udf_sphere_circum.h"
#include "udf_sphere_dist.h"
#include "udf_sphere_length.h"
#include "udf_sphere_npoints.h"
#include "udf_sphere_sstr.h"
#include "udf_sphere_swap.h"
#include "udf_sphere_operators.h"

SQLITE_EXTENSION_INIT1

#ifdef _WIN32
__declspec(dllexport)
#endif

extern "C" {

	int sqlite3_extension_init(
	  sqlite3 *db, 
	  char **pzErrMsg, 
	  const sqlite3_api_routines *pApi
	);

}

static void piFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  sqlite3_result_double(context, M_PI);
}

/*
** This function registered all of the above C functions as SQL
** functions.  This should be the only routine in this file with
** external linkage.
*/
int RegisterExtensionFunctions(sqlite3 *db) {
	static const struct FuncDef {
		const char *zName;
		signed char nArg;
		uint8_t argType;           /* 0: none.  1: db  2: (-1) */
		uint8_t eTextRep;          /* 1: UTF-16.  0: UTF-8 */
		uint8_t needCollSeq;
		void (*xFunc)(sqlite3_context*,int,sqlite3_value **);
	} aFuncs[] = {
		{ "spoint",               1, 0, SQLITE_UTF8,    0, spointFunc  },
		{ "spoint",               2, 0, SQLITE_UTF8,    0, spointFunc  },
		{ "spoint_long",          1, 0, SQLITE_UTF8,    0, spoint_longFunc  },
		{ "spoint_lat",           1, 0, SQLITE_UTF8,    0, spoint_latFunc  },
		{ "spoint_x",             1, 0, SQLITE_UTF8,    0, spoint_xFunc  },
		{ "spoint_y",             1, 0, SQLITE_UTF8,    0, spoint_yFunc  },
		{ "spoint_z",             1, 0, SQLITE_UTF8,    0, spoint_zFunc  },
		{ "spoint_equal",         2, 0, SQLITE_UTF8,    0, spoint_equalFunc  },
		{ "strans",               1, 0, SQLITE_UTF8,    0, stransFunc  },
		{ "strans",               3, 0, SQLITE_UTF8,    0, stransFunc  },
		{ "strans",               4, 0, SQLITE_UTF8,    0, stransFunc  },
		{ "strans_point",         2, 0, SQLITE_UTF8,    0, strans_pointFunc  },
		{ "strans_point_inverse", 2, 0, SQLITE_UTF8,    0, strans_point_inverseFunc  },
		{ "strans_equal",         2, 0, SQLITE_UTF8,    0, strans_equalFunc  },
		{ "strans_equal_neg",     2, 0, SQLITE_UTF8,    0, strans_equal_negFunc  },
		{ "strans_phi",           1, 0, SQLITE_UTF8,    0, strans_phiFunc  },
		{ "strans_theta",         1, 0, SQLITE_UTF8,    0, strans_thetaFunc  },
		{ "strans_psi",           1, 0, SQLITE_UTF8,    0, strans_psiFunc  },
		{ "strans_axes",          1, 0, SQLITE_UTF8,    0, strans_axesFunc  },
		{ "strans_invert",        1, 0, SQLITE_UTF8,    0, strans_invertFunc  },
		{ "strans_zxz",           1, 0, SQLITE_UTF8,    0, strans_zxzFunc  },
		{ "strans_trans",         2, 0, SQLITE_UTF8,    0, strans_transFunc  },
		{ "strans_trans_inv",     2, 0, SQLITE_UTF8,    0, strans_trans_invFunc  },
		{ "scircle",              2, 0, SQLITE_UTF8,    0, scircleFunc  },
		{ "scircle",              1, 0, SQLITE_UTF8,    0, scircleFunc  },
		{ "scircle_radius",       1, 0, SQLITE_UTF8,    0, scircle_radiusFunc  },
		{ "scircle_equal",        2, 0, SQLITE_UTF8,    0, scircle_equalFunc  },
		{ "scircle_equal_neg",    2, 0, SQLITE_UTF8,    0, scircle_equal_negFunc  },
		{ "scircle_overlap",      2, 0, SQLITE_UTF8,    0, scircle_overlapFunc  },
		{ "scircle_overlap_neg",  2, 0, SQLITE_UTF8,    0, scircle_overlap_negFunc  },
		{ "scircle_contained_by_circle",        2, 0, SQLITE_UTF8,    0, scircle_contained_by_circleFunc  },
		{ "scircle_contained_by_circle_neg",    2, 0, SQLITE_UTF8,    0, scircle_contained_by_circle_negFunc  },
		{ "scircle_contains_circle",            2, 0, SQLITE_UTF8,    0, scircle_contains_circleFunc  },
		{ "scircle_contains_circle_neg",        2, 0, SQLITE_UTF8,    0, scircle_contains_circle_negFunc  },
		{ "spoint_contained_by_circle",         2, 0, SQLITE_UTF8,    0, spoint_contained_by_circleFunc  },
		{ "spoint_contained_by_circle_neg",     2, 0, SQLITE_UTF8,    0, spoint_contained_by_circle_negFunc  },
		{ "spoint_contained_by_circle_com",     2, 0, SQLITE_UTF8,    0, spoint_contained_by_circle_comFunc  },
		{ "spoint_contained_by_circle_com_neg", 2, 0, SQLITE_UTF8,    0, spoint_contained_by_circle_com_negFunc  },
		{ "strans_circle",        2, 0, SQLITE_UTF8,    0, strans_circleFunc  },
		{ "strans_circle_inverse",              2, 0, SQLITE_UTF8,    0, strans_circle_inverseFunc  },
		{ "sline",                1, 0, SQLITE_UTF8,    0, slineFunc  },
		{ "sline",                2, 0, SQLITE_UTF8,    0, slineFunc  },
		{ "smeridian",            1, 0, SQLITE_UTF8,    0, smeridianFunc  },
		{ "sline_beg",            1, 0, SQLITE_UTF8,    0, sline_begFunc  },
		{ "sline_end",            1, 0, SQLITE_UTF8,    0, sline_endFunc  },
		{ "sline_equal",          2, 0, SQLITE_UTF8,    0, sline_equalFunc  },
		{ "sline_equal_neg",      2, 0, SQLITE_UTF8,    0, sline_equal_negFunc  },
		{ "sline_turn",           1, 0, SQLITE_UTF8,    0, sline_turnFunc  },
		{ "sline_crosses",        2, 0, SQLITE_UTF8,    0, sline_crossesFunc  },
		{ "sline_crosses_neg",    2, 0, SQLITE_UTF8,    0, sline_crosses_negFunc  },
		{ "sline_overlap",        2, 0, SQLITE_UTF8,    0, sline_overlapFunc  },
		{ "sline_overlap_neg",    2, 0, SQLITE_UTF8,    0, sline_overlap_negFunc  },
		{ "sline_contains_point",                2, 0, SQLITE_UTF8,    0, sline_contains_pointFunc  },
		{ "sline_contains_point_com",            2, 0, SQLITE_UTF8,    0, sline_contains_point_negFunc  },
		{ "sline_contains_point_neg",            2, 0, SQLITE_UTF8,    0, sline_contains_point_comFunc  },
		{ "sline_contains_point_com_neg",        2, 0, SQLITE_UTF8,    0, sline_contains_point_com_negFunc  },
		{ "strans_line",          2, 0, SQLITE_UTF8,    0, strans_lineFunc  },
		{ "strans_line_inverse",  2, 0, SQLITE_UTF8,    0, strans_line_inverseFunc  },
		{ "sline_overlap_circle",                2, 0, SQLITE_UTF8,    0, sline_overlap_circleFunc  },
		{ "sline_overlap_circle_com",            2, 0, SQLITE_UTF8,    0, sline_overlap_circle_negFunc  },
		{ "sline_overlap_circle_neg",            2, 0, SQLITE_UTF8,    0, sline_overlap_circle_comFunc  },
		{ "sline_overlap_circle_com_neg",        2, 0, SQLITE_UTF8,    0, sline_overlap_circle_com_negFunc  },
		{ "scircle_contains_line",               2, 0, SQLITE_UTF8,    0, scircle_contains_lineFunc  },
		{ "scircle_contains_line_com",           2, 0, SQLITE_UTF8,    0, scircle_contains_line_negFunc  },
		{ "scircle_contains_line_neg",           2, 0, SQLITE_UTF8,    0, scircle_contains_line_comFunc  },
		{ "scircle_contains_line_com_neg",       2, 0, SQLITE_UTF8,    0, scircle_contains_line_com_negFunc  },
		{ "sellipse",             1, 0, SQLITE_UTF8,    0, sellipseFunc  },
		{ "sellipse",             4, 0, SQLITE_UTF8,    0, sellipseFunc  },
		{ "sellipse_inc",         1, 0, SQLITE_UTF8,    0, sellipse_incFunc  },
		{ "sellipse_lrad",        1, 0, SQLITE_UTF8,    0, sellipse_lradFunc  },
		{ "sellipse_srad",        1, 0, SQLITE_UTF8,    0, sellipse_sradFunc  },
		{ "sellipse_equal",       2, 0, SQLITE_UTF8,    0, sellipse_equalFunc  },
		{ "sellipse_equal_neg",   2, 0, SQLITE_UTF8,    0, sellipse_equal_negFunc  },
		{ "sellipse_contains_ellipse",           2, 0, SQLITE_UTF8,    0, sellipse_contains_ellipseFunc  },
		{ "sellipse_contains_ellipse_neg",       2, 0, SQLITE_UTF8,    0, sellipse_contains_ellipse_negFunc  },
		{ "sellipse_contains_ellipse_com",       2, 0, SQLITE_UTF8,    0, sellipse_contains_ellipse_comFunc  },
		{ "sellipse_contains_ellipse_com_neg",   2, 0, SQLITE_UTF8,    0, sellipse_contains_ellipse_com_negFunc  },
		{ "sellipse_overlap_ellipse",            2, 0, SQLITE_UTF8,    0, sellipse_overlap_ellipseFunc  },
		{ "sellipse_overlap_ellipse_neg",        2, 0, SQLITE_UTF8,    0, sellipse_overlap_ellipse_negFunc  },
		{ "sellipse_contains_point",             2, 0, SQLITE_UTF8,    0, sellipse_contains_pointFunc  },
		{ "sellipse_contains_point_neg",         2, 0, SQLITE_UTF8,    0, sellipse_contains_point_negFunc  },
		{ "sellipse_contains_point_com",         2, 0, SQLITE_UTF8,    0, sellipse_contains_point_comFunc  },
		{ "sellipse_contains_point_com_neg",     2, 0, SQLITE_UTF8,    0, sellipse_contains_point_com_negFunc  },
		{ "sellipse_contains_circle",            2, 0, SQLITE_UTF8,    0, sellipse_contains_circleFunc  },
		{ "sellipse_contains_circle_neg",        2, 0, SQLITE_UTF8,    0, sellipse_contains_circle_negFunc  },
		{ "sellipse_contains_circle_com",        2, 0, SQLITE_UTF8,    0, sellipse_contains_circle_comFunc  },
		{ "sellipse_contains_circle_com_neg",    2, 0, SQLITE_UTF8,    0, sellipse_contains_circle_com_negFunc  },
		{ "scircle_contains_ellipse",            2, 0, SQLITE_UTF8,    0, scircle_contains_ellipseFunc  },
		{ "scircle_contains_ellipse_neg",        2, 0, SQLITE_UTF8,    0, scircle_contains_ellipse_negFunc  },
		{ "scircle_contains_ellipse_com",        2, 0, SQLITE_UTF8,    0, scircle_contains_ellipse_comFunc  },
		{ "scircle_contains_ellipse_com_neg",    2, 0, SQLITE_UTF8,    0, scircle_contains_ellipse_com_negFunc  },
		{ "sellipse_overlap_circle",             2, 0, SQLITE_UTF8,    0, sellipse_overlap_circleFunc  },
		{ "sellipse_overlap_circle_neg",         2, 0, SQLITE_UTF8,    0, sellipse_overlap_circle_negFunc  },
		{ "sellipse_overlap_circle_com",         2, 0, SQLITE_UTF8,    0, sellipse_overlap_circle_comFunc  },
		{ "sellipse_overlap_circle_com_neg",     2, 0, SQLITE_UTF8,    0, sellipse_overlap_circle_com_negFunc  },
		{ "sellipse_overlap_line",               2, 0, SQLITE_UTF8,    0, sellipse_overlap_lineFunc  },
		{ "sellipse_overlap_line_neg",           2, 0, SQLITE_UTF8,    0, sellipse_overlap_line_negFunc  },
		{ "sellipse_overlap_line_com",           2, 0, SQLITE_UTF8,    0, sellipse_overlap_line_comFunc  },
		{ "sellipse_overlap_line_com_neg",       2, 0, SQLITE_UTF8,    0, sellipse_overlap_line_com_negFunc  },
		{ "sellipse_contains_line",              2, 0, SQLITE_UTF8,    0, sellipse_contains_lineFunc  },
		{ "sellipse_contains_line_neg",          2, 0, SQLITE_UTF8,    0, sellipse_contains_line_negFunc  },
		{ "sellipse_contains_line_com",          2, 0, SQLITE_UTF8,    0, sellipse_contains_line_comFunc  },
		{ "sellipse_contains_line_com_neg",      2, 0, SQLITE_UTF8,    0, sellipse_contains_line_com_negFunc  },
		{ "strans_ellipse",                      2, 0, SQLITE_UTF8,    0, strans_ellipseFunc  },
		{ "strans_ellipse_inverse",              2, 0, SQLITE_UTF8,    0, strans_ellipse_inverseFunc  },
		{ "spoly",                1, 0, SQLITE_UTF8,    0, spolyFunc  },
		{ "spoly_equal",          2, 0, SQLITE_UTF8,    0, spoly_equalFunc  },
		{ "spoly_equal_neg",      2, 0, SQLITE_UTF8,    0, spoly_equal_negFunc  },
		{ "spoly_contains_polygon",              2, 0, SQLITE_UTF8,    0, spoly_contains_polygonFunc  },
		{ "spoly_contains_polygon_neg",          2, 0, SQLITE_UTF8,    0, spoly_contains_polygon_negFunc  },
		{ "spoly_contains_polygon_com",          2, 0, SQLITE_UTF8,    0, spoly_contains_polygon_comFunc  },
		{ "spoly_contains_polygon_com_neg",      2, 0, SQLITE_UTF8,    0, spoly_contains_polygon_com_negFunc  },
		{ "spoly_overlap_polygon",               2, 0, SQLITE_UTF8,    0, spoly_overlap_polygonFunc  },
		{ "spoly_overlap_polygon_neg",           2, 0, SQLITE_UTF8,    0, spoly_overlap_polygon_negFunc  },
		{ "spoly_contains_point",                2, 0, SQLITE_UTF8,    0, spoly_contains_pointFunc  },
		{ "spoly_contains_point_neg",            2, 0, SQLITE_UTF8,    0, spoly_contains_point_negFunc  },
		{ "spoly_contains_point_com",            2, 0, SQLITE_UTF8,    0, spoly_contains_point_comFunc  },
		{ "spoly_contains_point_com_neg",        2, 0, SQLITE_UTF8,    0, spoly_contains_point_com_negFunc  },
		{ "strans_poly",          2, 0, SQLITE_UTF8,    0, strans_polyFunc  },
		{ "strans_poly_inverse",  2, 0, SQLITE_UTF8,    0, strans_poly_inverseFunc  },
		{ "spoly_contains_circle",               2, 0, SQLITE_UTF8,    0, spoly_contains_circleFunc  },
		{ "spoly_contains_circle_neg",           2, 0, SQLITE_UTF8,    0, spoly_contains_circle_negFunc  },
		{ "spoly_contains_circle_com",           2, 0, SQLITE_UTF8,    0, spoly_contains_circle_comFunc  },
		{ "spoly_contains_circle_com_neg",       2, 0, SQLITE_UTF8,    0, spoly_contains_circle_com_negFunc  },
		{ "scircle_contains_polygon",            2, 0, SQLITE_UTF8,    0, scircle_contains_polygonFunc  },
		{ "scircle_contains_polygon_neg",        2, 0, SQLITE_UTF8,    0, scircle_contains_polygon_negFunc  },
		{ "scircle_contains_polygon_com",        2, 0, SQLITE_UTF8,    0, scircle_contains_polygon_comFunc  },
		{ "scircle_contains_polygon_com_neg",    2, 0, SQLITE_UTF8,    0, scircle_contains_polygon_com_negFunc  },
		{ "spoly_overlap_circle",                2, 0, SQLITE_UTF8,    0, spoly_overlap_circleFunc  },
		{ "spoly_overlap_circle_neg",            2, 0, SQLITE_UTF8,    0, spoly_overlap_circle_negFunc  },
		{ "spoly_overlap_circle_com",            2, 0, SQLITE_UTF8,    0, spoly_overlap_circle_comFunc  },
		{ "spoly_overlap_circle_com_neg",        2, 0, SQLITE_UTF8,    0, spoly_overlap_circle_com_negFunc  },
		{ "spoly_contains_line",                 2, 0, SQLITE_UTF8,    0, spoly_contains_lineFunc  },
		{ "spoly_contains_line_neg",             2, 0, SQLITE_UTF8,    0, spoly_contains_line_negFunc  },
		{ "spoly_contains_line_com",             2, 0, SQLITE_UTF8,    0, spoly_contains_line_comFunc  },
		{ "spoly_contains_line_com_neg",         2, 0, SQLITE_UTF8,    0, spoly_contains_line_com_negFunc  },
		{ "spoly_overlap_line",                  2, 0, SQLITE_UTF8,    0, spoly_overlaps_lineFunc  },
		{ "spoly_overlap_line_neg",              2, 0, SQLITE_UTF8,    0, spoly_overlaps_line_negFunc  },
		{ "spoly_overlap_line_com",              2, 0, SQLITE_UTF8,    0, spoly_overlaps_line_comFunc  },
		{ "spoly_overlap_line_com_neg",          2, 0, SQLITE_UTF8,    0, spoly_overlaps_line_com_negFunc  },
		{ "spoly_contains_ellipse",              2, 0, SQLITE_UTF8,    0, spoly_contains_ellipseFunc  },
		{ "spoly_contains_ellipse_neg",          2, 0, SQLITE_UTF8,    0, spoly_contains_ellipse_negFunc  },
		{ "spoly_contains_ellipse_com",          2, 0, SQLITE_UTF8,    0, spoly_contains_ellipse_comFunc  },
		{ "spoly_contains_ellipse_com_neg",      2, 0, SQLITE_UTF8,    0, spoly_contains_ellipse_com_negFunc  },
		{ "sellipse_contains_polygon",           2, 0, SQLITE_UTF8,    0, sellipse_contains_polygonFunc  },
		{ "sellipse_contains_polygon_neg",       2, 0, SQLITE_UTF8,    0, sellipse_contains_polygon_negFunc  },
		{ "sellipse_contains_polygon_com",       2, 0, SQLITE_UTF8,    0, sellipse_contains_polygon_comFunc  },
		{ "sellipse_contains_polygon_com_neg",   2, 0, SQLITE_UTF8,    0, sellipse_contains_polygon_com_negFunc  },
		{ "spoly_overlap_ellipse",               2, 0, SQLITE_UTF8,    0, spoly_overlap_ellipseFunc  },
		{ "spoly_overlap_ellipse_neg",           2, 0, SQLITE_UTF8,    0, spoly_overlap_ellipse_negFunc  },
		{ "spoly_overlap_ellipse_com",           2, 0, SQLITE_UTF8,    0, spoly_overlap_ellipse_comFunc  },
		{ "spoly_overlap_ellipse_com_neg",       2, 0, SQLITE_UTF8,    0, spoly_overlap_ellipse_com_negFunc  },
		{ "spath",                1, 0, SQLITE_UTF8,    0, spathFunc  },
		{ "spath_equal",          2, 0, SQLITE_UTF8,    0, spath_equalFunc  },
		{ "spath_equal_neg",      2, 0, SQLITE_UTF8,    0, spath_equal_negFunc  },
		{ "spath_overlap_path",                  2, 0, SQLITE_UTF8,    0, spath_overlap_pathFunc  },
		{ "spath_overlap_path_neg",              2, 0, SQLITE_UTF8,    0, spath_overlap_path_negFunc  },
		{ "spath_contains_point",                2, 0, SQLITE_UTF8,    0, spath_contains_pointFunc  },
		{ "spath_contains_point_neg",            2, 0, SQLITE_UTF8,    0, spath_contains_point_negFunc  },
		{ "spath_contains_point_com",            2, 0, SQLITE_UTF8,    0, spath_contains_point_comFunc  },
		{ "spath_contains_point_com_neg",        2, 0, SQLITE_UTF8,    0, spath_contains_point_com_negFunc  },
		{ "scircle_contains_path",               2, 0, SQLITE_UTF8,    0, strans_pathFunc  },
		{ "scircle_contains_path_neg",           2, 0, SQLITE_UTF8,    0, strans_path_inverseFunc  },
		{ "scircle_contains_path_com",           2, 0, SQLITE_UTF8,    0, scircle_contains_pathFunc  },
		{ "scircle_contains_path_com_neg",       2, 0, SQLITE_UTF8,    0, scircle_contains_path_negFunc  },
		{ "scircle_overlap_path",                2, 0, SQLITE_UTF8,    0, scircle_contains_path_comFunc  },
		{ "scircle_overlap_path_neg",            2, 0, SQLITE_UTF8,    0, scircle_contains_path_com_negFunc  },
		{ "scircle_overlap_path_com",            2, 0, SQLITE_UTF8,    0, scircle_overlap_pathFunc  },
		{ "scircle_overlap_path_com_neg",        2, 0, SQLITE_UTF8,    0, scircle_overlap_path_negFunc  },
		{ "spath_overlap_line",                  2, 0, SQLITE_UTF8,    0, scircle_overlap_path_comFunc  },
		{ "spath_overlap_line_neg",              2, 0, SQLITE_UTF8,    0, scircle_overlap_path_com_negFunc  },
		{ "spath_overlap_line_com",              2, 0, SQLITE_UTF8,    0, spath_overlap_line  },
		{ "spath_overlap_line_com_neg",          2, 0, SQLITE_UTF8,    0, spath_overlap_line_negFunc  },
		{ "sellipse_contains_path",              2, 0, SQLITE_UTF8,    0, spath_overlap_line_comFunc  },
		{ "sellipse_contains_path_neg",          2, 0, SQLITE_UTF8,    0, spath_overlap_line_com_negFunc  },
		{ "sellipse_contains_path_com",          2, 0, SQLITE_UTF8,    0, sellipse_contains_pathFunc  },
		{ "sellipse_contains_path_com_neg",      2, 0, SQLITE_UTF8,    0, sellipse_contains_path_negFunc  },
		{ "sellipse_overlap_path",               2, 0, SQLITE_UTF8,    0, sellipse_contains_path_comFunc  },
		{ "sellipse_overlap_path_neg",           2, 0, SQLITE_UTF8,    0, sellipse_contains_path_com_negFunc  },
		{ "sellipse_overlap_path_com",           2, 0, SQLITE_UTF8,    0, sellipse_overlap_pathFunc  },
		{ "sellipse_overlap_path_com_neg",       2, 0, SQLITE_UTF8,    0, sellipse_overlap_path_negFunc  },
		{ "spoly_contains_path",                 2, 0, SQLITE_UTF8,    0, sellipse_overlap_path_comFunc  },
		{ "spoly_contains_path_neg",             2, 0, SQLITE_UTF8,    0, sellipse_overlap_path_com_negFunc  },
		{ "spoly_contains_path_com",             2, 0, SQLITE_UTF8,    0, spoly_contains_pathFunc  },
		{ "spoly_contains_path_com_neg",         2, 0, SQLITE_UTF8,    0, spoly_contains_path_negFunc  },
		{ "spoly_overlap_path",                  2, 0, SQLITE_UTF8,    0, spoly_contains_path_comFunc  },
		{ "spoly_overlap_path_neg",              2, 0, SQLITE_UTF8,    0, spoly_contains_path_com_negFunc  },
		{ "spoly_overlap_path_com",              2, 0, SQLITE_UTF8,    0, spoly_overlap_pathFunc  },
		{ "spoly_overlap_path_com_neg",          2, 0, SQLITE_UTF8,    0, spoly_overlap_path_negFunc  },
		{ "strans_path",                         2, 0, SQLITE_UTF8,    0, spoly_overlap_path_comFunc  },
		{ "strans_path_inverse",                 2, 0, SQLITE_UTF8,    0, spoly_overlap_path_com_negFunc  },
		{ "sbox",                 1, 0, SQLITE_UTF8,    0, sboxFunc  },
		{ "sbox",                 2, 0, SQLITE_UTF8,    0, sboxFunc  },
		{ "sbox_sw",              1, 0, SQLITE_UTF8,    0, sbox_swFunc  },
		{ "sbox_se",              1, 0, SQLITE_UTF8,    0, sbox_seFunc  },
		{ "sbox_nw",              1, 0, SQLITE_UTF8,    0, sbox_nwFunc  },
		{ "sbox_ne",              1, 0, SQLITE_UTF8,    0, sbox_neFunc  },
		{ "sbox_equal",           2, 0, SQLITE_UTF8,    0, sbox_equalFunc  },
		{ "sbox_equal_neg",       2, 0, SQLITE_UTF8,    0, sbox_equal_negFunc  },
		{ "sbox_contains_box",                   2, 0, SQLITE_UTF8,    0, sbox_contains_boxFunc  },
		{ "sbox_contains_box_neg",               2, 0, SQLITE_UTF8,    0, sbox_contains_box_negFunc  },
		{ "sbox_contains_box_com",               2, 0, SQLITE_UTF8,    0, sbox_contains_box_comFunc  },
		{ "sbox_contains_box_com_neg",           2, 0, SQLITE_UTF8,    0, sbox_contains_box_com_negFunc  },
		{ "sbox_overlap_box",                    2, 0, SQLITE_UTF8,    0, sbox_overlap_boxFunc  },
		{ "sbox_overlap_box_neg",                2, 0, SQLITE_UTF8,    0, sbox_overlap_box_negFunc  },
		{ "sbox_contains_point",                 2, 0, SQLITE_UTF8,    0, sbox_contains_pointFunc  },
		{ "sbox_contains_point_neg",             2, 0, SQLITE_UTF8,    0, sbox_contains_point_negFunc  },
		{ "sbox_contains_point_com",             2, 0, SQLITE_UTF8,    0, sbox_contains_point_comFunc  },
		{ "sbox_contains_point_com_neg",         2, 0, SQLITE_UTF8,    0, sbox_contains_point_com_negFunc  },
		{ "sbox_contains_circle",                2, 0, SQLITE_UTF8,    0, sbox_contains_circleFunc  },
		{ "sbox_contains_circle_neg",            2, 0, SQLITE_UTF8,    0, sbox_contains_circle_negFunc  },
		{ "sbox_contains_circle_com",            2, 0, SQLITE_UTF8,    0, sbox_contains_circle_comFunc  },
		{ "sbox_contains_circle_com_neg",        2, 0, SQLITE_UTF8,    0, sbox_contains_circle_com_negFunc  },
		{ "scircle_contains_box",                2, 0, SQLITE_UTF8,    0, scircle_contains_boxFunc  },
		{ "scircle_contains_box_neg",            2, 0, SQLITE_UTF8,    0, scircle_contains_box_negFunc  },
		{ "scircle_contains_box_com",            2, 0, SQLITE_UTF8,    0, scircle_contains_box_comFunc  },
		{ "scircle_contains_box_com_neg",        2, 0, SQLITE_UTF8,    0, scircle_contains_box_com_negFunc  },
		{ "sbox_overlap_circle",                 2, 0, SQLITE_UTF8,    0, sbox_overlap_circleFunc  },
		{ "sbox_overlap_circle_neg",             2, 0, SQLITE_UTF8,    0, sbox_overlap_circle_negFunc  },
		{ "sbox_overlap_circle_com",             2, 0, SQLITE_UTF8,    0, sbox_overlap_circle_comFunc  },
		{ "sbox_overlap_circle_com_neg",         2, 0, SQLITE_UTF8,    0, sbox_overlap_circle_com_negFunc  },
		{ "sbox_contains_line",                  2, 0, SQLITE_UTF8,    0, sbox_contains_lineFunc  },
		{ "sbox_contains_line_neg",              2, 0, SQLITE_UTF8,    0, sbox_contains_line_negFunc  },
		{ "sbox_contains_line_com",              2, 0, SQLITE_UTF8,    0, sbox_contains_line_comFunc  },
		{ "sbox_contains_line_com_neg",          2, 0, SQLITE_UTF8,    0, sbox_contains_line_com_negFunc  },
		{ "sbox_overlap_line",                   2, 0, SQLITE_UTF8,    0, sbox_overlap_lineFunc  },
		{ "sbox_overlap_line_neg",               2, 0, SQLITE_UTF8,    0, sbox_overlap_line_negFunc  },
		{ "sbox_overlap_line_com",               2, 0, SQLITE_UTF8,    0, sbox_overlap_line_comFunc  },
		{ "sbox_overlap_line_com_neg",           2, 0, SQLITE_UTF8,    0, sbox_overlap_line_com_negFunc  },
		{ "sbox_contains_ellipse",               2, 0, SQLITE_UTF8,    0, sbox_contains_ellipseFunc  },
		{ "sbox_contains_ellipse_neg",           2, 0, SQLITE_UTF8,    0, sbox_contains_ellipse_negFunc  },
		{ "sbox_contains_ellipse_com",           2, 0, SQLITE_UTF8,    0, sbox_contains_ellipse_comFunc  },
		{ "sbox_contains_ellipse_com_neg",       2, 0, SQLITE_UTF8,    0, sbox_contains_ellipse_com_negFunc  },
		{ "sellipse_contains_box",               2, 0, SQLITE_UTF8,    0, sellipse_contains_boxFunc  },
		{ "sellipse_contains_box_neg",           2, 0, SQLITE_UTF8,    0, sellipse_contains_box_negFunc  },
		{ "sellipse_contains_box_com",           2, 0, SQLITE_UTF8,    0, sellipse_contains_box_comFunc  },
		{ "sellipse_contains_box_com_neg",       2, 0, SQLITE_UTF8,    0, sellipse_contains_box_com_negFunc  },
		{ "sbox_overlap_ellipse",                2, 0, SQLITE_UTF8,    0, sbox_overlap_ellipseFunc  },
		{ "sbox_overlap_ellipse_neg",            2, 0, SQLITE_UTF8,    0, sbox_overlap_ellipse_negFunc  },
		{ "sbox_overlap_ellipse_com",            2, 0, SQLITE_UTF8,    0, sbox_overlap_ellipse_comFunc  },
		{ "sbox_overlap_ellipse_com_neg",        2, 0, SQLITE_UTF8,    0, sbox_overlap_ellipse_com_negFunc  },
		{ "sbox_contains_poly",                  2, 0, SQLITE_UTF8,    0, sbox_contains_polyFunc  },
		{ "sbox_contains_poly_neg",              2, 0, SQLITE_UTF8,    0, sbox_contains_poly_negFunc  },
		{ "sbox_contains_poly_com",              2, 0, SQLITE_UTF8,    0, sbox_contains_poly_comFunc  },
		{ "sbox_contains_poly_com_neg",          2, 0, SQLITE_UTF8,    0, sbox_contains_poly_com_negFunc  },
		{ "spoly_contains_box",                  2, 0, SQLITE_UTF8,    0, spoly_contains_boxFunc  },
		{ "spoly_contains_box_neg",              2, 0, SQLITE_UTF8,    0, spoly_contains_box_negFunc  },
		{ "spoly_contains_box_com",              2, 0, SQLITE_UTF8,    0, spoly_contains_box_comFunc  },
		{ "spoly_contains_box_com_neg",          2, 0, SQLITE_UTF8,    0, spoly_contains_box_com_negFunc  },
		{ "sbox_overlap_poly",                   2, 0, SQLITE_UTF8,    0, sbox_overlap_polyFunc  },
		{ "sbox_overlap_poly_neg",               2, 0, SQLITE_UTF8,    0, sbox_overlap_poly_negFunc  },
		{ "sbox_overlap_poly_com",               2, 0, SQLITE_UTF8,    0, sbox_overlap_poly_comFunc  },
		{ "sbox_overlap_poly_com_neg",           2, 0, SQLITE_UTF8,    0, sbox_overlap_poly_com_negFunc  },
		{ "sbox_contains_path",                  2, 0, SQLITE_UTF8,    0, sbox_contains_pathFunc  },
		{ "sbox_contains_path_neg",              2, 0, SQLITE_UTF8,    0, sbox_contains_path_negFunc  },
		{ "sbox_contains_path_com",              2, 0, SQLITE_UTF8,    0, sbox_contains_path_comFunc  },
		{ "sbox_contains_path_com_neg",          2, 0, SQLITE_UTF8,    0, sbox_contains_path_com_negFunc  },
		{ "sbox_overlap_path",                   2, 0, SQLITE_UTF8,    0, sbox_overlap_pathFunc  },
		{ "sbox_overlap_path_neg",               2, 0, SQLITE_UTF8,    0, sbox_overlap_path_negFunc  },
		{ "sbox_overlap_path_com",               2, 0, SQLITE_UTF8,    0, sbox_overlap_path_comFunc  },
		{ "sbox_overlap_path_com_neg",           2, 0, SQLITE_UTF8,    0, sbox_overlap_path_com_negFunc  },
		{ "sarea",                1, 0, SQLITE_UTF8,    0, sareaFunc  },
		{ "scenter",              1, 0, SQLITE_UTF8,    0, scenterFunc  },
		{ "scircum",              1, 0, SQLITE_UTF8,    0, scircumFunc  },
		{ "sdist",                2, 0, SQLITE_UTF8,    0, sdistFunc  },
		{ "slength",              1, 0, SQLITE_UTF8,    0, slengthFunc  },
		{ "snpoints",             1, 0, SQLITE_UTF8,    0, snpointsFunc  },
		{ "sstr",                 1, 0, SQLITE_UTF8,    0, sstrFunc  },
		{ "sstr",                 2, 0, SQLITE_UTF8,    0, sstrFunc  },
		{ "sstr",                 3, 0, SQLITE_UTF8,    0, sstrFunc  },
		{ "sswap",                1, 0, SQLITE_UTF8,    0, sswapFunc  },
		{ "srcontainsl",          2, 0, SQLITE_UTF8,    0, srcontainslFunc  },
		{ "slcontainsr",          2, 0, SQLITE_UTF8,    0, slcontainsrFunc  },
		{ "srnotcontainsl",       2, 0, SQLITE_UTF8,    0, srnotcontainslFunc  },
		{ "slnotcontainsr",       2, 0, SQLITE_UTF8,    0, slnotcontainsrFunc  },
		{ "soverlaps",            2, 0, SQLITE_UTF8,    0, soverlapsFunc  },
		{ "snotoverlaps",         2, 0, SQLITE_UTF8,    0, snotoverlapsFunc  },
		{ "sequal",               2, 0, SQLITE_UTF8,    0, sequalFunc  },
		{ "snotequal",            2, 0, SQLITE_UTF8,    0, snotequalFunc  },
		{ "stransform",           2, 0, SQLITE_UTF8,    0, stransformFunc  },
		{ "sinverse",             2, 0, SQLITE_UTF8,    0, sinverseFunc  },
		{ "pi",                   0, 0, SQLITE_UTF8,    0, piFunc  },
	};

	/* Aggregate functions */
	static const struct FuncDefAgg {
		const char *zName;
		signed char nArg;
		uint8_t argType;
		uint8_t needCollSeq;
		void (*xStep)(sqlite3_context*,int,sqlite3_value**);
		void (*xFinalize)(sqlite3_context*);
	} aAggs[] = {
		{ "spoly_add_point_aggr",            2, 0, 0, spoly_add_point_aggrStep, spoly_add_point_aggrFinalize  },
		{ "spoly_aggr",                      1, 0, 0, spoly_aggrStep, spoly_aggrFinalize  },
		{ "spath_add_point_aggr",            2, 0, 0, spath_add_point_aggrStep, spath_add_point_aggrFinalize  },
		{ "spath_aggr",                      1, 0, 0, spath_aggrStep, spath_aggrFinalize  },
	};

	int i;

	for(i = 0; i < sizeof(aFuncs) / sizeof(aFuncs[0]); i++) {
		void *pArg = 0;
		switch( aFuncs[i].argType ) {
			case 1: pArg = db; break;
			case 2: pArg = (void *)(-1); break;
		}

		sqlite3_create_function(db, aFuncs[i].zName, aFuncs[i].nArg,
							aFuncs[i].eTextRep, pArg, aFuncs[i].xFunc, 0, 0);
	}

	for(i = 0; i < sizeof(aAggs) / sizeof(aAggs[0]); i++) {
		void *pArg = 0;
		switch( aAggs[i].argType ) {
			case 1: pArg = db; break;
			case 2: pArg = (void *)(-1); break;
		}

		sqlite3_create_function(db, aAggs[i].zName, aAggs[i].nArg, SQLITE_UTF8, 
							pArg, 0, aAggs[i].xStep, aAggs[i].xFinalize);
	}

	return 0;
}  

int sqlite3_extension_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi) {
	int rc = SQLITE_OK;
	SQLITE_EXTENSION_INIT2(pApi);
	RegisterExtensionFunctions(db);
	return rc;
}