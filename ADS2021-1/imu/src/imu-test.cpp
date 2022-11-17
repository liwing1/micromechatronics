#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdexcept>
#include <getopt.h>
#include <math.h>
#include <unistd.h>
#include "inpData_imu.hpp"

#define DRAW

#ifdef DRAW
#include "GraphDrawerIMU.hpp"
static unsigned long counter = 0;
#else
#endif

using namespace std;

static int gShutOff = 0;
static void setSigInt( void );
static unsigned int dT = 1;

int main( int aArgc, char *aArgv[ ] )
{

	try {
		setSigInt( );
#ifdef DRAW
		GraphDrawerIMU gdrawer;			// gnuplotによる描画
		gdrawer.initGnuplot( );				// gnuplot初期化
#else
#endif				
		printf( "\nStart imu-test\n" );

		inpData_imu imu_fs;
		imu_fs.initialize( "../data/log_imu.dat", "r" );
		double time;
		double *angvel;
		double *accel;

		while( !gShutOff ){

			if( !imu_fs.readNew_imu( ) ) break;
			time = imu_fs.getTime( );

			angvel = imu_fs.getAngvel( );
			accel = imu_fs.getAccel( );

#ifdef DRAW
//			gdrawer.setIMU( accel, time );
			gdrawer.setIMU( angvel, time );
			if( counter == 0 ){
				gdrawer.drawGraph( );
			}
			counter++;
			if( counter == 30 ) counter = 0;
#else
#endif
			printf( "%f %f %f %f %f %f %f\n", time, angvel[0], angvel[1], angvel[2], accel[0], accel[1], accel[2] );

			usleep( dT * 1000 );
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
