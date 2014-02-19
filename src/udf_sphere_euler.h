#include <sqlite3.h>

#ifndef __SQLITE_UDF_SPHERE_EULER_H__
#define __SQLITE_UDF_SPHERE_EULER_H__

void stransFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_pointFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_point_inverseFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_equalFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_equal_negFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_phiFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_thetaFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_psiFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_axesFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_invertFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_zxzFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_transFunc(sqlite3_context *context, int argc, sqlite3_value **argv);
void strans_trans_invFunc(sqlite3_context *context, int argc, sqlite3_value **argv);

#endif