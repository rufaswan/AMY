#include "data-image.hpp"

namespace amy
{
	Image::Image()  { Image::clear(); }
	Image::~Image() {}

	void Image::clear()
	{
		Image::has_image = false;
		Image::w = 0;
		Image::h = 0;
		Image::palette.clear();
		Image::pixel.clear();
	}

	bool Image::setimage( rfs::Blob &blob )
	{
		sf::Image sfimg;
		if ( ! sfimg.loadFromMemory( blob.getdataptr(), blob.getsize() ) )
			return false;
		return Image::setimage( sfimg );
	}

	bool Image::setimage( const void *data, uint size )
	{
		sf::Image sfimg;
		if ( ! sfimg.loadFromMemory( data, size ) )
			return false;
		return Image::setimage( sfimg );
	}

	bool Image::setimage( sf::Image &img )
	{
		Image::clear();

		sf::Vector2u  imgsize( img.getSize() );
		Image::w = imgsize.x;
		Image::h = imgsize.y;
		Image::pixel.resize( Image::w * Image::h );

		uint x, y, i;
		i = 0;
		for ( y=0; y < Image::h; y++ )
		{
			for ( x=0; y < Image::w; x++ )
			{
				sf::Color  c = img.getPixel(x, y);
				Image::pixel[i] = Image::addpalette( c );
			}
		}
		Image::has_image = true;
		return true;
	}

	uint Image::addpalette( sf::Color &color )
	{
		uint s = Image::palette.size();
		if ( s > 0 )
		{
			uint i;
			for ( i=0; i < s; i++ )
			{
				if ( Image::palette[i] == color )
					return i;
			}
		}
		Image::palette.push_back( color );
		return s;
	}

	void Image::getimage( sf::Image &img, uint x, uint y, uint w, uint h, uint palette_pos )
	{
		if ( ! Image::has_image )  return;

		uint i, j, k;
		sf::Color pxl;

		for ( j=0; j < h; j++ )
		{
			for ( i=0; i < w; i++ )
			{
				k   = Image::pixel[ Image::getpos( x+i, y+j ) ];
				pxl = Image::palette[ k + palette_pos ];
				img.setPixel(i, j, pxl);
			}
		}
	}

	uint Image::getpos( uint x, uint y )  { return ((y * Image::w) + x); }

	void Image::debug()
	{
		if ( ! Image::has_image )  return;

		uint i;
		printf("--- Image info ---\n");
		printf("palettes:\n");
		for ( i=0; i < Image::palette.size(); i++ )
			printf("> %i : r %x g %x b %x a %x\n",
				i,
				Image::palette[i].r,
				Image::palette[i].g,
				Image::palette[i].b,
				Image::palette[i].a );
		printf("pixels: w %i h %i = %i/%i\n",
			Image::w,
			Image::h,
			Image::w * Image::h,
			Image::pixel.size() );
		printf("\n-----\n");
	}
}
