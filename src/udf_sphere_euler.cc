/* Add your header comment here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "line.h"
#include "euler.h"
#include "ellipse.h"
#include "udf_helpers.h"
#include "udf_sphere_euler.h"

//supporting strans(phi, theta, psi) or strans(phi, theta, psi, trans_axis_string), strans(line), strans(ellipse) or strans(trans_string)
void stransFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEuler * euler = NULL;

	assert( argc==1 || argc==2 || argc == 3 || argc == 4 );

	double phi = 0.0;
	double theta = 0.0;
	double psi = 0.0;
	char * data = NULL;
	int dataLen = 0;

	switch(argc) {
		case 4:
			//trans_axis_string
			SQLITE_UDF_CHK_PARAM_CHAR( 3, data, dataLen, 
										"strans(phi, theta, psi, trans_axis_string) requires trans_axis_string to be a string.");
		case 3:
			//phi, theta, psi
	        SQLITE_UDF_CHK_PARAM_REAL( 0, phi, "strans(phi, theta, psi, (trans_axis_string)) requires phi to be a number.");
	        SQLITE_UDF_CHK_PARAM_REAL( 1, theta, "strans(phi, theta, psi, (trans_axis_string)) requires theta to be a number.");
	        SQLITE_UDF_CHK_PARAM_REAL( 2, psi, "strans(phi, theta, psi, (trans_axis_string)) requires psi to be a number.");
			break;
		case 1:
			//string
			SQLITE_UDF_CHK_PARAM_CHAR( 0, data, dataLen,
										"strans(transformation_string) requires a string. strans(sline) a SQLITE sphere object.");
			break;
		default:
			sqlite3_result_error(context, "wrong number of arguments: strans() requires one, three or four parameters", 666);
			return;
	}

	//creating transformation object
	if(argc == 4) {
		euler = spheretrans_from_float8_and_type( phi, theta, psi, data );
	} else if (argc == 3) {
		euler = spheretrans_from_float8( phi, theta, psi );
	} else if (argc == 1) {
		//decode object - if this is not a line or ellipse, throw error, if corrupted, the parser will throw error
    	void * objBuf = NULL;
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, objBuf, decodedDataType,
									PROTECT({SQLITE_SPHERE_LINE, SQLITE_SPHERE_ELLIPSE, SQLITE_SPHERE_UNKNOWN}), 
									"strans(sline), strans(sellipse) error decoding first parameter. Corrupted or not the correct type." );

    	if(decodedDataType == SQLITE_SPHERE_LINE) {
    		//strans(sline)
    		euler = spheretrans_from_line((SLine *)objBuf);
    		free(objBuf);
    	} else if(decodedDataType == SQLITE_SPHERE_ELLIPSE) {
    		//strans(sline)
    		euler = sphereellipse_trans((SEllipse *)objBuf);
    		free(objBuf);
    	} else {
    		//strans(transformation_string)
			euler = spheretrans_in( (char*)data );
    		if(objBuf != NULL)
    			free(objBuf);
		}
	}

	if(euler == NULL) {
		sqlite3_result_error(context, "an error occurred while generating the eulerian transformation", 666);
		return;
	}

	char * result = NULL;
	int resultLen;
	result = serialise(euler);
	resultLen = getSerialisedLen(euler);
	free(euler);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);

	free(result);
}

//strans_point(SPoint, SEuler)...
void strans_pointFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_point", point, euler, PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	SPoint * resPoint = spheretrans_point(point, euler);

	char * result = NULL;
	int resultLen;
	result = serialise(resPoint);
	resultLen = getSerialisedLen(resPoint);
	free(resPoint);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(point);
	free(euler);
	free(result);
}

//strans_point_inverse(SPoint, SEuler)...
void strans_point_inverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_point_inverse", point, euler, PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	SPoint * resPoint = spheretrans_point_inverse(point, euler);

	char * result = NULL;
	int resultLen;
	result = serialise(resPoint);
	resultLen = getSerialisedLen(resPoint);
	free(resPoint);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(point);
	free(euler);
	free(result);
}

//strans_equal(SEuler, SEuler)...
void strans_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEuler * euler0 = NULL;
	SEuler * euler1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_equal", euler0, euler1, PROTECT({SQLITE_SPHERE_EULER}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	long res = spheretrans_equal(euler0, euler1);
	sqlite3_result_int(context, res);
	free(euler0);
	free(euler1);
}

//strans_equal_neg(SEuler, SEuler)...
void strans_equal_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEuler * euler0 = NULL;
	SEuler * euler1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_equal_neg", euler0, euler1, PROTECT({SQLITE_SPHERE_EULER}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	long res = spheretrans_not_equal(euler0, euler1);
	sqlite3_result_int(context, res);
	free(euler0);
	free(euler1);
}

//strans_phi(SEuler)
void strans_phiFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "strans_phi", euler, PROTECT({SQLITE_SPHERE_EULER}) ); 
	double res = spheretrans_phi(euler);
	sqlite3_result_double(context, res);
	free(euler);
}

//strans_theta(SEuler)
void strans_thetaFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "strans_theta", euler, PROTECT({SQLITE_SPHERE_EULER}) ); 
	double res = spheretrans_theta(euler);
	sqlite3_result_double(context, res);
	free(euler);
}

//strans_psi(SEuler)
void strans_psiFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "strans_psi", euler, PROTECT({SQLITE_SPHERE_EULER}) ); 
	double res = spheretrans_psi(euler);
	sqlite3_result_double(context, res);
	free(euler);
}

//strans_axes(SEuler)
void strans_axesFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "strans_axes", euler, PROTECT({SQLITE_SPHERE_EULER}) ); 
	char * res = spheretrans_type(euler);
	sqlite3_result_text(context, res, strlen(res), SQLITE_TRANSIENT);
	free(euler);
	free(res);
}

//strans_invert(SEuler)...
void strans_invertFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "strans_invert", euler, PROTECT({SQLITE_SPHERE_EULER}) ); 
	SEuler * resEuler = spheretrans_invert(euler);

	char * result = NULL;
	int resultLen;
	result = serialise(resEuler);
	resultLen = getSerialisedLen(resEuler);
	free(resEuler);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(euler);
	free(result);
}

//strans_zxz(SEuler)...
void strans_zxzFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "strans_zxz", euler, PROTECT({SQLITE_SPHERE_EULER}) ); 
	SEuler * resEuler = spheretrans_zxz(euler);

	char * result = NULL;
	int resultLen;
	result = serialise(resEuler);
	resultLen = getSerialisedLen(resEuler);
	free(resEuler);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(euler);
	free(result);
}

//strans_trans(SEuler, SEuler)...
void strans_transFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEuler * euler0 = NULL;
	SEuler * euler1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_trans", euler0, euler1, PROTECT({SQLITE_SPHERE_EULER}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	SEuler * resEuler = spheretrans_trans(euler0, euler1);

	char * result = NULL;
	int resultLen;
	result = serialise(resEuler);
	resultLen = getSerialisedLen(resEuler);
	free(resEuler);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(euler0);
	free(euler1);
	free(result);
}

//strans_trans_inv(SEuler, SEuler)...
void strans_trans_invFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEuler * euler0 = NULL;
	SEuler * euler1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_trans_inv", euler0, euler1, PROTECT({SQLITE_SPHERE_EULER}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	SEuler * resEuler = spheretrans_trans(euler0, euler1);

	char * result = NULL;
	int resultLen;
	result = serialise(resEuler);
	resultLen = getSerialisedLen(resEuler);
	free(resEuler);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(euler0);
	free(euler1);
	free(result);
}
