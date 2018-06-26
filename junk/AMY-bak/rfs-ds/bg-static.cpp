#include "header.hpp"
using namespace amy;
extern PlayData DATA;

/*************************************************
 *
 * For scenery BG like moonlight, sunset, etc. Camera moved, but BG don't.
 *  * can be animated (waterfall)
 *  * need position for BG placement
 *
*/

BGStatic::BGStatic() {}
BGStatic::~BGStatic() {}

void BGStatic::set_texture( const std::string &file, bool repeat )
{
	printf(">> BGStatic::set_texture( %s )\n", file.c_str() );
	Background::repeat = repeat;
	Background::texture.loadFromImage( DATA.file.data[ file ].get_sfimage() );
	Background::texture.setRepeated( repeat );

	sf::Vector2u tsize( Background::texture.getSize() );

	Background::tex_w  = tsize.x;
	Background::tex_h  = tsize.y;

	Background::tex_ready = true;
	BGStatic::set_draw();
}

void BGStatic::set_draw()
{
	if ( Background::tex_ready )
	{
		if ( Background::repeat )
		{
			while ( Background::draw_w <= WIDTH )
				Background::draw_w += Background::tex_w;
			while ( Background::draw_h <= HEIGHT )
				Background::draw_h += Background::tex_h;
		}
		else
		{
			Background::draw_w = Background::tex_w;
			Background::draw_h = Background::tex_h;
		}

		Background::drw_ready = true;
	}
}

void BGStatic::draw( sf::RenderTexture &screen )
{
	if ( Background::pos_ready )
	{
		sf::Sprite SPR;
		SPR.setTexture( Background::texture );
		SPR.setTextureRect( sf::IntRect(0, 0, Background::draw_w, Background::draw_h) );
		SPR.setPosition( Background::draw_x, Background::draw_y );
		screen.draw( SPR );
	}
}
