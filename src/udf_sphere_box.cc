/* Add your header comment here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "box.h"
#include "udf_helpers.h"
#include "udf_sphere_box.h"

//supporting sbox(spoint, spoint), sbox(box_string)
void sboxFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box = NULL;

	assert( argc==1 || argc==2 );

	if(argc == 2) {
		//SPoint
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
										"sbox(spoint, spoint) requires the first parameter to be a SQLITE sphere object.");

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										"sbox(spoint, spoint) requires the first parameter to be a SQLITE sphere object.");

		SPoint * point0 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;

		SPoint * point1 = NULL;
		SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;

		SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, point0, decodedDataType0, PROTECT({SQLITE_SPHERE_POINT}), 
									"sbox(spoint, spoint) error decoding first parameter. Corrupted or not the correct type." );

		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, point1, decodedDataType1, PROTECT({SQLITE_SPHERE_POINT}), 
									"sbox(spoint, spoint) error decoding first parameter. Corrupted or not the correct type." );

		box = spherebox_in_from_points( point0, point1 );

		free(point0);
		free(point1);
	} else if(argc == 1) {
		//string
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHAR( 0, data, dataLen, 
										"sbox(box_string) requires a string.");

		//sbox(box_string)
		box = spherebox_in( (char*)data );
	} else {
		sqlite3_result_error(context, "wrong number of arguments: sbox() requires one or two parameters", 666);
		return;
	}

	if(box == NULL) {
		sqlite3_result_error(context, "an error occurred while generating the box", 666);
		return;
	}

	char * result;
	int resultLen;
	result = serialise(box);
	resultLen = getSerialisedLen(box);
	free(box);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);

	free(result);
}

//sbox_sw(SBox)...
void sbox_swFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "sbox_sw", box, PROTECT({SQLITE_SPHERE_BOX}) ); 
	SPoint * resPoint = spherebox_sw(box);

	char * result = NULL;
	int resultLen;
	result = serialise(resPoint);
	resultLen = getSerialisedLen(resPoint);
	free(resPoint);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(box);
	free(result);
}

//sbox_se(SBox)...
void sbox_seFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "sbox_se", box, PROTECT({SQLITE_SPHERE_BOX}) ); 
	SPoint * resPoint = spherebox_se(box);

	char * result = NULL;
	int resultLen;
	result = serialise(resPoint);
	resultLen = getSerialisedLen(resPoint);
	free(resPoint);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(box);
	free(result);
}

//sbox_nw(SBox)...
void sbox_nwFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "sbox_nw", box, PROTECT({SQLITE_SPHERE_BOX}) ); 
	SPoint * resPoint = spherebox_nw(box);

	char * result = NULL;
	int resultLen;
	result = serialise(resPoint);
	resultLen = getSerialisedLen(resPoint);
	free(resPoint);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(box);
	free(result);
}

//sbox_ne(SBox)...
void sbox_neFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_ONEPARAM_INIT( "sbox_ne", box, PROTECT({SQLITE_SPHERE_BOX}) ); 
	SPoint * resPoint = spherebox_ne(box);

	char * result = NULL;
	int resultLen;
	result = serialise(resPoint);
	resultLen = getSerialisedLen(resPoint);
	free(resPoint);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(box);
	free(result);
}

//sbox_equal(SBox, SBox)...
void sbox_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box0 = NULL;
	SBox * box1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_equal", box0, box1, PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_equal(box0, box1);
	sqlite3_result_int(context, res);
	free(box0);
	free(box1);
}

//sbox_equal_neg(SBox, SBox)...
void sbox_equal_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box0 = NULL;
	SBox * box1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_equal_neg", box0, box1, PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_equal_neg(box0, box1);
	sqlite3_result_int(context, res);
	free(box0);
	free(box1);
}

//sbox_contains_box(SBox, SBox)...
void sbox_contains_boxFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box0 = NULL;
	SBox * box1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_box", box0, box1, PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_box(box0, box1);
	sqlite3_result_int(context, res);
	free(box0);
	free(box1);
}

//sbox_contains_box_neg(SBox, SBox)...
void sbox_contains_box_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box0 = NULL;
	SBox * box1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_box_neg", box0, box1, PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_box_neg(box0, box1);
	sqlite3_result_int(context, res);
	free(box0);
	free(box1);
}

//sbox_contains_box_com(SBox, SBox)...
void sbox_contains_box_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box0 = NULL;
	SBox * box1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_box_com", box0, box1, PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_box_com(box0, box1);
	sqlite3_result_int(context, res);
	free(box0);
	free(box1);
}

//sbox_contains_box_com_neg(SBox, SBox)...
void sbox_contains_box_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box0 = NULL;
	SBox * box1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_box_com_neg", box0, box1, PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_box_com_neg(box0, box1);
	sqlite3_result_int(context, res);
	free(box0);
	free(box1);
}

//sbox_overlap_box(SBox, SBox)...
void sbox_overlap_boxFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box0 = NULL;
	SBox * box1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_overlap_box", box0, box1, PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_overlap_box(box0, box1);
	sqlite3_result_int(context, res);
	free(box0);
	free(box1);
}

//sbox_overlap_box_neg(SBox, SBox)...
void sbox_overlap_box_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box0 = NULL;
	SBox * box1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_overlap_box_neg", box0, box1, PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_overlap_box_neg(box0, box1);
	sqlite3_result_int(context, res);
	free(box0);
	free(box1);
}

//sbox_contains_point(SBox, SPoint)...
void sbox_contains_pointFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_contains_point", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_POINT}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_cont_point((SBox*) objData0, (SPoint*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_POINT) {
		res = (long long)spherebox_cont_point_com((SPoint*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_contains_point_neg(SBox, SPoint)...
void sbox_contains_point_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_contains_point_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_POINT}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_cont_point_neg((SBox*) objData0, (SPoint*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_POINT) {
		res = (long long)spherebox_cont_point_com_neg((SPoint*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_contains_point_com(SPoint, SBox)...
void sbox_contains_point_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_point_com", point, box, PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_point_com(point, box);
	sqlite3_result_int(context, res);
	free(point);
	free(box);
}

//sbox_contains_point_com_neg(SPoint, SBox)...
void sbox_contains_point_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_point_com_neg", point, box, PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_point_com_neg(point, box);
	sqlite3_result_int(context, res);
	free(point);
	free(box);
}

//sbox_contains_circle(SBox, SCircle)...
void sbox_contains_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_contains_circle", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_cont_circle((SBox*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherebox_cont_circle_com((SCircle*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_contains_circle_neg(SBox, SCircle)...
void sbox_contains_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_contains_circle_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_cont_circle_neg((SBox*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherebox_cont_circle_com_neg((SCircle*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_contains_circle_com(SCircle, SBox)...
void sbox_contains_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_circle_com", circle, box, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_circle_com(circle, box);
	sqlite3_result_int(context, res);
	free(circle);
	free(box);
}

//sbox_contains_circle_com_neg(SCircle, SBox)...
void sbox_contains_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_circle_com_neg", circle, box, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_circle_com_neg(circle, box);
	sqlite3_result_int(context, res);
	free(circle);
	free(box);
}

//scircle_contains_box(SCircle, SBox)...
void scircle_contains_boxFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "scircle_contains_box", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_BOX}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherecircle_cont_box((SCircle*) objData0, (SBox*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherecircle_cont_box_com((SBox*) objData0, (SCircle*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//scircle_contains_box_neg(SCircle, SBox)...
void scircle_contains_box_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "scircle_contains_box_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_BOX}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherecircle_cont_box_neg((SCircle*) objData0, (SBox*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherecircle_cont_box_com_neg((SBox*) objData0, (SCircle*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//scircle_contains_box_com(SBox, SCircle)...
void scircle_contains_box_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box = NULL;
	SCircle * circle = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_contains_box_com", box, circle, PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_cont_box_com(box, circle);
	sqlite3_result_int(context, res);
	free(box);
	free(circle);
}

//scircle_contains_box_com_neg(SBox, SCircle)...
void scircle_contains_box_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box = NULL;
	SCircle * circle = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_contains_box_com_neg", box, circle, PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_cont_box_com_neg(box, circle);
	sqlite3_result_int(context, res);
	free(box);
	free(circle);
}

//sbox_overlap_circle(SBox, SCircle)...
void sbox_overlap_circleFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_overlap_circle", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_overlap_circle((SBox*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherebox_overlap_circle_com((SCircle*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_overlap_circle_neg(SBox, SCircle)...
void sbox_overlap_circle_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_overlap_circle_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_overlap_circle_neg((SBox*) objData0, (SCircle*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherebox_overlap_circle_com_neg((SCircle*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_overlap_circle_com(SCircle, SBox)...
void sbox_overlap_circle_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_overlap_circle_com", circle, box, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_overlap_circle_com(circle, box);
	sqlite3_result_int(context, res);
	free(circle);
	free(box);
}

//sbox_overlap_circle_com_neg(SCircle, SBox)...
void sbox_overlap_circle_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SCircle * circle = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_overlap_circle_com_neg", circle, box, PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_overlap_circle_com_neg(circle, box);
	sqlite3_result_int(context, res);
	free(circle);
	free(box);
}

//sbox_contains_line(SBox, SLine)...
void sbox_contains_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_contains_line", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_cont_line((SBox*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)spherebox_cont_line_com((SLine*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_contains_line_neg(SBox, SLine)...
void sbox_contains_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_contains_line_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_cont_line_neg((SBox*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)spherebox_cont_line_com_neg((SLine*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_contains_line_com(SLine, SBox)...
void sbox_contains_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_line_com", line, box, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_line_com(line, box);
	sqlite3_result_int(context, res);
	free(line);
	free(box);
}

//sbox_contains_line_com_neg(SLine, SBox)...
void sbox_contains_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_line_com_neg", line, box, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_line_com_neg(line, box);
	sqlite3_result_int(context, res);
	free(line);
	free(box);
}

//sbox_overlap_line(SBox, SLine)...
void sbox_overlap_lineFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_overlap_line", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_overlap_line((SBox*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)spherebox_overlap_line_com((SLine*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_overlap_line_neg(SBox, SLine)...
void sbox_overlap_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_overlap_line_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_overlap_line_neg((SBox*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)spherebox_overlap_line_com_neg((SLine*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_overlap_line_com(SLine, SBox)...
void sbox_overlap_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_overlap_line_com", line, box, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_overlap_line_com(line, box);
	sqlite3_result_int(context, res);
	free(line);
	free(box);
}

//sbox_overlap_line_com_neg(SLine, SBox)...
void sbox_overlap_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_overlap_line_com_neg", line, box, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_overlap_line_com_neg(line, box);
	sqlite3_result_int(context, res);
	free(line);
	free(box);
}

//sbox_contains_ellipse(SBox, SEllipse)...
void sbox_contains_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_contains_ellipse", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_cont_ellipse((SBox*) objData0, (SEllipse*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)spherebox_cont_ellipse_com((SEllipse*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_contains_ellipse_neg(SBox, SEllipse)...
void sbox_contains_ellipse_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_contains_ellipse_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_cont_ellipse_neg((SBox*) objData0, (SEllipse*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)spherebox_cont_ellipse_com_neg((SEllipse*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_contains_ellipse_com(SEllipse, SBox)...
void sbox_contains_ellipse_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_ellipse_com", ellipse, box, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_ellipse_com(ellipse, box);
	sqlite3_result_int(context, res);
	free(ellipse);
	free(box);
}

//sbox_contains_ellipse_com_neg(SEllipse, SBox)...
void sbox_contains_ellipse_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_ellipse_com_neg", ellipse, box, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_ellipse_com_neg(ellipse, box);
	sqlite3_result_int(context, res);
	free(ellipse);
	free(box);
}

//sellipse_contains_box(SEllipse, SBox)...
void sellipse_contains_boxFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_contains_box", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_BOX}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)sphereellipse_cont_box((SEllipse*) objData0, (SBox*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_cont_box_com((SBox*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_contains_box_neg(SEllipse, SBox)...
void sellipse_contains_box_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_contains_box_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_BOX}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)sphereellipse_cont_box_neg((SEllipse*) objData0, (SBox*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_cont_box_com_neg((SBox*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_contains_box_com(SBox, SEllipse)...
void sellipse_contains_box_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_box_com", box, ellipse, PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_box_com(box, ellipse);
	sqlite3_result_int(context, res);
	free(box);
	free(ellipse);
}

//sellipse_contains_box_com_neg(SBox, SEllipse)...
void sellipse_contains_box_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_box_com_neg", box, ellipse, PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_box_com_neg(box, ellipse);
	sqlite3_result_int(context, res);
	free(box);
	free(ellipse);
}

//sbox_overlap_ellipse(SBox, SEllipse)...
void sbox_overlap_ellipseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_overlap_ellipse", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_overlap_ellipse((SBox*) objData0, (SEllipse*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)spherebox_overlap_ellipse_com((SEllipse*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_overlap_ellipse_neg(SBox, SEllipse)...
void sbox_overlap_ellipse_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_overlap_ellipse_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_overlap_ellipse_neg((SBox*) objData0, (SEllipse*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)spherebox_overlap_ellipse_com_neg((SEllipse*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_overlap_ellipse_com(SEllipse, SBox)...
void sbox_overlap_ellipse_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_overlap_ellipse_com", ellipse, box, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_overlap_ellipse_com(ellipse, box);
	sqlite3_result_int(context, res);
	free(ellipse);
	free(box);
}

//sbox_overlap_ellipse_com_neg(SEllipse, SBox)...
void sbox_overlap_ellipse_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SEllipse * ellipse = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_overlap_ellipse_com_neg", ellipse, box, PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_overlap_ellipse_com_neg(ellipse, box);
	sqlite3_result_int(context, res);
	free(ellipse);
	free(box);
}

//sbox_contains_poly(SBox, SPoly)...
void sbox_contains_polyFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_contains_poly", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_cont_poly((SBox*) objData0, (SPoly*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherebox_cont_poly_com((SPoly*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_contains_poly_neg(SBox, SPoly)...
void sbox_contains_poly_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_contains_poly_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_cont_poly_neg((SBox*) objData0, (SPoly*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherebox_cont_poly_com_neg((SPoly*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_contains_poly_com(SPoly, SBox)...
void sbox_contains_poly_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_poly_com", polygon, box, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_poly_com(polygon, box);
	sqlite3_result_int(context, res);
	free(polygon);
	free(box);
}

//sbox_contains_poly_com_neg(SPoly, SBox)...
void sbox_contains_poly_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_poly_com_neg", polygon, box, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_poly_com_neg(polygon, box);
	sqlite3_result_int(context, res);
	free(polygon);
	free(box);
}

//spoly_contains_box(SPoly, SBox)...
void spoly_contains_boxFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_contains_box", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_BOX}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherepoly_cont_box((SPoly*) objData0, (SBox*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_cont_box_com((SBox*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_contains_box_neg(SPoly, SBox)...
void spoly_contains_box_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_contains_box_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_BOX}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherepoly_cont_box_neg((SPoly*) objData0, (SBox*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_cont_box_com_neg((SBox*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_contains_box_com(SBox, SPoly)...
void spoly_contains_box_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_box_com", box, polygon, PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_box_com(box, polygon);
	sqlite3_result_int(context, res);
	free(box);
	free(polygon);
}

//spoly_contains_box_com_neg(SBox, SPoly)...
void spoly_contains_box_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SBox * box = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_box_com_neg", box, polygon, PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_box_com_neg(box, polygon);
	sqlite3_result_int(context, res);
	free(box);
	free(polygon);
}

//sbox_overlap_poly(SBox, SPoly)...
void sbox_overlap_polyFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_overlap_poly", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_overlap_poly((SBox*) objData0, (SPoly*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherebox_overlap_poly_com((SPoly*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_overlap_poly_neg(SBox, SPoly)...
void sbox_overlap_poly_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_overlap_poly_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_overlap_poly_neg((SBox*) objData0, (SPoly*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherebox_overlap_poly_com_neg((SPoly*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_overlap_poly_com(SPoly, SBox)...
void sbox_overlap_poly_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_overlap_poly_com", polygon, box, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_overlap_poly_com(polygon, box);
	sqlite3_result_int(context, res);
	free(polygon);
	free(box);
}

//sbox_overlap_poly_com_neg(SPoly, SBox)...
void sbox_overlap_poly_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoly * polygon = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_overlap_poly_com_neg", polygon, box, PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_overlap_poly_com_neg(polygon, box);
	sqlite3_result_int(context, res);
	free(polygon);
	free(box);
}

//sbox_contains_path(SBox, SPath)...
void sbox_contains_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_contains_path", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_cont_path((SBox*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherebox_cont_path_com((SPath*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_contains_path_neg(SBox, SPath)...
void sbox_contains_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_contains_path_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_cont_path_neg((SBox*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherebox_cont_path_com_neg((SPath*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_contains_path_com(SPath, SBox)...
void sbox_contains_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_path_com", path, box, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_path_com(path, box);
	sqlite3_result_int(context, res);
	free(path);
	free(box);
}

//sbox_contains_path_com_neg(SPath, SBox)...
void sbox_contains_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_contains_path_com_neg", path, box, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_cont_path_com_neg(path, box);
	sqlite3_result_int(context, res);
	free(path);
	free(box);
}

//sbox_overlap_path(SBox, SPath)...
void sbox_overlap_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_overlap_path", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_overlap_path((SBox*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherebox_overlap_path_com((SPath*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_overlap_path_neg(SBox, SPath)...
void sbox_overlap_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sbox_overlap_path_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_BOX}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_BOX) {
		res = (long long)spherebox_overlap_path_neg((SBox*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_BOX && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherebox_overlap_path_com_neg((SPath*) objData0, (SBox*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sbox_overlap_path_com(SPath, SBox)...
void sbox_overlap_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_overlap_path_com", path, box, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_overlap_path_com(path, box);
	sqlite3_result_int(context, res);
	free(path);
	free(box);
}

//sbox_overlap_path_com_neg(SPath, SBox)...
void sbox_overlap_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SBox * box = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sbox_overlap_path_com_neg", path, box, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_BOX}) ); 
	long res = spherebox_overlap_path_com_neg(path, box);
	sqlite3_result_int(context, res);
	free(path);
	free(box);
}
