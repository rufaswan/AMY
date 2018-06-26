#include "header.hpp"
using namespace amy;
extern PlayData DATA;

/*************************************************
 *
 * For display effects. Examples can be
 *  a) Snowing    = Top-to-Bottom scroll
 *  b) Wind Blow  = Left-to-Right scroll
 *  c) Water/Tide = T2B + B2T event script
 *
*/
BGForced::BGForced() {}
BGForced::~BGForced() {}

void BGForced::set_texture( const std::string &file )
{
	printf(">> BGForced::set_texture( %s )\n", file.c_str() );
	Background::repeat = true;
	Background::texture.loadFromImage( DATA.file.data[ file ].get_sfimage() );
	Background::texture.setRepeated(true);

	sf::Vector2u tsize( Background::texture.getSize() );

	Background::tex_w  = tsize.x;
	Background::tex_h  = tsize.y;
	Background::tex_w  = tsize.x;
	Background::tex_h  = tsize.y;

	Background::tex_ready = true;
	BGForced::set_draw();
}

void BGForced::set_draw()
{
	if ( Background::tex_ready )
	{
		uint tx, ty;

		tx = ( WIDTH  / Background::tex_w ) + 1;
		ty = ( HEIGHT / Background::tex_h ) + 1;
		Background::draw_w = tx * Background::tex_w;
		Background::draw_h = ty * Background::tex_h;

		Background::drw_ready = true;
	}
}

void BGForced::draw( sf::RenderTexture &screen )
{
	if ( Background::pos_ready && Background::spd_ready )
	{
		if ( Background::speed_x != 0 )
		{
			// recalculate the pos_x
			// reset back if overflow
			// OUTSIDE of screen only, the value must be <= 0
			Background::draw_x += Background::speed_x;

				while ( Background::draw_x > 0 )
					Background::draw_x -= Background::tex_w;
				while ( Background::draw_x < ( Background::tex_w * -1 ) )
					Background::draw_x += Background::tex_w;

		}

		if ( Background::speed_y != 0 )
		{
			// recalculate the pos_y
			// reset back if overflow
			// OUTSIDE of screen only, the value must be <= 0
			Background::draw_y += Background::speed_y;

				while ( Background::draw_y > 0 )
					Background::draw_y -= Background::tex_h;
				while ( Background::draw_y < ( Background::tex_h * -1 ) )
					Background::draw_y += Background::tex_h;
		}
		//printf("draw x %i y %i\n", Background::draw_x, Background::draw_y);

		sf::Sprite SPR;
		SPR.setTexture( Background::texture );
		SPR.setTextureRect( sf::IntRect(0, 0, Background::draw_w, Background::draw_h) );
		SPR.setPosition( Background::draw_x, Background::draw_y );
		screen.draw( SPR );
	}
}
