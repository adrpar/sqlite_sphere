/* Add your header comment here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "circle.h"
#include "ellipse.h"
#include "polygon.h"
#include "udf_helpers.h"
#include "udf_sphere_center.h"

//supporting scenter(SCircle), scenter(SEllipse) and others to come...
void scenterFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * resPoint = NULL;

	assert( argc==1 );

	if(argc == 1) {
		//SPoint
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, 
										"scenter() requires the first parameter to be a SQLITE sphere object.");

		void * objData = NULL;
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, objData, decodedDataType, PROTECT({SQLITE_SPHERE_CIRCLE, SQLITE_SPHERE_ELLIPSE}), 
									"scenter() error decoding first parameter. Corrupted or not the correct type." );

		if(decodedDataType == SQLITE_SPHERE_CIRCLE)  {
			resPoint = spherecircle_center( (SCircle*) objData );
		} else if(decodedDataType == SQLITE_SPHERE_ELLIPSE) {
			resPoint = sphereellipse_center( (SEllipse*) objData );
		}

		free(objData);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: sarea() requires one parameter", 666);
		return;
	}

	char * result = NULL;
	int resultLen;
	result = serialise(resPoint);
	resultLen = getSerialisedLen(resPoint);
	free(resPoint);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(result);
}
