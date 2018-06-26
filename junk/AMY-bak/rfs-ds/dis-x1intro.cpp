#include "header.hpp"
extern amy::PlayData DATA;

X1Intro::X1Intro()
{
	DATA.file.load("map", "x1-intro.map");
	DATA.file.load("map", "x1-intro-512x30.wall1");
	DATA.file.load("map", "x1-intro-tileset.png");

	DATA.camera.set_pos(0,0);
	DATA.camera.set_rangetile(0,0,511,29);

	DATA.world.clear();
	DATA.world.set_wall ( "map/x1-intro-512x30.wall1", 512, 30 );
	DATA.world.set_tiles( "map/x1-intro-tileset.png" );
	DATA.world.set_mapbg( "map/x1-intro.map" );


}
X1Intro::~X1Intro()
{
	DATA.file.unload("map", "x1-intro.map");
	DATA.file.unload("map", "x1-intro-512x30.wall1");
	DATA.file.unload("map", "x1-intro-tileset.png");
}

void X1Intro::on_enter( sf::RenderWindow &screen ) {}
void X1Intro::on_exit ( sf::RenderWindow &screen ) {}

void X1Intro::updates ( sf::RenderWindow &screen )
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
