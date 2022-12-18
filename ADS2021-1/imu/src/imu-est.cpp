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
//#include "GraphDrawerODM.hpp"
static unsigned long counter = 0;
#else
#endif

using namespace std;

#define GRAVITY	9.80665

class LocalizerIMU {
private:
	double time, time_old;
	double *angvel;
	double *accel;
	
	double theta[ 3 ];
	double pos[ 3 ];
	bool flag_first_loop;
	
public:
	LocalizerIMU( void ) : flag_first_loop( true )
	{
		for( int i = 0 ; i < 3 ; i++ )
			theta[ i ] = pos[ i ] = 0.0;
	}
	~LocalizerIMU( void ) { }
	
	void setIMU( double t, double *v, double *a )
	{
		if( flag_first_loop ){
			time_old = time = t;
			flag_first_loop = false;
		} else {
			time_old = time;
			time = t;
		}
		angvel = v;
		accel = a;
		for( int i = 0 ; i < 3 ; i++ ){
			accel[ i ] *= GRAVITY;
		}
	}
	
	double* getPos( void ) { return pos; }
	double* getTheta( void ) { return theta; }
	void ComplementaryFilter( void )
	{
		// Programming on your own.
	}
	void localizer( double time, double *a_sens, double *w_sens )
	{
		const double K = 0.999;
		static double old_time = 0;
		static double theta_gyro[3] = {0};
		double theta_accel[3] = {0};
		double g_angvel[3] = {0};

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
};

static int gShutOff = 0;
static void setSigInt( void );
static unsigned int dT = 10;

int main( int aArgc, char *aArgv[ ] )
{

	try {
		setSigInt( );
		
#ifdef DRAW
		GraphDrawerIMU gdrawer;			// gnuplotによる描画
		gdrawer.initGnuplot( );				// gnuplot初期化
		
//		GraphDrawerODM gdrawer_odm;			// gnuplotによる描画
//		gdrawer_odm.initGnuplot( );				// gnuplot初期化
#else
#endif					
		printf( "\nStart imu-test\n" );

		inpData_imu imu_fs;
		imu_fs.initialize( "../data/log_imu.dat", "r" );
		LocalizerIMU local;
		
		double time;
		double *angvel;
		double *accel;
		double *theta;
		double *pos;
		
		while( !gShutOff ){

			if( !imu_fs.readNew_imu( ) ) break;
			time = imu_fs.getTime( );
			angvel = imu_fs.getAngvel( );
			accel = imu_fs.getAccel( ); 
			
			local.setIMU( time, angvel, accel );
			local.ComplementaryFilter( );
			theta = local.getTheta( );

#ifdef DRAW
			gdrawer.setIMU( theta, time );
			if( counter == 0 ){
				gdrawer.drawGraph( );
			}
#else
#endif
			local.localizer( time, accel, angvel );
//			pos = local.getPos( );

#ifdef DRAW
//			gdrawer_odm.setODM( pos, time );
			
//			if( counter == 0 ){
//				gdrawer_odm.drawGraph( );
//			}
			counter++;
			if( counter == 30 ) counter = 0;
#else
#endif
//			printf( "%f %f %f %f %f %f %f\n", time, angvel[0], angvel[1], angvel[2], accel[0], accel[1], accel[2] );
			printf( "%f %f %f %f\n", time, theta[0], theta[1], theta[2] );
			
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
