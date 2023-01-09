/*
 * Date : 2020.09.03
 * Author : T.Hasegawa
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "ioData_base.hpp"
#include "ioMap.hpp"

using namespace std;
static int null = UNKNOWN;

static char* getWord( FILE *fp );
bool ioMap::readMap( void )
{
	char *buf;
	buf = getWord( fp_base );
	if( buf == NULL ) return false;
	width = atof( buf );
	length = atof( getWord( fp_base ) );
	resolution = atof( getWord( fp_base ) );

	row = atoi( getWord( fp_base ) );
	col = atoi( getWord( fp_base ) );
// Allocation
	if( ( map = new int*[ row ] ) == NULL ){
		std::cerr << "Cannot allocate Map area." << endl;
		exit( EXIT_FAILURE );
	}
	for( int i = 0 ; i < row ; i++ ){
		if( ( map[ i ] = new int[ col ] ) == NULL ){
			std::cerr << "Cannot allocate Map area." << endl;
			exit( EXIT_FAILURE );
		}
	}
// Initialize
	for( int i = 0 ; i < row ; i++ ){
		for( int j = 0 ; j < col ; j++ ){
			map[ i ][ j ] = null;
		}
	}
// start point
	start_pos[ 0 ] = atof( getWord( fp_base ) );
	start_pos[ 1 ] = atof( getWord( fp_base ) );
	start_grid[ 0 ] = atoi( getWord( fp_base ) );
	start_grid[ 1 ] = atoi( getWord( fp_base ) );
// Skip data
	for( int i = 0 ; i < 6 ; i++ ){
		buf = getWord( fp_base );
	}
// Read map data
	for( int i = 0 ; i < row ; i++ ){
		for( int j = 0 ; j < col ; j++ ){
			map[ i ][ j ] = atoi( getWord( fp_base ) );
		}
	}
	return true;
}

unsigned int ioMap::getStatus( double x, double y )
{
	unsigned int status;
	unsigned int tr, tc;
	tr = X2Row( x );
	tc = Y2Col( y );

	if( tr <= 0 || tc <= 0 ) return 0;
	if( tr > row || tc > col ) return 0;

	int n = map[ tr ][ tc ];
	if( n == OBSTACLE ) status = 1;
	else status = 0;
//	printf( "%d ", status );
	return status;	
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
