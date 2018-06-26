#include "data-file.hpp"

namespace rfs
{
	File::File()
	{
		File::fp == NULL;
		File::fs == 0;
	}

	File::File( const ascii &file )
	{
		File::fp == NULL;
		File::fs == 0;
		File::fopen( file );
	}

	File::~File()  { File::fclose(); }

	bool File::fnew( const ascii &file )
	{
		if ( File::fp != NULL )  return false;
		if (( File::fp = fopen( file.c_str(), "wb+" ) ))
		{
			File::fs = 0;
			return true;
		}
		return false;
	}

	bool File::fopen( const ascii &file )
	{
		if ( File::fp != NULL )  return false;
		if (( File::fp = fopen( file.c_str(), "rb+" ) ))
		{
			File::fs = File::fsize();
			return true;
		}
		return false;
	}

	uint File::fsize()
	{
		uint s = 0;
		fseek( File::fp, 0, SEEK_END );
		s = ftell(File::fp);
		rewind(File::fp);
		return s;
	}

	uint File::fget_uint( uint pos, uint byte, bool bigend )
	{
		if ( File::fp == NULL )  return 0;
		uint i, j;
		uint n = 0;

		File::fseek( pos );
		if ( bigend )
		{
			// read 00 98 96 7f
			i = byte;
			while ( i > 0 )
			{
				i--;
				j = fgetc( File::fp );
				n = n + ( j << (8*i) );
			}
		}
		else
		{
			// read 7f 96 98 00
			i = 0;
			while ( i < byte )
			{
				j = fgetc( File::fp );
				n = n + ( j << (8*i) );
				i++;
			}
		}
		return n;
	}

	int File::fget_int( uint pos, uint byte, bool bigend )
	{
		if ( File::fp == NULL )  return 0;
		uint n = File::fget_uint( pos, byte, bigend );

		// check negative flag on the highest bit
		char nf = ( n >> ( byte*8-1 ) ) & 1;
		if ( nf )
		{
			// uint 252 = int -4 ( 11111100 )
			// invert bits       ( 00000011 )
			// add 1 = (u)int 4  ( 00000100 )
			// return in negative
			return ( ~n + 1 ) * -1;
		}
		else
			return n;
	}

	ascii File::fget_ascii( uint pos, uint byte )
	{
		if ( File::fp == NULL )  return "";
		std::stringstream ss;

		File::fseek( pos );
		uint  i;
		uchar j;
		if ( byte > 0 )
		{
			for ( i=0; i < byte; i++ )
			{
				j = fgetc( File::fp );
				if ( isprint(j) )
					ss << j;
				else
					i += byte;
			}
		}
		else
		{
			j = fgetc( File::fp );
			while ( isprint(j) )
			{
				ss << j;
				j = fgetc( File::fp );
			}
		}
		return ss.str();
	}

	void File::fput_uint( uint n, uint pos, uint byte, bool bigend )
	{
		if ( File::fp == NULL )  return 0;
		uint i, j;

		File::fseek( pos );
		if ( bigend )
		{
			// write 0x98967f as 00 98 96 7f
			i = byte;
			while( i > 0 )
			{
				i--;
				j = ( n >> (i*8) ) & 0xff;
				fputc( File::fp, j );
			}
		}
		else
		{
			// write 0x98967f as 7f 96 98 00
			i = 0;
			while( i < byte )
			{
				j = ( n >> (i*8) ) & 0xff;
				fputc( File::fp, j );
				i++;
			}
		}
	}

	void File::fput_int( int n, uint pos, uint byte, bool bigend )
	{
		if ( File::fp == NULL )  return 0;
		uint pn;

		if ( n < 0 )
		{
			// turned -4 into +4 ( 00000100 )
			// minus 1           ( 00000011 )
			// int -4 = uint 252 ( 11111100 )
			pn = (n * -1) - 1;
			File::fput_uint( (~pn), pos, byte, bigend );
		}
		else
			File::fput_uint( n, pos, byte, bigend );
	}

	void File::fput_ascii( const ascii &str, uint pos, uint byte )
	{
		if ( File::fp == NULL )  return 0;
		File::fseek( pos );

		uint i = 0;
		while ( i < byte && i < str.size() )
		{
			fputc( File::fp, str[i] );
			i++;
		}
	}

	void File::fseek( uint pos )
	{
		if ( File::fp != NULL )
			fseek( File::fp, pos, SEEK_SET );
	}

	void File::fclose()  { if ( File::fp != NULL )  fclose( File::fp ); }
}
