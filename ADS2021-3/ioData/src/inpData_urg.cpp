/*
 * Date : 2020.09.03
 * Author : T.Hasegawa
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include "ioData_base.hpp"
#include "inpData_urg.hpp"

static char* getWord( FILE *fp );
//static bool flag_first_loop = true;
//static double time_start;
bool inpData_urg::readNew_urg( void )
{
	char *buf;
	buf = getWord( fp_base );
	if( buf == NULL ) return false;
	time = atof( buf );
//	time = atof( getWord( fp_base ) );

	size = atoi( getWord( fp_base ) );
	for( int i = 0 ; i < size ; i++ ){
		x[ i ] = atof( getWord( fp_base ) ) / 1000.0;
		y[ i ] = atof( getWord( fp_base ) ) / 1000.0;
		intensity[ i ] = atoi( getWord( fp_base ) );
	}	
	return true;
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
