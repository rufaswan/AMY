#include "data-tilemap.hpp"

namespace rfs
{
	TileMap::TileMap()  { TileMap::clear(); }
	TileMap::~TileMap() {}

	void TileMap::clear()
	{
		TileMap::w = 0;
		TileMap::h = 0;
		TileMap::has_map = false;
		TileMap::map.clear();
	}

	bool TileMap::setchar( uint w, uint h, ... )
	{
		TileMap::clear();

		TileMap::w = w;
		TileMap::h = h;
		TileMap::newmap( w, h );

		// setmap with ASCII charset only, mainly for testing
		// setmap( 5, 3,
		//   "..1..",
		//   ".111.",
		//   "11111"
		// );
		va_list vl;
		va_start(vl, h);
		uint x, y;
		uint i = 0;
		char*  row;
		for ( y=0; y < h; y++ )
		{
			row = va_arg(vl, char*);
			for ( x=0; x < w; x++ )
			{
				TileMap::map[y][x] = row[x];
				i++;
			}
		}
		va_end(vl);

		TileMap::has_map = true;
		return true;
	}

	bool TileMap::setint( uint w, uint h, ... )
	{
		TileMap::clear();

		TileMap::w = w;
		TileMap::h = h;
		TileMap::newmap( w, h );

		// setmap with a bunch of int/char, can be hard to read+edit
		// setmap( 5, 3,
		//   '.',  '.',  '1',  '.', '.',
		//   '.', 0x31, 0x31, 0x31, '.',
		//    49,   49,   49,   49,  49
		va_list vl;
		va_start(vl, h);
		uint x, y;
		for ( y=0; y<h; y++ )
		{
			for ( x=0; x<w; x++ )
			{
				TileMap::map[y][x] = va_arg(vl, int);
			}
		}
		va_end(vl);

		TileMap::has_map = true;
		return true;
	}

	bool TileMap::loadblob( uint w, uint h, rfs::Blob &blob, uint byte )
	{
		uint bs = blob.getsize() / byte;
		uint br = blob.getsize() % byte;
		if ( bs != ( w*h ) )  return false;
		if ( br != 0       )  return false;

		TileMap::w = w;
		TileMap::h = h;
		TileMap::newmap( w, h );

		uint x, y, b;
		uint i = 0;
		for ( y=0; y<h; y++ )
		{
			for ( x=0; x<w; x++ )
			{
				for ( b=0; b < byte; b++ )
				{
					TileMap::map[y][x] += blob.data[i] << b*8;
					i++;
				}
			}
		}

		TileMap::has_map = true;
		return true;
	}

	bool TileMap::loaduint( uint w, uint h, uint data[] )
	{
		TileMap::w = w;
		TileMap::h = h;
		TileMap::newmap( w, h );

		uint i = 0;
		uint x, y;
		for ( y=0; y<h; y++ )
		{
			for ( x=0; x<w; x++ )
			{
				TileMap::map[y][x] = data[i];
				i++;
			}
		}

		TileMap::has_map = true;
		return true;
	}

	void TileMap::newmap( uint w, uint h )
	{
		TileMap::map.clear();
		TileMap::map.resize( h );
		uint i;
		for ( i=0; i<h; i++ )
			TileMap::map[i].resize( w );
	}

	void TileMap::debug( int showdata )
	{
		if ( ! TileMap::has_map )  return;

		uint i, j;
		printf("--- TileMap info ---\n");
		printf("Width  : %i\n", TileMap::w);
		printf("Height : %i\n", TileMap::h);
		printf("Total  : %i of %i\n", TileMap::w * TileMap::h, TileMap::map.size() );
		printf("data\n");
		for ( j=0; j < TileMap::h; j++ )
		{
			for ( i=0; i < TileMap::w; i++ )
			{
				if ( showdata == 1 )
					printf("%x ", TileMap::map[j][i]);
				else if ( showdata == 2 )
					printf("%c", TileMap::map[j][i]);
			}
			printf("\n");
		}
	}
}
