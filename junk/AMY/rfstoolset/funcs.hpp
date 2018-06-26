#ifndef RFS_FUNCS
#define RFS_FUNCS

#include "defs.hpp"

namespace rfs
{
	int   mysleep( uint msec );
	bool  fopen( std::fstream &fs, const ascii &file, const ascii &mode );
	uint  fsize( std::fstream &fs );
	uint  freadint ( std::fstream &fs, uint byte, uint addr );
	void  fwriteint( std::fstream &fs, uint byte, uint addr, uint intr );
	ascii pathjoin( const ascii &f1, const ascii &f2 );
	ascii fname( const ascii &file );
	ascii fext ( const ascii &file );
	int   strchar  ( const ascii &file, char c );
	int   strrchar ( const ascii &file, char c );
	void  vectmerge( strvect &result, strvect &v1, strvect &v2 );
	void  vectmerge( intvect &result, intvect &v1, intvect &v2 );
	ascii dosname  ( const ascii &file );
	ascii hppname  ( const ascii &file );
	ascii sec2time ( uint sec, uint type=1 );

	ascii decstr  (  int n, uint pad=0 );
	ascii hexstr  (  int n, uint pad=0 );
	ascii octstr  (  int n, uint pad=0 );
	int   uint2int( uint n, uint byte=1 );
	float distance( int p1x, int p1y, int p2x, int p2y );
	int   clamp( int d, int min, int max );
}

#endif
