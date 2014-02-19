/* Add your header comment here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "output.h"
#include "udf_helpers.h"
#include "udf_sphere_sstr.h"

typedef struct st_typelib {
	unsigned int count;
	const char *name;
	const char **type_names;
	unsigned int *type_lengths;
} TYPELIB;

static const char *outputTypes[] = { "RAD", "DEG", "DMS", "HMS" };
static TYPELIB outputModes = { 4, NULL, outputTypes, NULL };

//supporting sstr(SPoint), sstr(SCircle), sstr(SLine), sstr(SEuler), sstr(SPoly),
//   sstr(SPath), sstr(SEllipse), sstr(SBox)
//optional parameters: type, precission
void sstrFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	char * result = NULL;
	unsigned long outType = OUTPUT_RAD;
	unsigned int precission = 10;

	assert( argc==1 || argc==2 || argc==3 );

	if(argc==3) {
		SQLITE_UDF_CHK_PARAM_INT( 2, precission, "sstr(object, type, precission) requires precission to be an integer number." );

		if(precission <= 0) {
			sqlite3_result_error(context, "sstr(object, type, precission) requires precission to larger than zero.", 666);
			return;
		}
	}

	if(argc>=2) {
		char * charType = NULL;
		int charTypeLen = 0;

		SQLITE_UDF_CHK_PARAM_CHAR( 1, charType, charTypeLen, "sstr(object, type, precission) requires type to be a string." );

		//checking validity
		//check if value is sane
		if(charTypeLen != 3) {
			sqlite3_result_error(context, "sstr(object, type) or sstr(object, type, precission) invalid type string", 666);
			return;
		}

		int i = 0;
		bool found = false;
		for(i = 0; i < outputModes.count ; i++) {
			if(strncmp(charType, outputModes.type_names[i], 3) == 0) {
				found = true;
				outType = i;
				break;
			}
		}
		
		if(found != true) {
			sqlite3_result_error(context, "sstr(object, type) or sstr(object, type, precission) legal types are: RAD, DEG, DMS, HMS", 666);
			return;
		}
	}

	if(argc >= 1) {
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, 
										"sstr() requires the first parameter to be a SQLITE sphere object.");

		void * objData = NULL;
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, objData, decodedDataType, PROTECT({SQLITE_SPHERE_POINT, SQLITE_SPHERE_CIRCLE, 
        											SQLITE_SPHERE_LINE, SQLITE_SPHERE_EULER, 
        											SQLITE_SPHERE_POLYGON, SQLITE_SPHERE_PATH,
        											SQLITE_SPHERE_ELLIPSE, SQLITE_SPHERE_BOX}), 
									"sstr() error decoding first parameter. Corrupted or not the correct type." );

		switch(decodedDataType) {
			case SQLITE_SPHERE_POINT:
				result = spherepoint_out((SPoint*) objData, outType, precission);
				break;
			case SQLITE_SPHERE_CIRCLE:
				result = spherecircle_out((SCircle*) objData, outType, precission);
				break;
			case SQLITE_SPHERE_LINE:
				result = sphereline_out((SLine*) objData, outType, precission);
				break;
			case SQLITE_SPHERE_EULER:
				result = spheretrans_out((SEuler*) objData, outType, precission);
				break;
			case SQLITE_SPHERE_POLYGON:
				result = spherepoly_out((SPoly*) objData, outType, precission);
				break;
			case SQLITE_SPHERE_PATH:
				result = spherepath_out((SPath*) objData, outType, precission);
				break;
			case SQLITE_SPHERE_ELLIPSE:
				result = sphereellipse_out((SEllipse*) objData, outType, precission);
				break;
			case SQLITE_SPHERE_BOX:
				result = spherebox_out((SBox*) objData, outType, precission);
				break;
		}

		free(objData);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: sstr() requires one parameter", 666);
		return;
	}

	sqlite3_result_text(context, result, strlen(result), SQLITE_TRANSIENT);
	free(result);
}
