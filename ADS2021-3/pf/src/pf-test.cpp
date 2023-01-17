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
#include <random>

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
	double std[3] = {0.5, 0.5, 0.5}; // Standard Deviation

	//Set up generator for drawing particle initial states from normal distributionss
	default_random_engine gen;
	normal_distribution<double> dist_x(pos[_X], std[0]);
	normal_distribution<double> dist_y(pos[_Y], std[1]);
	normal_distribution<double> dist_theta(pos[_YAW], std[2]);

	//Initialize all the particle positions and weights
	for(int i = 0; i<PARTICLE_SIZE; ++i){
		particle[i][_X] = dist_x(gen);
		particle[i][_Y] = dist_y(gen);
		particle[i][_YAW] = dist_theta(gen);
		weight[i] = 1.;

		particle_copy[i][_X] = particle[i][_X];
		particle_copy[i][_Y] = particle[i][_Y];
		particle_copy[i][_YAW] = particle[i][_YAW];
	}

	//Set initialized to true
	flag_first_loop = false;

	cout<<"Initialized particle filter with";

	}
	void prediction( void )
	{
// Programming on your own
		//Calculate mean (x,y,theta) using exact motion models
		for(int i = 0; i<PARTICLE_SIZE; ++i){
			particle[i][_X] += vt * cos(particle[i][_YAW]) * dt;
			particle[i][_Y] += vt * sin(particle[i][_YAW]) * dt;
			particle[i][_YAW] += wt * dt;
		}
	}
	void likelihood( void )
	{
// Programming on your own
		double summation = 0;

		for(int i = 0; i<PARTICLE_SIZE; ++i){
			weight[i] = sqrt(pow(pos[_X], 2) + pow(pos[_Y],2));
		}

		for(int i = 0; i<PARTICLE_SIZE; ++i){
			summation += weight[i];
		}

		for(int i = 0; i<PARTICLE_SIZE; ++i){
			weight[i] /= summation;
		}
	}
	void update( void )
	{
// Programming on your own
		pos[_X] = 0;
		pos[_Y] = 0;
		pos[_YAW] = 0;

		for(int i = 0; i<PARTICLE_SIZE; ++i){
			pos[_X] += weight[i] * particle[i][_X];
			pos[_Y] += weight[i] * particle[i][_Y];
			pos[_YAW] += weight[i] * particle[i][_YAW];
		}
	}
	void resampling( void )
	{
		default_random_engine gen;
		double std[3] = {0.5, 0.5, 0.5}; // Standard Deviation

		//Setup the normal distributions using the std dev of the position (x,y,theta)
		for(int i = 0; i<PARTICLE_SIZE; ++i){
			normal_distribution<double> norm_x(pos[_X], std[_X]);
			normal_distribution<double> norm_y(pos[_Y], std[_Y]);
			normal_distribution<double> norm_theta(pos[_YAW], std[_YAW]);

			//Draw uncertain positions from the distributions to get a noisy state
			particle[i][_X] = norm_x(gen);
			particle[i][_Y] = norm_y(gen);
			particle[i][_YAW] = norm_theta(gen);
		}
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
