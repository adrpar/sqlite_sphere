/* Add your header comment here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "polygon.h"
#include "path.h"
#include "udf_helpers.h"
#include "udf_sphere_npoints.h"

//supporting snpoints(SPoly), snpoints(SPath) and others to come...
void snpointsFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	long long result = 0;

	assert( argc==1 );

	if(argc == 1) {
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, 
										"snpoints() requires the first parameter to be a SQLITE sphere object.");

		void * objData = NULL;
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, objData, decodedDataType, PROTECT({SQLITE_SPHERE_POLYGON, SQLITE_SPHERE_PATH}), 
									"snpoints() error decoding first parameter. Corrupted or not the correct type." );

		if(decodedDataType == SQLITE_SPHERE_POLYGON) {
			result = spherepoly_npts((SPoly*) objData);
		} else if (decodedDataType == SQLITE_SPHERE_PATH) {
			result = spherepath_npts((SPath*) objData);
		}

		free(objData);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: snpoints() requires one parameter", 666);
		return;
	}

	sqlite3_result_int(context, result);
}
