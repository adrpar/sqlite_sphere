/* Add your header comment here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "line.h"
#include "udf_helpers.h"
#include "udf_sphere_line.h"

//supporting sline(spoint, spoint), sline(strans, double), sline(spoint) or sline(line_string)
void slineFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;

	assert( argc==1 || argc==2 );

	if(argc == 2) {
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;
		double l = 0.0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
										"sline(spoint, spoint) or sline(strans, double) requires the first parameter to be a SQLITE sphere object.");

		if( sqlite3_value_type(argv[ 1 ]) != SQLITE_TEXT && sqlite3_value_type(argv[ 1 ]) != SQLITE_BLOB ) {
			SQLITE_UDF_CHK_PARAM_REAL( 1, l, "sline(strans, double) requires double to be a number." );
		}

		void * objBuf0 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, objBuf0, decodedDataType0,
									PROTECT({SQLITE_SPHERE_POINT, SQLITE_SPHERE_EULER}), 
									"sline(spoint, spoint) or sline(strans, double) error decoding first parameter. Corrupted or not the correct type." );

		if( sqlite3_value_type(argv[ 1 ]) == SQLITE_TEXT || sqlite3_value_type(argv[ 1 ]) == SQLITE_BLOB  ) {
			//spoint
			SPoint * point = NULL;
			SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;

			SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										"sline(spoint, spoint) requires the second parameter to be a SQLITE sphere object.");

			SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, point, decodedDataType1,
										PROTECT({SQLITE_SPHERE_POINT}), 
										"sline(spoint, spoint) error decoding second parameter. Corrupted or not the correct type." );

			line = sphereline_from_points((SPoint *)objBuf0, point);

			free(point);
		} else {
			if( decodedDataType0 != SQLITE_SPHERE_EULER ) {
				sqlite3_result_error(context, "sline(strans, double) needs first parameter to be of object type strans.", 666);
				free(objBuf0);
				return;
			}

			line = sphereline_from_trans((SEuler *)objBuf0, l);
		}

		free(objBuf0);
	} else if(argc == 1) {
		//string
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, 
										"sline(line_string) requires a string. sline(spoint) a SQLITE sphere object.");

    	SPoint * point = NULL;
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, point, decodedDataType,
									PROTECT({SQLITE_SPHERE_POINT, SQLITE_SPHERE_UNKNOWN}), 
									"sline(spoint) error decoding first parameter. Corrupted or not the correct type." );

    	if(decodedDataType == SQLITE_SPHERE_POINT) {
    		//sline(spoint)
    		line = sphereline_from_point(point);
    		free(point);
    	} else {
    		//sline(line_string)
			line = sphereline_in( (char*)data );
			if(point != NULL)
				free(point);
		}
	} else {
		sqlite3_result_error(context, "wrong number of arguments: sline() requires one or two parameters", 666);
		return;
	}

	if(line == NULL) {
		sqlite3_result_error(context, "an error occurred while generating the line", 666);
		return;
	}

	char * result;
	int resultLen;
	result = serialise(line);
	resultLen = getSerialisedLen(line);
	free(line);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);

	free(result);
}

//smeridian(double)
void smeridianFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	double l = 0.0;

	assert( argc==1 );

	if(argc != 1) {
		sqlite3_result_error(context, "wrong number of arguments: meridian() requires one parameter", 666);
		return;
	}

	SQLITE_UDF_CHK_PARAM_REAL( 1, l, "meridian(double) requires double to be a number." );

	line = sphereline_meridian( l );

	char * result = NULL;
	int resultLen;
	result = serialise(line);
	resultLen = getSerialisedLen(line);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(line);
	free(result);
}

//sline_beg(sline)
void sline_begFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "sline_beg", line, PROTECT({SQLITE_SPHERE_LINE}) ); 
	SPoint * resPoint = sphereline_begin(line);

	char * result = NULL;
	int resultLen;
	result = serialise(resPoint);
	resultLen = getSerialisedLen(resPoint);
	free(resPoint);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(line);
	free(result);
}

//sline_end(sline)
void sline_endFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "sline_end", line, PROTECT({SQLITE_SPHERE_LINE}) ); 
	SPoint * resPoint = sphereline_end(line);

	char * result = NULL;
	int resultLen;
	result = serialise(resPoint);
	resultLen = getSerialisedLen(resPoint);
	free(resPoint);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(line);
	free(result);
}

//sline_equal(SLine, SLine)...
void sline_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line0 = NULL;
	SLine * line1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sline_equal", line0, line1, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_LINE}) ); 
	long res = sphereline_equal(line0, line1);
	sqlite3_result_int(context, res);
	free(line0);
	free(line1);
}

//sline_equal_neg(SLine, SLine)...
void sline_equal_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line0 = NULL;
	SLine * line1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sline_equal_neg", line0, line1, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_LINE}) ); 
	long res = sphereline_equal_neg(line0, line1);
	sqlite3_result_int(context, res);
	free(line0);
	free(line1);
}

//sline_turn(sline)
void sline_turnFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "sline_turn", line, PROTECT({SQLITE_SPHERE_LINE}) ); 
	SLine * resLine = sphereline_turn(line);

	char * result = NULL;
	int resultLen;
	result = serialise(resLine);
	resultLen = getSerialisedLen(resLine);
	free(resLine);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(line);
	free(result);
}

//sline_crosses(SLine, SLine)...
void sline_crossesFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line0 = NULL;
	SLine * line1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sline_crosses", line0, line1, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_LINE}) ); 
	long res = sphereline_crosses(line0, line1);
	sqlite3_result_int(context, res);
	free(line0);
	free(line1);
}

//sline_crosses_neg(SLine, SLine)...
void sline_crosses_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line0 = NULL;
	SLine * line1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sline_crosses_neg", line0, line1, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_LINE}) ); 
	long res = sphereline_crosses_neg(line0, line1);
	sqlite3_result_int(context, res);
	free(line0);
	free(line1);
}

//sline_overlap(SLine, SLine)...
void sline_overlapFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line0 = NULL;
	SLine * line1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sline_overlap", line0, line1, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_LINE}) ); 
	long res = sphereline_overlap(line0, line1);
	sqlite3_result_int(context, res);
	free(line0);
	free(line1);
}

//sline_overlap_neg(SLine, SLine)...
void sline_overlap_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line0 = NULL;
	SLine * line1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sline_overlap_neg", line0, line1, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_LINE}) ); 
	long res = sphereline_overlap_neg(line0, line1);
	sqlite3_result_int(context, res);
	free(line0);
	free(line1);
}

//sline_contains_point(SLine, SPoint)...
void sline_contains_pointFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sline_contains_point", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_POINT}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_POINT) {
		res = (long long)sphereline_cont_point((SLine*) objData0, (SPoint*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)sphereline_cont_point_com((SPoint*) objData0, (SLine*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sline_contains_point_neg(SLine, SPoint)...
void sline_contains_point_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sline_contains_point_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_POINT}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_POINT) {
		res = (long long)sphereline_cont_point_neg((SLine*) objData0, (SPoint*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)sphereline_cont_point_com_neg((SPoint*) objData0, (SLine*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sline_contains_point_com(SPoint, SLine)...
void sline_contains_point_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SLine * line = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sline_contains_point_com", point, line, PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_LINE}) ); 
	long res = sphereline_cont_point_com(point, line);
	sqlite3_result_int(context, res);
	free(point);
	free(line);
}

//sline_contains_point_com_neg(SPoint, SLine)...
void sline_contains_point_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SLine * line = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sline_contains_point_com_neg", point, line, PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_LINE}) ); 
	long res = sphereline_cont_point_com_neg(point, line);
	sqlite3_result_int(context, res);
	free(point);
	free(line);
}

//strans_line(SLine, SEuler)...
void strans_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_line", line, euler, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	SLine * resLine = spheretrans_line(line, euler);

	char * result = NULL;
	int resultLen;
	result = serialise(resLine);
	resultLen = getSerialisedLen(resLine);
	free(resLine);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(line);
	free(euler);
	free(result);
}

//strans_line_inverse(SLine, SEuler)...
void strans_line_inverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_line_inverse", line, euler, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	SLine * resLine = spheretrans_line_inverse(line, euler);

	char * result = NULL;
	int resultLen;
	result = serialise(resLine);
	resultLen = getSerialisedLen(resLine);
	free(resLine);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(line);
	free(euler);
	free(result);
}

//sline_overlap_circle(SLine, SCircle)...
void sline_overlap_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sline_overlap_circle", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)sphereline_overlap_circle((SLine*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)sphereline_overlap_circle_com((SCircle*) objData0, (SLine*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sline_overlap_circle_neg(SLine, SCircle)...
void sline_overlap_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sline_overlap_circle_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)sphereline_overlap_circle_neg((SLine*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)sphereline_overlap_circle_com_neg((SCircle*) objData0, (SLine*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sline_overlap_circle_com(SCircle, SLine)...
void sline_overlap_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SLine * line = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sline_overlap_circle_com", circle, line, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_LINE}) ); 
	long res = sphereline_overlap_circle_com(circle, line);
	sqlite3_result_int(context, res);
	free(circle);
	free(line);
}

//sline_overlap_circle_com_neg(SCircle, SLine)...
void sline_overlap_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SLine * line = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sline_overlap_circle_com_neg", circle, line, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_LINE}) ); 
	long res = sphereline_overlap_circle_com_neg(circle, line);
	sqlite3_result_int(context, res);
	free(circle);
	free(line);
}

//scircle_contains_line(SCircle, SLine)...
void scircle_contains_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "scircle_contains_line", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)spherecircle_cont_line((SCircle*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherecircle_cont_line_com((SLine*) objData0, (SCircle*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//scircle_contains_line_neg(SCircle, SLine)...
void scircle_contains_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "scircle_contains_line_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)spherecircle_cont_line_neg((SCircle*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherecircle_cont_line_com_neg((SLine*) objData0, (SCircle*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//scircle_contains_line_com(SLine, SCircle)...
void scircle_contains_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SCircle * circle = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_contains_line_com", line, circle, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_cont_line_com(line, circle);
	sqlite3_result_int(context, res);
	free(line);
	free(circle);
}

//scircle_contains_line_com_neg(SLine, SCircle)...
void scircle_contains_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SCircle * circle = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_contains_line_com_neg", line, circle, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_cont_line_com_neg(line, circle);
	sqlite3_result_int(context, res);
	free(line);
	free(circle);
}
