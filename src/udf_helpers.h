/*****************************************************************
 * *******                  UDF_SPHERE                     *******
 *****************************************************************
 * some macros for making UDF life easier
 * 
 *****************************************************************
 */

#ifndef __SQLITE_SPHERE_UDFHELPERS__
#define __SQLITE_SPHERE_UDFHELPERS__

#include "types.h"
#include "serialiser.h"

#define SQLITE_UDF_CHK_PARAM_REAL( PARAMNUM, VARIABLE, ERRTEXT ) \
		if( sqlite3_value_type(argv[ PARAMNUM ]) == SQLITE_INTEGER || sqlite3_value_type(argv[ PARAMNUM ]) == SQLITE_FLOAT ) { \
			VARIABLE = sqlite3_value_double(argv[ PARAMNUM ]); \
		} else if( sqlite3_value_type(argv[ PARAMNUM ]) == SQLITE_NULL ) { \
			sqlite3_result_null(context); \
			return; \
		} else { \
			sqlite3_result_error(context, ERRTEXT, 666); \
			return; \
		} \

#define SQLITE_UDF_CHK_PARAM_CHAR( PARAMNUM, VARIABLE, VARIABLELEN, ERRTEXT ) \
		if( sqlite3_value_type(argv[ PARAMNUM ]) == SQLITE_TEXT ) { \
			VARIABLE = (char*)sqlite3_value_text(argv[ PARAMNUM ]); \
			VARIABLELEN = sqlite3_value_bytes( argv[ PARAMNUM ]); \
		} else if( sqlite3_value_type(argv[ PARAMNUM ]) == SQLITE_NULL ) { \
			sqlite3_result_null(context); \
			return; \
		} else { \
			sqlite3_result_error(context, ERRTEXT, 666); \
			return; \
		} \

#define SQLITE_UDF_CHK_PARAM_CHARBLOB( PARAMNUM, VARIABLE, VARIABLELEN, ERRTEXT ) \
		if( sqlite3_value_type(argv[ PARAMNUM ]) == SQLITE_TEXT || sqlite3_value_type(argv[ PARAMNUM ]) == SQLITE_BLOB ) { \
			VARIABLE = (char*)sqlite3_value_text(argv[ PARAMNUM ]); \
			VARIABLELEN = sqlite3_value_bytes( argv[ PARAMNUM ]); \
		} else if( sqlite3_value_type(argv[ PARAMNUM ]) == SQLITE_NULL ) { \
			sqlite3_result_null(context); \
			return; \
		} else { \
			sqlite3_result_error(context, ERRTEXT, 666); \
			return; \
		} \

#define SQLITE_UDF_CHK_PARAM_INT( PARAMNUM, VARIABLE, ERRTEXT ) \
		if( sqlite3_value_type(argv[ PARAMNUM ]) == SQLITE_INTEGER ) { \
			VARIABLE = sqlite3_value_int(argv[ PARAMNUM ]); \
		} else if( sqlite3_value_type(argv[ PARAMNUM ]) == SQLITE_NULL ) { \
			sqlite3_result_null(context); \
			return; \
		} else { \
			sqlite3_result_error(context, ERRTEXT, 666); \
			return; \
		} \


#define PROTECT(...) __VA_ARGS__
#define SQLITE_UDF_CHK_SPHERETYPE( DATA, DATALEN, OUTPUTOBJECT, OUTPUTOBJECTTYPE, VALIDTYPES, ERROR ) \
		if(DATA != NULL) { \
			OUTPUTOBJECTTYPE = decode(DATA, DATALEN, (void**)&OUTPUTOBJECT); \
 \
			SQLITE_SPHERE_TYPES types[] = VALIDTYPES; \
 \
			int sphereTypesArrayLen = sizeof(types) / sizeof(SQLITE_SPHERE_TYPES); \
			int sphereTypesCounter = 0; \
			int sphereTypesFound = 0; \
			for(sphereTypesCounter = 0; sphereTypesCounter < sphereTypesArrayLen; sphereTypesCounter++) { \
		    	if(OUTPUTOBJECTTYPE == types[sphereTypesCounter]) { \
		    		sphereTypesFound = 1; \
		    		break; \
		    	} \
			} \
 \
			if(sphereTypesFound == 0) { \
				sqlite3_result_error(context, ERROR, 666); \
				free( OUTPUTOBJECT ); \
				return; \
			} \
		} \

#define SQLITE_UDF_CHK_SPHERETYPE_COM( DATA, DATALEN, OUTPUTOBJECT, OUTPUTOBJECTTYPE, VALIDTYPES1, VALIDTYPES2, ERROR ) \
		if(DATA != NULL) { \
			OUTPUTOBJECTTYPE = decode(DATA, DATALEN, (void**)&OUTPUTOBJECT); \
 \
			SQLITE_SPHERE_TYPES types1[] = VALIDTYPES1; \
			int sphereTypes1ArrayLen = sizeof(types1) / sizeof(SQLITE_SPHERE_TYPES); \
			int sphereTypes1Counter = 0; \
			int sphereTypes1Found = 0; \
			SQLITE_SPHERE_TYPES types2[] = VALIDTYPES2; \
			int sphereTypes2ArrayLen = sizeof(types2) / sizeof(SQLITE_SPHERE_TYPES); \
			int sphereTypes2Counter = 0; \
			int sphereTypes2Found = 0; \
			for(sphereTypes1Counter = 0; sphereTypes1Counter < sphereTypes1ArrayLen; sphereTypes1Counter++) { \
		    	if(OUTPUTOBJECTTYPE == types1[sphereTypes1Counter]) { \
		    		sphereTypes1Found = 1; \
		    		break; \
		    	} \
			} \
			for(sphereTypes2Counter = 0; sphereTypes2Counter < sphereTypes2ArrayLen; sphereTypes2Counter++) { \
		    	if(OUTPUTOBJECTTYPE == types2[sphereTypes2Counter]) { \
		    		sphereTypes2Found = 1; \
		    		break; \
		    	} \
			} \
 \
			if(sphereTypes1Found == 0 && sphereTypes2Found == 0) { \
				sqlite3_result_error(context, ERROR, 666); \
				free( OUTPUTOBJECT ); \
				return; \
			} \
		} \

#define SQLITE_UDF_SPHERE_ONEPARAM_INIT( FUNCNAME, OUTPUTOBJECT, VALIDTYPES ) \
	if(argc == 1) { \
		char * data = NULL; \
		int dataLen = 0; \
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN; \
 \
		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, FUNCNAME "() requires the first parameter to be a MySQL sphere object." ); \
 \
		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, OUTPUTOBJECT, decodedDataType, PROTECT(VALIDTYPES),   \
                                            FUNCNAME "() error decoding first parameter. Corrupted or not the correct type." ); \
	} else { \
		sqlite3_result_error(context, "wrong number of arguments: " FUNCNAME "() requires one parameter", 666); \
		return; \
	} \

#define SQLITE_UDF_SPHERE_TWOPARAM_INIT( FUNCNAME, OUTPUTOBJECT1, OUTPUTOBJECT2, VALIDTYPES1, VALIDTYPES2 ) \
	if(argc == 2) { \
		char * data0 = NULL; \
		int dataLen0 = 0; \
		char * data1 = NULL; \
		int dataLen1 = 0; \
		SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN; \
		SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN; \
 \
		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, FUNCNAME "() requires the first parameter to be a MySQL sphere object."); \
		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, FUNCNAME "() requires the second parameter to be a MySQL sphere object."); \
 \
		SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, OUTPUTOBJECT1, decodedDataType0, PROTECT(VALIDTYPES1),  \
											FUNCNAME "() error decoding first parameter. Corrupted or not the correct type." ); \
 \
		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, OUTPUTOBJECT2, decodedDataType1, PROTECT(VALIDTYPES2),  \
											FUNCNAME "() error decoding second parameter. Corrupted or not the correct type." ); \
	} else { \
		sqlite3_result_error(context, "wrong number of arguments: " FUNCNAME "() requires two parameters", 666); \
		return; \
	} \

#define SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( FUNCNAME, OUTPUTOBJECT1, OUTPUTOBJECT2, OUTPUTOBJECTTYPE1, OUTPUTOBJECTTYPE2, VALIDTYPES1, VALIDTYPES2 ) \
	if(argc == 2) { \
		char * data0 = NULL; \
		int dataLen0 = 0; \
		char * data1 = NULL; \
		int dataLen1 = 0; \
 \
		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, FUNCNAME "() requires the first parameter to be a MySQL sphere object."); \
		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, FUNCNAME "() requires the second parameter to be a MySQL sphere object."); \
 \
		SQLITE_UDF_CHK_SPHERETYPE_COM( data0, dataLen0, OUTPUTOBJECT1, OUTPUTOBJECTTYPE1, PROTECT(VALIDTYPES1), PROTECT(VALIDTYPES2), \
											FUNCNAME "() error decoding first parameter. Corrupted or not the correct type." ); \
 \
		SQLITE_UDF_CHK_SPHERETYPE_COM( data1, dataLen1, OUTPUTOBJECT2, OUTPUTOBJECTTYPE2, PROTECT(VALIDTYPES2), PROTECT(VALIDTYPES1), \
											FUNCNAME "() error decoding second parameter. Corrupted or not the correct type." ); \
	} else { \
		sqlite3_result_error(context, "wrong number of arguments: " FUNCNAME "() requires two parameters", 666); \
		return; \
	} \


/*
#define SQLITE_UDF_DYNCHK_SPHERETYPE( PARANUM, BUFFEROBJ, VALIDTYPES ) \
    	if(BUFFEROBJ->isDynParams[PARANUM] == true) { \
    		if(BUFFEROBJ->memBufs[PARANUM] != NULL) { \
    			free(BUFFEROBJ->memBufs[PARANUM]); \
    			if(BUFFEROBJ->resBuf != NULL) { \
    				free(BUFFEROBJ->resBuf); \
    			} \
    		} \
 \
			BUFFEROBJ->argTypes[ PARANUM ] = decode(args->args[ PARANUM ], args->lengths[ PARANUM ], (void**)&BUFFEROBJ->memBufs[ PARANUM ]); \
 \
			SQLITE_SPHERE_TYPES types[] = VALIDTYPES; \
			int sphereTypesArrayLen = sizeof(types) / sizeof(SQLITE_SPHERE_TYPES); \
			int sphereTypesCounter = 0; \
			int sphereTypesFound = 0; \
			for(sphereTypesCounter = 0; sphereTypesCounter < sphereTypesArrayLen; sphereTypesCounter++) { \
		    	if(BUFFEROBJ->argTypes[ PARANUM ] == types[sphereTypesCounter]) { \
		    		sphereTypesFound = 1; \
		    		break; \
		    	} \
			} \
 \
			if(sphereTypesFound == 0) { \
				*is_null = 1; \
				return 0; \
			} \
    	} \


#define SQLITE_UDF_DYNCHK_SPHERETYPE_COM( PARANUM, BUFFEROBJ, VALIDTYPES1, VALIDTYPES2 ) \
    	if(BUFFEROBJ->isDynParams[PARANUM] == true) { \
    		if(BUFFEROBJ->memBufs[PARANUM] != NULL) { \
    			free(BUFFEROBJ->memBufs[PARANUM]); \
    			BUFFEROBJ->memBufs[PARANUM] = NULL;\
    			if(BUFFEROBJ->resBuf != NULL) { \
    				free(BUFFEROBJ->resBuf); \
    			} \
    		} \
 \
			BUFFEROBJ->argTypes[ PARANUM ] = decode(args->args[ PARANUM ], args->lengths[ PARANUM ], (void**)&BUFFEROBJ->memBufs[ PARANUM ]); \
 \
			SQLITE_SPHERE_TYPES types1[] = VALIDTYPES1; \
			int sphereTypes1ArrayLen = sizeof(types1) / sizeof(SQLITE_SPHERE_TYPES); \
			int sphereTypes1Counter = 0; \
			int sphereTypes1Found = 0; \
			SQLITE_SPHERE_TYPES types2[] = VALIDTYPES2; \
			int sphereTypes2ArrayLen = sizeof(types2) / sizeof(SQLITE_SPHERE_TYPES); \
			int sphereTypes2Counter = 0; \
			int sphereTypes2Found = 0; \
			for(sphereTypes1Counter = 0; sphereTypes1Counter < sphereTypes1ArrayLen; sphereTypes1Counter++) { \
		    	if(BUFFEROBJ->argTypes[ PARANUM ] == types1[sphereTypes1Counter]) { \
		    		sphereTypes1Found = 1; \
		    		break; \
		    	} \
			} \
			for(sphereTypes2Counter = 0; sphereTypes2Counter < sphereTypes2ArrayLen; sphereTypes2Counter++) { \
		    	if(BUFFEROBJ->argTypes[ PARANUM ] == types2[sphereTypes2Counter]) { \
		    		sphereTypes2Found = 1; \
		    		break; \
		    	} \
			} \
 \
			if(sphereTypes1Found == 0 && sphereTypes2Found == 0) { \
				*is_null = 1; \
				return 0; \
			} \
    	} \

#define SQLITE_UDF_DEINIT_BUFFER() \
	buffer * memBuf = (buffer*)initid->ptr; \
	if(memBuf != NULL) { \
		delete memBuf; \
	} \





#define SQLITE_UDF_SPHERE_ONEPARAM_INIT( FUNCNAME, VALIDTYPES ) \
	buffer * buf; \
	SQLITE_SPHERE_TYPES argType; \
 \
	if(args->arg_count == 1) { \
		SQLITE_UDF_CHK_PARAM_CHAR(0, FUNCNAME "() requires the first parameter to be a MySQL sphere object."); \
 \
    	buf = new buffer(1); \
 \
        SQLITE_UDF_CHK_SPHERETYPE( 0, buf, PROTECT(VALIDTYPES),  \
                                            FUNCNAME "() error decoding first parameter. Corrupted or not the correct type." ); \
	} else { \
		strcpy(message, "wrong number of arguments: " FUNCNAME "() requires one parameter"); \
		return 1; \
	} \
    \
	initid->decimals = 31; \
	initid->maybe_null = 1; \
	initid->max_length = 1024; \
	initid->ptr = (char*)buf; \
 \
	return 0; \


class buffer {
public:
	int len;
	SQLITE_SPHERE_TYPES * argTypes;
	void ** memBufs;
	bool * isDynParams;
	char * resBuf;

	buffer(int nlen) {
		len = nlen;
		argTypes = (SQLITE_SPHERE_TYPES*)malloc(len * sizeof(SQLITE_SPHERE_TYPES));
		memBufs = (void**)malloc(len * sizeof(void*));
		memset(memBufs, 0, len * sizeof(void*));
		isDynParams = (bool*)malloc(len * sizeof(bool));
		resBuf = NULL;
	}

	~buffer() {
		int i;
		if(memBufs != NULL) {
			for(i = 0; i < len; i++) {
				if(memBufs[i] != NULL) {
					free(memBufs[i]);
				}
			}

			free(memBufs);
		}

		if(resBuf != NULL) {
			free(resBuf);
		}

		if(argTypes != NULL) {
			free(argTypes);
		}

		if(isDynParams != NULL) {
			free(isDynParams);
		}
	}
};

*/
#endif