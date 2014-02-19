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

//supporting sarea(SCircle), sarea(SPoly), sarea(SBox) and others to come...
void sareaFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	double result = 0.0;

	assert( argc==1 );

	if(argc == 1) {
		//SPoint
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, 
										"sarea() requires the first parameter to be a SQLITE sphere object.");

		void * objData = NULL;
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, objData, decodedDataType, PROTECT({SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_POLYGON, SQLITE_SPHERE_BOX}), 
									"sarea() error decoding first parameter. Corrupted or not the correct type." );

		if(decodedDataType == SQLITE_SPHERE_CIRCLE)  {
			result = spherecircle_area( (SCircle*) objData );
		} else if(decodedDataType == SQLITE_SPHERE_POLYGON) {
			result = spherepoly_area( (SPoly*) objData );
		} else if(decodedDataType == SQLITE_SPHERE_BOX) {
			result = spherebox_area( (SBox*) objData );
		}

		free(objData);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: sarea() requires one parameter", 666);
		return;
	}

	sqlite3_result_double(context, result);
}
