 /*
  * Date : 2018.07.09
  * Update : 2019.01.12
  * Update : 2019.02.01
  * Update : 2020.09.03
  * Author : T.Hasegawa
  */
#ifndef _GRAPH_DRAWER_IMU_HPP_
#define _GRAPH_DRAWER_IMU_HPP_

#include <stdio.h>
#include <iostream>
#include <cstddef>

#define DATA_MAX_IMU 5000
#define TIME_WIDTH 15 // sec

class GraphDrawerIMU
{
private:
	FILE *gp;               // gnuplotへのパイプ
	double xmin;            // 描画範囲[m]
	double xmax;
	double ymin;
	double ymax;
	double aspectR;         // xy比
	
	double curTime;
	double time[ DATA_MAX_IMU ];
	double data_ring[ DATA_MAX_IMU ][ 3 ];
	int data_ring_head;
	int data_ring_cnt;

public:
	GraphDrawerIMU( void );
	~GraphDrawerIMU( void ) 
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
	
	void setIMU( double *data, double t );
	void drawGraph( void );
};

#endif


