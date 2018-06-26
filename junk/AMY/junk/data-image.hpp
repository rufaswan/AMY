#ifndef AMY_IMAGE
#define AMY_IMAGE

#include "common.hpp"
#include "rfstoolset/data-blob.hpp"

namespace amy
{
	class Image
	{
		public:
			Image();
			~Image();

			bool setimage( rfs::Blob &blob );
			bool setimage( const void *data, uint size );
			bool setimage( sf::Image &img );
			uint addpalette( sf::Color &color );
			void clear();
			void debug();

			void getimage( sf::Image &img, uint x, uint y, uint w, uint h, uint palette_pos=0 );
			uint getpos( uint x, uint y );

			bool has_image;
			uint w;
			uint h;
			std::vector < sf::Color > palette;
			std::vector < uint >      pixel;
	};
}

#endif
