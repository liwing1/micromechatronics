#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <signal.h>
#include <stdexcept>
#include <getopt.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <ctime>        // time
#include <cstdlib>      // srand,rand
#include "inpData_odm.hpp"
#include "inpData_urg.hpp"
#include "ioMap.hpp"

#define DRAW

#ifdef DRAW
#include "GraphDrawerPF.hpp"
static unsigned long counter = 0;
#else
#endif

using namespace std;

static int gShutOff = 0;
static void setSigInt( void );
static unsigned int dT = 10;

#define PARTICLE_SIZE	1000

class ParticleFilter {
private:
	double pos[ 3 ];	// x, y, theta
	ioMap map;
	double particle[ PARTICLE_SIZE ][ 3 ], particle_copy[ PARTICLE_SIZE ][ 3 ];
	double copy_x[ PARTICLE_SIZE ], copy_y[ PARTICLE_SIZE ], copy_th[ PARTICLE_SIZE ]; 
	double weight[ PARTICLE_SIZE ];
	// PCD variables
	int urg_size;
	double urg_tx[ URG_DATA_MAX ], urg_ty[ URG_DATA_MAX ];
	// Odm variables
	double vt, wt, time, time_old, dt;
	bool flag_first_loop;
	
	void readMap( const char *filename, const char* mode )
	{
		map.initialize( filename, mode );
		map.readMap( );
	}
	
public:
	ParticleFilter( void ) { }
	~ParticleFilter( void ) { }
	
	void initialize( const char *filename, const char* mode, double x, double y, double th )
	{
		pos[ 0 ] = x; pos[ 1 ] = y; pos[ 2 ] = th;
		flag_first_loop = true;
		time = time_old = dt = 0.0;
		readMap( filename, mode );
		// Initialize particles
		initialParticles( );
	}
	void initialParticles( void )
	{
	// Programming on your own
	}
	void prediction( void )
	{
// Programming on your own
	}
	void likelihood( void )
	{
// Programming on your own
	}
	void update( void )
	{
// Programming on your own
	}
	void resampling( void )
	{
// Programming on your own
	}
	void setPCD( double *x, double *y, int s )
	{
		urg_size = s;
		for( int i = 0 ; i < urg_size ; i++ ){
			urg_tx[ i ] = x[ i ];
			urg_ty[ i ] = y[ i ];
		}
	}
	bool setOdm( double v, double w, double t )
	{
		if( flag_first_loop ){
			time_old = t;
			flag_first_loop = false;
			return false;
		} else {
			vt = v;
			wt = w;
			time = t;
			dt = time - time_old;
			time_old = t;
		}
		return true;
	}
	void setInitalPos( double x, double y, double th )
	{
		pos[ 0 ] = x;
		pos[ 1 ] = y;
		pos[ 2 ] = th;
	}
	double* getPos( void ) { return pos; }
	double* getParticleX( void ) { return copy_x; }
	double* getParticleY( void ) { return copy_y; }	
};
static void chkKeyHit( void );
int main( int aArgc, char *aArgv[ ] )
{
	try {
		setSigInt( );

#ifdef DRAW		
		GraphDrawerPF gdrawer;			// gnuplotによる描画
		gdrawer.initGnuplot( );				// gnuplot初期化
		gdrawer.readMap( "../data/map_11F.dat", "r" );
#else
#endif		
		printf( "\nStart pf-test\n" );

		inpData_odm odm_gl;
		odm_gl.initialize( "../data/log_motor.dat", "r" );
		inpData_urg urg_fs;
		urg_fs.initialize( "../data/log_urg.dat", "r" );

		ParticleFilter pf_gl;
		pf_gl.initialize( "../data/map_11F.dat", "r", -4.5, 0.0, 0.0 );

		double time_odm = 0, time_urg = 0, dt;
		double *pos;
//		unsigned long cnt = 0;
		bool flag_update = false;
		while( !gShutOff ){

			if( !odm_gl.readNew_odm( ) ) break;
			time_odm = odm_gl.getTime( );
//			cnt++;
			double dt = time_urg - time_odm;
			if( fabs( dt ) < 0.005 ){
				flag_update = true;
			} else if( 0 < dt ){
				flag_update = false;
			} else if( dt < 0 ){
				if( !urg_fs.readNew_urg( ) ) break;
				time_urg = urg_fs.getTime( );
				flag_update = false;
			}

			if( flag_update ){
// User program when synchronize				
				pf_gl.setPCD( urg_fs.getX( ), urg_fs.getY( ), urg_fs.getSize( ) );
				bool ret = pf_gl.setOdm( odm_gl.getVel( ), odm_gl.getAngvel( ), odm_gl.getTime( ) );
				if( ret ){
					pf_gl.prediction( );
					pf_gl.likelihood( );
					pf_gl.update( );
					pf_gl.resampling( );
				}
				
			// Update URG data
				if( !urg_fs.readNew_urg( ) ) break;
				time_urg = urg_fs.getTime( );
				flag_update = false;
//				printf( "time_odm=%f, time_urg=%f, dt=%f, cnt=%ld\n", time_odm, time_urg, dt, cnt );
//				cnt = 0;

#ifdef DRAW
				pos = pf_gl.getPos( );
				gdrawer.setODM( pos );
				gdrawer.setURG( urg_fs.getX( ), urg_fs.getY( ), urg_fs.getSize( ), pos );
				gdrawer.setPF( pf_gl.getParticleX( ), pf_gl.getParticleY( ), PARTICLE_SIZE );
				if( counter == 0 ){
					gdrawer.drawGraph( );
				}
				counter++;
				if( counter == 20 ) counter = 0;

#else
#endif	
			}
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
