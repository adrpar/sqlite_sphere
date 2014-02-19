/*****************************************************************
 * *******                  UDF_SPHERE                     *******
 *****************************************************************
 * output routines that cast objects to strings
 * 
 *****************************************************************
 */

#include "point.h"
#include "circle.h"
#include "line.h"
#include "euler.h"
#include "polygon.h"
#include "path.h"
#include "ellipse.h"
#include "box.h"

#ifndef __SQLITE_SPHERE_OUTPUT_H__
#define __SQLITE_SPHERE_OUTPUT_H__

#define OUTPUT_RAD  0    //!< output in radians
#define OUTPUT_DEG  1    //!< output in degrees
#define OUTPUT_DMS  2    //!< output in degrees, minutes, seconds
#define OUTPUT_HMS  3    //!< output in hour, minutes, seconds

/*!
	\brief the output function of spherical point
	\return cstring
*/
char * spherepoint_out(SPoint * spoint, unsigned long outType, unsigned int outPrec);

/*!
	\brief the output function of spherical circle
	\return cstring
*/
char * spherecircle_out(SCircle * scircle, unsigned long outType, unsigned int outPrec);

/*!
	\brief the output function of spherical ellipse
	\return cstring
*/
char * sphereellipse_out(SEllipse * sellipse, unsigned long outType, unsigned int outPrec);

/*!
	\brief the output function of spherical line
	\return cstring
*/
char * sphereline_out(SLine * sline, unsigned long outType, unsigned int outPrec);

/*!
	\brief the output function of Euler transformation
	\return cstring
*/
char * spheretrans_out(SEuler * strans, unsigned long outType, unsigned int outPrec);

/*!
	\brief the output function of spherical path
	\return cstring
*/
char * spherepath_out(SPath * spath, unsigned long outType, unsigned int outPrec);

/*!
	\brief the output function of spherical polygon
	\return cstring
*/
char * spherepoly_out(SPoly * spoly, unsigned long outType, unsigned int outPrec);

/*!
	\brief the output function of spherical box
	\return cstring
*/
char * spherebox_out(SBox * sbox, unsigned long outType, unsigned int outPrec);

/*!
	\brief output of version
	\return cstring
*/
char * sqlite_sphere_version();
#endif
