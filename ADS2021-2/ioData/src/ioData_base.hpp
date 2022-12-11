#ifndef IO_DATA_BASE_HPP
#define IO_DATA_BASE_HPP

class ioData_base
{
protected:
	char flag_mode[ 5 ];	// "r" or "w"
	FILE *fp_base;

public:
	ioData_base( void ) { }
	~ioData_base( void ) { }

	int initialize( const char *filename, const char* mode );

};

#endif
