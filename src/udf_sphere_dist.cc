/* Add your header comment here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "circle.h"
#include "polygon.h"
#include "box.h"
#include "udf_helpers.h"
#include "udf_sphere_area.h"

//supporting dist(SPoint, SPoint), dist(SCircle, SCircle), dist(SCircle, SPoint), dist(SPoint, SCircle) and others to come...
void sdistFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	double result = 0.0;

	assert( argc==2 );

	if(argc == 2) {
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
										"dist() requires the first parameter to be a SQLITE sphere object.");

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										"dist() requires the second parameter to be a SQLITE sphere object.");

		void * objData0 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
		void * objData1 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, objData0, decodedDataType0, PROTECT({SQLITE_SPHERE_POINT, SQLITE_SPHERE_CIRCLE}), 
									"dist() error decoding first parameter. Corrupted or not the correct type." );

		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, objData1, decodedDataType1, PROTECT({SQLITE_SPHERE_POINT, SQLITE_SPHERE_CIRCLE}), 
									"dist() error decoding second parameter. Corrupted or not the correct type." );

    	if( !(
    		(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_POINT) ||
    		(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_CIRCLE) ||
    		(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_POINT) ||
    		(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_CIRCLE)
    		) ) {
			sqlite3_result_error(context, "dist() error combining SQLITE sphere objects. Unsupported combination.", 666);
			free(objData0);
			free(objData1);
			return;
    	}

		if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_POINT) {
			result = spherepoint_distance((SPoint*) objData0, (SPoint*) objData1);
		} else if (decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
			result = spherecircle_distance((SCircle*) objData0, (SCircle*) objData1);
		} else if (decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_POINT) {
			result = spherecircle_point_distance((SCircle*) objData0, (SPoint*) objData1);
		} else if (decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
			result = spherecircle_point_distance_com((SPoint*) objData0, (SCircle*) objData1);
		}

		free(objData0);
		free(objData1);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: dist() requires two parameter", 666);
		return;
	}

	sqlite3_result_double(context, result);
}
