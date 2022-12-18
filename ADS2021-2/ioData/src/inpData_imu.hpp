#ifndef INP_DATA_IMU_HPP
#define INP_DATA_IMU_HPP

#include "ioData_base.hpp"

#define _ROLL		0
#define _PITCH		1
#define _YAW		2
#define _X			0
#define _Y			1
#define _Z			2

class inpData_imu : public ioData_base
{
private:
	double time;
	double angvel[ 3 ];
	double accel[ 3 ];
	double theta[ 3 ];
	int calcIMU( void );
//	double mag[ 3 ];
//	double temperature;

public:
	inpData_imu( void ) 
	{ 
		theta[ 0 ] = 0; theta[ 1 ] = 0; theta[ 2 ] = 0;
	}
	~inpData_imu( void ) { }

	bool readNew_imu( void );
	double getTime( void ){ return time; }
	double* getAngvel( void ){ return angvel; }
	double* getAccel( void ){ return accel; }
	double* getTheta( void ){ calcIMU();return theta; }

};

#endif
