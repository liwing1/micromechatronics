/*
 * Date : 2020.09.03
 * Author : T.Hasegawa
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include <math.h>
#include "ioData_base.hpp"
#include "inpData_odm.hpp"

#define _R		0
#define _L		1

static char* getWord( FILE *fp );
bool inpData_odm::readNew_odm( void )
{
	char *buf;
	buf = getWord( fp_base );
	if( buf == NULL ) return false;
	time = atof( buf );
//	time = atof( getWord( fp_base ) );
// for Beego
//	counter[ _R ] = -1.0*atof( getWord( fp_base ) );//-1.0 * atof( getWord( fp_base ) );
//	counter[ _L ] = atof( getWord( fp_base ) );//atof( getWord( fp_base ) );
// for i-Cart
	counter[ _R ] = atof( getWord( fp_base ) );
	counter[ _L ] = -1.0 * atof( getWord( fp_base ) );
	// for Skip
	double tmp = atof( getWord( fp_base ) );
	tmp = atof( getWord( fp_base ) );
	// Estimate a position
	calcOdometory( );
	
	return true;
}
// for i_Cart
#define REV	( 2*M_PI/400.0 )//( 2*M_PI/2048.0 )	// [ rad/cnt ]
#define GEAR	150	//54.0699
#define RADIUS	0.14457758	//0.0406 // [m]
#define TREAD	-0.3045	//0.2765 // [m]
// for Beego
//#define REV	( 2*M_PI/2048.0 )	// [ rad/cnt ]
//#define GEAR	54.0699
//#define RADIUS	0.0406 // [m]
//#define TREAD	0.2765 // [m]
static bool flag_first_loop = true;
static double time_old;
int inpData_odm::calcOdometory( void )
{
	// Programming on your own.
	if( flag_first_loop ){
		time_old = time;
		pos[ _X ] = 0;
		pos[ _Y ] = 0;
		pos[ _YAW ] = 0;
		flag_first_loop = false;
	} else {
		double dt = time - time_old;
		time_old = time;
		double dcnt_r = counter[ _R ] / GEAR;
		double dcnt_l = counter[ _L ] / GEAR;
		double Wr = ( dcnt_r * REV ) / dt;
		double Wl = ( dcnt_l * REV ) / dt;
		double Vr = RADIUS * Wr;
		double Vl = RADIUS * Wl;
		vel = ( Vr + Vl ) / 2.0;
		angvel = ( Vr - Vl ) / TREAD;
		pos[ _X ] += ( cos( pos[ _YAW ] ) * vel * dt );
		pos[ _Y ] += ( sin( pos[ _YAW ] ) * vel * dt );
		pos[ _YAW ] += ( angvel * dt );
//		printf( "%f %f %f\n", dt, dcnt_x, dcnt_y );
//		printf( "%f %f %f\n", time, vel, angvel );
	}
		
	return EXIT_SUCCESS;
}

//*******************************
// ファイルからデータをchar型で返す関数
// 引数 FILEポインタ
// 戻値 char型のデータ
//*******************************
#define MaxLine 30000
#define CmtChar '#'
static char linebuf[MaxLine] = {'\0'};
static char *cur_ptr = linebuf;
static int skipComment( FILE* fp, char* buf, int size )
{
  do {
    if( !fgets( buf, size, fp ) ) return EOF;
  } while( buf[ 0 ] == CmtChar );
  return EXIT_SUCCESS ;
}
static char* getWord( FILE *fp )
{
  char *ptr;
  while( isspace( *cur_ptr ) || *cur_ptr == '\0' ){
    if( *cur_ptr == '\0' ){
      if( skipComment( fp, linebuf, MaxLine ) == EOF ) return NULL;
      cur_ptr = linebuf;
    } else cur_ptr++;
  }
  ptr = cur_ptr;
  while( !isspace( *cur_ptr ) && *cur_ptr != '\0' ){
    if( *cur_ptr == '\0' ){
      if( skipComment( fp, linebuf, MaxLine ) == EOF) break;
      cur_ptr = linebuf;
    } else cur_ptr++;
  }
  if( *cur_ptr != '\0' ) *cur_ptr++ = '\0';
  return ptr;
}
