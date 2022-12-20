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
#include "inpData_imu.hpp"
#include <math.h>

static char* getWord( FILE *fp );
static bool flag_first_loop = true;
static double time_start;
bool inpData_imu::readNew_imu( void )
{
	char *buf;
	buf = getWord( fp_base );
	if( buf == NULL ) return false;
	time = atof( buf );
//	time = atof( getWord( fp_base ) );
/*	if( flag_first_loop ){
		time_start = time;
		time = 0;
		flag_first_loop = false;
	} else {
		double t = time - time_start;
		time = t;
	}
*/
	angvel[ _ROLL ] = atof( getWord( fp_base ) );
	angvel[ _PITCH ] = atof( getWord( fp_base ) );
	angvel[ _YAW ] = atof( getWord( fp_base ) );
	accel[ _X ] = atof( getWord( fp_base ) );
	accel[ _Y ] = atof( getWord( fp_base ) );
	accel[ _Z ] = atof( getWord( fp_base ) );

	for( int i = 0 ; i < 4 ; i++ ){
		double a = atof( getWord( fp_base ) );
	}

	// calcIMU();
	
	return true;
}

int inpData_imu::calcIMU( void )
{
	const double K = 0.999;
	static double old_time = 0;
	static double theta_gyro[3] = {0};
	double theta_accel[3] = {0};
	double g_angvel[3] = {0};
	static bool first_time = true;

	if(first_time){
		first_time = false;
		old_time = time;
	} else {
		theta_accel[_ROLL] = atan(accel[_Y]/accel[_Z]);
		theta_accel[_PITCH] = atan(accel[_Y]/sqrt(pow(accel[_X], 2) + pow(accel[_Z], 2)));

		g_angvel[_ROLL] = 	angvel[_X] + 
							angvel[_Y] * sin(theta_gyro[_ROLL]) * tan(theta_gyro[_PITCH]) +
							angvel[_Z] * cos(theta_gyro[_ROLL]) * tan(theta_gyro[_PITCH]) ;
		g_angvel[_PITCH] =	angvel[_Y] * cos(theta_gyro[_ROLL]) -
							angvel[_Z] * sin(theta_gyro[_ROLL]);
		g_angvel[_YAW] = 	angvel[_Y] * (sin(theta_gyro[_ROLL])/cos(theta_gyro[_PITCH])) +
							angvel[_Z] * (cos(theta_gyro[_ROLL])/cos(theta_gyro[_PITCH]));

		double dt = time - old_time;
		old_time = time;

		for(uint8_t i = 0; i < 3; i++){
			theta_gyro[i] += g_angvel[i] * dt;
		}

		for(uint8_t i = 0; i < 3; i++){
			theta[i] = theta_accel[i] + K*(theta_gyro[i] - theta_accel[i]);
		}
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
