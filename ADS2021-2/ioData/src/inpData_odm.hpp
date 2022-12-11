#ifndef INP_DATA_ODM_HPP
#define INP_DATA_ODM_HPP

#include "ioData_base.hpp"

#define _X			0
#define _Y			1
#define _YAW		2

class inpData_odm : public ioData_base
{
private:
	double time;
	double counter[ 2 ];
	double pos[ 3 ];	// x, y, theta
	double vel;
	double angvel;
	int calcOdometory( void );

public:
	inpData_odm( void ) : time( 0 ), vel( 0 ), angvel( 0 )
	{
		pos[ 0 ] = 0; pos[ 1 ] = 0; pos[ 2 ] = 0;
	}
	~inpData_odm( void ) { }

	bool readNew_odm( void );
	double getTime( void ){ return time; }
	double getAngvel( void ){ return angvel; }
	double getVel( void ){ return vel; }
	double* getPos( void ){ return pos; }

};

#endif
