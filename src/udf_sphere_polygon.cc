/* Add your header comment here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "polygon.h"
#include "udf_helpers.h"
#include "udf_sphere_polygon.h"

//supporting spoly(polygon_string)
void spolyFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon = NULL;

	assert( argc==1 );

	if(argc == 1) {
		//string
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHAR( 0, data, dataLen, 
										"spoly(polygon_string) requires a string.");

		//spoly(polygon_string)
		polygon = spherepoly_in( (char*)data );
	} else {
		sqlite3_result_error(context, "wrong number of arguments: spoly() requires one parameter", 666);
		return;
	}

	if(polygon == NULL) {
		sqlite3_result_error(context, "an error occurred while generating the polygon", 666);
		return;
	}

	char * result;
	int resultLen;
	result = serialise(polygon);
	resultLen = getSerialisedLen(polygon);
	free(polygon);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);

	free(result);
}

//spoly_equal(SPoly, SPoly)...
void spoly_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon0 = NULL;
	SPoly * polygon1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_equal", polygon0, polygon1, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_equal(polygon0, polygon1);
	sqlite3_result_int(context, res);
	free(polygon0);
	free(polygon1);
}

//spoly_equal_neg(SPoly, SPoly)...
void spoly_equal_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon0 = NULL;
	SPoly * polygon1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_equal_neg", polygon0, polygon1, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_equal_neg(polygon0, polygon1);
	sqlite3_result_int(context, res);
	free(polygon0);
	free(polygon1);
}

//spoly_contains_polygon(SPoly, SPoly)...
void spoly_contains_polygonFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon0 = NULL;
	SPoly * polygon1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_polygon", polygon0, polygon1, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_poly(polygon0, polygon1);
	sqlite3_result_int(context, res);
	free(polygon0);
	free(polygon1);
}

//spoly_contains_polygon_neg(SPoly, SPoly)...
void spoly_contains_polygon_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon0 = NULL;
	SPoly * polygon1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_polygon_neg", polygon0, polygon1, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_poly_neg(polygon0, polygon1);
	sqlite3_result_int(context, res);
	free(polygon0);
	free(polygon1);
}

//spoly_contains_polygon_com(SPoly, SPoly)...
void spoly_contains_polygon_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon0 = NULL;
	SPoly * polygon1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_pol_comygon", polygon0, polygon1, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_poly_com(polygon0, polygon1);
	sqlite3_result_int(context, res);
	free(polygon0);
	free(polygon1);
}

//spoly_contains_polygon_com_neg(SPoly, SPoly)...
void spoly_contains_polygon_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon0 = NULL;
	SPoly * polygon1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_polygon_com_neg", polygon0, polygon1, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_poly_com_neg(polygon0, polygon1);
	sqlite3_result_int(context, res);
	free(polygon0);
	free(polygon1);
}

//spoly_overlap_polygon(SPoly, SPoly)...
void spoly_overlap_polygonFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon0 = NULL;
	SPoly * polygon1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_overlap_polygon", polygon0, polygon1, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_overlap_poly(polygon0, polygon1);
	sqlite3_result_int(context, res);
	free(polygon0);
	free(polygon1);
}

//spoly_overlap_polygon_neg(SPoly, SPoly)...
void spoly_overlap_polygon_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon0 = NULL;
	SPoly * polygon1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_overlap_polygon_neg", polygon0, polygon1, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_overlap_poly_neg(polygon0, polygon1);
	sqlite3_result_int(context, res);
	free(polygon0);
	free(polygon1);
}

//spoly_contains_point(SPoly, SPoint)...
void spoly_contains_pointFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_contains_point", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_POINT}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_cont_point((SPoly*) objData0, (SPoint*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_POINT) {
		res = (long long)spherepoly_cont_point_com((SPoint*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_contains_point_neg(SPoly, SPoint)...
void spoly_contains_point_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_contains_point_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_POINT}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_cont_point_neg((SPoly*) objData0, (SPoint*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_POINT) {
		res = (long long)spherepoly_cont_point_com_neg((SPoint*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_contains_point_com(SPoint, SPoly)...
void spoly_contains_point_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_point_com", point, polygon, PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_point_com(point, polygon);
	sqlite3_result_int(context, res);
	free(point);
	free(polygon);
}

//spoly_contains_point_com_neg(SPoly, SPoly)...
void spoly_contains_point_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_point_com_neg", point, polygon, PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_point_com_neg(point, polygon);
	sqlite3_result_int(context, res);
	free(point);
	free(polygon);
}

//strans_poly(SPoly, SEuler)...
void strans_polyFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon = NULL;
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_poly", polygon, euler, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	SPoly * resPolygon = spheretrans_poly(polygon, euler);

	char * result = NULL;
	int resultLen;
	result = serialise(resPolygon);
	resultLen = getSerialisedLen(resPolygon);
	free(resPolygon);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(polygon);
	free(euler);
	free(result);
}

//strans_poly_inverse(SPoly, SEuler)...
void strans_poly_inverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon = NULL;
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_poly_inverse", polygon, euler, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	SPoly * resPolygon = spheretrans_poly_inverse(polygon, euler);

	char * result = NULL;
	int resultLen;
	result = serialise(resPolygon);
	resultLen = getSerialisedLen(resPolygon);
	free(resPolygon);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(polygon);
	free(euler);
	free(result);
}

//spoly_contains_circle(SPoly, SCircle)...
void spoly_contains_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_contains_circle", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_cont_circle((SPoly*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherepoly_cont_circle_com((SCircle*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_contains_circle_neg(SPoly, SCircle)...
void spoly_contains_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_contains_circle_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_cont_circle_neg((SPoly*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherepoly_cont_circle_com_neg((SCircle*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_contains_circle_com(SCircle, SPoly)...
void spoly_contains_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_circle_com", circle, polygon, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_circle_com(circle, polygon);
	sqlite3_result_int(context, res);
	free(circle);
	free(polygon);
}

//spoly_contains_circle_com_neg(SCircle, SPoly)...
void spoly_contains_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_circle_com_neg", circle, polygon, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_circle_com_neg(circle, polygon);
	sqlite3_result_int(context, res);
	free(circle);
	free(polygon);
}

//scircle_contains_polygon(SCircle, SPoly)...
void scircle_contains_polygonFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "scircle_contains_polygon", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherecircle_cont_poly((SCircle*) objData0, (SPoly*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherecircle_cont_poly_com((SPoly*) objData0, (SCircle*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//scircle_contains_polygon_neg(SCircle, SPoly)...
void scircle_contains_polygon_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "scircle_contains_polygon_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherecircle_cont_poly_neg((SCircle*) objData0, (SPoly*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherecircle_cont_poly_com_neg((SPoly*) objData0, (SCircle*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//scircle_contains_polygon_com(SPoly, SCircle)...
void scircle_contains_polygon_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon = NULL;
	SCircle * circle = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_contains_polygon_com", polygon, circle, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_cont_poly_com(polygon, circle);
	sqlite3_result_int(context, res);
	free(polygon);
	free(circle);
}

//scircle_contains_polygon_com_neg(SPoly, SCircle)...
void scircle_contains_polygon_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon = NULL;
	SCircle * circle = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_contains_polygon_com_neg", polygon, circle, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_cont_poly_com_neg(polygon, circle);
	sqlite3_result_int(context, res);
	free(polygon);
	free(circle);
}

//spoly_overlap_circle(SPoly, SCircle)...
void spoly_overlap_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_overlap_circle", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_overlap_circle((SPoly*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherepoly_overlap_circle_com((SCircle*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_overlap_circle_neg(SPoly, SCircle)...
void spoly_overlap_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_overlap_circle_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_overlap_circle_neg((SPoly*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherepoly_overlap_circle_com_neg((SCircle*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_overlap_circle_com(SCircle, SPoly)...
void spoly_overlap_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_overlap_circle_com", circle, polygon, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_overlap_circle_com(circle, polygon);
	sqlite3_result_int(context, res);
	free(circle);
	free(polygon);
}

//spoly_overlap_circle_com_neg(SCircle, SPoly)...
void spoly_overlap_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_overlap_circle_com_neg", circle, polygon, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_overlap_circle_com_neg(circle, polygon);
	sqlite3_result_int(context, res);
	free(circle);
	free(polygon);
}

//spoly_contains_line(SPoly, SLine)...
void spoly_contains_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_contains_line", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_cont_line((SPoly*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)spherepoly_cont_line_com((SLine*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_contains_line_neg(SPoly, SLine)...
void spoly_contains_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_contains_line_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_cont_line_neg((SPoly*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)spherepoly_cont_line_com_neg((SLine*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_contains_line_com(SLine, SPoly)...
void spoly_contains_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_line_com", line, polygon, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_line_com(line, polygon);
	sqlite3_result_int(context, res);
	free(line);
	free(polygon);
}

//spoly_contains_line_com_neg(SLine, SPoly)...
void spoly_contains_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_line_com_neg", line, polygon, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_line_com_neg(line, polygon);
	sqlite3_result_int(context, res);
	free(line);
	free(polygon);
}

//spoly_overlaps_line(SPoly, SLine)...
void spoly_overlaps_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_overlaps_line", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_overlap_line((SPoly*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)spherepoly_overlap_line_com((SLine*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_overlaps_line_neg(SPoly, SLine)...
void spoly_overlaps_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_overlaps_line_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_overlap_line_neg((SPoly*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)spherepoly_overlap_line_com_neg((SLine*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_overlaps_line_com(SLine, SPoly)...
void spoly_overlaps_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_overlaps_line_com", line, polygon, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_overlap_line_com(line, polygon);
	sqlite3_result_int(context, res);
	free(line);
	free(polygon);
}

//spoly_overlaps_line_com_neg(SLine, SPoly)...
void spoly_overlaps_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_overlaps_line_com_neg", line, polygon, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_overlap_line_com_neg(line, polygon);
	sqlite3_result_int(context, res);
	free(line);
	free(polygon);
}

//spoly_contains_ellipse(SPoly, SEllipse)...
void spoly_contains_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_contains_ellipse", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_cont_ellipse((SPoly*) objData0, (SEllipse*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)spherepoly_cont_ellipse_com((SEllipse*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_contains_ellipse_neg(SPoly, SEllipse)...
void spoly_contains_ellipse_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_contains_ellipse_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_cont_ellipse_neg((SPoly*) objData0, (SEllipse*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)spherepoly_cont_ellipse_com_neg((SEllipse*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_contains_ellipse_com(SEllipse, SPoly)...
void spoly_contains_ellipse_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_ellipse_com", ellipse, polygon, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_ellipse_com(ellipse, polygon);
	sqlite3_result_int(context, res);
	free(ellipse);
	free(polygon);
}

//spoly_contains_ellipse_com_neg(SEllipse, SPoly)...
void spoly_contains_ellipse_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_ellipse_com_neg", ellipse, polygon, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_ellipse_com_neg(ellipse, polygon);
	sqlite3_result_int(context, res);
	free(ellipse);
	free(polygon);
}

//sellipse_contains_polygon(SEllipse, SPoly)...
void sellipse_contains_polygonFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_contains_polygon", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)sphereellipse_cont_poly((SEllipse*) objData0, (SPoly*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_cont_poly_com((SPoly*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_contains_polygon_neg(SEllipse, SPoly)...
void sellipse_contains_polygon_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_contains_polygon_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)sphereellipse_cont_poly_neg((SEllipse*) objData0, (SPoly*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_cont_poly_com_neg((SPoly*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_contains_polygon_com(SPoly, SEllipse)...
void sellipse_contains_polygon_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_polygon_com", polygon, ellipse, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_poly_com(polygon, ellipse);
	sqlite3_result_int(context, res);
	free(polygon);
	free(ellipse);
}

//sellipse_contains_polygon_com_neg(SPoly, SEllipse)...
void sellipse_contains_polygon_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_polygon_com_neg", polygon, ellipse, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_poly_com_neg(polygon, ellipse);
	sqlite3_result_int(context, res);
	free(polygon);
	free(ellipse);
}

//spoly_overlap_ellipse(SPoly, SEllipse)...
void spoly_overlap_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_overlap_ellipse", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_overlap_ellipse((SPoly*) objData0, (SEllipse*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)spherepoly_overlap_ellipse_com((SEllipse*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_overlap_ellipse_neg(SPoly, SEllipse)...
void spoly_overlap_ellipse_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_overlap_ellipse_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_overlap_ellipse_neg((SPoly*) objData0, (SEllipse*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)spherepoly_overlap_ellipse_com_neg((SEllipse*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_overlap_ellipse_com(SEllipse, SPoly)...
void spoly_overlap_ellipse_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_overlap_ellipse_com", ellipse, polygon, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_overlap_ellipse_com(ellipse, polygon);
	sqlite3_result_int(context, res);
	free(ellipse);
	free(polygon);
}

//spoly_overlap_ellipse_com_neg(SEllipse, SPoly)...
void spoly_overlap_ellipse_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_overlap_ellipse_com_neg", ellipse, polygon, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_overlap_ellipse_com_neg(ellipse, polygon);
	sqlite3_result_int(context, res);
	free(ellipse);
	free(polygon);
}

typedef struct SPolyBuf {
	SPoly * polygon;
} SPolyBuf;

//spoly_add_point_aggr(SPoly, SPoint)...
void spoly_add_point_aggrStep(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPolyBuf * polyBuf = NULL;

	assert( argc==2 );

	polyBuf = (SPolyBuf*)sqlite3_aggregate_context(context, sizeof(*polyBuf));

	if(argc == 2) {
		//string
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										"spoly_add_point_aggr() requires the second parameter to be a SQLITE sphere object.");

		//we can only process one polygon at the time - if multiple come in here out of no reason
		//then we just use the first one and stick to it - in case of GROUP BY, it will be reread
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		if(polyBuf->polygon == NULL) {
			SPoly * tmpPoly = polyBuf->polygon;
			
			SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
											"spoly_add_point_aggr() requires the first parameter to be a SQLITE sphere object.");

			SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, tmpPoly, decodedDataType, PROTECT({SQLITE_SPHERE_POLYGON}), 
										"spoly_add_point_aggr() error decoding first parameter. Corrupted or not the correct type." );
		}

		SPoint * point = NULL;
		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, point, decodedDataType, PROTECT({SQLITE_SPHERE_POINT}), 
									"spoly_add_point_aggr() error decoding second parameter. Corrupted or not the correct type." );


		polyBuf->polygon = spherepoly_add_point(polyBuf->polygon, point);

		free(point);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: spoly_add_point_aggr() requires two parameters", 666);
		return;
	}
}

void spoly_add_point_aggrFinalize(sqlite3_context *context) {
	SPolyBuf * polyBuf = NULL;

	polyBuf = (SPolyBuf*)sqlite3_aggregate_context(context, 0);

	if(polyBuf && polyBuf->polygon) {
		char * result;
		int resultLen;
		result = serialise(polyBuf->polygon);
		resultLen = getSerialisedLen(polyBuf->polygon);
		free(polyBuf->polygon);

		sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);

		free(result);
	} else {
		sqlite3_result_null(context);
	}
}

//spoly_aggr(SPoint)...
void spoly_aggrStep(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPolyBuf * polyBuf = NULL;

	assert( argc==1 );

	polyBuf = (SPolyBuf*)sqlite3_aggregate_context(context, sizeof(*polyBuf));

	if(argc == 1) {
		//string
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, 
										"spoly_aggr() requires the first parameter to be a SQLITE sphere object.");

		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		SPoint * point = NULL;
		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, point, decodedDataType, PROTECT({SQLITE_SPHERE_POINT}), 
									"spoly_aggr() error decoding first parameter. Corrupted or not the correct type." );


		polyBuf->polygon = spherepoly_add_point(polyBuf->polygon, point);

		free(point);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: spoly_aggr() requires two parameters", 666);
		return;
	}
}

void spoly_aggrFinalize(sqlite3_context *context) {
	SPolyBuf * polyBuf = NULL;

	polyBuf = (SPolyBuf*)sqlite3_aggregate_context(context, 0);

	if(polyBuf && polyBuf->polygon) {
		char * result;
		int resultLen;
		result = serialise(polyBuf->polygon);
		resultLen = getSerialisedLen(polyBuf->polygon);
		free(polyBuf->polygon);

		sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);

		free(result);
	} else {
		sqlite3_result_null(context);
	}
}


