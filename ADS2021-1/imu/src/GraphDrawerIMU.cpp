/*
 * Date : 2018.07.09
 * Update : 2019.01.12
 * Update : 2019.02.01
 * Author : T.Hasegawa
 */
#include <stdlib.h>
#include "GraphDrawerIMU.hpp"
#include <math.h>

using namespace std;

////////// Gnuplotによる軌跡の描画 //////////

GraphDrawerIMU::GraphDrawerIMU( ) : /*gp( nullptr ),*/ xmin( -10 ), xmax( 10 ), ymin( -10 ), ymax( 10 ), aspectR( -1.0 )
{
	data_ring_head = data_ring_cnt = 0;
	
	for( int i = 0 ; i < DATA_MAX_IMU ; i++ ){
		for( int j = 0 ; j < 3 ; j++ ){
			time[ i ] = 0.0;
			data_ring[ i ][ j ] = 0.0;
		}
	}
}
void GraphDrawerIMU::setIMU( double *data, double t )
{
	data_ring[ data_ring_head ][ 0 ] = data[ 0 ];
	data_ring[ data_ring_head ][ 1 ] = data[ 1 ];
	data_ring[ data_ring_head ][ 2 ] = data[ 2 ];
	time[ data_ring_head ] = t;
	curTime = t;
	data_ring_head++;
	if( data_ring_head >= DATA_MAX_IMU )
		data_ring_head = 0;
	if( data_ring_cnt < DATA_MAX_IMU )
		data_ring_cnt++;


}
void GraphDrawerIMU::drawGraph( void )
{
	if( data_ring_head > 0 ){
					
		fprintf( gp,"reset\n unset mouse\nset grid\n set key outside below\n" );		

		fprintf( gp, "set title 'IMU data'\n" );
		fprintf( gp, "set yrange[-3.5:3.5]\nset xrange[%lf:%lf]\n", curTime-TIME_WIDTH, curTime );
//		fprintf( gp, "set yrange[-10:10]\nset xrange[%lf:%lf]\n", curTime-TIME_WIDTH, curTime );
		fprintf( gp, "set xtics 5\n" );
		fprintf( gp, "p " );
		fprintf( gp, " '-' pt 5 lc rgb 'blue' t 'Roll'," );
		fprintf( gp, " '-' pt 5 lc rgb 'green' t 'Pitch'," );
		fprintf( gp, " '-' pt 5 lc rgb 'red' t 'Yaw'" );
		fprintf( gp, "\n" );
		for( int i = 0 ; i < data_ring_cnt ; i++ ){
			fprintf( gp,"%lf %lf\n", time[ i ], data_ring[ i ][ 0 ] );
		}
		fprintf( gp,"e\n" );
		for( int i = 0 ; i < data_ring_cnt ; i++ ){
			fprintf( gp,"%lf %lf\n", time[ i ], data_ring[ i ][ 1 ] );
		}
		fprintf( gp,"e\n" );
		for( int i = 0 ; i < data_ring_cnt ; i++ ){
			fprintf( gp,"%lf %lf\n", time[ i ], data_ring[ i ][ 2 ] );
		}
		fprintf( gp,"e\n" );
		fflush( gp );
	}
}
