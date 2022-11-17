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
		GraphDrawerODM gdrawer;			// drawer using gnuplot
		gdrawer.initGnuplot( );			// initilize gnuplot
#else
#endif		
		printf( "\nStart odm-test\n" );

		inpData_odm odm_gl;
//		odm_gl.initialize( "../data/log_motor-1.dat", "r" );
		odm_gl.initialize( "../data/log_motor.dat", "r" );
		double time;
		double angvel, vel, *pos;


		while( !gShutOff ){

			if( !odm_gl.readNew_odm( ) ) break;
			time = odm_gl.getTime( );

			angvel = odm_gl.getAngvel( );
			vel = odm_gl.getVel( );
			pos = odm_gl.getPos( ); 
#ifdef DRAW
			gdrawer.setODM( pos, time );
			if( counter == 0 ){
				gdrawer.drawGraph( );
			}
			counter++;
			if( counter == 30 ) counter = 0;
#else
#endif			
			printf( "%f %f %f %f %f %f\n", time, pos[0], pos[1], pos[2], vel, angvel );
			
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
