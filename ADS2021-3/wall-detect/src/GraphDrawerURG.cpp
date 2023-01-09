/*
 * Date : 2018.07.09
 * Update : 2019.01.12
 * Update : 2019.02.01
 * Author : T.Hasegawa
 */
#include <stdlib.h>
#include "GraphDrawerURG.hpp"
#include <math.h>

using namespace std;

////////// Gnuplotによる軌跡の描画 //////////

GraphDrawerURG::GraphDrawerURG( ) : /*gp( nullptr ),*/ xmin( -10 ), xmax( 10 ), ymin( -10 ), ymax( 10 ), aspectR( -1.0 )
{
//	data_ring_head = data_ring_cnt = 0;
	
	for( int i = 0 ; i < DATA_MAX ; i++ ){
		for( int j = 0 ; j < 2 ; j++ ){
//			time[ i ] = 0.0;
			data_ring[ i ][ j ] = 0.0;
		}
	}
}
void GraphDrawerURG::setURG( double *tx, double *ty, int s )
{
	size = s;
	for( int i = 0 ; i < size ; i++ ){
		data_ring[ i ][ 0 ] = tx[ i ];
		data_ring[ i ][ 1 ] = ty[ i ];
	}

}
#define WIDTH 15
void GraphDrawerURG::drawGraph( void )
{
	fprintf( gp, "set title 'URG data'\n" );
	fprintf( gp, "set yrange[%f:%f]\nset xrange[%lf:%lf]\n", -1.0*(double)WIDTH, (double)WIDTH, -1.0*(double)WIDTH, (double)WIDTH );
	fprintf( gp, "set xtics 5\n" );
	
	fprintf( gp, "p " );
	fprintf( gp, " '-' w l lc rgb 'green', " );
	fprintf( gp, "'-' pt 7 ps 0.2 lc rgb 'red'" );
	fprintf( gp, "\n" );

	for( int i = 0 ; i < size ; i++ ){
		fprintf( gp, "0 0\n%lf %lf\n\n", data_ring[ i ][ 0 ], data_ring[ i ][ 1 ] );
	}
	fprintf( gp, "e\n" );
	
	for( int i = 0 ; i < size ; i++ ){
		fprintf( gp, "%lf %lf\n", data_ring[ i ][ 0 ], data_ring[ i ][ 1 ] );
	}
	fprintf( gp, "e\n" );
	fflush( gp );
}
