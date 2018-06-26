#ifndef RFSTK_HPP
#define RFSTK_HPP
/*
 * RUFAS Toolkit
 * A collection of simple and useful funcs in making progs
 *
 * WARNING : DO NOT COMPILE THIS .CPP INTO .O FILE !
 * #include "rfstk.hpp"
 * will copy all the content from this file
 *
 * The problem is that compiling both main.cpp and foop.cpp,
 * means two copies of foop.cpp are being linked together.
 * The linker is complaining about the duplication.
 *
 * Function arguments order:
 *   DATA - FILTER - RESULT - OPTIONAL
 *   You are given a BOOK, flip to PAGE_NO (and then LINE_NO) and return the WORD
 *   1)
 *     char WORD[255];
 *     read( FILE* BOOK, int PAGE_NO, char WORD[], int LINE_NO = 0 )
 *   2)
 *     char WORD[] = read( FILE* BOOK, int PAGE_NO, int LINE_NO = 0 )
 */
/*
 * LFS - Large File Support
 * https://en.wikipedia.org/wiki/Large_file_support
 *   gcc -D_OS_LINUX_32   -o script.out   script.cpp  // fseeko64
 *   gcc -D_OS_LINUX_64   -o script.out   script.cpp  // fseek
 *   gcc -D_OS_WIN_32     -o script.out   script.cpp  // _fseeki64
 *   gcc -D_OS_WIN_64     -o script.out   script.cpp  // fseek
 * Only 32-bit is the problem, 64-bit is same as usual
 */
#if defined _OS_LINUX_32
#  define _FILE_OFFSET_BITS 64
#  define _fseek_   fseeko64
#  define _ftell_   ftello64
#elif defined _OS_WIN_32
#  define _fseek_   _fseeki64
#  define _ftell_   _ftelli64
#else
#  define _fseek_   fseek
#  define _ftell_   ftell
#endif
/*
 * https://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Opening-Streams.html
 * https://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/File-Positioning.html
 *
 * __REDIRECT (from /usr/local/include/stdio.h)
 * tmpfile -> tmpfile64
 * fopen   -> fopen64
 * freopen -> freopen64
 * fseeko  -> fseeko64    __USE_XOPEN2K   __off_t -> __off64_t
 * ftello  -> ftello64    __USE_XOPEN2K   __off_t -> __off64_t
 * fgetpos -> fgetpos64   fpos_t -> fpos64_t
 * fsetpos -> fsetpos64   fpos_t -> fpos64_t
*/
#include <cstdio>
#include <cstdlib>

typedef unsigned char   uchar;
typedef unsigned int    uint;

namespace rfs {
	///////////////////
	// memset series //
	///////////////////
	void memset( uchar bin[], uint size, uchar c=0 )
	{
		uint i;
		for ( i=0; i < size; i++ )
			bin[i] = c;
	}
	void memset( char  bin[], uint size, char  c=0 )
	{
		uint i;
		for ( i=0; i < size; i++ )
			bin[i] = c;
	}

	//////////////////
	// fseek series //
	//////////////////
	uint fseek( FILE* fp )
	{
		_fseek_( fp, 0, SEEK_END );
		return (uint)_ftell_(fp);
	}
	uint fseek( FILE* fp, uint pos )
	{
		_fseek_( fp, pos, SEEK_SET );
		return (uint)_ftell_(fp);
	}
	uint fseek( FILE* fp, uint pos, uint max )
	{
		if ( pos > max )
			printf("FATAL rfs::fseek() POS = %8x , MAX = %8x\n", pos, max);
		else
			return rfs::fseek( fp, pos );
	}
	void fseek( FILE* fp1, FILE* fp2, uint pos1, uint pos2 )
	{
		rfs::fseek( fp1, pos1 );
		rfs::fseek( fp2, pos2 );
	}
	void fseek( FILE* fp1, FILE* fp2, uint pos1, uint pos2, uint max1, uint max2 )
	{
		rfs::fseek( fp1, pos1, max1 );
		rfs::fseek( fp2, pos2, max2 );
	}

	/////////////////////
	// filesize series //
	/////////////////////
	uint filesize( FILE* fp )
	{
		uint sz = rfs::fseek( fp );
		rfs::fseek( fp, 0 );
		return sz;
	}
	uint filesize( char fname[] )
	{
		uint sz = 0;
		FILE* fp = fopen( fname, "rb" );
			if ( ! fp )   return sz;
		sz = rfs::filesize( fp );
		fclose(fp);
		return sz;
	}

	//////////////////
	// fread series //
	//////////////////
	void fread( FILE* fp, char str[] )
	{
		uint n = 0;
		char c;
		while(1)
		{
			c = fgetc(fp);
			if ( c == '\t' )    c = ' ';
			if ( c < 0x20 || c > 0x7e )
			{
				str[n] = 0;
				return;
			}

			str[n] = c;
			n++;
		}
	}
	void fread( FILE* fp, char str[], uint bytes)
	{
		rfs::memset( str, bytes );
		uint i;
		char c;
		bool zero = false;
		for ( i=0; i < bytes; i++ )
		{
			c = fgetc(fp);
			if ( c == '\t' )    c = ' ';
			if ( c < 0x20 || c > 0x7e )
				zero = true;
			str[i] = ( zero ) ? 0 : c;
		}
	}
	void fread( FILE* fp, uchar bin[], uint bytes)
	{
		rfs::memset( bin, bytes );
		uint i;
		for ( i=0; i < bytes; i++ )
			bin[i] = fgetc(fp);
	}
	uint fread( FILE* fp, uint bytes, bool big=false)
	{
		uint i, j;
		uint res = 0;
		if ( big ) // big endian, 12 34 56 78 = 0x12345678
		{
			i = bytes;
			while ( i > 0 )
			{
				i--;
				j = ( fgetc(fp) << (i*8) );
				res += j;
			}
		}
		else // little endian, 12 34 56 78 = 0x78563412
		{
			for ( i=0; i < bytes; i++ )
			{
				j = ( fgetc(fp) << (i*8) );
				res += j;
			}
		}
		return res;
	}

	void mvfread( FILE* fp, uint pos, char str[] )
	{
		rfs::fseek( fp, pos );
		rfs::fread( fp, str );
	}
	void mvfread( FILE* fp, uint pos, char str[], uint bytes)
	{
		rfs::fseek( fp, pos );
		rfs::fread( fp, str, bytes );
	}
	void mvfread( FILE* fp, uint pos, uchar bin[], uint bytes)
	{
		rfs::fseek( fp, pos );
		rfs::fread( fp, bin, bytes );
	}
	uint mvfread( FILE* fp, uint pos, uint bytes, bool big=false)
	{
		rfs::fseek( fp, pos );
		return rfs::fread( fp, bytes, big );
	}

	///////////////////
	// fwrite series //
	///////////////////
	void fwrite( FILE* fp, const char str[])
	{
		fputs( str, fp );
	}
	void fwrite( FILE* fp, char str[])
	{
		fputs( str, fp );
	}
	void fwrite( FILE* fp, char str[], uint bytes)
	{
		uint i;
		bool zero = false;
		for ( i=0; i < bytes; i++ )
		{
			if ( zero ) // pad the rest with zeroes
				fputc( 0, fp );
			else
			{
				if ( str[i] == 0 ) // string end
					zero = true;
				fputc( str[i], fp );
			}
		}
	}
	void fwrite( FILE* fp, uchar bin[], uint bytes)
	{
		uint i;
		for ( i=0; i < bytes; i++ )
			fputc( bin[i], fp );
	}
	void fwrite( FILE* fp, uint n, uint bytes, bool big=false )
	{
		uint i, j;
		if ( big ) // big endian, 0x12345678 = 12 34 56 78
		{
			i = bytes;
			while ( i > 0 )
			{
				i--;
				j = (n >> (i*8)) & 0xff;
				fputc(j, fp);
			}
		}
		else // little endian, 0x12345678 = 78 56 34 12
		{
			for ( i=0; i < bytes; i++ )
			{
				j = (n >> (i*8)) & 0xff;
				fputc(j, fp);
			}
		}
	}
	void fwrite( FILE* fputp, FILE* fgetp, uint bytes )
	{
		uint i;
		for ( i=0; i < bytes; i++ )
			fputc( fgetc(fgetp), fputp );
	}

	void mvfwrite( FILE* fp, uint pos, char  str[])
	{
		rfs::fseek ( fp, pos );
		rfs::fwrite( fp, str );
	}
	void mvfwrite( FILE* fp, uint pos, char  str[], uint bytes)
	{
		rfs::fseek ( fp, pos );
		rfs::fwrite( fp, str, bytes);
	}
	void mvfwrite( FILE* fp, uint pos, uchar bin[], uint bytes)
	{
		rfs::fseek ( fp, pos );
		rfs::fwrite( fp, bin, bytes );
	}
	void mvfwrite( FILE* fp, uint pos, uint n, uint bytes, bool big=false )
	{
		rfs::fseek ( fp, pos );
		rfs::fwrite( fp, n, bytes, big );
	}
	void mvfwrite( FILE* fputp, uint fput_pos, FILE* fgetp, uint fget_pos, uint bytes )
	{
		rfs::fseek ( fputp, fput_pos );
		rfs::fseek ( fgetp, fget_pos );
		rfs::fwrite( fputp, fgetp, bytes );
	}

	///////////////
	// is series //
	///////////////
	bool is_file( char fname[] )
	{
		FILE* fp = fopen( fname, "rb" );
		if ( fp ) // file existed and readable
		{
			fclose(fp);
			return true;
		}
		else // cant open file
			return false;
	}
	bool is_readable( char fname[] )
	{
		return is_file( fname );
	}
	bool is_writable( char fname[] )
	{
		FILE* fp = fopen( fname, "wb" );
		if ( fp ) // file created and writable
		{
			fclose(fp);
			remove(fname);
			return true;
		}
		else // cant create file
			return false;
	}
	bool is_writeable( char fname[] )
	{
		return is_writable( fname );
	}

	////////////////////
	// is_same series //
	////////////////////
	bool is_same( char a, char b )
	{
		return a == b;
	}
	bool is_same( char a, char b[] )
	{
		return a == b[0];
	}
	bool is_same( char a, const char b[] )
	{
		return a == b[0];
	}
	bool is_same( char a[], char b[] )
	{
		uint n = 0;
		while (1)
		{
			if ( a[n] != b[n] )
				return false;
			if ( a[n] == 0 )
				return true;
			n++;
		}
	}
	bool is_same( char a[], const char b[] )
	{
		return rfs::is_same(a, (char*)b);
	}

	////////////////
	// str series //
	////////////////
	void strtoupper( char &c )
	{
		if ( c >= 0x61 && c <= 0x7a )
			c -= 0x20;
	}
	void strtoupper( char str[] )
	{
		uint i = 0;
		while( str[i] != 0 )
		{
			rfs::strtoupper( str[i] );
			i++;
		}
	}

	void strtolower( char &c )
	{
		if ( c >= 0x41 && c <= 0x5a )
			c += 0x20;
	}
	void strtolower( char str[] )
	{
		uint i = 0;
		while( str[i] != 0 )
		{
			rfs::strtolower( str[i] );
			i++;
		}
	}

	int strpos( char str[], char find )
	{
		int i = 0;
		while( str[i] != 0 )
		{
			if ( str[i] == find )
				return i;
			i++;
		}
		return 0;
	}
	int strrpos( char str[], char find )
	{
		int pos = 0;
		int i   = 0;
		while( str[i] != 0 )
		{
			if ( str[i] == find )
				pos = i;
			i++;
		}
		return pos;
	}

	void str_replace( char str[], char find, char replace )
	{
		uint i = 0;
		while( str[i] != 0 )
		{
			if ( str[i] == find )
				str[i] = replace;
			i++;
		}
	}

	/////////////////
	// padd series //
	/////////////////
	uint padd( uint n, uint block )
	{
		while ( (n % block) != 0 )
			n++;
		return n;
	}
	uint padd( FILE* fp, uint block )
	{
		uint pos, cnt;

		pos = rfs::fseek( fp );
		cnt = 0;
		while ( (pos % block) != 0 )
		{
			fputc(0, fp);
			pos++;
			cnt++;
		}
		return cnt;
	}
	/////////////////
	// math series //
	/////////////////
	int clamp( int a, int min, int max )
	{
		if ( a < min )   return min;
		if ( a > max )   return max;
		return a;
	}
	int max( int a, int max )
	{
		return ( a > max ) ? max : a;
	}
	int min( int a, int min )
	{
		return ( a < min ) ? min : a;
	}
	bool in_range( int c, int min, int max )
	{
		if ( c < min || c > max )  return false;
		return true;
	}

	uint max( uint a, uint max )
	{
		return ( a > max ) ? max : a;
	}
	uint min( uint a, uint min )
	{
		return ( a < min ) ? min : a;
	}
	bool in_range( uint c, uint min, uint max )
	{
		if ( c < min || c > max )  return false;
		return true;
	}

} // namespace rfs
#endif // RFSTK_HPP
