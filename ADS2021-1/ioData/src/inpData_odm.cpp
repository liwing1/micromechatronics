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

//#define BEEGO		1

static char* getWord( FILE *fp );
bool inpData_odm::readNew_odm( void )
{
	char *buf;
	buf = getWord( fp_base );
	if( buf == NULL ) return false;
	time = atof( buf );
//	time = atof( getWord( fp_base ) );
#ifdef BEEGO
// for Beego
	counter[ _R ] = -1.0*atof( getWord( fp_base ) );//-1.0 * atof( getWord( fp_base ) );
	counter[ _L ] = atof( getWord( fp_base ) );//atof( getWord( fp_base ) );
#else
// for i-Cart
	counter[ _R ] = atof( getWord( fp_base ) );
	counter[ _L ] = -1.0 * atof( getWord( fp_base ) );
#endif
	// for Skip
	double tmp = atof( getWord( fp_base ) );
	tmp = atof( getWord( fp_base ) );
	// Estimate a position
	calcOdometory( );
	
	return true;
}
int inpData_odm::calcOdometory( void )
{
	// Programming on your own.
		
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
