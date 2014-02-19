/* Add your header comment here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "ellipse.h"
#include "udf_helpers.h"
#include "udf_sphere_ellipse.h"

//supporting sellipse(spoint, lrad, srad, inc), sellipse(spoint), sellipse(scircle), sellipse(ellipse_string)
void sellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;

	assert( argc==4 || argc==1 );

	if(argc == 4) {
		char * data = NULL;
		int dataLen = 0;
		double lrad = 0.0;
		double srad = 0.0;
		double inc = 0.0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, 
										"sellipse(spoint, lrad, srad, inc) requires the first parameter to be a SQLITE sphere object.");

		SQLITE_UDF_CHK_PARAM_REAL( 1, lrad, "sellipse(spoint, lrad, srad, inc) requires lrad to be a number." );

		SQLITE_UDF_CHK_PARAM_REAL( 2, srad, "sellipse(spoint, lrad, srad, inc) requires lrad to be a number." );

		SQLITE_UDF_CHK_PARAM_REAL( 3, inc, "sellipse(spoint, lrad, srad, inc) requires lrad to be a number." );

		SPoint * point = NULL;
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, point, decodedDataType, PROTECT({SQLITE_SPHERE_POINT}), 
									"sellipse(spoint, lrad, srad, inc) error decoding first parameter. Corrupted or not the correct type." );

		ellipse = sphereellipse_infunc(point, lrad, srad, inc);

		free(point);
	} else if(argc == 1) {
		//string
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, 
										"sellipse(ellipse_string) requires a string. sellipse(spoint) a SQLITE sphere object.");

    	void * objBuf = NULL;
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, objBuf, decodedDataType,
									PROTECT({SQLITE_SPHERE_POINT, SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_UNKNOWN}), 
									"sellipse(spoint), sellipse(sellipse) error decoding first parameter. Corrupted or not the correct type." );

    	if(decodedDataType == SQLITE_SPHERE_POINT) {
    		//sellipse(spoint)
    		ellipse = spherepoint_ellipse((SPoint *)objBuf);
    		free(objBuf);
    	} else if(decodedDataType == SQLITE_SPHERE_CIRCLE) {
    		//sellipse(scircle)
    		ellipse = spherecircle_ellipse((SCircle *)objBuf);
    		free(objBuf);
    	} else {
    		//sellipse(ellipse_string)
			ellipse = sphereellipse_in( (char*)data );
			if(objBuf != NULL)
				free(objBuf);
		}
	} else {
		sqlite3_result_error(context, "wrong number of arguments: sellipse() requires one or four parameters", 666);
		return;
	}

	if(ellipse == NULL) {
		sqlite3_result_error(context, "an error occurred while generating the ellipse", 666);
		return;
	}

	char * result;
	int resultLen;
	result = serialise(ellipse);
	resultLen = getSerialisedLen(ellipse);
	free(ellipse);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);

	free(result);
}

//sellipse_inc(SEllipse)...
void sellipse_incFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "sellipse_inc", ellipse, PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	double res = sphereellipse_incl(ellipse);
	sqlite3_result_double(context, res);
	free(ellipse);
}

//sellipse_lrad(SEllipse)...
void sellipse_lradFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "sellipse_lrad", ellipse, PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	double res = sphereellipse_rad1(ellipse);
	sqlite3_result_double(context, res);
	free(ellipse);
}

//sellipse_srad(SEllipse)...
void sellipse_sradFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "sellipse_srad", ellipse, PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	double res = sphereellipse_rad2(ellipse);
	sqlite3_result_double(context, res);
	free(ellipse);
}

//sellipse_equal(SEllipse, SEllipse)...
void sellipse_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse0 = NULL;
	SEllipse * ellipse1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_equal", ellipse0, ellipse1, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_equal(ellipse0, ellipse1);
	sqlite3_result_int(context, res);
	free(ellipse0);
	free(ellipse1);
}

//sellipse_equal_neg(SEllipse, SEllipse)...
void sellipse_equal_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse0 = NULL;
	SEllipse * ellipse1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_equal_neg", ellipse0, ellipse1, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_equal_neg(ellipse0, ellipse1);
	sqlite3_result_int(context, res);
	free(ellipse0);
	free(ellipse1);
}

//sellipse_contains_ellipse(SEllipse, SEllipse)...
void sellipse_contains_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse0 = NULL;
	SEllipse * ellipse1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_ellipse", ellipse0, ellipse1, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_ellipse(ellipse0, ellipse1);
	sqlite3_result_int(context, res);
	free(ellipse0);
	free(ellipse1);
}

//sellipse_contains_ellipse_neg(SEllipse, SEllipse)...
void sellipse_contains_ellipse_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse0 = NULL;
	SEllipse * ellipse1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_ellipse_neg", ellipse0, ellipse1, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_ellipse_neg(ellipse0, ellipse1);
	sqlite3_result_int(context, res);
	free(ellipse0);
	free(ellipse1);
}

//sellipse_contains_ellipse_com(SEllipse, SEllipse)...
void sellipse_contains_ellipse_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse0 = NULL;
	SEllipse * ellipse1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_ellipse_com", ellipse0, ellipse1, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_ellipse_com(ellipse0, ellipse1);
	sqlite3_result_int(context, res);
	free(ellipse0);
	free(ellipse1);
}

//sellipse_contains_ellipse_com_neg(SEllipse, SEllipse)...
void sellipse_contains_ellipse_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse0 = NULL;
	SEllipse * ellipse1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_ellipse_com_neg", ellipse0, ellipse1, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_ellipse_com_neg(ellipse0, ellipse1);
	sqlite3_result_int(context, res);
	free(ellipse0);
	free(ellipse1);
}

//sellipse_overlap_ellipse(SEllipse, SEllipse)...
void sellipse_overlap_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse0 = NULL;
	SEllipse * ellipse1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_overlap_ellipse", ellipse0, ellipse1, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_overlap_ellipse(ellipse0, ellipse1);
	sqlite3_result_int(context, res);
	free(ellipse0);
	free(ellipse1);
}

//sellipse_overlap_ellipse_neg(SEllipse, SEllipse)...
void sellipse_overlap_ellipse_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse0 = NULL;
	SEllipse * ellipse1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_overlap_ellipse_neg", ellipse0, ellipse1, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_overlap_ellipse_neg(ellipse0, ellipse1);
	sqlite3_result_int(context, res);
	free(ellipse0);
	free(ellipse1);
}

//sellipse_contains_point(SEllipse, SPoint)...
void sellipse_contains_pointFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_contains_point", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_POINT}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)sphereellipse_cont_point((SEllipse*) objData0, (SPoint*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_POINT) {
		res = (long long)sphereellipse_cont_point_com((SPoint*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_contains_point_neg(SEllipse, SPoint)...
void sellipse_contains_point_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_contains_point_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_POINT}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)sphereellipse_cont_point_neg((SEllipse*) objData0, (SPoint*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_POINT) {
		res = (long long)sphereellipse_cont_point_com_neg((SPoint*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_contains_point_com(SPoint, SEllipse)...
void sellipse_contains_point_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_point_com", point, ellipse, PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_point_com(point, ellipse);
	sqlite3_result_int(context, res);
	free(point);
	free(ellipse);
}

//sellipse_contains_point_com_neg(SPoint, SEllipse)...
void sellipse_contains_point_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_point_com_neg", point, ellipse, PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_point_com_neg(point, ellipse);
	sqlite3_result_int(context, res);
	free(point);
	free(ellipse);
}

//strans_ellipse(SEllipse, SEuler)...
void strans_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_ellipse", ellipse, euler, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	SEllipse * resEllipse = spheretrans_ellipse(ellipse, euler);

	char * result = NULL;
	int resultLen;
	result = serialise(resEllipse);
	resultLen = getSerialisedLen(resEllipse);
	free(resEllipse);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(ellipse);
	free(euler);
	free(result);
}

//strans_ellipse_inverse(SEllipse, SEuler)...
void strans_ellipse_inverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_ellipse_inverse", ellipse, euler, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	SEllipse * resEllipse = spheretrans_ellipse_inv(ellipse, euler);

	char * result = NULL;
	int resultLen;
	result = serialise(resEllipse);
	resultLen = getSerialisedLen(resEllipse);
	free(resEllipse);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(ellipse);
	free(euler);
	free(result);
}

//sellipse_contains_circle(SEllipse, SCircle)...
void sellipse_contains_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_contains_circle", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)sphereellipse_cont_circle((SEllipse*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_cont_circle_com((SCircle*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_contains_circle_neg(SEllipse, SCircle)...
void sellipse_contains_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_contains_circle_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)sphereellipse_cont_circle_neg((SEllipse*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_cont_circle_com_neg((SCircle*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_contains_circle_com(SCircle, SEllipse)...
void sellipse_contains_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_circle_com", circle, ellipse, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_circle_com(circle, ellipse);
	sqlite3_result_int(context, res);
	free(circle);
	free(ellipse);
}

//sellipse_contains_circle_com_neg(SCircle, SEllipse)...
void sellipse_contains_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_circle_com_neg", circle, ellipse, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_circle_com_neg(circle, ellipse);
	sqlite3_result_int(context, res);
	free(circle);
	free(ellipse);
}

//scircle_contains_ellipse(SCircle, SEllipse)...
void scircle_contains_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "scircle_contains_ellipse", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)spherecircle_cont_ellipse((SCircle*) objData0, (SEllipse*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherecircle_cont_ellipse_com((SEllipse*) objData0, (SCircle*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//scircle_contains_ellipse_neg(SCircle, SEllipse)...
void scircle_contains_ellipse_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "scircle_contains_ellipse_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)spherecircle_cont_ellipse_neg((SCircle*) objData0, (SEllipse*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherecircle_cont_ellipse_com_neg((SEllipse*) objData0, (SCircle*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//scircle_contains_ellipse_com(SEllipse, SCircle)...
void scircle_contains_ellipse_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SCircle * circle = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_contains_ellipse_com", ellipse, circle, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_cont_ellipse_com(ellipse, circle);
	sqlite3_result_int(context, res);
	free(ellipse);
	free(circle);
}

//scircle_contains_ellipse_com_neg(SEllipse, SCircle)...
void scircle_contains_ellipse_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SCircle * circle = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_contains_ellipse_com_neg", ellipse, circle, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_cont_ellipse_com_neg(ellipse, circle);
	sqlite3_result_int(context, res);
	free(ellipse);
	free(circle);
}

//sellipse_overlap_circle(SEllipse, SCircle)...
void sellipse_overlap_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_overlap_circle", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)sphereellipse_overlap_circle((SEllipse*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_overlap_circle_com((SCircle*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_overlap_circle_neg(SEllipse, SCircle)...
void sellipse_overlap_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_overlap_circle_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)sphereellipse_overlap_circle_neg((SEllipse*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_overlap_circle_com_neg((SCircle*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_overlap_circle_com(SCircle, SEllipse)...
void sellipse_overlap_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_overlap_circle_com", circle, ellipse, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_overlap_circle_com(circle, ellipse);
	sqlite3_result_int(context, res);
	free(circle);
	free(ellipse);
}

//sellipse_overlap_circle_com_neg(SCircle, SEllipse)...
void sellipse_overlap_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_overlap_circle_com_neg", circle, ellipse, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_overlap_circle_com_neg(circle, ellipse);
	sqlite3_result_int(context, res);
	free(circle);
	free(ellipse);
}

//sellipse_overlap_line(SEllipse, SLine)...
void sellipse_overlap_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_overlap_line", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)sphereellipse_overlap_line((SEllipse*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_overlap_line_com((SLine*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_overlap_line_neg(SEllipse, SLine)...
void sellipse_overlap_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_overlap_line_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)sphereellipse_overlap_line_neg((SEllipse*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_overlap_line_com_neg((SLine*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_overlap_line_com(SLine, SEllipse)...
void sellipse_overlap_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_overlap_line_com", line, ellipse, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_overlap_line_com(line, ellipse);
	sqlite3_result_int(context, res);
	free(line);
	free(ellipse);
}

//sellipse_overlap_line_com_neg(SLine, SEllipse)...
void sellipse_overlap_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_overlap_line_com_neg", line, ellipse, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_overlap_line_com_neg(line, ellipse);
	sqlite3_result_int(context, res);
	free(line);
	free(ellipse);
}

//sellipse_contains_line(SEllipse, SLine)...
void sellipse_contains_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_contains_line", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)sphereellipse_cont_line((SEllipse*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_cont_line_com((SLine*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_contains_line_neg(SEllipse, SLine)...
void sellipse_contains_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_contains_line_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)sphereellipse_cont_line_neg((SEllipse*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_cont_line_com_neg((SLine*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_contains_line_com(SLine, SEllipse)...
void sellipse_contains_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_line_com", line, ellipse, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_line_com(line, ellipse);
	sqlite3_result_int(context, res);
	free(line);
	free(ellipse);
}

//sellipse_contains_line_com_neg(SLine, SEllipse)...
void sellipse_contains_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_line_com_neg", line, ellipse, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_line_com_neg(line, ellipse);
	sqlite3_result_int(context, res);
	free(line);
	free(ellipse);
}
