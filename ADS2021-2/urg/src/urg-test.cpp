#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <signal.h>
#include <stdexcept>
#include <getopt.h>
#include <math.h>
#include <unistd.h>
#include "inpData_urg.hpp"

#define DRAW

#ifdef DRAW
#include "GraphDrawerURG.hpp"
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
		GraphDrawerURG gdrawer;			// gnuplotによる描画
		gdrawer.initGnuplot( );				// gnuplot初期化
#else
#endif		
		printf( "\nStart urg-test\n" );

		inpData_urg urg_fs;
		urg_fs.initialize( "../data/log_urg.dat", "r" );
		unsigned size;
		double *x, *y;


		while( !gShutOff ){

			if( !urg_fs.readNew_urg( ) ) break;
			size = urg_fs.getSize( );
			x = urg_fs.getX( );
			y = urg_fs.getY( );
			
			// Programming on your own.
			
#ifdef DRAW
			gdrawer.setURG( x, y, size );
			if( counter == 0 ){
				gdrawer.drawGraph( );
			}
			counter++;
			if( counter == 5 ) counter = 0;
#else
#endif			
			printf( "%d\n", size );
			
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
