#include "funcs.hpp"

int rfs::mysleep( uint msec )
{
	struct timespec timeout0;
	struct timespec timeout1;
	struct timespec* tmp;
	struct timespec* t0 = &timeout0;
	struct timespec* t1 = &timeout1;

	t0->tv_sec  =  msec / 1000;
	t0->tv_nsec = (msec % 1000) * (1000 * 1000);

	while( nanosleep(t0, t1) == -1 )
	{
		if( errno == EINTR )
		{
			tmp = t0;
			t0 = t1;
			t1 = tmp;
		}
		else
			return -1;
	}
	return 0;
}

bool rfs::fopen( std::fstream &fs, const ascii &file, const ascii &mode )
{
	// fopen is always new fs
	// so close existing fs, if it is already fopen'd
	if ( fs.good() )
		fs.close();

	// C style fopen with C++
	std::fstream fp;

	if ( mode == "r" )
		fs.open(file.c_str(), fp.in);
	else if ( mode == "rb" )
		fs.open(file.c_str(), fp.in | fp.binary);
	else if ( mode == "r+" )
		fs.open(file.c_str(), fp.in | fp.out);
	else if ( mode == "rb+" )
		fs.open(file.c_str(), fp.in | fp.out | fp.binary);

	else if ( mode == "w" )
		fs.open(file.c_str(), fp.out | fp.trunc);
	else if ( mode == "wb" )
		fs.open(file.c_str(), fp.out | fp.trunc | fp.binary);
	else if ( mode == "w+" )
		fs.open(file.c_str(), fp.in | fp.out | fp.trunc);
	else if ( mode == "wb+" )
		fs.open(file.c_str(), fp.in | fp.out | fp.trunc | fp.binary);

	else if ( mode == "a" )
		fs.open(file.c_str(), fp.out | fp.ate | fp.app);
	else if ( mode == "ab" )
		fs.open(file.c_str(), fp.out | fp.ate | fp.app | fp.binary);
	else if ( mode == "a+" )
		fs.open(file.c_str(), fp.in | fp.out | fp.ate | fp.app);
	else if ( mode == "ab+" )
		fs.open(file.c_str(), fp.in | fp.out | fp.ate | fp.app | fp.binary);

	// return if success
	return fs.good();
}

uint rfs::fsize( std::fstream &fs )
{
	uint s;
	if ( fs.good() )
	{
		fs.seekg (0, fs.end);
		s = fs.tellg();
		fs.seekg (0, fs.beg);
		return s;
	}
	return 0;
}

uint rfs::freadint( std::fstream &fs, uint byte, uint addr )
{
	uint read = 0;
	uint i, j;

	fs.seekg (addr, fs.beg);
	for ( i=0; i < byte; i++ )
	{
		j = fs.get();
		read = read + ( j << (8*i) );
	}
	return read;
}

void rfs::fwriteint( std::fstream &fs, uint byte, uint addr, uint intr )
{
	uint i, j;
	fs.seekg (addr, fs.beg);

	for ( i=0; i < byte; i++ )
	{
		j = ( intr >> (8*i) ) & 0xff;
		fs.put(j);
	}
}

ascii rfs::pathjoin( const ascii &f1, const ascii &f2 )
{
	if ( f1 == "" )  return f2;
	if ( f2 == "" )  return f1;
	return f1 + "/" + f2;
}

ascii rfs::fname( const ascii &file )
{
	int cpos = strrchar( file, '.' );
	if ( cpos < 0 )  return "";

	std::stringstream ss;
	int i;
	for ( i=0; i < cpos; i++ )
		ss << file[i];

	return ss.str();
}

ascii rfs::fext( const ascii &file )
{
	int cpos = strrchar( file, '.' );
	if ( cpos < 0 )  return "";

	std::stringstream ss;
	uint s = file.size();
	uint i;
	for ( i=cpos+1; i < s; i++ )
		ss << file[i];

	return ss.str();
}

int rfs::strchar( const ascii &file, char c )
{
	uint s = file.size();
	uint i;
	for ( i=0; i < s; i++ )
	{
		if ( file[i] == c )
			return i;
	}
	return -1;
}

int rfs::strrchar( const ascii &file, char c )
{
	uint s = file.size();
	uint i;
	for ( i=s; i > 0; --i )
	{
		if ( file[i] == c )
			return i;
	}
	return -1;
}

void rfs::vectmerge( strvect &result, strvect &v1, strvect &v2 )
{
	strvect r;
	r.reserve( v1.size() + v2.size() );
	r.insert( r.end(), v1.begin(), v1.end() );
	r.insert( r.end(), v2.begin(), v2.end() );
	result.swap( r );
}

void rfs::vectmerge( intvect &result, intvect &v1, intvect &v2 )
{
	intvect r;
	r.reserve( v1.size() + v2.size() );
	r.insert( r.end(), v1.begin(), v1.end() );
	r.insert( r.end(), v2.begin(), v2.end() );
	result.swap( r );
}

ascii rfs::dosname( const ascii &file )
{
	uint s = file.size();
	uint i;

	std::stringstream ss;
	for ( i=0; i < s; i++ )
	{
		if ( std::islower(file[i]) )
			ss << (char)std::toupper(file[i]);
		else if ( std::isupper(file[i])
			|| std::isdigit(file[i])
			|| file[i] == '\\'
			|| file[i] == '/'
			|| file[i] == '.'
			|| file[i] == '_' )
			ss << file[i];
		else
			ss << '_';
	}

	return ss.str();
}

ascii rfs::hppname( const ascii &file )
{
	uint s = file.size();
	uint i;

	std::stringstream ss;
	for ( i=0; i < s; i++ )
	{
		if ( std::islower(file[i]) )
			ss << (char)std::toupper(file[i]);
		else if ( std::isupper(file[i])
			|| std::isdigit(file[i])
			|| file[i] == '_' )
			ss << file[i];
		else
			ss << '_';
	}

	return ss.str();
}

ascii rfs::sec2time ( uint sec, uint type )
{
	std::stringstream str;

	uint hh, mm, ss, hx, mx;
	hh = mm = ss = 0;
	hx = 60*60;
	mx = 60;

	// calculation
		if ( sec >= hx )  { hh = sec / hx;  sec %= hx; }
		if ( sec >= mx )  { mm = sec / mx;  sec %= mx; }
		ss = sec;

	// display format
		// format = "02:30:05"
		if ( type == 1 )
		{
			str << rfs::decstr( hh, 2 ) << ":";
			str << rfs::decstr( mm, 2 ) << ":";
			str << rfs::decstr( ss, 2 );
		}
		// format = "2h 30m 5s"
		else if ( type == 2 )
		{
			if ( hh > 0 )  str << " " << hh << "h";
			if ( mm > 0 )  str << " " << mm << "m";
			if ( ss > 0 )  str << " " << ss << "s";
		}
		// format = "2 hours 30 minutes 5 seconds"
		else if ( type == 3 )
		{
			if ( hh > 0 )  str << " " << hh << "hours";
			if ( mm > 0 )  str << " " << mm << "minutes";
			if ( ss > 0 )  str << " " << ss << "seconds";
		}

	return str.str();
}

ascii rfs::decstr( int n, uint pad )
{
	// use string stream to pad zeroes
	// 2 become 002
	std::stringstream ss;
	ss << std::setw( pad ) << std::setfill('0')
		<< std::dec
		<< n;
	return ss.str();
}

ascii rfs::hexstr( int n, uint pad )
{
	// use string stream to pad zeroes
	// 12 become 00c
	std::stringstream ss;
	ss << std::setw( pad ) << std::setfill('0')
		<< std::hex
		<< n;
	return ss.str();
}

ascii rfs::octstr( int n, uint pad )
{
	// use string stream to pad zeroes
	// 12 become 014
	std::stringstream ss;
	ss << std::setw( pad ) << std::setfill('0')
		<< std::oct
		<< n;
	return ss.str();
}

int rfs::uint2int( uint n, uint byte )
{
	if ( byte > 4 )  byte = 4; // 32-bit limit

	uint max = 1 << (byte * 8);
	if ( max < 1 )  return 0;
	if ( max < n )  return n;

	// if the uint is on the negative half
	if ( n > (max / 2) )
	{
		// uint 252 = int -4 ( 11111100 )
		// uint XOR 0xff     ( 00000011 )
		// add 1 = (u)int 4  ( 00000100 )
		// return in negative
		return ( ( n ^ (max - 1) ) + 1 ) * -1;
	}
	// positive half
	else
		return n;
}

float rfs::distance( int p1x, int p1y, int p2x, int p2y )
{
	int df1 = p2x - p1x;
	int df2 = p2y - p1y;
	int df  = ( df1 * df1 ) + ( df2 * df2 );
	return (float)std::sqrt( df );
}

int rfs::clamp( int d, int min, int max )
{
	if ( d < min ) return min;
	if ( d > max ) return max;
	return d;
}
