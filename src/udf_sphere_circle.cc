/* Add your header comment here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "circle.h"
#include "ellipse.h"
#include "udf_helpers.h"
#include "udf_sphere_circle.h"

//supporting scircle(spoint, rad), scircle(spoint),scircle(sellipse) or scircle(circle_string)
void scircleFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;

	assert( argc==1 || argc==2 );

	if(argc == 2) {
		char * data = NULL;
		int dataLen = 0;
		double rad = 0.0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, 
										"scircle(spoint, rad) requires the first parameter to be a SQLITE sphere object.");
		SQLITE_UDF_CHK_PARAM_REAL( 1, rad, "scircle(spoint, rad) requires rad to be a number." );

    	SPoint * point = NULL;
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, point, decodedDataType, PROTECT({SQLITE_SPHERE_POINT}), 
									"scircle(spoint, rad) error decoding first parameter. Corrupted or not the correct type." );

		circle = spherecircle_by_center(point, rad);

		free(point);
	} else if(argc == 1) {
		//string
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, 
										"scircle(circle_string) requires a string. scircle(spoint) a SQLITE sphere object.");

    	void * objBuf = NULL;
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, objBuf, decodedDataType,
									PROTECT({SQLITE_SPHERE_POINT, SQLITE_SPHERE_ELLIPSE, SQLITE_SPHERE_UNKNOWN}), 
									"scircle(spoint), scircle(sellipse) error decoding first parameter. Corrupted or not the correct type." );

    	if(decodedDataType == SQLITE_SPHERE_POINT) {
    		//scircle(spoint)
    		circle = spherepoint_to_circle((SPoint *)objBuf);
    		free(objBuf);
    	} else if(decodedDataType == SQLITE_SPHERE_ELLIPSE) {
    		//scircle(spoint)
    		circle = sphereellipse_circle((SEllipse *)objBuf);
    		free(objBuf);
    	} else {
    		//scircle(circle_string)
			circle = spherecircle_in( (char*)data );
    		if(objBuf != NULL)
    			free(objBuf);
		}
	} else {
		sqlite3_result_error(context, "wrong number of arguments: scircle() requires one or two parameters", 666);
		return;
	}

	if(circle == NULL) {
		sqlite3_result_error(context, "an error occurred while generating the circle", 666);
		return;
	}

	char * result;
	int resultLen;
	result = serialise(circle);
	resultLen = getSerialisedLen(circle);
	free(circle);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);

	free(result);
}

//scircle_radius(SCircle)
void scircle_radiusFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "scircle_radius", circle, PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	double res = spherecircle_radius(circle);
	sqlite3_result_double(context, res);
	free(circle);
}

//scircle_equal(SCircle, SCircle)...
void scircle_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle0 = NULL;
	SCircle * circle1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_equal", circle0, circle1, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_equal(circle0, circle1);
	sqlite3_result_int(context, res);
	free(circle0);
	free(circle1);
}

//scircle_equal_neg(SCircle, SCircle)...
void scircle_equal_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle0 = NULL;
	SCircle * circle1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_equal_neg", circle0, circle1, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_equal_neg(circle0, circle1);
	sqlite3_result_int(context, res);
	free(circle0);
	free(circle1);
}

//scircle_overlap(SCircle, SCircle)...
void scircle_overlapFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle0 = NULL;
	SCircle * circle1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_overlap", circle0, circle1, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_overlap(circle0, circle1);
	sqlite3_result_int(context, res);
	free(circle0);
	free(circle1);
}

//scircle_overlap_neg(SCircle, SCircle)...
void scircle_overlap_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle0 = NULL;
	SCircle * circle1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_overlap_neg", circle0, circle1, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_overlap_neg(circle0, circle1);
	sqlite3_result_int(context, res);
	free(circle0);
	free(circle1);
}

//scircle_contained_by_circle(SCircle, SCircle)...
void scircle_contained_by_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle0 = NULL;
	SCircle * circle1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_contained_by_circle", circle0, circle1, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_in_circle(circle0, circle1);
	sqlite3_result_int(context, res);
	free(circle0);
	free(circle1);
}

//scircle_contained_by_circle_neg(SCircle, SCircle)...
void scircle_contained_by_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle0 = NULL;
	SCircle * circle1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_contained_by_circle_neg", circle0, circle1, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_in_circle_neg(circle0, circle1);
	sqlite3_result_int(context, res);
	free(circle0);
	free(circle1);
}

//scircle_contains_circle(SCircle, SCircle)...
void scircle_contains_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle0 = NULL;
	SCircle * circle1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_contains_circle", circle0, circle1, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_in_circle_com(circle0, circle1);
	sqlite3_result_int(context, res);
	free(circle0);
	free(circle1);
}

//scircle_contains_circle_neg(SCircle, SCircle)...
void scircle_contains_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle0 = NULL;
	SCircle * circle1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_contains_circle_neg", circle0, circle1, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_in_circle_com_neg(circle0, circle1);
	sqlite3_result_int(context, res);
	free(circle0);
	free(circle1);
}

//spoint_contained_by_circle(SPoint, SCircle)...
void spoint_contained_by_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoint_contained_by_circle", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherepoint_in_circle((SPoint*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_POINT) {
		res = (long long)spherepoint_in_circle_com((SCircle*) objData0, (SPoint*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoint_contained_by_circle_neg(SPoint, SCircle)...
void spoint_contained_by_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoint_contained_by_circle_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherepoint_in_circle_neg((SPoint*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_POINT) {
		res = (long long)spherepoint_in_circle_com_neg((SCircle*) objData0, (SPoint*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoint_contained_by_circle_com(SCircle, SPoint)...
void spoint_contained_by_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SPoint * point = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoint_contained_by_circle_com", circle, point, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_POINT}) ); 
	long res = spherepoint_in_circle_com(circle, point);
	sqlite3_result_int(context, res);
	free(circle);
	free(point);
}

//spoint_contained_by_circle_com_neg(SCircle, SPoint)...
void spoint_contained_by_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SPoint * point = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoint_contained_by_circle_com_neg", circle, point, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_POINT}) ); 
	long res = spherepoint_in_circle_com_neg(circle, point);
	sqlite3_result_int(context, res);
	free(circle);
	free(point);
}

//strans_circle(SCircle, SEuler)...
void strans_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_circle", circle, euler, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	SCircle * resCircle = spheretrans_circle(circle, euler);

	char * result = NULL;
	int resultLen;
	result = serialise(resCircle);
	resultLen = getSerialisedLen(resCircle);
	free(resCircle);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(circle);
	free(euler);
	free(result);
}

//strans_circle_inverse(SCircle, SEuler)...
void strans_circle_inverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_circle_inverse", circle, euler, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	SCircle * resCircle = spheretrans_circle_inverse(circle, euler);

	char * result = NULL;
	int resultLen;
	result = serialise(resCircle);
	resultLen = getSerialisedLen(resCircle);
	free(resCircle);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(circle);
	free(euler);
	free(result);
}