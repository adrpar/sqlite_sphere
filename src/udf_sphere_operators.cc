/* Add your header comment here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "ellipse.h"
#include "circle.h"
#include "line.h"
#include "polygon.h"
#include "box.h"
#include "path.h"
#include "point.h"
#include "udf_helpers.h"
#include "udf_sphere_operators.h"

//supporting srcontainsl(SBox, SBox), srcontainsl(SPoint, SBox), srcontainsl(SCircle, SBox), 
//		srcontainsl(SBox, SCircle), srcontainsl(SLine, SBox), srcontainsl(SEllipse, SBox), 
//		srcontainsl(SBox, SEllipse), srcontainsl(SPoly, SBox), srcontainsl(SBox, SPoly),
//		srcontainsl(SPath, SBox),
//
//		srcontainsl(SCircle, SCircle), srcontainsl(SPoint, SCircle), 
//
//		srcontainsl(SEllipse, SEllipse), srcontainsl(SPoint, SEllipse), srcontainsl(SCircle, SEllipse),
//		srcontainsl(SEllipse, SCircle), srcontainsl(SLine, SEllipse), 
//
//		srcontainsl(SPoint, SLine), srcontainsl(SLine, SCircle),
//
//		srcontainsl(SPoint, SPath), srcontainsl(SPath, SCircle), srcontainsl(SPath, SEllipse),
//		srcontainsl(SPath, SPoly), 
//
//		srcontainsl(SPoly, SPoly), srcontainsl(SPoint, SPoly), srcontainsl(SCircle, SPoly), 
//		srcontainsl(SPoly, SCircle), srcontainsl(SLine, SPoly), srcontainsl(SEllipse, SPoly), 
//		srcontainsl(SPoly, SEllipse), 
void srcontainslFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	int result;

	assert( argc==2 );

	if(argc == 2) {
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
										"srcontainsl() requires the first parameter to be a SQLITE sphere object.");

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										"srcontainsl() requires the first parameter to be a SQLITE sphere object.");

		void * objData0 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;

		void * objData1 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, objData0, decodedDataType0, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_POINT, SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON}), 
									"srcontainsl() error decoding first parameter. Corrupted or not the correct type." );

		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, objData1, decodedDataType1, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON}), 
									"srcontainsl() error decoding second parameter. Corrupted or not the correct type." );

		//some convenience
		SBox * abox = (SBox *) objData0;
		SCircle * acir = (SCircle *) objData0;
		SEllipse * aell = (SEllipse *) objData0;
		SPoint * apoi = (SPoint *) objData0;
		SLine * alin = (SLine *) objData0;
		SPath * apat = (SPath *) objData0;
		SPoly * apol = (SPoly *) objData0;

		switch(decodedDataType0) {
			case SQLITE_SPHERE_BOX:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_box_com(abox, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_cont_box_com(abox, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_box_com(abox, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_box_com(abox, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_CIRCLE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_circle_com(acir, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_in_circle(acir, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_circle_com(acir, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_circle_com(acir, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_ELLIPSE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_ellipse_com(aell, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_cont_ellipse_com(aell, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_ellipse_com(aell, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_ellipse_com(aell, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_POINT:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_point_com(apoi, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherepoint_in_circle(apoi, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = sphereline_cont_point_com(apoi, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_point_com(apoi, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherepath_cont_point_com(apoi, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_point_com(apoi, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_LINE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_line_com(alin, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_cont_line_com(alin, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_line_com(alin, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_line_com(alin, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_PATH:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_path_com(apat, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_cont_path_com(apat, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_path_com(apat, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_path_com(apat, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_POLYGON:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_poly_com(apol, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_cont_poly_com(apol, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_poly_com(apol, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_poly_com(apol, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			default:
				result = 0;
		}

		free(objData0);
		free(objData1);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: srcontainsl() requires two parameters", 666);
		return;
	}

	sqlite3_result_int(context, result);
}

//supporting slcontainsr(SBox, SBox), slcontainsr(SBox, SPoint), slcontainsr(SBox, SCircle), 
//				slcontainsr(SCircle, SBox), slcontainsr(SBox, SLine), slcontainsr(SBox, SEllipse), 
//				slcontainsr(SEllipse, SBox), slcontainsr(SBox, SPoly), slcontainsr(SPoly, SBox), 
//				slcontainsr(SBox, SPath), 
//
//				slcontainsr(SCircle, SCircle), slcontainsr(SCircle, SPoint), 
//
//				slcontainsr(SEllipse, SEllipse), slcontainsr(SEllipse, SPoint), slcontainsr(SEllipse, SCircle), 
//				slcontainsr(SCircle, SEllipse), slcontainsr(SEllipse, SLine), 
//
//				slcontainsr(SLine, SPoint), slcontainsr(SCircle, SLine), 
//
//				slcontainsr(SPath, SPoint), slcontainsr(SCircle, SPath), slcontainsr(SEllipse, SPath), 
//				slcontainsr(SPoly, SPath), 
//
//				slcontainsr(SPoly, SPoly), slcontainsr(SPoly, SPoint), slcontainsr(SPoly, SCircle), 
//				slcontainsr(SCircle, SPoly), slcontainsr(SPoly, SLine), slcontainsr(SPoly, SEllipse), 
//				slcontainsr(SEllipse, SPoly), 
void slcontainsrFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	int result;

	assert( argc==2 );

	if(argc == 2) {
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
										"slcontainsr() requires the first parameter to be a SQLITE sphere object.");

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										"slcontainsr() requires the first parameter to be a SQLITE sphere object.");

		void * objData0 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;

		void * objData1 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, objData0, decodedDataType0, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON}), 
									"slcontainsr() error decoding first parameter. Corrupted or not the correct type." );

		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, objData1, decodedDataType1, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_POINT, SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON}), 
									"slcontainsr() error decoding second parameter. Corrupted or not the correct type." );

		//some convenience
		SBox * abox = (SBox *) objData0;
		SCircle * acir = (SCircle *) objData0;
		SEllipse * aell = (SEllipse *) objData0;
		SPoint * apoi = (SPoint *) objData0;
		SLine * alin = (SLine *) objData0;
		SPath * apat = (SPath *) objData0;
		SPoly * apol = (SPoly *) objData0;

		switch(decodedDataType0) {
			case SQLITE_SPHERE_BOX:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_box(abox, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_POINT:
						result = spherebox_cont_point(abox, (SPoint *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherebox_cont_circle(abox, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = spherebox_cont_line(abox, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = spherebox_cont_ellipse(abox, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherebox_cont_path(abox, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherebox_cont_poly(abox, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_CIRCLE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherecircle_cont_box(acir, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_POINT:
						result = spherepoint_in_circle_com(acir, (SPoint *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_in_circle_com(acir, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = spherecircle_cont_line(acir, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = spherecircle_cont_ellipse(acir, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherecircle_cont_path(acir, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherecircle_cont_poly(acir, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_ELLIPSE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = sphereellipse_cont_box(aell, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_POINT:
						result = sphereellipse_cont_point(aell, (SPoint *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = sphereellipse_cont_circle(aell, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = sphereellipse_cont_line(aell, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_ellipse(aell, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = sphereellipse_cont_path(aell, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = sphereellipse_cont_poly(aell, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_LINE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_POINT:
						result = sphereline_cont_point(alin, (SPoint *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_PATH:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_POINT:
						result = spherepath_cont_point(apat, (SPoint *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_POLYGON:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherepoly_cont_box(apol, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_POINT:
						result = spherepoly_cont_point(apol, (SPoint *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherepoly_cont_circle(apol, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = spherepoly_cont_line(apol, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = spherepoly_cont_ellipse(apol, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherepoly_cont_path(apol, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_poly(apol, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			default:
				result = 0;
		}

		free(objData0);
		free(objData1);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: slcontainsr() requires two parameters", 666);
		return;
	}

	sqlite3_result_int(context, result);
}

//supporting srnotcontainsl(SBox, SBox), srnotcontainsl(SPoint, SBox), srnotcontainsl(SCircle, SBox), 
//		srnotcontainsl(SBox, SCircle), srnotcontainsl(SLine, SBox), srnotcontainsl(SEllipse, SBox), 
//		srnotcontainsl(SBox, SEllipse), srnotcontainsl(SPoly, SBox), srnotcontainsl(SBox, SPoly),
//		srnotcontainsl(SPath, SBox),
//
//		srnotcontainsl(SCircle, SCircle), srnotcontainsl(SPoint, SCircle), 
//
//		srnotcontainsl(SEllipse, SEllipse), srnotcontainsl(SPoint, SEllipse), srnotcontainsl(SCircle, SEllipse),
//		srnotcontainsl(SEllipse, SCircle), srnotcontainsl(SLine, SEllipse), 
//
//		srnotcontainsl(SPoint, SLine), srnotcontainsl(SLine, SCircle),
//
//		srnotcontainsl(SPoint, SPath), srnotcontainsl(SPath, SCircle), srnotcontainsl(SPath, SEllipse),
//		srnotcontainsl(SPath, SPoly), 
//
//		srnotcontainsl(SPoly, SPoly), srnotcontainsl(SPoint, SPoly), srnotcontainsl(SCircle, SPoly), 
//		srnotcontainsl(SPoly, SCircle), srnotcontainsl(SLine, SPoly), srnotcontainsl(SEllipse, SPoly), 
//		srnotcontainsl(SPoly, SEllipse), 
void srnotcontainslFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	int result;

	assert( argc==2 );

	if(argc == 2) {
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
										"srnotcontainsl() requires the first parameter to be a SQLITE sphere object.");

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										"srnotcontainsl() requires the first parameter to be a SQLITE sphere object.");

		void * objData0 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;

		void * objData1 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, objData0, decodedDataType0, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_POINT, SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON}), 
									"srnotcontainsl() error decoding first parameter. Corrupted or not the correct type." );

		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, objData1, decodedDataType1, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON}), 
									"srnotcontainsl() error decoding second parameter. Corrupted or not the correct type." );

		//some convenience
		SBox * abox = (SBox *) objData0;
		SCircle * acir = (SCircle *) objData0;
		SEllipse * aell = (SEllipse *) objData0;
		SPoint * apoi = (SPoint *) objData0;
		SLine * alin = (SLine *) objData0;
		SPath * apat = (SPath *) objData0;
		SPoly * apol = (SPoly *) objData0;

		switch(decodedDataType0) {
			case SQLITE_SPHERE_BOX:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_box_com_neg(abox, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_cont_box_com_neg(abox, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_box_com_neg(abox, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_box_com_neg(abox, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_CIRCLE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_circle_com_neg(acir, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_in_circle_neg(acir, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_circle_com_neg(acir, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_circle_com_neg(acir, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_ELLIPSE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_ellipse_com_neg(aell, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_cont_ellipse_com_neg(aell, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_ellipse_com_neg(aell, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_ellipse_com_neg(aell, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_POINT:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_point_com_neg(apoi, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherepoint_in_circle_neg(apoi, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = sphereline_cont_point_com_neg(apoi, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_point_com_neg(apoi, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherepath_cont_point_com_neg(apoi, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_point_com_neg(apoi, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_LINE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_line_com_neg(alin, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_cont_line_com_neg(alin, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_line_com_neg(alin, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_line_com_neg(alin, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_PATH:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_path_com_neg(apat, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_cont_path_com_neg(apat, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_path_com_neg(apat, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_path_com_neg(apat, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_POLYGON:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_poly_com_neg(apol, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_cont_poly_com_neg(apol, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_poly_com_neg(apol, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_poly_com_neg(apol, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			default:
				result = 0;
		}

		free(objData0);
		free(objData1);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: srnotcontainsl() requires two parameters", 666);
		return;
	}

	sqlite3_result_int(context, result);
}

//supporting slnotcontainsr(SBox, SBox), slnotcontainsr(SBox, SPoint), slnotcontainsr(SBox, SCircle), 
//				slnotcontainsr(SCircle, SBox), slnotcontainsr(SBox, SLine), slnotcontainsr(SBox, SEllipse), 
//				slnotcontainsr(SEllipse, SBox), slnotcontainsr(SBox, SPoly), slnotcontainsr(SPoly, SBox), 
//				slnotcontainsr(SBox, SPath), 
//
//				slnotcontainsr(SCircle, SCircle), slnotcontainsr(SCircle, SPoint), 
//
//				slnotcontainsr(SEllipse, SEllipse), slnotcontainsr(SEllipse, SPoint), slnotcontainsr(SEllipse, SCircle), 
//				slnotcontainsr(SCircle, SEllipse), slnotcontainsr(SEllipse, SLine), 
//
//				slnotcontainsr(SLine, SPoint), slnotcontainsr(SCircle, SLine), 
//
//				slnotcontainsr(SPath, SPoint), slnotcontainsr(SCircle, SPath), slnotcontainsr(SEllipse, SPath), 
//				slnotcontainsr(SPoly, SPath), 
//
//				slnotcontainsr(SPoly, SPoly), slnotcontainsr(SPoly, SPoint), slnotcontainsr(SPoly, SCircle), 
//				slnotcontainsr(SCircle, SPoly), slnotcontainsr(SPoly, SLine), slnotcontainsr(SPoly, SEllipse), 
//				slnotcontainsr(SEllipse, SPoly), 
void slnotcontainsrFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	int result;

	assert( argc==2 );

	if(argc == 2) {
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
										"slnotcontainsr() requires the first parameter to be a SQLITE sphere object.");

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										"slnotcontainsr() requires the first parameter to be a SQLITE sphere object.");

		void * objData0 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;

		void * objData1 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, objData0, decodedDataType0, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON}), 
									"slnotcontainsr() error decoding first parameter. Corrupted or not the correct type." );

		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, objData1, decodedDataType1, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_POINT, SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON}), 
									"slnotcontainsr() error decoding second parameter. Corrupted or not the correct type." );

		//some convenience
		SBox * abox = (SBox *) objData0;
		SCircle * acir = (SCircle *) objData0;
		SEllipse * aell = (SEllipse *) objData0;
		SPoint * apoi = (SPoint *) objData0;
		SLine * alin = (SLine *) objData0;
		SPath * apat = (SPath *) objData0;
		SPoly * apol = (SPoly *) objData0;

		switch(decodedDataType0) {
			case SQLITE_SPHERE_BOX:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_cont_box_neg(abox, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_POINT:
						result = spherebox_cont_point_neg(abox, (SPoint *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherebox_cont_circle_neg(abox, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = spherebox_cont_line_neg(abox, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = spherebox_cont_ellipse_neg(abox, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherebox_cont_path_neg(abox, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherebox_cont_poly_neg(abox, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_CIRCLE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherecircle_cont_box_neg(acir, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_POINT:
						result = spherepoint_in_circle_com_neg(acir, (SPoint *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_in_circle_com_neg(acir, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = spherecircle_cont_line_neg(acir, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = spherecircle_cont_ellipse_neg(acir, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherecircle_cont_path_neg(acir, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherecircle_cont_poly_neg(acir, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_ELLIPSE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = sphereellipse_cont_box_neg(aell, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_POINT:
						result = sphereellipse_cont_point_neg(aell, (SPoint *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = sphereellipse_cont_circle_neg(aell, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = sphereellipse_cont_line_neg(aell, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_cont_ellipse_neg(aell, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = sphereellipse_cont_path_neg(aell, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = sphereellipse_cont_poly_neg(aell, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_LINE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_POINT:
						result = sphereline_cont_point_neg(alin, (SPoint *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_PATH:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_POINT:
						result = spherepath_cont_point_neg(apat, (SPoint *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_POLYGON:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherepoly_cont_box_neg(apol, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_POINT:
						result = spherepoly_cont_point_neg(apol, (SPoint *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherepoly_cont_circle_neg(apol, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = spherepoly_cont_line_neg(apol, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = spherepoly_cont_ellipse_neg(apol, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherepoly_cont_path_neg(apol, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_cont_poly_neg(apol, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			default:
				result = 0;
		}

		free(objData0);
		free(objData1);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: slnotcontainsr() requires two parameters", 666);
		return;
	}

	sqlite3_result_int(context, result);
}

//supports every combination except SPoint
void  soverlapsFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	int result;

	assert( argc==2 );

	if(argc == 2) {
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
										" soverlaps() requires the first parameter to be a SQLITE sphere object.");

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										" soverlaps() requires the first parameter to be a SQLITE sphere object.");

		void * objData0 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;

		void * objData1 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, objData0, decodedDataType0, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_POINT, SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON}), 
									" soverlaps() error decoding first parameter. Corrupted or not the correct type." );

		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, objData1, decodedDataType1, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_POINT, SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON}), 
									" soverlaps() error decoding second parameter. Corrupted or not the correct type." );

		//some convenience
		SBox * abox = (SBox *) objData0;
		SCircle * acir = (SCircle *) objData0;
		SEllipse * aell = (SEllipse *) objData0;
		SPoint * apoi = (SPoint *) objData0;
		SLine * alin = (SLine *) objData0;
		SPath * apat = (SPath *) objData0;
		SPoly * apol = (SPoly *) objData0;

		switch(decodedDataType0) {
			case SQLITE_SPHERE_BOX:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_overlap_box(abox, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherebox_overlap_circle(abox, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = spherebox_overlap_line(abox, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = spherebox_overlap_ellipse(abox, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherebox_overlap_path(abox, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherebox_overlap_poly(abox, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_CIRCLE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_overlap_circle_com(acir, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_overlap(acir, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = sphereline_overlap_circle_com(acir, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_overlap_circle_com(acir, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherecircle_overlap_path(acir, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_overlap_circle_com(acir, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_ELLIPSE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_overlap_ellipse_com(aell, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = sphereellipse_overlap_circle(aell, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = sphereellipse_overlap_line(aell, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_overlap_ellipse(aell, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = sphereellipse_overlap_path(aell, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_overlap_ellipse_com(aell, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_LINE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_overlap_line_com(alin, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = sphereline_overlap_circle(alin, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = sphereline_overlap(alin, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_overlap_line_com(alin, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherepath_overlap_line_com(alin, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_overlap_line_com(alin, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_PATH:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_overlap_path_com(apat, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_overlap_path_com(apat, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = spherepath_overlap_line(apat, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_overlap_path_com(apat, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherepath_overlap_path(apat, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_overlap_path_com(apat, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_POLYGON:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_overlap_poly_com(apol, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherepoly_overlap_circle(apol, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = spherepoly_overlap_line(apol, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = spherepoly_overlap_ellipse(apol, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherepoly_overlap_path(apol, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_overlap_poly(apol, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			default:
				result = 0;
		}

		free(objData0);
		free(objData1);
	} else {
		sqlite3_result_error(context, "wrong number of arguments:  soverlaps() requires two parameters", 666);
		return;
	}

	sqlite3_result_int(context, result);
}

//supports every combination except SPoint
void  snotoverlapsFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	int result;

	assert( argc==2 );

	if(argc == 2) {
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
										" snotoverlaps() requires the first parameter to be a SQLITE sphere object.");

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										" snotoverlaps() requires the first parameter to be a SQLITE sphere object.");

		void * objData0 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;

		void * objData1 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, objData0, decodedDataType0, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_POINT, SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON}), 
									" snotoverlaps() error decoding first parameter. Corrupted or not the correct type." );

		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, objData1, decodedDataType1, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_POINT, SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON}), 
									" snotoverlaps() error decoding second parameter. Corrupted or not the correct type." );

		//some convenience
		SBox * abox = (SBox *) objData0;
		SCircle * acir = (SCircle *) objData0;
		SEllipse * aell = (SEllipse *) objData0;
		SPoint * apoi = (SPoint *) objData0;
		SLine * alin = (SLine *) objData0;
		SPath * apat = (SPath *) objData0;
		SPoly * apol = (SPoly *) objData0;

		switch(decodedDataType0) {
			case SQLITE_SPHERE_BOX:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_overlap_box_neg(abox, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherebox_overlap_circle_neg(abox, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = spherebox_overlap_line_neg(abox, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = spherebox_overlap_ellipse_neg(abox, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherebox_overlap_path_neg(abox, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherebox_overlap_poly_neg(abox, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_CIRCLE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_overlap_circle_com_neg(acir, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_overlap_neg(acir, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = sphereline_overlap_circle_com_neg(acir, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_overlap_circle_com_neg(acir, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherecircle_overlap_path_neg(acir, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_overlap_circle_com_neg(acir, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_ELLIPSE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_overlap_ellipse_com_neg(aell, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = sphereellipse_overlap_circle_neg(aell, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = sphereellipse_overlap_line_neg(aell, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_overlap_ellipse_neg(aell, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = sphereellipse_overlap_path_neg(aell, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_overlap_ellipse_com_neg(aell, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_LINE:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_overlap_line_com_neg(alin, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = sphereline_overlap_circle_neg(alin, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = sphereline_overlap_neg(alin, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_overlap_line_com_neg(alin, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherepath_overlap_line_com_neg(alin, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_overlap_line_com_neg(alin, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_PATH:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_overlap_path_com_neg(apat, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherecircle_overlap_path_com_neg(apat, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = spherepath_overlap_line_neg(apat, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = sphereellipse_overlap_path_com_neg(apat, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherepath_overlap_path_neg(apat, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_overlap_path_com_neg(apat, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			case SQLITE_SPHERE_POLYGON:
				switch(decodedDataType1) {
					case SQLITE_SPHERE_BOX:
						result = spherebox_overlap_poly_com_neg(apol, (SBox *) objData1);
						break;
					case SQLITE_SPHERE_CIRCLE:
						result = spherepoly_overlap_circle_neg(apol, (SCircle *) objData1);
						break;
					case SQLITE_SPHERE_LINE:
						result = spherepoly_overlap_line_neg(apol, (SLine *) objData1);
						break;
					case SQLITE_SPHERE_ELLIPSE:
						result = spherepoly_overlap_ellipse_neg(apol, (SEllipse *) objData1);
						break;
					case SQLITE_SPHERE_PATH:
						result = spherepoly_overlap_path_neg(apol, (SPath *) objData1);
						break;
					case SQLITE_SPHERE_POLYGON:
						result = spherepoly_overlap_poly_neg(apol, (SPoly *) objData1);
						break;
					default:
						result = 0;
				}

				break;

			default:
				result = 0;
		}

		free(objData0);
		free(objData1);
	} else {
		sqlite3_result_error(context, "wrong number of arguments:  snotoverlaps() requires two parameters", 666);
		return;
	}

	sqlite3_result_int(context, result);
}

//supports sequal(SBox, SBox), sequal(SCircle, SCirlce), sequal(SEllipse, SEllipse), sequal(STransform, STransform), 
//			sequal(SLine, SLine), sequal(SPath, SPath), sequal(SPoint, SPoint), sequal(SPoly, SPoly), 
void  sequalFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	int result;

	assert( argc==2 );

	if(argc == 2) {
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
										" sequal() requires the first parameter to be a SQLITE sphere object.");

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										" sequal() requires the first parameter to be a SQLITE sphere object.");

		void * objData0 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;

		void * objData1 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, objData0, decodedDataType0, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_POINT, SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON, SQLITE_SPHERE_EULER}), 
									" sequal() error decoding first parameter. Corrupted or not the correct type." );

		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, objData1, decodedDataType1, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_POINT, SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON, SQLITE_SPHERE_EULER}), 
									" sequal() error decoding second parameter. Corrupted or not the correct type." );

		if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_BOX) {
			result = spherebox_equal((SBox*) objData0, (SBox*) objData1);
		} else if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
			result = spherecircle_equal((SCircle*) objData0, (SCircle*) objData1);
		} else if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
			result = sphereellipse_equal((SEllipse*) objData0, (SEllipse*) objData1);
		} else if(decodedDataType0 == SQLITE_SPHERE_EULER && decodedDataType1 == SQLITE_SPHERE_EULER) {
			result = spheretrans_equal((SEuler*) objData0, (SEuler*) objData1);
		} else if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_LINE) {
			result = sphereline_equal((SLine*) objData0, (SLine*) objData1);
		} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_PATH) {
			result = spherepath_equal((SPath*) objData0, (SPath*) objData1);
		} else if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_POINT) {
			result = spherepoint_equal((SPoint*) objData0, (SPoint*) objData1);
		} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
			result = spherepoly_equal((SPoly*) objData0, (SPoly*) objData1);
		} else {
			result = 0;
		}

		free(objData0);
		free(objData1);
	} else {
		sqlite3_result_error(context, "wrong number of arguments:  sequal() requires two parameters", 666);
		return;
	}

	sqlite3_result_int(context, result);
}

//supports snotequal(SBox, SBox), snotequal(SCircle, SCirlce), snotequal(SEllipse, SEllipse), snotequal(STransform, STransform), 
//			snotequal(SLine, SLine), snotequal(SPath, SPath), snotequal(SPoint, SPoint), snotequal(SPoly, SPoly), 
void  snotequalFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	int result;

	assert( argc==2 );

	if(argc == 2) {
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
										" snotequal() requires the first parameter to be a SQLITE sphere object.");

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										" snotequal() requires the first parameter to be a SQLITE sphere object.");

		void * objData0 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;

		void * objData1 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, objData0, decodedDataType0, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_POINT, SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON, SQLITE_SPHERE_EULER}), 
									" snotequal() error decoding first parameter. Corrupted or not the correct type." );

		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, objData1, decodedDataType1, 
									PROTECT({SQLITE_SPHERE_BOX, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_POINT, SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON, SQLITE_SPHERE_EULER}), 
									" snotequal() error decoding second parameter. Corrupted or not the correct type." );

		if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_BOX) {
			result = spherebox_equal_neg((SBox*) objData0, (SBox*) objData1);
		} else if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
			result = spherecircle_equal_neg((SCircle*) objData0, (SCircle*) objData1);
		} else if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
			result = sphereellipse_equal_neg((SEllipse*) objData0, (SEllipse*) objData1);
		} else if(decodedDataType0 == SQLITE_SPHERE_EULER && decodedDataType1 == SQLITE_SPHERE_EULER) {
			result = spheretrans_not_equal((SEuler*) objData0, (SEuler*) objData1);
		} else if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_LINE) {
			result = sphereline_equal_neg((SLine*) objData0, (SLine*) objData1);
		} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_PATH) {
			result = spherepath_equal_neg((SPath*) objData0, (SPath*) objData1);
		} else if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_POINT) {
			result = !spherepoint_equal((SPoint*) objData0, (SPoint*) objData1);
		} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
			result = spherepoly_equal_neg((SPoly*) objData0, (SPoly*) objData1);
		} else {
			result = 0;
		}

		free(objData0);
		free(objData1);
	} else {
		sqlite3_result_error(context, "wrong number of arguments:  snotequal() requires two parameters", 666);
		return;
	}

	sqlite3_result_int(context, result);
}

//supports stransform(SCircle, STrans), stransform(SEllipse, STrans), stransform(STrans, STrans),
//			 stransform(SPoint, STrans), stransform(SLine, STrans), stransform(SPath, STrans),
//			 stransform(SPoly, STrans)
void  stransformFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	char * strResult = NULL;
	int resultLen = 0;

	assert( argc==2 );

	if(argc == 2) {
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
										" stransform() requires the first parameter to be a SQLITE sphere object.");

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										" stransform() requires the first parameter to be a SQLITE sphere object.");

		void * objData0 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;

		void * objData1 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, objData0, decodedDataType0, 
									PROTECT({SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_POINT, SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON, SQLITE_SPHERE_EULER}), 
									" stransform() error decoding first parameter. Corrupted or not the correct type." );

		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, objData1, decodedDataType1, 
									PROTECT({SQLITE_SPHERE_EULER}), 
									" stransform() error decoding second parameter. Corrupted or not the correct type." );

		if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_EULER) {
			SCircle * circle = spheretrans_circle((SCircle*) objData0, (SEuler*) objData1);
			strResult = serialise(circle);
			resultLen = getSerialisedLen(circle);
			free(circle);
		} else if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_EULER) {
			SEllipse * ellipse = spheretrans_ellipse((SEllipse*) objData0, (SEuler*) objData1);
			strResult = serialise(ellipse);
			resultLen = getSerialisedLen(ellipse);
			free(ellipse);
		} else if(decodedDataType0 == SQLITE_SPHERE_EULER && decodedDataType1 == SQLITE_SPHERE_EULER) {
			SEuler * euler = spheretrans_trans((SEuler*) objData0, (SEuler*) objData1);
			strResult = serialise(euler);
			resultLen = getSerialisedLen(euler);
			free(euler);
		} else if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_EULER) {
			SLine * line = spheretrans_line((SLine*) objData0, (SEuler*) objData1);
			strResult = serialise(line);
			resultLen = getSerialisedLen(line);
			free(line);
		} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_EULER) {
			SPath * path = spheretrans_path((SPath*) objData0, (SEuler*) objData1);
			strResult = serialise(path);
			resultLen = getSerialisedLen(path);
			free(path);
		} else if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_EULER) {
			SPoint * point = spheretrans_point((SPoint*) objData0, (SEuler*) objData1);
			strResult = serialise(point);
			resultLen = getSerialisedLen(point);
			free(point);
		} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_EULER) {
			SPoly * poly = spheretrans_poly((SPoly*) objData0, (SEuler*) objData1);
			strResult = serialise(poly);
			resultLen = getSerialisedLen(poly);
			free(poly);
		} else {
			sqlite3_result_null(context);
			return;
		}

		free(objData0);
		free(objData1);
	} else {
		sqlite3_result_error(context, "wrong number of arguments:  stransform() requires two parameters", 666);
		return;
	}

	sqlite3_result_blob(context, strResult, resultLen, SQLITE_TRANSIENT);

	free(strResult);
}

//supports sinverse(SCircle, STrans), sinverse(SEllipse, STrans), sinverse(STrans, STrans),
//			 sinverse(SPoint, STrans), sinverse(SLine, STrans), sinverse(SPath, STrans),
//			 sinverse(SPoly, STrans)

void  sinverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	char * strResult = NULL;
	int resultLen = 0;

	assert( argc==2 );

	if(argc == 2) {
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
										" sinverse() requires the first parameter to be a SQLITE sphere object.");

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										" sinverse() requires the first parameter to be a SQLITE sphere object.");

		void * objData0 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;

		void * objData1 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, objData0, decodedDataType0, 
									PROTECT({SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE,
    											SQLITE_SPHERE_POINT, SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH,
    											SQLITE_SPHERE_POLYGON, SQLITE_SPHERE_EULER}), 
									" sinverse() error decoding first parameter. Corrupted or not the correct type." );

		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, objData1, decodedDataType1, 
									PROTECT({SQLITE_SPHERE_EULER}), 
									" sinverse() error decoding second parameter. Corrupted or not the correct type." );

		if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_EULER) {
			SCircle * circle = spheretrans_circle_inverse((SCircle*) objData0, (SEuler*) objData1);
			strResult = serialise(circle);
			resultLen = getSerialisedLen(circle);
			free(circle);
		} else if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_EULER) {
			SEllipse * ellipse = spheretrans_ellipse_inv((SEllipse*) objData0, (SEuler*) objData1);
			strResult = serialise(ellipse);
			resultLen = getSerialisedLen(ellipse);
			free(ellipse);
		} else if(decodedDataType0 == SQLITE_SPHERE_EULER && decodedDataType1 == SQLITE_SPHERE_EULER) {
			SEuler * euler = spheretrans_trans_inv((SEuler*) objData0, (SEuler*) objData1);
			strResult = serialise(euler);
			resultLen = getSerialisedLen(euler);
			free(euler);
		} else if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_EULER) {
			SLine * line = spheretrans_line_inverse((SLine*) objData0, (SEuler*) objData1);
			strResult = serialise(line);
			resultLen = getSerialisedLen(line);
			free(line);
		} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_EULER) {
			SPath * path = spheretrans_path_inverse((SPath*) objData0, (SEuler*) objData1);
			strResult = serialise(path);
			resultLen = getSerialisedLen(path);
			free(path);
		} else if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_EULER) {
			SPoint * point = spheretrans_point_inverse((SPoint*) objData0, (SEuler*) objData1);
			strResult = serialise(point);
			resultLen = getSerialisedLen(point);
			free(point);
		} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_EULER) {
			SPoly * poly = spheretrans_poly_inverse((SPoly*) objData0, (SEuler*) objData1);
			strResult = serialise(poly);
			resultLen = getSerialisedLen(poly);
			free(poly);
		} else {
			sqlite3_result_null(context);
			return;
		}

		free(objData0);
		free(objData1);
	} else {
		sqlite3_result_error(context, "wrong number of arguments:  sinverse() requires two parameters", 666);
		return;
	}

	sqlite3_result_blob(context, strResult, resultLen, SQLITE_TRANSIENT);

	free(strResult);
}


