#include "data-vmem.hpp"

namespace rfs
{
	VMem::VMem( uint size )
	{
		VMem::memblk = (uchar *)malloc( size );
		VMem::size = size;

		uint i;
		for ( i=0; i < size; i++ )
			VMem::memblk[i] = 0;
	}

	VMem::~VMem()
	{
		free( VMem::memblk );
	}

	void VMem::pad( uint front, uint back )
	{
		uint newsize = VMem::size + front + back;
		uchar* newblk = (uchar *)malloc( newsize );

		uint i;
		for ( i=0; i < newsize; i++ )
			newblk[i] = 0;

		for ( i=0; i < VMem::size; i++ )
			newblk[ front+i ] = VMem::memblk[i];

		free( VMem::memblk );
		VMem::memblk = newblk;
		VMem::size   = newsize;
	}

	void VMem::dim( uint front, uint back )
	{
		uint newsize = VMem::size - front - back;
		uchar* newblk = (uchar *)malloc( newsize );

		uint i;
		for ( i=0; i < newsize; i++ )
			newblk[i] = VMem::memblk[ front+i ];

		free( VMem::memblk );
		VMem::memblk = newblk;
		VMem::size   = newsize;
	}

	void VMem::pad2fit( uint blksize )
	{
		uint rem = VMem::size % blksize;
		VMem::pad( 0, rem );
	}

	void VMem::dim2fit()
	{
		uint front = 0;
		uint back  = 0;
		uint i;

		for ( i=0; i < VMem::size; i++ )
		{
			if ( VMem::memblk[i] == 0 )
				front++;
			else
				break;
		}
		if ( VMem::size == front )  return;

		for ( i=VMem::size; i > 0; --i )
		{
			if ( VMem::memblk[i] == 0 )
				back++;
			else
				break;
		}
		if ( VMem::size == back )  return;

		VMem::dim( front, back );
	}

	bool VMem::set_char( uint pos, uint size, uchar* memblk )
	{
		if ( (pos+size) > VMem::size )  return false;

		uint i;
		for ( i=0; i < size; i++ )
			VMem::memblk[ pos+i ] = memblk[i];

		return true;
	}

	uchar* VMem::get_char( uint pos, uint size )
	{
		if ( (pos+size) > VMem::size )  return NULL;

		uchar* newblk = (uchar *)malloc( size );

		uint i;
		for ( i=0; i < size; i++ )
			newblk[i] = VMem::memblk[ pos+i ];

		return newblk;
	}

	bool VMem::set_int( uint pos, uint size, uint n, bool bigend )
	{
		if ( (pos+size) > VMem::size )  return false;

		uint i, val, bsh;
		if ( bigend )
		{
			// 0xABCDEFGH = AB CD EF GH
			for ( i=0; i < size; i++ )
			{
				bsh = ( size - i - 1 ) * 8;
				val = ( n >> bsh ) & 0xff;
				VMem::memblk[ pos+i ] = val;
			}
		}
		else
		{
			// 0xABCDEFGH = GH EF CD AB
			for ( i=0; i < size; i++ )
			{
				bsh = i * 8;
				val = ( n >> bsh ) & 0xff;
				VMem::memblk[ pos+i ] = val;
			}
		}
		return true;
	}

	uint VMem::get_int( uint pos, uint size, bool bigend )
	{
		if ( (pos+size) > VMem::size )  return 0;

		uint result = 0;
		uint i, val, bsh;
		if ( bigend )
		{
			// AB CD ED GH = 0xABCDEFGH
			for ( i=0; i < size; i++ )
			{
				bsh = ( size - i - 1 ) * 8;
				val = VMem::memblk[ pos+i ] << bsh;
				result += val;
			}
		}
		else
		{
			// AB CD ED GH = 0xGHEFCDAB
			for ( i=0; i < size; i++ )
			{
				bsh = i * 8;
				val = VMem::memblk[ pos+i ] << bsh;
				result += val;
			}
		}
		return result;
	}

	ascii VMem::get_str( uint pos, uint size )
	{
		if ( size == 0 || (pos+size) > VMem::size )
			size = VMem::size - pos;

		std::stringstream ss;
		uint i;
		for ( i=0; i < size; i++ )
		{
			if ( isprint( VMem::memblk[pos+i] ) )
				ss << VMem::memblk[pos+i];
			else
				break;
		}
		return ss.str();
	}
}
