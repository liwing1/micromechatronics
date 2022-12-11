#ifndef INP_DATA_URG_HPP
#define INP_DATA_URG_HPP

#include "ioData_base.hpp"

#define URG_DATA_MAX 1080	// = 270[deg] / 0.25[deg]

class inpData_urg : public ioData_base
{
private:
	double time;
	unsigned int size;		// ポイント数 (=URG_DATA_MAX)
    double x[ URG_DATA_MAX ];
    double y[ URG_DATA_MAX ];
    unsigned intensity[ URG_DATA_MAX ];	// 受信強度

public:
	inpData_urg( void ) { }
	~inpData_urg( void ) { }

	bool readNew_urg( void );
	double getTime( void ){ return time; }
	double* getX( void ){ return x; }
	double* getY( void ){ return y; }
	unsigned* getIntensity( void ){ return intensity; }
	unsigned int getSize( void ) { return size; }
};

#endif
