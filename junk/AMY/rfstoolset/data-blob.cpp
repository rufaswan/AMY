#include "data-blob.hpp"

namespace rfs
{
	Blob::Blob()  { Blob::clear(); }
	Blob::~Blob() {}

	void Blob::clear()
	{
		Blob::has_data = false;
		Blob::size     = 0;
		Blob::data.clear();
	}

	bool Blob::loadfile( const ascii &file )
	{
		Blob::clear();

		std::fstream fs;
		if ( ! rfs::fopen( fs, file, "rb" ) )  return false;

		Blob::size = rfs::fsize( fs );
		if ( Blob::size < 1 )
		{
			fs.close();
			return false;
		}

		Blob::data.resize( Blob::size );
		uint i;
		for ( i=0; i < Blob::size; i++ )
			Blob::data[i] = fs.get();

		fs.close();
		Blob::has_data = true;
		return true;
	}

	bool Blob::loadblob( rfs::Blob &bin, uint offset, uint size )
	{
		uint bs = bin.getsize();
		uint ns = offset + size;
		if ( offset >= bs )  return false;
		if ( ns > bs )       return false;

		// if size=0 or undefined, copy until the end of file
		uint end = ( size > 0 ) ? ns : bs;
		Blob::data.resize( end - offset );

		uint i, j;
		j = 0;
		for ( i = offset; i < end; i++ )
		{
			Blob::data[j] = bin.data[i];
			j++;
		}

		Blob::size = j;
		Blob::has_data = true;
		return true;
	}

	bool Blob::savefile( const ascii &file )
	{
		if ( ! Blob::has_data )  return false;

		std::fstream fs;
		if ( ! rfs::fopen( fs, file, "wb" ) )  return false;

		uint i;
		for ( i=0; i < Blob::size; i++ )
			fs.put( (char)Blob::data[i] );

		fs.close();
		return true;
	}

	void Blob::append( rfs::Blob &bin )
	{
		if ( ! Blob::has_data )
			Blob::loadblob( bin );
		else
		{
			uint sa = Blob::size;
			uint sb = bin.getsize();
			Blob::data.resize( sa + sb );
			uint i;
			for ( i=0; i < sb; i++ )
				Blob::data[ sa + i ] = bin.data[ sb + i ];
			Blob::size = sa + sb;
		}
	}

	void Blob::update( rfs::Blob &bin, uint offset )
	{
		if ( ! Blob::has_data )     return;
		if ( offset > Blob::size )  return;

		uint s  = bin.getsize();
		uint ns = offset + s;
		if ( ns > Blob::size )
			Blob::data.resize( ns );

		uint i;
		for ( i=0; i < s; i++ )
			Blob::data[ offset + i ] = bin.data[ i ];

		Blob::size = ns;
	}

	void Blob::insert( rfs::Blob &bin, uint offset )
	{
		if ( ! Blob::has_data )     return;
		if ( offset > Blob::size )  return;

		uint s  = bin.getsize();
		uint ns = Blob::size + s;

		std::vector < uchar > tmpvec;
		tmpvec.reserve( ns );

		// stop copying & insert the new blob when on offset
		uint i, j;
		for ( i=0; i < Blob::size; i++ )
		{
			if ( i == offset )
			{
				for ( j=0; j < s; j++ )
					tmpvec.push_back( bin.data[j] );
			}
			else
				tmpvec.push_back( Blob::data[i] );
		}
		Blob::data.swap( tmpvec );
		Blob::size = ns;
	}

	uint Blob::getint( uint offset, uint byte )
	{
		if ( ! Blob::has_data )     return 0;
		if ( offset >= Blob::size ) return 0;
		if ( byte > 4 )  byte = 4; // 32-bit limit
		if ( byte < 1 )  byte = 1; // minimum

		uint t = 0;
		uint i, b1, b2;
		for ( i=0; i < byte; i++ )
		{
			b1 = Blob::data[ offset + i ];
			b2 = i * 8;
			t += b1 << b2;
		}
		return t;
	}

	ascii Blob::getascii( uint offset, uint len )
	{
		if ( ! Blob::has_data )     return "";
		if ( offset >= Blob::size ) return "";
		if ( len < 1 )  len = 65535;

		std::stringstream ss;
		uint i = 0;
		while ( isprint( Blob::data[ offset + i ] ) && i < len )
		{
			ss << Blob::data[ offset + i ];
			i++;
		}
		return ss.str();
	}

	void* Blob::getdataptr()  { return ( Blob::has_data ) ? (void*)&Blob::data[0] : NULL; }
	uint  Blob::getsize()     { return Blob::size; }

	void Blob::debug( bool showdata )
	{
		if ( ! Blob::has_data )  return;

		printf("--- Blob info ---\n");
		printf("size : %d\n", Blob::size);
		if ( showdata )
		{
			printf("data : ");
			uint i;
			for ( i=0; i < Blob::size; i++ )
				printf("%x ", Blob::data[i]);
			printf("\n-----\n");
		}
	}
}
