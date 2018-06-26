#include "header.hpp"
using namespace amy;
extern amy::PlayData DATA;

Tiles::Tiles() {}
Tiles::~Tiles() {}

void Tiles::set_tileset( const std::string &file )
{
	printf(">> Tiles::set_tileset( %s )\n", file.c_str() );
	DATA.sfimage = DATA.file.data[ file ].get_sfimage();

	sf::Vector2u imgsize( DATA.sfimage.getSize() );
	uint col = imgsize.x / TILE_W;
	uint row = imgsize.y / TILE_H;
	uint i, j, k;

	Tiles::tiletex.clear();
	Tiles::tiletex.resize( col * row );
	k = 0;
	for ( i=0; i < row; i++ )
	{
		for ( j=0; j < col; j++ )
		{
			sf::Texture tmptex;
			tmptex.setSmooth(false);
			tmptex.loadFromImage( DATA.sfimage, sf::IntRect( j*TILE_W, i*TILE_H, TILE_W, TILE_H) );

			Tiles::tiletex[k] = tmptex;
			k++;
		}
	}

	printf("Tiles::tiletex.size() : %i\n", Tiles::tiletex.size() );
}

void Tiles::draw( sf::RenderTexture &screen, uint type, int pos_x, int pos_y )
{
	if ( type == 0 ) return;
	//if ( type >= Tiles::tiletex.size() ) return;

	sf::Sprite spr;
	spr.setTexture( Tiles::tiletex[ type ] );
	spr.setPosition( pos_x, pos_y );
	screen.draw( spr );
}
