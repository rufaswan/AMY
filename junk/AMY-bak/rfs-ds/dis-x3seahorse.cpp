#include "header.hpp"
extern amy::PlayData DATA;

X3Seahorse::X3Seahorse()
{
	DATA.file.load("map", "x3-8seahorse.map");
	DATA.file.load("map", "x3-8seahorse-384x128.wall1");
	DATA.file.load("map", "x3-8seahorse-tileset.png");

	DATA.camera.set_pos(0,1540);
	DATA.camera.set_rangetile(0,0,383,127);

	DATA.world.clear();
	DATA.world.set_wall ( "map/x3-8seahorse-384x128.wall1", 384, 128 );
	DATA.world.set_tiles( "map/x3-8seahorse-tileset.png" );
	DATA.world.set_mapbg( "map/x3-8seahorse.map" );


}
X3Seahorse::~X3Seahorse()
{
	DATA.file.unload("map", "x3-8seahorse.map");
	DATA.file.unload("map", "x3-8seahorse-384x128.wall1");
	DATA.file.unload("map", "x3-8seahorse-tileset.png");
}

void X3Seahorse::on_enter( sf::RenderWindow &screen ) {}
void X3Seahorse::on_exit ( sf::RenderWindow &screen ) {}

void X3Seahorse::updates ( sf::RenderWindow &screen )
{
	if ( DATA.input.is_pressed('U') )  DATA.camera.mv_up();
	if ( DATA.input.is_pressed('D') )  DATA.camera.mv_down();
	if ( DATA.input.is_pressed('L') )  DATA.camera.mv_left();
	if ( DATA.input.is_pressed('R') )  DATA.camera.mv_right();

	if ( DATA.world.is_redraw() )
	{
		// inform other data system when map is updated
	}

	// draw bg and wall
	DATA.world.draw_wall( Display::preScreen_d );
	DATA.world.draw_bg  ( Display::preScreen_n );

	// draw entities and objects

	// draw foreground, animated tiles
	//DATA.world.draw_fg( Display::preScreen_n );
}
