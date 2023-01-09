#ifndef IO_MAP_HPP
#define IO_MAP_HPP

#include "ioData_base.hpp"

#define		UNKNOWN		-1
#define		SAFETY		 0
#define		OBSTACLE	10

class ioMap : public ioData_base
{
private:
	int **map;
	double width, length, resolution;
	unsigned int row, col;
	
	unsigned int start_grid[ 2 ];
	double start_pos[ 2 ];

public:
	ioMap( void ) { }
	~ioMap( void ) { }

	bool readMap( void );
	unsigned int getStatus( double x, double y );
	unsigned int Row( void ) { return row; }
	unsigned int Col( void ) { return col; }
	unsigned int X2Row( double x ){ return ( ( unsigned int )( ( x / resolution ) + start_grid[ 0 ] ) ); }
	unsigned int Y2Col( double y ){ return ( ( unsigned int )( start_grid[ 1 ] - ( y / resolution ) ) ); }
	double Row2X( unsigned int i ){ return ( ( double )i - ( double )start_grid[ 0 ] ) * resolution;  }
	double Col2Y( unsigned int j ){ return ( ( double )start_grid[ 1 ] - ( double )j ) * resolution;  }
};

#endif
