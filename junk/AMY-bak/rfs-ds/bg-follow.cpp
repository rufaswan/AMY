#include "header.hpp"
using namespace amy;
extern PlayData DATA;

/*************************************************
 *
 * Follow the pos of player. Can be:
 *  * scroll only slower or equal speed with camera
 *  * speed_x = 0, force vertical scroll only
 *  * speed_y = 0, force horizontal scroll only
 *  * need position for default y or default x
 *
*/

BGFollow::BGFollow() {}
BGFollow::~BGFollow() {}

void BGFollow::set_texture( const std::string &file )
{
	printf(">> BGFollow::set_texture( %s )\n", file.c_str() );
	Background::repeat = true;
	Background::texture.loadFromImage( DATA.file.data[ file ].get_sfimage() );
	Background::texture.setRepeated(true);

	sf::Vector2u tsize( Background::texture.getSize() );

	Background::tex_w  = tsize.x;
	Background::tex_h  = tsize.y;

	Background::tex_ready = true;
	BGFollow::set_draw();
}

void BGFollow::set_draw()
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

void BGFollow::draw( sf::RenderTexture &screen )
{
	if ( Background::pos_ready && Background::spd_ready )
	{
		if ( Background::speed_x > 0 )
		{
			int tx = ( Background::tex_w ) * -1;
			Background::draw_x = ( DATA.camera.pos_x / Background::speed_x ) * -1;
				while ( Background::draw_x > 0 )
					Background::draw_x -= Background::tex_w;
				while ( Background::draw_x < tx )
					Background::draw_x += Background::tex_w;
		}

		if ( Background::speed_y > 0 )
		{
			int ty = ( Background::tex_h ) * -1;
			Background::draw_y = ( DATA.camera.pos_y / Background::speed_y ) * -1;
				while ( Background::draw_y > 0 )
					Background::draw_y -= Background::tex_h;
				while ( Background::draw_y < ty )
					Background::draw_y += Background::tex_h;
		}

		sf::Sprite SPR;
		SPR.setTexture( Background::texture );
		SPR.setTextureRect( sf::IntRect(0, 0, Background::draw_w, Background::draw_h) );
		SPR.setPosition( Background::draw_x, Background::draw_y );
		screen.draw( SPR );
	}
}
