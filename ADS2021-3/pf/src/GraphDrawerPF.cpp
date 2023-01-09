/*
 * Date : 2020.10.16
 * Author : T.Hasegawa
 */
#include <stdlib.h>
#include "GraphDrawerPF.hpp"
#include <math.h>

using namespace std;

////////// Gnuplotによる軌跡の描画 //////////

GraphDrawerPF::GraphDrawerPF( ) : /*gp( nullptr ),*/ xmin( -10 ), xmax( 10 ), ymin( -10 ), ymax( 10 ), aspectR( -1.0 )
{
	pos_data_ring_head = pos_data_ring_cnt = 0;
	
	for( int i = 0 ; i < DATA_MAX ; i++ ){
		for( int j = 0 ; j < 3 ; j++ ){
//			time[ i ] = 0.0;
			pos_data_ring[ i ][ j ] = 0.0;
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
void GraphDrawerPF::setODM( double *data )
{
	robot_pos[ 0 ] = data[ 0 ];	
	robot_pos[ 1 ] = data[ 1 ];
	robot_pos[ 2 ] = data[ 2 ];
	
	pos_data_ring[ pos_data_ring_head ][ 0 ] = data[ 0 ];
	pos_data_ring[ pos_data_ring_head ][ 1 ] = data[ 1 ];
	pos_data_ring[ pos_data_ring_head ][ 2 ] = data[ 2 ];
//	time[ data_ring_head ] = t;
//	curTime = t;
	pos_data_ring_head++;
	if( pos_data_ring_head >= DATA_MAX )
		pos_data_ring_head = 0;
	if( pos_data_ring_cnt < DATA_MAX )
		pos_data_ring_cnt++;
		
//	printf( "%f %f %f\n", data[0], data[1], data[2] );
}
static int robot_edge_num = 5;
void GraphDrawerPF::fprintf_robot( double *edges )
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
void GraphDrawerPF::setURG( double *tx, double *ty, int s, double *p )
{
	scan_cnt = s;
	for( int i = 0 ; i < scan_cnt ; i++ ){
		scan[ i ][ 0 ] = tx[ i ] * cos( p[2] ) - ty[ i ] * sin( p[2] ) + p[0];
		scan[ i ][ 1 ] = tx[ i ] * sin( p[2] ) + ty[ i ] * cos( p[2] ) + p[1];
//		scan[ i ][ 0 ] = tx[ i ];
//		scan[ i ][ 1 ] = ty[ i ];
	}
}
/*
void GraphDrawerPF::setPF( double **data, double s )
{
	particle_size = s;
	if( particle_size > 1000 ) fprintf( stderr, "Error. Particle size\n" );
	for( int i = 0 ; i < particle_size ; i++ ){
		particle[ i ][ 0 ] = data[ i ][ 0 ];
		particle[ i ][ 1 ] = data[ i ][ 1 ];
		particle[ i ][ 2 ] = data[ i ][ 2 ];
	}
}*/
void GraphDrawerPF::setPF( double *dx, double *dy, double s )
{
	particle_size = s;
	if( particle_size > 1000 ) fprintf( stderr, "Error. Particle size\n" );
	for( int i = 0 ; i < particle_size ; i++ ){
		particle[ i ][ 0 ] = dx[ i ];
		particle[ i ][ 1 ] = dy[ i ];
	}
}
#define WIDTH 5
void GraphDrawerPF::drawGraph( void )
{
	fprintf( gp, "set title 'PDF data'\n" );
	setRange( robot_pos[ 0 ]-WIDTH, robot_pos[ 0 ]+WIDTH, robot_pos[ 1 ]-WIDTH, robot_pos[ 1 ]+WIDTH );
	fprintf( gp, "set xtics 5\n" );
	
	fprintf( gp, "p " );
	
	// MAPの画設定
	fprintf( gp, "'-' pt 7 ps 0.5 lc rgb 'grey', " );
	//スキャンデータの描画
	if( scan_cnt > 0 )
		fprintf( gp, "'-' pt 7 ps 0.5 lc rgb 'green'," );
	// Particleの描画設定
	fprintf( gp, "'-' pt 5 ps 0.5 lc rgb 'purple', " );
	// 軌跡の描画設定
	if( pos_data_ring_cnt > 0 )
		fprintf( gp, "'-' pt 5 ps 0.5 lc rgb 'red', " );
	//ロボットの描画設定
	fprintf( gp," '-' w l lc rgb 'black'" );
	fprintf( gp, "\n" );


	// MAPの描画
	for( int i = 0 ; i < map.Row( ) ; i++ ){
		for( int j = 0 ; j < map.Col( ) ; j++ ){
			double x = map.Row2X( i );
			double y = map.Col2Y( j );
			if( map.getStatus( x, y ) == 1 ){
				fprintf( gp, "%lf %lf\n", x, y );
//				printf( "[%f,%f]", x, y );
			}
		}
	}
	fprintf( gp, "e\n" );
	//スキャンデータの描画
	if( scan_cnt > 0 ){
		for( int i = 0 ; i < scan_cnt ; i++ )
			fprintf( gp, "%lf %lf\n", scan[ i ][ 0 ], scan[ i ][ 1 ] );
		fprintf( gp, "e\n" );
	}

	// Particleの描画
	for( int i = 0 ; i < particle_size ; i++ ){
		fprintf( gp, "%lf %lf\n", particle[ i ][ 0 ], particle[ i ][ 1 ] );
	}
	fprintf( gp, "e\n" );
	//軌跡の描画
	if( pos_data_ring_cnt > 0 ){
		for( int i = 0 ; i < pos_data_ring_cnt ; i++ ){
			fprintf( gp, "%lf %lf\n", pos_data_ring[ i ][ 0 ], pos_data_ring[ i ][ 1 ] );
		}
		fprintf( gp, "e\n" );
	}
	//ロボットの描画
	fprintf_robot( &robot[ 0 ][ 0 ] );
	fprintf( gp,"e\n" );
	fflush( gp );
}
