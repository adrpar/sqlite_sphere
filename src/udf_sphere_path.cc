/* Add your header comment here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "polygon.h"
#include "udf_helpers.h"
#include "udf_sphere_path.h"

//supporting spath(path_string)
void spathFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;

	assert( argc==1 );

	if(argc == 1) {
		//string
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHAR( 0, data, dataLen, 
										"spath(path_string) requires a string.");

		//spath(path_string)
		path = spherepath_in( (char*)data );
	} else {
		sqlite3_result_error(context, "wrong number of arguments: spath() requires one parameter", 666);
		return;
	}

	if(path == NULL) {
		sqlite3_result_error(context, "an error occurred while generating the path", 666);
		return;
	}

	char * result;
	int resultLen;
	result = serialise(path);
	resultLen = getSerialisedLen(path);
	free(path);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);

	free(result);
}

//spath_equal(SPath, SPath)...
void spath_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path0 = NULL;
	SPath * path1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spath_equal", path0, path1, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_PATH}) ); 
	long res = spherepath_equal(path0, path1);
	sqlite3_result_int(context, res);
	free(path0);
	free(path1);
}

//spath_equal_neg(SPath, SPath)...
void spath_equal_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path0 = NULL;
	SPath * path1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spath_equal_neg", path0, path1, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_PATH}) ); 
	long res = spherepath_equal_neg(path0, path1);
	sqlite3_result_int(context, res);
	free(path0);
	free(path1);
}

//spath_overlap_path(SPath, SPath)...
void spath_overlap_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path0 = NULL;
	SPath * path1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spath_overlap_path", path0, path1, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_PATH}) ); 
	long res = spherepath_overlap_path(path0, path1);
	sqlite3_result_int(context, res);
	free(path0);
	free(path1);
}

//spath_overlap_path_neg(SPath, SPath)...
void spath_overlap_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path0 = NULL;
	SPath * path1 = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spath_overlap_path_neg", path0, path1, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_PATH}) ); 
	long res = spherepath_overlap_path_neg(path0, path1);
	sqlite3_result_int(context, res);
	free(path0);
	free(path1);
}

//spath_contains_point(SPath, SPoint)...
void spath_contains_pointFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spath_contains_point", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_POINT}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherepath_cont_point((SPath*) objData0, (SPoint*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_POINT) {
		res = (long long)spherepath_cont_point_com((SPoint*) objData0, (SPath*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spath_contains_point_neg(SPath, SPoint)...
void spath_contains_point_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spath_contains_point_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_POINT}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POINT && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherepath_cont_point_neg((SPath*) objData0, (SPoint*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_POINT) {
		res = (long long)spherepath_cont_point_com_neg((SPoint*) objData0, (SPath*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spath_contains_point_com(SPoint, SPath)...
void spath_contains_point_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SPath * path = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spath_contains_point_com", point, path, PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_PATH}) ); 
	long res = spherepath_cont_point_com(point, path);
	sqlite3_result_int(context, res);
	free(point);
	free(path);
}

//spath_contains_point_com_neg(SPoint, SPath)...
void spath_contains_point_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPoint * point = NULL;
	SPath * path = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spath_contains_point_com_neg", point, path, PROTECT({SQLITE_SPHERE_POINT}), PROTECT({SQLITE_SPHERE_PATH}) ); 
	long res = spherepath_cont_point_com_neg(point, path);
	sqlite3_result_int(context, res);
	free(point);
	free(path);
}

//strans_path(SPath, SEuler)...
void strans_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_path", path, euler, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	SPath * resPath = spheretrans_path(path, euler);

	char * result = NULL;
	int resultLen;
	result = serialise(resPath);
	resultLen = getSerialisedLen(resPath);
	free(resPath);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(path);
	free(euler);
	free(result);
}

//strans_path_inverse(SPath, SEuler)...
void strans_path_inverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SEuler * euler = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "strans_path_inverse", path, euler, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_EULER}) ); 
	SPath * resPath = spheretrans_path_inverse(path, euler);

	char * result = NULL;
	int resultLen;
	result = serialise(resPath);
	resultLen = getSerialisedLen(resPath);
	free(resPath);

	sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);
	free(path);
	free(euler);
	free(result);
}

//scircle_contains_path(SCircle, SPath)...
void scircle_contains_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "scircle_contains_path", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherecircle_cont_path((SCircle*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherecircle_cont_path_com((SPath*) objData0, (SCircle*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//scircle_contains_path_neg(SCircle, SPath)...
void scircle_contains_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "scircle_contains_path_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherecircle_cont_path_neg((SCircle*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherecircle_cont_path_com_neg((SPath*) objData0, (SCircle*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//scircle_contains_path_com(SPath, SCircle)...
void scircle_contains_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SCircle * circle = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_contains_path_com", path, circle, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_cont_path_com(path, circle);
	sqlite3_result_int(context, res);
	free(path);
	free(circle);
}

//scircle_contains_path_com_neg(SPath, SCircle)...
void scircle_contains_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SCircle * circle = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_contains_path_com_neg", path, circle, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_cont_path_com_neg(path, circle);
	sqlite3_result_int(context, res);
	free(path);
	free(circle);
}

//scircle_overlap_path(SCircle, SPath)...
void scircle_overlap_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "scircle_overlap_path", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherecircle_overlap_path((SCircle*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherecircle_overlap_path_com((SPath*) objData0, (SCircle*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//scircle_overlap_path_neg(SCircle, SPath)...
void scircle_overlap_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "scircle_overlap_path_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_CIRCLE}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_CIRCLE && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherecircle_overlap_path_neg((SCircle*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_CIRCLE) {
		res = (long long)spherecircle_overlap_path_com_neg((SPath*) objData0, (SCircle*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//scircle_overlap_path_com(SPath, SCircle)...
void scircle_overlap_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SCircle * circle = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_overlap_path_com", path, circle, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_overlap_path_com(path, circle);
	sqlite3_result_int(context, res);
	free(path);
	free(circle);
}

//scircle_overlap_path_com_neg(SPath, SCircle)...
void scircle_overlap_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SCircle * circle = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "scircle_overlap_path_com_neg", path, circle, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_CIRCLE}) ); 
	long res = spherecircle_overlap_path_com_neg(path, circle);
	sqlite3_result_int(context, res);
	free(path);
	free(circle);
}

//spath_overlap_line(SPath, SLine)...
void spath_overlap_line(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spath_overlap_line", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherepath_overlap_line((SPath*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)spherepath_overlap_line_com((SLine*) objData0, (SPath*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spath_overlap_line_neg(SPath, SLine)...
void spath_overlap_line_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spath_overlap_line_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_LINE}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_LINE && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherepath_overlap_line_neg((SPath*) objData0, (SLine*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_LINE) {
		res = (long long)spherepath_overlap_line_com_neg((SLine*) objData0, (SPath*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spath_overlap_line_com(SLine, SPath)...
void spath_overlap_line_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SPath * path = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spath_overlap_line_com", line, path, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_PATH}) ); 
	long res = spherepath_overlap_line_com(line, path);
	sqlite3_result_int(context, res);
	free(line);
	free(path);
}

//spath_overlap_line_com_neg(SLine, SPath)...
void spath_overlap_line_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SLine * line = NULL;
	SPath * path = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spath_overlap_line_com_neg", line, path, PROTECT({SQLITE_SPHERE_LINE}), PROTECT({SQLITE_SPHERE_PATH}) ); 
	long res = spherepath_overlap_line_com_neg(line, path);
	sqlite3_result_int(context, res);
	free(line);
	free(path);
}

//sellipse_contains_path(SEllipse, SPath)...
void sellipse_contains_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_contains_path", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)sphereellipse_cont_path((SEllipse*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_cont_path_com((SPath*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_contains_path_neg(SEllipse, SPath)...
void sellipse_contains_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_contains_path_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)sphereellipse_cont_path_neg((SEllipse*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_cont_path_com_neg((SPath*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_contains_path_com(SPath, SEllipse)...
void sellipse_contains_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_path_com", path, ellipse, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_path_com(path, ellipse);
	sqlite3_result_int(context, res);
	free(path);
	free(ellipse);
}

//sellipse_contains_path_com_neg(SPath, SEllipse)...
void sellipse_contains_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_contains_path_com_neg", path, ellipse, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_cont_path_com_neg(path, ellipse);
	sqlite3_result_int(context, res);
	free(path);
	free(ellipse);
}

//sellipse_overlap_path(SEllipse, SPath)...
void sellipse_overlap_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_overlap_path", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)sphereellipse_overlap_path((SEllipse*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_overlap_path_com((SPath*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_overlap_path_neg(SEllipse, SPath)...
void sellipse_overlap_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "sellipse_overlap_path_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_ELLIPSE}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_ELLIPSE && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)sphereellipse_overlap_path_neg((SEllipse*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_ELLIPSE) {
		res = (long long)sphereellipse_overlap_path_com_neg((SPath*) objData0, (SEllipse*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//sellipse_overlap_path_com(SPath, SEllipse)...
void sellipse_overlap_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_overlap_path_com", path, ellipse, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_overlap_path_com(path, ellipse);
	sqlite3_result_int(context, res);
	free(path);
	free(ellipse);
}

//sellipse_overlap_path_com_neg(SPath, SEllipse)...
void sellipse_overlap_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SEllipse * ellipse = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "sellipse_overlap_path_com_neg", path, ellipse, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_ELLIPSE}) ); 
	long res = sphereellipse_overlap_path_com_neg(path, ellipse);
	sqlite3_result_int(context, res);
	free(path);
	free(ellipse);
}

//spoly_contains_path(SPoly, SPath)...
void spoly_contains_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_contains_path", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherepoly_cont_path((SPoly*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_cont_path_com((SPath*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_contains_path_neg(SPoly, SPath)...
void spoly_contains_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_contains_path_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherepoly_cont_path_neg((SPoly*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_cont_path_com_neg((SPath*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_contains_path_com(SPath, SPoly)...
void spoly_contains_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_path_com", path, polygon, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_path_com(path, polygon);
	sqlite3_result_int(context, res);
	free(path);
	free(polygon);
}

//spoly_contains_path_com_neg(SPath, SPoly)...
void spoly_contains_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_contains_path_com_neg", path, polygon, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_cont_path_com_neg(path, polygon);
	sqlite3_result_int(context, res);
	free(path);
	free(polygon);
}

//spoly_overlap_path(SPoly, SPath)...
void spoly_overlap_pathFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_overlap_path", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherepoly_overlap_path((SPoly*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_overlap_path_com((SPath*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_overlap_path_neg(SPoly, SPath)...
void spoly_overlap_path_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	void * objData0 = NULL;
	void * objData1 = NULL;
	SQLITE_SPHERE_TYPES decodedDataType0 = SQLITE_SPHERE_UNKNOWN;
	SQLITE_SPHERE_TYPES decodedDataType1 = SQLITE_SPHERE_UNKNOWN;	
	SQLITE_UDF_SPHERE_TWOPARAM_COM_INIT( "spoly_overlap_path_neg", objData0, objData1, decodedDataType0, decodedDataType1,
											PROTECT({SQLITE_SPHERE_POLYGON}), PROTECT({SQLITE_SPHERE_PATH}) ); 

	long res = 0;
	if(decodedDataType0 == SQLITE_SPHERE_POLYGON && decodedDataType1 == SQLITE_SPHERE_PATH) {
		res = (long long)spherepoly_overlap_path_neg((SPoly*) objData0, (SPath*) objData1);
	} else if(decodedDataType0 == SQLITE_SPHERE_PATH && decodedDataType1 == SQLITE_SPHERE_POLYGON) {
		res = (long long)spherepoly_overlap_path_com_neg((SPath*) objData0, (SPoly*) objData1);
	}

	sqlite3_result_int(context, res);
	free(objData0);
	free(objData1);
}

//spoly_overlap_path_com(SPath, SPoly)...
void spoly_overlap_path_comFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_overlap_path_com", path, polygon, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_overlap_path_com(path, polygon);
	sqlite3_result_int(context, res);
	free(path);
	free(polygon);
}

//spoly_overlap_path_com_neg(SPath, SPoly)...
void spoly_overlap_path_com_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPath * path = NULL;
	SPoly * polygon = NULL;
	SQLITE_UDF_SPHERE_TWOPARAM_INIT( "spoly_overlap_path_com_neg", path, polygon, PROTECT({SQLITE_SPHERE_PATH}), PROTECT({SQLITE_SPHERE_POLYGON}) ); 
	long res = spherepoly_overlap_path_com_neg(path, polygon);
	sqlite3_result_int(context, res);
	free(path);
	free(polygon);
}

typedef struct SPathBuf {
	SPath * path;
} SPathBuf;

//spath_add_point_aggr(SPath, SPoint)...
void spath_add_point_aggrStep(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPathBuf * pathBuf = NULL;

	assert( argc==2 );

	pathBuf = (SPathBuf*)sqlite3_aggregate_context(context, sizeof(*pathBuf));

	if(argc == 2) {
		//string
		char * data0 = NULL;
		int dataLen0 = 0;
		char * data1 = NULL;
		int dataLen1 = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 1, data1, dataLen1, 
										"spath_add_point_aggr() requires the second parameter to be a SQLITE sphere object.");

		//we can only process one polygon at the time - if multiple come in here out of no reason
		//then we just use the first one and stick to it - in case of GROUP BY, it will be reread
		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		if(pathBuf->path == NULL) {
			SPath * tmpPath = pathBuf->path;
			
			SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data0, dataLen0, 
											"spath_add_point_aggr() requires the first parameter to be a SQLITE sphere object.");

			SQLITE_UDF_CHK_SPHERETYPE( data0, dataLen0, tmpPath, decodedDataType, PROTECT({SQLITE_SPHERE_PATH}), 
										"spath_add_point_aggr() error decoding first parameter. Corrupted or not the correct type." );
		}

		SPoint * point = NULL;
		SQLITE_UDF_CHK_SPHERETYPE( data1, dataLen1, point, decodedDataType, PROTECT({SQLITE_SPHERE_POINT}), 
									"spath_add_point_aggr() error decoding second parameter. Corrupted or not the correct type." );


		pathBuf->path = spherepath_add_point(pathBuf->path, point);

		free(point);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: spath_add_point_aggr() requires two parameters", 666);
		return;
	}
}

void spath_add_point_aggrFinalize(sqlite3_context *context) {
	SPathBuf * pathBuf = NULL;

	pathBuf = (SPathBuf*)sqlite3_aggregate_context(context, 0);

	if(pathBuf && pathBuf->path) {
		char * result;
		int resultLen;
		result = serialise(pathBuf->path);
		resultLen = getSerialisedLen(pathBuf->path);
		free(pathBuf->path);

		sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);

		free(result);
	} else {
		sqlite3_result_null(context);
	}
}

//spath_aggr(SPoint)...
void spath_aggrStep(sqlite3_context *context, int argc, sqlite3_value **argv) {
	SPathBuf * pathBuf = NULL;

	assert( argc==1 );

	pathBuf = (SPathBuf*)sqlite3_aggregate_context(context, sizeof(*pathBuf));

	if(argc == 1) {
		//string
		char * data = NULL;
		int dataLen = 0;

		SQLITE_UDF_CHK_PARAM_CHARBLOB( 0, data, dataLen, 
										"spath_aggr() requires the first parameter to be a SQLITE sphere object.");

		SQLITE_SPHERE_TYPES decodedDataType = SQLITE_SPHERE_UNKNOWN;

		SPoint * point = NULL;
		SQLITE_UDF_CHK_SPHERETYPE( data, dataLen, point, decodedDataType, PROTECT({SQLITE_SPHERE_POINT}), 
									"spath_aggr() error decoding first parameter. Corrupted or not the correct type." );


		pathBuf->path = spherepath_add_point(pathBuf->path, point);

		free(point);
	} else {
		sqlite3_result_error(context, "wrong number of arguments: spath_aggr() requires two parameters", 666);
		return;
	}
}

void spath_aggrFinalize(sqlite3_context *context) {
	SPathBuf * pathBuf = NULL;

	pathBuf = (SPathBuf*)sqlite3_aggregate_context(context, 0);

	if(pathBuf && pathBuf->path) {
		char * result;
		int resultLen;
		result = serialise(pathBuf->path);
		resultLen = getSerialisedLen(pathBuf->path);
		free(pathBuf->path);

		sqlite3_result_blob(context, result, resultLen, SQLITE_TRANSIENT);

		free(result);
	} else {
		sqlite3_result_null(context);
	}
}






