#include "header.hpp"
using namespace amy;
extern PlayData DATA;

ABat::ABat( int x, int y )
{
	// size   = 32x32, square (all)
	// rect   = 16x16, center
	// pos_xy = center
	// animations:
	//   0- 3 idle
	//   8-12 moving - horizontal
	//   16   moving - vertical

	// base state
		Entity::set_basepos(x, y);
		Entity::type = "bat";
		Entity::fps  = 30;

	// load texture
		DATA.file.load("actor", "bat-fixed.png");
		DATA.sfimage = DATA.file.data[ "actor/bat-fixed.png" ].get_sfimage();

		int i, j;
		for ( j=0; j<3; j++ )
		{
			for ( i=0; i<8; i++ )
			{
				sf::Texture tmptex;
				tmptex.setSmooth(false);
				tmptex.loadFromImage( DATA.sfimage, sf::IntRect( i*32, j*32, 32, 32) );
				Entity::tileset.push_back( tmptex );
			}
		}

/*
Entity::tileset
			amy::RectData rect;
*/

}
ABat::~ABat() {}

void ABat::update( amy::World &world )
{
	if ( !Entity::timer.can_fps_update( Entity::fps ) )
		return;

}

void ABat::update_npc( amy::World &world )
{
	if ( !Entity::timer.can_fps_update( Entity::fps ) )
		return;

}

void ABat::draw( sf::RenderTexture &preN, sf::RenderTexture &preD )
{
}
