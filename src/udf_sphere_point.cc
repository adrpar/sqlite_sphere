/* Add your header comment here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "point.h"
#include "path.h"
#include "udf_helpers.h"
#include "udf_sphere_point.h"

//supporting spoint(long, lat) or spoint(geom_string)
void spointFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;

	assert( argc==1 || argc==2 );

	if(argc == 2) {
		int type0 = sqlite3_value_type(argv[0]);

		if( type0 == SQLITE_NULL ) {
			sqlite3_result_null(context);
			return;
		} else if (type0 == SQLITE_BLOB || type0 == SQLITE_TEXT ) {
			char * data = NULL;
			int dataLen = 0;

			SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, 
											"spoint(SPath, i) requires the first parameter to be a SQLITE sphere object.");

			SPath * path = NULL;
			SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

			SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, path, decodedDataType, PROTECT({SQLITE_SPHERE_PATH}), 
										"spoint(SPath, i) error decoding first parameter. Corrupted or not the correct type." );

			if(sqlite3_value_type(argv[1]) == SQLITE_INTEGER) {
				int i;

				SQLITE_UDF_CHK_PARAM_INT( 1, i, "spoint(SPath, i) requires a integer number as i." );

				point = spherepath_get_point(path, i);
			} else {
				double i;

				SQLITE_UDF_CHK_PARAM_REAL( 1, i, "spoint(SPath, i) requires a numerical or integer parameter as i." );

				point = spherepath_point(path, i);
			}

			free(path);			
		} else {
			//long/lat
			double longitude = 0.0;
			double latitute = 0.0;

			SQLITE_UDF_CHK_PARAM_REAL( 0, longitude, "spoint(long, lat) requires a numerical parameter as long." );
			SQLITE_UDF_CHK_PARAM_REAL( 1, longitude, "spoint(long, lat) requires a numerical parameter as lat." );

			point = spherepoint_from_long_lat(longitude, latitute);
		}
	} else if(argc == 1) {
		//string
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHAR( 0, data, dataLen, "spoint(coordinate_string) requires a string." );

		point =  spherepoint_in( data );
	} else {
		sqlite3_result_error(context, "wrong number of arguments: spoint() requires one or two parameter", 666);
		return;
	}

	if(point == NULL) {
		sqlite3_result_error(context, "an error occurred while generating the spherical point", 666);
		return;
	}

	char * result;
	int resultLen;
	result = serialise(point);
	resultLen = getSerialisedLen(point);
	free(point);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);

	free(result);
}

//spoint_long(SPoint)...
void spoint_longFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "spoint_long", point, PROTECT({SQLITE_SPHERE_POINT}) ); 
	double res = spherepoint_long(point);
	sqlite3_result_double(context, res);
	free(point);
}

//spoint_lat(SPoint)...
void spoint_latFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "spoint_lat", point, PROTECT({SQLITE_SPHERE_POINT}) ); 
	double res = spherepoint_lat(point);
	sqlite3_result_double(context, res);
	free(point);
}

//spoint_x(SPoint)...
void spoint_xFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "spoint_x", point, PROTECT({SQLITE_SPHERE_POINT}) ); 
	double res = spherepoint_x(point);
	sqlite3_result_double(context, res);
	free(point);
}

//spoint_y(SPoint)...
void spoint_yFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "spoint_y", point, PROTECT({SQLITE_SPHERE_POINT}) ); 
	double res = spherepoint_y(point);
	sqlite3_result_double(context, res);
	free(point);
}

//spoint_z(SPoint)...
void spoint_zFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "spoint_z", point, PROTECT({SQLITE_SPHERE_POINT}) ); 
	double res = spherepoint_z(point);
	sqlite3_result_double(context, res);
	free(point);
}

//spoint_equal(SPoint, SPoint)...
void spoint_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point0 = NULL;
	SPoint * point1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoint_equal", point0, point1, PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_POINT}) ); 
	long res = spherepoint_equal(point0, point1);
	sqlite3_result_int(context, res);
	free(point0);
	free(point1);
}
