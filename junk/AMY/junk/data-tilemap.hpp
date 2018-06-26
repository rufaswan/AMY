#ifndef RFS_TILEMAP
#define RFS_TILEMAP

#include "defs.hpp"
#include "data-blob.hpp"

namespace rfs
{
	class TileMap
	{
		public:
			TileMap();
			~TileMap();

			bool setchar ( uint w, uint h, ... );
			bool setint  ( uint w, uint h, ... );
			bool loadblob( uint w, uint h, rfs::Blob &blob, uint byte=1 );
			bool loaduint( uint w, uint h, uint data[] );
			void clear();
			void newmap( uint w, uint h );
			void debug( int showdata=1 );

			uint w;
			uint h;
			bool has_map;
			std::vector < intvect >  map;
	};
}

#endif