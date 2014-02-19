/* Add your header comment here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "line.h"
#include "path.h"
#include "udf_helpers.h"
#include "udf_sphere_length.h"

//supporting slength(SLine), slength(SPath) and others to come...
void slengthFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	double result = 0.0;

	assert( argc==1 );

	if(argc == 1) {
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, 
										"slength() requires the first parameter to be a SQLITE sphere object.");

		void * objData = NULL;
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, objData, decodedDataType, PROTECT({SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH}), 
									"slength() error decoding first parameter. Corrupted or not the correct type." );

		if(decodedDataType == SQLITE_SPHERE_LINE) {
			result = sphereline_length((SLine*) objData);
		} else if (decodedDataType == SQLITE_SPHERE_PATH) {
			result = spherepath_length((SPath*) objData);
		}

		free(objData);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: slength() requires one parameter", 666);
		return;
	}

	sqlite3_result_double(context, result);
}
