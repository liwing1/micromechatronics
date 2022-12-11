/*
 * Date : 2018.07.09
 * Update : 2019.01.12
 * Update : 2019.02.01
 * Author : T.Hasegawa
 */
#include <stdlib.h>
#include "GraphDrawerODM.hpp"
#include <math.h>

using namespace std;

////////// Gnuplotによる軌跡の描画 //////////

GraphDrawerODM::GraphDrawerODM( ) : /*gp( nullptr ),*/ xmin( -10 ), xmax( 10 ), ymin( -10 ), ymax( 10 ), aspectR( -1.0 )
{
	data_ring_head = data_ring_cnt = 0;
	
	for( int i = 0 ; i < DATA_MAX ; i++ ){
		for( int j = 0 ; j < 3 ; j++ ){
			time[ i ] = 0.0;
			data_ring[ i ][ j ] = 0.0;
		}
	}
	
	double width = 0.5;
	double length = 0.8;
	robot[ 0 ][ 0 ] = ( length / 2 ) - 0.2;	robot[ 0 ][ 1 ] = width / 2;
	robot[ 1 ][ 0 ] = ( length / 2 );			robot[ 1 ][ 1 ] = 0.0;
	robot[ 2 ][ 0 ] = ( length / 2 ) - 0.2;	robot[ 2 ][ 1 ] = -1.0 * ( width / 2 );
	robot[ 3 ][ 0 ] = -1.0 * ( length / 2 );	robot[ 3 ][ 1 ] = -1.0 * ( width / 2 );
	robot[ 4 ][ 0 ] = -1.0 * ( length / 2 );	robot[ 4 ][ 1 ] = width / 2;
}
void GraphDrawerODM::setODM( double *data, double t )
{
	robot_pos[ 0 ] = data[ 0 ];	
	robot_pos[ 1 ] = data[ 1 ];
	robot_pos[ 2 ] = data[ 2 ];
	
	data_ring[ data_ring_head ][ 0 ] = data[ 0 ];
	data_ring[ data_ring_head ][ 1 ] = data[ 1 ];
	data_ring[ data_ring_head ][ 2 ] = data[ 2 ];
	time[ data_ring_head ] = t;
	curTime = t;
	data_ring_head++;
	if( data_ring_head >= DATA_MAX )
		data_ring_head = 0;
	if( data_ring_cnt < DATA_MAX )
		data_ring_cnt++;
		
//	printf( "**%f %f %f %f\n", t, data[0], data[1], data[2] );
}
static int robot_edge_num = 5;
void GraphDrawerODM::fprintf_robot( double *edges )
{
	int ii;
	double x, y;

	for( int i = 0 ; i <= robot_edge_num; i++ ){
		ii = i % robot_edge_num;

		x = edges[ ii * 2 ] * cos( robot_pos[ 2 ] ) - edges[ ii * 2 + 1 ] * sin( robot_pos[ 2 ] ) + robot_pos[ 0 ];
		y = edges[ ii * 2 ] * sin( robot_pos[ 2 ] ) + edges[ ii * 2 + 1 ] * cos( robot_pos[ 2 ] ) + robot_pos[ 1 ];
				
		fprintf( gp, "%lf %lf\n", x, y );
	}

	fprintf( gp, "\n\n" );
}
#define WIDTH 5
void GraphDrawerODM::drawGraph( void )
{
	fprintf( gp, "set title 'ODM data'\n" );
	fprintf( gp, "set yrange[%f:%f]\nset xrange[%lf:%lf]\n", robot_pos[1]-WIDTH, robot_pos[1]+WIDTH, robot_pos[0]-WIDTH, robot_pos[0]+WIDTH );
	fprintf( gp, "set xtics 5\n" );
	
	fprintf( gp, "p " );
	// 軌跡の描画設定
	if( data_ring_head > 0 )
		fprintf( gp, "'-' pt 5 ps 0.5 lc rgb 'red', " );
	//ロボットの描画設定
	fprintf( gp," '-' w l lc rgb 'black'" );
	fprintf( gp, "\n" );

	//軌跡の描画
	if( data_ring_head > 0 ){
		for( int i = 0 ; i < data_ring_cnt ; i++ ){
			fprintf( gp, "%lf %lf\n", data_ring[ i ][ 0 ], data_ring[ i ][ 1 ] );
		}
		fprintf( gp, "e\n" );
	}
	//ロボットの描画
	fprintf_robot( &robot[ 0 ][ 0 ] );
	fprintf( gp,"e\n" );
	fflush( gp );
}
