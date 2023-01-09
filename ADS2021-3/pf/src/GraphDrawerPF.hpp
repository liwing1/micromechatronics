 /*
  * Date : 2020.09.03
  * Author : T.Hasegawa
  */
#ifndef _GRAPH_DRAWER_PF_HPP_
#define _GRAPH_DRAWER_PF_HPP_

#include <stdio.h>
#include <iostream>
#include <cstddef>
#include "inpData_urg.hpp"
#include "ioMap.hpp"

#define DATA_MAX 3000

class GraphDrawerPF
{
private:
	FILE *gp;               // gnuplotへのパイプ
	double xmin;            // 描画範囲[m]
	double xmax;
	double ymin;
	double ymax;
	double aspectR;         // xy比
	
//	double curTime;
//	double time[ DATA_MAX ];
	double pos_data_ring[ DATA_MAX ][ 3 ];
	int pos_data_ring_head;
	int pos_data_ring_cnt;

	ioMap map;		// 全体地図
	// resample PC
	double scan[ URG_DATA_MAX ][ 2 ];
	int scan_cnt;
	// particle
	double particle[ 1000 ][ 2 ];
	int particle_size;
	// ロボットの描写パラメータ
	double robot_pos[ 3 ];
	double robot[ 5 ][ 2 ];
	void fprintf_robot( double *edges );
public:
	GraphDrawerPF( void );
	~GraphDrawerPF( void ) 
	{
		finishGnuplot( );
	}

	void initGnuplot( void )
	{
		gp = popen( "gnuplot", "w" );		// パイプオープン.Linux
		setGrid( );
		setAspectRatio( -0.9 );		// x軸とy軸の比（負にすると中身が一定）
	}

	void finishGnuplot( void )
	{
//		if( gp != nullptr )
			pclose( gp );
	}
	void setAspectRatio( double a )
	{
		aspectR = a;
		fprintf( gp, "set size ratio %lf\n", aspectR );
	}

	void setRange( double R )		// 描画範囲をR四方にする
	{
		xmin = ymin = -R;
		xmax = ymax = R;
		fprintf( gp, "set xrange [%lf:%lf]\n", xmin, xmax );
		fprintf( gp, "set yrange [%lf:%lf]\n", ymin, ymax );
	}

	void setRange( double xR, double yR )		// 描画範囲を±xR、±yRにする
	{
		xmin = -xR;
		xmax = xR;
		ymin = -yR; 
		ymax = yR;
		fprintf( gp, "set xrange [%lf:%lf]\n", xmin, xmax );
		fprintf( gp, "set yrange [%lf:%lf]\n", ymin, ymax );
	}

	void setRange( double xm, double xM, double ym, double yM )		// 描画範囲を全部指定
	{
		xmin = xm;
		xmax = xM;
		ymin = ym; 
		ymax = yM;
		fprintf( gp, "set xrange [%lf:%lf]\n", xmin, xmax );
		fprintf( gp, "set yrange [%lf:%lf]\n", ymin, ymax );
	}
	void setGrid( void )
	{
		fprintf( gp, "set grid\n" );
	}
	void readMap( const char *filename, const char* mode )
	{
		map.initialize( filename, mode );
		map.readMap( );
	}
	void setURG( double *tx, double *ty, int s, double* p );
//	void setPF( double **data, double s );
	void setPF( double *dx, double *dy, double s );
	void setODM( double *data );
	void drawGraph( void );
};

#endif


