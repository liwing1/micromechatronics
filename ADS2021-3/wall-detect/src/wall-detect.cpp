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
#include <fstream>

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

class detectWall {
private:
	double angle, distance;
	
	int size;
	double tx[ DATA_MAX ], ty[ DATA_MAX ];
	
public:
	detectWall( void ) { }
	~detectWall( void ) { }
	
	double getAngle( void ) { return angle; }
	double getDistance( void ) { return distance; }
	void setPCD( double *x, double *y, int s )
	{
		size = s;
		for( int i = 0 ; i < size ; i++ ){
			tx[ i ] = x[ i ];
			ty[ i ] = y[ i ];
		}
	}
	void detect( void )
	{
		// Calculate Least-square method
		double A = 0, B = 0, xd = 0, yd = 0;
		double Ex = 0, Ey = 0, Exy = 0, Exx = 0;

		for(unsigned int i = 0; i < size; i++)
		{
			Ex += tx[i];
			Ey += ty[i];
			Exy += tx[i] * ty[i];
			Exx += tx[i] * tx[i];
		}

		A = ((size * Exy)-(Ex * Ey))/((size * Exx)-(Ex * Ex));
		B = ((Exx * Ey)-(Exy * Ex))/((size * Exx)-(Ex * Ex));
		// y = A x + BS

		xd = (-A * B)/(pow(A, 2) + 1);
		yd = (B)/(pow(A, 2) + 1);

		distance = sqrt( xd*xd + yd*yd );
		angle = atan( A );
	}
};

int main( int aArgc, char *aArgv[ ] )
{
	ofstream MyFile("data.csv");

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
		detectWall dw_fs;
		unsigned size;
		double *x, *y;
		double time, angle, distance;

		while( !gShutOff ){

			if( !urg_fs.readNew_urg( ) ) break;
			time = urg_fs.getTime( );
			size = urg_fs.getSize( );
			x = urg_fs.getX( );
			y = urg_fs.getY( );
			
			dw_fs.setPCD( x, y, size );
			dw_fs.detect( );
			angle = dw_fs.getAngle( );
			distance = dw_fs.getDistance( );
			
#ifdef DRAW
			gdrawer.setURG( x, y, size );
			if( counter == 0 ){
				gdrawer.drawGraph( );
			}
			counter++;
			if( counter == 10 ) counter = 0;
#else
#endif			
//			printf( "%d\n", size );
			printf("size: %u\n", size);
			char buf[100] = {0};
			sprintf( buf, "%f %f %f\n", time, angle, distance );
			MyFile << buf;
			
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
