/*****************************************************************
 * *******                  UDF_SPHERE                     *******
 *****************************************************************
 * output routines that cast objects to strings
 * 
 *****************************************************************
 */

#include "output.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "util.h"

/*!
	\brief Converts radians to DEG ( degrees, minutes, seconds )
	\param rad input in radians
	\param deg pointer to degrees
	\param min pointer to minutes
	\param sec pointer to seconds
	\return nothing
*/
static void rad_to_dms ( double rad, unsigned int * deg, unsigned int * min, double * sec ) {
	char rsign = ( rad < 0 ) ? ( -1 ) : ( 1 );
	const double sec_dig = pow ( 10, FLT_DIG );

	rad *= ( rsign * RADIANS );

	*deg = floor ( rad );
	*min = floor ( 60 * ( rad - *deg ) );
	*sec = rad * 3600 - *min * 60 - *deg * 3600 ;
	*sec = rint ( ( double ) ( *sec * sec_dig ) ) / sec_dig;

	if ( *sec >= 60.0 ) {
		*sec -= 60.0 ;
		(*min)++;
	}
	if ( *min >= 60 ) {
		*min -= 60 ;
		(*deg)++;
	}
}

char * spherepoint_out(SPoint * spoint, unsigned long outType, unsigned int outPrec) {
	SPoint * sp = spoint;
	char * buffer = ( char * ) malloc( 255 ) ;
	unsigned int latdeg , latmin, lngdeg, lngmin;
	double latsec, lngsec;


	latdeg = latmin = lngdeg = lngmin = 0;
	latsec = lngsec = 0.0;

	switch ( outType ) {
		case OUTPUT_DEG : 
			sprintf ( buffer , "(%.*gd , %.*gd)",
						outPrec, RADIANS * sp->lng, 
						outPrec, RADIANS * sp->lat );
			break;
		case OUTPUT_DMS : 
			rad_to_dms( sp->lng, &lngdeg, &lngmin, &lngsec );
			rad_to_dms( sp->lat, &latdeg, &latmin, &latsec );
			sprintf (buffer , "(%3ud %2um %.*gs , %c%2ud %2um %.*gs)",
						lngdeg, lngmin, outPrec, lngsec,
						( sp->lat < 0 ) ? ('-') : ('+'),
						latdeg, latmin, outPrec, latsec);
			break;
		case OUTPUT_HMS : 
			rad_to_dms( sp->lng / 15, &lngdeg, &lngmin, &lngsec );
			rad_to_dms( sp->lat, &latdeg, &latmin, &latsec );
			sprintf ( buffer, "(%3uh %2um %.*gs , %c%2ud %2um %.*gs)",
						lngdeg, lngmin, outPrec, lngsec,
						( sp->lat < 0 ) ? ('-') : ('+'),
						latdeg, latmin, outPrec, latsec);
			break;
		default : 
			sprintf ( buffer , "(%.*g , %.*g)", 
						outPrec, sp->lng, outPrec, sp->lat );
			break;
	}

	return buffer;
}

char * spherecircle_out(SCircle * scircle, unsigned long outType, unsigned int outPrec) {
	SCircle * c = scircle;
	char * buffer = ( char * ) malloc (255);
	char * pointstr = spherepoint_out(&c->center, outType, outPrec);

	unsigned int rdeg, rmin;
	double rsec;

	rdeg = rmin = 0;
	rsec = 0.0;

	switch ( outType ) {
		case OUTPUT_DEG : 
			sprintf ( buffer , "<%s , %.*gd>", 
						pointstr, outPrec, RADIANS * c->radius );
			break;
		case OUTPUT_HMS :
		case OUTPUT_DMS : 
			rad_to_dms ( c->radius, &rdeg, &rmin, &rsec );
			sprintf ( buffer, "<%s , %2ud %2um %.*gs>",
						pointstr, rdeg, rmin, outPrec, rsec);
			break;
		default : 
			sprintf ( buffer , "<%s , %.*g>", pointstr, outPrec, c->radius );
			break;
	}

	free( pointstr );
	return buffer;
}

char * sphereellipse_out(SEllipse * sellipse, unsigned long outType, unsigned int outPrec) {
	SEllipse * e = sellipse;
	char * buffer = ( char * ) malloc (255);
	char * pointstr;
	unsigned int rdeg[3] , rmin[3];
	double rsec[3];
	SPoint sp;

	sp.lng = e->psi;
	sp.lat = - e->theta;
	pointstr = spherepoint_out(&sp, outType, outPrec);

	switch ( outType ) {
		case OUTPUT_DEG : 
			sprintf ( buffer, "<{ %.*gd , %.*gd }, %s , %.*gd>",
						outPrec, RADIANS * (e->rad[0]), 
						outPrec, RADIANS * (e->rad[1]),
						pointstr, outPrec, RADIANS * (e->phi) );
			break;
		case OUTPUT_HMS :
		case OUTPUT_DMS : 
			rad_to_dms ( e->rad[0], &rdeg[0], &rmin[0], &rsec[0] );
			rad_to_dms ( e->rad[1], &rdeg[1], &rmin[1], &rsec[1] );
			rad_to_dms ( e->phi, &rdeg[2], &rmin[2], &rsec[2] );
			sprintf ( buffer, "<{ %2ud %2um %.*gs , %2ud %2um %.*gs }, %s , %2ud %2um %.*gs>",
						rdeg[0], rmin[0], outPrec, rsec[0],
						rdeg[1], rmin[1], outPrec, rsec[1],
						pointstr, rdeg[2], rmin[2], outPrec, rsec[2] );
			break;
		default : 
			sprintf ( buffer, "<{ %.*g , %.*g }, %s , %.*g>",
						outPrec, e->rad[0], outPrec, e->rad[1],
						pointstr, outPrec, e->phi );
			break;
	}

	free ( pointstr );
	return ( buffer );
}

char * sphereline_out(SLine * sline, unsigned long outType, unsigned int outPrec) {
	SLine * sl = sline;
	char * out = ( char * ) malloc (255);
	char * tstr =  NULL;
	SEuler se;

	unsigned int rdeg, rmin;
	double rsec;

	rdeg = rmin = 0;
	rsec = 0.0;

	seuler_set_zxz ( &se );
	se.phi = sl->phi;
	se.theta = sl->theta;
	se.psi = sl->psi;
	tstr = spheretrans_out( &se, outType, outPrec );

	switch ( outType ) {
		case OUTPUT_DEG : 
			sprintf ( out , "( %s ), %.*gd", 
						tstr, outPrec, RADIANS * sl->length );
			break;
		case OUTPUT_HMS :
		case OUTPUT_DMS : 
			rad_to_dms ( sl->length, &rdeg, &rmin, &rsec );
			sprintf ( out , "( %s ), %2ud %2um %.*gs",
						tstr, rdeg, rmin, outPrec, rsec);
			break;
		default : 
			sprintf ( out , "( %s ), %.*g", 
						tstr, outPrec, sl->length );
			break;
	}

	free(tstr);
	return ( out );
}

char * spheretrans_out(SEuler * strans, unsigned long outType, unsigned int outPrec) {
	SEuler * se = strans;
	char * buffer = ( char * ) malloc (255);
	char buf[100];
	char etype[4];
	SPoint val[3];
	unsigned char i, t = 0;
	unsigned int rdeg , rmin;
	double rsec;

	val[0].lat = val[1].lat = val[2].lat = 0.0;
	val[0].lng = se->phi;
	val[1].lng = se->theta;
	val[2].lng = se->psi;

	spoint_check ( &val[0] );
	spoint_check ( &val[1] );
	spoint_check ( &val[2] );

	buffer[0] ='\0';
	for ( i=0; i<3; i++) {
		rdeg = rmin = 0;
		rsec = 0.0;
		switch ( outType ) {
			case OUTPUT_DEG : 
				sprintf ( &buf[0] , "%.*gd", 
							outPrec, RADIANS * val[i].lng );
				break;
			case OUTPUT_HMS :
			case OUTPUT_DMS : 
				rad_to_dms ( val[i].lng , &rdeg, &rmin, &rsec );
				sprintf ( &buf[0], "%2ud %2um %.*gs",
							rdeg, rmin, outPrec, rsec);
				break;
			default : 
				sprintf ( &buf[0] , "%.*g", 
							outPrec, val[i].lng );
				break;
		}
		strcat( &buf[0], ", ");
		strcat ( buffer, &buf[0] );
	}

	for ( i=0; i<3; i++ ) {
		switch ( i ) {
			case 0: 
				t = se->phi_a; 
				break;
			case 1: 
				t = se->theta_a; 
				break;
			case 2: 
				t = se->psi_a; 
				break;
		}

		switch ( t ) {
			case EULER_AXIS_X : 
				etype[i]='X'; 
				break;
			case EULER_AXIS_Y : 
				etype[i]='Y'; 
				break;
			case EULER_AXIS_Z : 
				etype[i]='Z'; 
				break;
		}
	}

	etype[3] = '\0';
	strcat( buffer, etype );

	return ( buffer );
}

char * spherepath_out(SPath * spath, unsigned long outType, unsigned int outPrec) {
	SPath * path = spath;
	long i;
	char * out = ( char * ) malloc ( 128 * path->npts );
	char * tmp;

	strcpy(out,"{");
	for ( i = 0; i<path->npts; i++ ) {
		if ( i > 0 ) {
			strcat(out, ",");
		}
		tmp = spherepoint_out( &path->p[i], outType, outPrec );
		strcat(out, tmp);
		free ( tmp );
	}
	strcat(out, "}");
	return ( out );
}

char * spherepoly_out(SPoly * spoly, unsigned long outType, unsigned int outPrec) {
	SPoly * poly = spoly;
	long i;
	char * out = ( char * ) malloc ( 128 * poly->npts );
	char * tmp;

	strcpy(out, "{");
	for ( i = 0; i<poly->npts; i++ ) {
		if ( i > 0 ) {
			strcat(out, ",");
		}
		tmp = spherepoint_out( &poly->p[i], outType, outPrec );
		strcat(out, tmp);
		free ( tmp );
	}
	strcat(out, "}");
	return ( out );
}


char * spherebox_out(SBox * sbox, unsigned long outType, unsigned int outPrec) {
	SBox * box = sbox;
	char * buffer = ( char * ) malloc(255);
	char * str1 = spherepoint_out( &box->sw, outType, outPrec );
	char * str2 = spherepoint_out( &box->ne, outType, outPrec );

	sprintf ( buffer, "(%s, %s)", str1, str2 );
	free ( str1 );
	free ( str2 );
	return ( buffer );
}

char * sqlite_sphere_version() {
	char * buffer = ( char * ) malloc (255);
	sprintf ( buffer, "sqlite_sphere 0.1.0 - pgSphere 1.2.0");
	return ( buffer );
}
