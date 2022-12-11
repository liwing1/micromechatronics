#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <signal.h>
#include <stdexcept>
#include <getopt.h>
#include <math.h>
#include <unistd.h>
#include "inpData_odm.hpp"
#include "inpData_imu.hpp"

#define DRAW

#ifdef DRAW
#include "GraphDrawerODM.hpp"
static unsigned long counter = 0;
#else
#endif

using namespace std;

static int gShutOff = 0;
static void setSigInt( void );
static unsigned int dT = 10;

int main( int aArgc, char *aArgv[ ] )
{

	try {
		setSigInt( );

#ifdef DRAW		
		GraphDrawerODM gdrawer;			// gnuplotによる描画
		gdrawer.initGnuplot( );				// gnuplot初期化
#else
#endif		
		printf( "\nStart gyro-odm-test\n" );

		inpData_odm odm_gl;
		odm_gl.initialize( "../data/log_motor.dat", "r" );
		inpData_imu imu_fs;
		imu_fs.initialize( "../data/log_imu.dat", "r" );
		double time_odm = 0, time_imu = 0, dt;
		double angvel = 0, vel = 0, pos[ 3 ] = { 0.0 };
		unsigned long cnt = 0;
		
		bool flag_update = false;
		while( !gShutOff ){

			if( !odm_gl.readNew_odm( ) ) break;
			time_odm = odm_gl.getTime( );
//			pos = odm_gl.getPos( ); 
			cnt++;
			double dt = time_imu - time_odm;
			if( fabs( dt ) < 0.005 ){
				flag_update = true;
			} else if( 0 < dt ){
				flag_update = false;
			} else if( dt < 0 ){
				if( !imu_fs.readNew_imu( ) ) break;
				time_imu = imu_fs.getTime( );
				flag_update = false;
			}

// User program when synchronize
			if( flag_update ){
				flag_update = false;

		// Programming on your own.		
				
				printf( "time_odm=%f, time_imu=%f, dt=%f, cnt=%ld\n", time_odm, time_imu, dt, cnt );
				cnt = 0;
			}
#ifdef DRAW
			gdrawer.setODM( pos, time_odm );
			if( counter == 0 ){
				gdrawer.drawGraph( );
			}
			counter++;
			if( counter == 30 ) counter = 0;
#else
#endif		
//			printf( "%f %f %f %f %f %f\n", time_odm, pos[0], pos[1], pos[2], vel, angvel );
		}
	}
	catch ( std::runtime_error const & error ){
		std::cout << error.what() << std::endl;
	}
	catch ( ... ){
		std::cout << "An unknown fatal error has occured. Aborting." << std::endl;
	}

	return EXIT_SUCCESS;
}
static void ctrlC( int aStatus )
{
	signal( SIGINT, NULL );
	gShutOff = 1;
}
static void setSigInt(  )
{
	struct sigaction sig;
	memset( &sig, 0, sizeof ( sig ) );
	sig.sa_handler = ctrlC;
	sigaction( SIGINT, &sig, NULL );
}
