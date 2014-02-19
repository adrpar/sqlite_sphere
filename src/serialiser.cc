#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "base64.h"

#include "serialiser.h"
#include "serialiser_helper.h"

#define SQLITE_SERVER 1

size_t SERIAL_LEN_SPOINT = 8 + sizeof(SPoint) + 1;    //8 for id tag and rest for data + 1 for \0
size_t SERIAL_LEN_SEULER = 8 + sizeof(SEuler) + 1;    //8 for id tag and rest for data + 1 for \0
size_t SERIAL_LEN_SCIRCLE = 8 + sizeof(SCircle) + 1;    //8 for id tag and rest for data + 1 for \0
size_t SERIAL_LEN_SLINE = 8 + sizeof(SLine) + 1;    //8 for id tag and rest for data + 1 for \0
size_t SERIAL_LEN_SELLIPSE = 8 + sizeof(SEllipse) + 1;    //8 for id tag and rest for data + 1 for \0
size_t SERIAL_LEN_SBOX = 8 + sizeof(SBox) + 1;    //8 for id tag and rest for data + 1 for \0

/* *** PRIVATE FUNCTIONS *** */
SQLITE_SPHERE_TYPES getObjType(char *data);
SPoint * readSPoint(char *data);
SEuler * readSEuler(char *data);
SCircle * readSCircle(char *data);
SLine * readSLine(char *data);
SEllipse * readSEllipse(char *data);
SPoly * readSPoly(char *data);
SPath * readSPath(char *data);
SBox * readSBox(char *data);

char * serialise(SPoint * point) {
	SQLITE_SPHERE_SERIALISER_SERIALISE( point, SPoint, SERIAL_LEN_SPOINT, "pt", "tp" );
}

size_t getSerialisedLen(SPoint * point) {
	SQLITE_SPHERE_SERIALISER_GETSERIALISEDLEN( SERIAL_LEN_SPOINT );
}


char * serialise(SEuler * trans) {
	SQLITE_SPHERE_SERIALISER_SERIALISE( trans, SEuler, SERIAL_LEN_SEULER, "et", "te" );
}

size_t getSerialisedLen(SEuler * trans) {
	SQLITE_SPHERE_SERIALISER_GETSERIALISEDLEN( SERIAL_LEN_SEULER );
}


char * serialise(SCircle * circle) {
	SQLITE_SPHERE_SERIALISER_SERIALISE( circle, SCircle, SERIAL_LEN_SCIRCLE, "cl", "lc" );
}

size_t getSerialisedLen(SCircle * circle) {
	SQLITE_SPHERE_SERIALISER_GETSERIALISEDLEN( SERIAL_LEN_SCIRCLE );
}


char * serialise(SLine * line) {
	SQLITE_SPHERE_SERIALISER_SERIALISE( line, SLine, SERIAL_LEN_SLINE, "li", "il" );
}

size_t getSerialisedLen(SLine * line) {
	SQLITE_SPHERE_SERIALISER_GETSERIALISEDLEN( SERIAL_LEN_SLINE );
}


char * serialise(SEllipse * ellipse) {
	SQLITE_SPHERE_SERIALISER_SERIALISE( ellipse, SEllipse, SERIAL_LEN_SELLIPSE, "el", "le" );
}

size_t getSerialisedLen(SEllipse * ellipse) {
	SQLITE_SPHERE_SERIALISER_GETSERIALISEDLEN( SERIAL_LEN_SELLIPSE );
}


char * serialise(SPoly * poly) {
	size_t polytype_len = 8 + poly->size + 1;
	char * result = (char*) malloc(polytype_len);
	if(result == NULL) {
		fprintf(stderr, "Error MySQL Sphere: Out of memory\n");
		return NULL;
	}

	strncpy(result, "<pg>", 4);
	memcpy(result+4, (const char*)poly, poly->size);
	strncpy(result+4+poly->size, "<gp>\0", 5);

	if(BASE64ENCODE == true) {
		size_t base64Len = getSerialisedLen(poly);
		char * base64Result = (char*) malloc(base64Len);

		if(base64Result == NULL) {
			fprintf(stderr, "Error MySQL Sphere: Out of memory\n");
			return NULL;
		}

		if(base64_encode(result, polytype_len, base64Result) != 0) {
			fprintf(stderr, "Error MySQL Sphere: Could not convert to base64!\n");
			return NULL;
		}

		free(result);
		result = base64Result;
	}

	return result;

}

size_t getSerialisedLen(SPoly * poly) {
	size_t polytype_len = 8 + poly->size + 1;
	if(BASE64ENCODE == true) {
		return base64_needed_encoded_length(polytype_len);
	} else {
		return polytype_len;
	} 
}


char * serialise(SPath * path) {
	size_t pathtype_len = 8 + path->size + 1;
	char * result = (char*) malloc(pathtype_len);
	if(result == NULL) {
		fprintf(stderr, "Error MySQL Sphere: Out of memory\n");
		return NULL;
	}

	strncpy(result, "<pa>", 4);
	memcpy(result+4, (const char*)path, path->size);
	strncpy(result+4+path->size, "<ap>\0", 5);

	if(BASE64ENCODE == true) {
		size_t base64Len = getSerialisedLen(path);
		char * base64Result = (char*) malloc(base64Len);

		if(base64Result == NULL) {
			fprintf(stderr, "Error MySQL Sphere: Out of memory\n");
			return NULL;
		}

		if(base64_encode(result, pathtype_len, base64Result) != 0) {
			fprintf(stderr, "Error MySQL Sphere: Could not convert to base64!\n");
			return NULL;
		}

		free(result);
		result = base64Result;
	}

	return result;

}

size_t getSerialisedLen(SPath * path) {
	size_t pathtype_len = 8 + path->size + 1;
	if(BASE64ENCODE == true) {
		return base64_needed_encoded_length(pathtype_len);
	} else {
		return pathtype_len;
	} 
}

char * serialise(SBox * box) {
	SQLITE_SPHERE_SERIALISER_SERIALISE( box, SBox, SERIAL_LEN_SBOX, "bx", "xb" );
}

size_t getSerialisedLen(SBox * box) {
	SQLITE_SPHERE_SERIALISER_GETSERIALISEDLEN( SERIAL_LEN_SBOX );
}

SQLITE_SPHERE_TYPES decode(char * input, size_t inputLen, void ** output) {
	char *dataString = input;
	char * decodedString = NULL;
	size_t dataLen = inputLen;
	SQLITE_SPHERE_TYPES objType = SQLITE_SPHERE_UNKNOWN;

	//there is something wrong, if the string does not even at least have the size of
	//the data identifiers
	if(inputLen <= 8 || input == NULL) {
		return objType;
	}

	//check whether this is a base64 string or not. 
	//If this is a valid non-bad64 string, the first character needs to be a '<'
	//and the fourth a '>'
	if(input[0] != '<' && input[3] != '>') {
		//is this base64 or just plain wrong?
		//try decoding...
		size_t decodedLen = base64_needed_decoded_length(inputLen);
		decodedString = (char*) malloc(decodedLen);

		if(decodedString == NULL) {
			fprintf(stderr, "Error MySQL Sphere: Out of memory\n");
			*output = NULL;
			return objType;
		}

#if SQLITE_VERSION_ID >= 50601
		if(base64_decode(input, inputLen, decodedString, NULL, NULL) <= 0) {
#else
		if(base64_decode(input, inputLen, decodedString, NULL) <= 0) {
#endif
			fprintf(stderr, "Error MySQL Sphere: Could not decode the MySQL object provided!\n");
			free(decodedString);
			*output = NULL;
			return objType;
		}

		dataString = decodedString;
		dataLen = decodedLen;
	}

	//check if the decoded stuff is actually a valid MySQL object string
	objType = getObjType(dataString);
	if(objType == SQLITE_SPHERE_UNKNOWN) {
		if(decodedString != NULL) {
			free(decodedString);
		}
		*output = NULL;
		return objType;
	}

	//if we are here, we can start the decoding
	switch (objType) {
		case SQLITE_SPHERE_POINT:
			*output = (void*)readSPoint(dataString);
			break;
		case SQLITE_SPHERE_EULER:
			*output = (void*)readSEuler(dataString);
			break;
		case SQLITE_SPHERE_CIRCLE:
			*output = (void*)readSCircle(dataString);
			break;
		case SQLITE_SPHERE_LINE:
			*output = (void*)readSLine(dataString);
			break;
		case SQLITE_SPHERE_ELLIPSE:
			*output = (void*)readSEllipse(dataString);
			break;
		case SQLITE_SPHERE_POLYGON:
			*output = (void*)readSPoly(dataString);
			break;
		case SQLITE_SPHERE_PATH:
			*output = (void*)readSPath(dataString);
			break;
		case SQLITE_SPHERE_BOX:
			*output = (void*)readSBox(dataString);
			break;
	}

	if(decodedString != NULL) {
		free(decodedString);
	}
	return objType;
}

SQLITE_SPHERE_TYPES getObjType(char *data) {
	const char *pt = "<pt>";
	const char *endPt = "<tp>";
	const char *et = "<et>";
	const char *endEt = "<te>";
	const char *cl = "<cl>";
	const char *endCl = "<lc>";
	const char *li = "<li>";
	const char *endLi = "<il>";
	const char *el = "<el>";
	const char *endEl = "<le>";
	const char *pg = "<pg>";
	const char *endPg = "<gp>";
	const char *pa = "<pa>";
	const char *endPa = "<ap>";
	const char *bx = "<bx>";
	const char *endBx = "<xb>";

	size_t dataLen = strlen(data);

	//get datatype (reading the first 4 bytes from the string to compare)
	int openTag = *(int*) data;

	if(openTag == *(int *)pt) {
		int endTag = *(int*)(data + SERIAL_LEN_SPOINT - 5);
		if(endTag == *(int *)endPt) {
			return SQLITE_SPHERE_POINT;
		} else {
			return SQLITE_SPHERE_UNKNOWN;
		}
	} else if(openTag == *(int *)et) {
		int endTag = *(int*)(data + SERIAL_LEN_SEULER - 5);
		if(endTag == *(int *)endEt) {
			return SQLITE_SPHERE_EULER;
		} else {
			return SQLITE_SPHERE_UNKNOWN;
		}
	} else if(openTag == *(int *)cl) {
		int endTag = *(int*)(data + SERIAL_LEN_SCIRCLE - 5);
		if(endTag == *(int *)endCl) {
			return SQLITE_SPHERE_CIRCLE;
		} else {
			return SQLITE_SPHERE_UNKNOWN;
		}
	} else if(openTag == *(int *)li) {
		int endTag = *(int*)(data + SERIAL_LEN_SLINE - 5);
		if(endTag == *(int *)endLi) {
			return SQLITE_SPHERE_LINE;
		} else {
			return SQLITE_SPHERE_UNKNOWN;
		}
	} else if(openTag == *(int *)el) {
		int endTag = *(int*)(data + SERIAL_LEN_SELLIPSE - 5);
		if(endTag == *(int *)endEl) {
			return SQLITE_SPHERE_ELLIPSE;
		} else {
			return SQLITE_SPHERE_UNKNOWN;
		}
	} else if(openTag == *(int *)pg) {
		//the first long in data gives the size of the polygon object
		long size = *(long*)(data + 4);
		int endTag = *(int*)(data + 4 + size);

		if(endTag == *(int *)endPg) {
			return SQLITE_SPHERE_POLYGON;
		} else {
			return SQLITE_SPHERE_UNKNOWN;
		}
	} else if(openTag == *(int *)pa) {
		//the first long in data gives the size of the polygon object
		long size = *(long*)(data + 4);
		int endTag = *(int*)(data + 4 + size);

		if(endTag == *(int *)endPa) {
			return SQLITE_SPHERE_PATH;
		} else {
			return SQLITE_SPHERE_UNKNOWN;
		}
	} else if(openTag == *(int *)bx) {
		int endTag = *(int*)(data + SERIAL_LEN_SBOX - 5);
		if(endTag == *(int *)endBx) {
			return SQLITE_SPHERE_BOX;
		} else {
			return SQLITE_SPHERE_UNKNOWN;
		}
	} else {
		return SQLITE_SPHERE_UNKNOWN;
	}
}

SPoint * readSPoint(char *data) {
	SPoint * result = (SPoint*)malloc(sizeof(SPoint));

	result = (SPoint*)memcpy(result, data+4, sizeof(SPoint));

	return result;
}

SEuler * readSEuler(char *data) {
	SEuler * result = (SEuler*)malloc(sizeof(SEuler));

	result = (SEuler*)memcpy(result, data+4, sizeof(SEuler));

	return result;
}

SCircle * readSCircle(char *data) {
	SCircle * result = (SCircle*)malloc(sizeof(SCircle));

	result = (SCircle*)memcpy(result, data+4, sizeof(SCircle));

	return result;
}

SLine * readSLine(char *data) {
	SLine * result = (SLine*)malloc(sizeof(SLine));

	result = (SLine*)memcpy(result, data+4, sizeof(SLine));

	return result;
}

SEllipse * readSEllipse(char *data) {
	SEllipse * result = (SEllipse*)malloc(sizeof(SEllipse));

	result = (SEllipse*)memcpy(result, data+4, sizeof(SEllipse));

	return result;
}

SPoly * readSPoly(char *data) {
	long size = *(long *)(data + 4);

	SPoly * result = (SPoly*)malloc(size);

	result = (SPoly*)memcpy(result, data+4, size);

	return result;
}

SPath * readSPath(char *data) {
	long size = *(long *)(data + 4);

	SPath * result = (SPath*)malloc(size);

	result = (SPath*)memcpy(result, data+4, size);

	return result;
}

SBox * readSBox(char *data) {
	SBox * result = (SBox*)malloc(sizeof(SBox));

	result = (SBox*)memcpy(result, data+4, sizeof(SBox));

	return result;
}

