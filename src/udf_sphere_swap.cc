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
#include "udf_sphere_swap.h"

//supporting sswap(SLine), sswap(SPath) and others to come...
void sswapFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	char * strResult;
	int resultLen;

	assert( argc==1 );

	if(argc == 1) {
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, 
										"sswap() requires the first parameter to be a SQLITE sphere object.");

		void * objData = NULL;
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, objData, decodedDataType, PROTECT({SQLITE_SPHERE_LINE, SQLITE_SPHERE_PATH}), 
									"sswap() error decoding first parameter. Corrupted or not the correct type." );

		if(decodedDataType == SQLITE_SPHERE_LINE)  {
			SLine * line = sphereline_swap_beg_end( (SLine*) objData );

			strResult = serialise(line);
			resultLen = getSerialisedLen(line);
			free(line);
		} else if(decodedDataType == SQLITE_SPHERE_PATH) {
			SPath * path = spherepath_swap( (SPath*) objData );

			strResult = serialise(path);
			resultLen = getSerialisedLen(path);
			free(path);
		}

		free(objData);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: circum() requires two parameter", 666);
		return;
	}

	sqlite3_result_blob(context, strResult, resultLen, SQLITE_TRANSIENT);

	free(strResult);
}
