#include "header.hpp"
using namespace amy;
extern PlayData DATA;

STGDemo::STGDemo()
{
	// setting stage files
		DATA.file.load("map", "twilight-tiles.png");
		DATA.file.load("map", "debugtile-16x16.png");
		DATA.file.load("map", "exterior-parallaxBG1.png");
		DATA.file.load("map", "exterior-parallaxBG2.png");
		DATA.file.load("map", "twilight-wall-464x28.map");

	// setting camera
		DATA.camera.set_rangetile(16, 16, 0, 0, 190, 14);

	// setting backgrounds
		STGDemo::layer1.set_texture( "map/exterior-parallaxBG1.png" );
		STGDemo::layer1.set_speed(5, 0);
		STGDemo::layer1.set_location( Background::CC );

		STGDemo::layer2.set_texture( "map/exterior-parallaxBG2.png" );
		STGDemo::layer2.set_location( Background::CC );

	// setting world
		DATA.world.clear();
		DATA.world.set_tileset(   "map/twilight-tiles.png", 16, 16, 16, 16 );
		DATA.world.set_tiledebug( "map/debugtile-16x16.png", 8, 4 );
		DATA.world.set_mapfile( "map/twilight-wall-200x14.map", 200, 14 );

			DATA.world.set_tiletype( 16, 8 );
			DATA.world.set_tiletype( 17, 8 );
			DATA.world.set_tiletype( 18, 8 );
			DATA.world.set_tiletype( 25, 8 );
			DATA.world.set_tiletype( 26, 8 );
			DATA.world.set_tiletype( 27, 8 );
			DATA.world.set_tiletype( 32, 8 );
			DATA.world.set_tiletype( 33, 8 );
			DATA.world.set_tiletype( 34, 8 );
			DATA.world.set_tiletype( 41, 8 );
			DATA.world.set_tiletype( 42, 8 );
			DATA.world.set_tiletype( 43, 8 );
			DATA.world.set_tiletype( 45, 8 );
			DATA.world.set_tiletype( 46, 8 );
			DATA.world.set_tiletype( 47, 8 );
			DATA.world.set_tiletype( 57, 8 );
			DATA.world.set_tiletype( 58, 8 );
			DATA.world.set_tiletype( 59, 8 );
			DATA.world.set_tiletype( 61, 8 );
			DATA.world.set_tiletype( 91, 8 );
			DATA.world.set_tiletype( 92, 8 );
			DATA.world.set_tiletype( 93, 8 );
			DATA.world.set_tiletype( 106, 8 );
			DATA.world.set_tiletype( 107, 8 );
			DATA.world.set_tiletype( 108, 8 );
			DATA.world.set_tiletype( 109, 8 );
			DATA.world.set_tiletype( 110, 8 );
			DATA.world.set_tiletype( 122, 8 );
			DATA.world.set_tiletype( 123, 8 );
			DATA.world.set_tiletype( 124, 8 );
			DATA.world.set_tiletype( 125, 8 );
			DATA.world.set_tiletype( 126, 8 );
			DATA.world.set_tiletype( 138, 8 );
			DATA.world.set_tiletype( 139, 8 );
			DATA.world.set_tiletype( 140, 8 );
			DATA.world.set_tiletype( 141, 8 );
			DATA.world.set_tiletype( 142, 8 );
			DATA.world.set_tiletype( 154, 8 );
			DATA.world.set_tiletype( 155, 8 );
			DATA.world.set_tiletype( 156, 8 );
			DATA.world.set_tiletype( 157, 8 );
			DATA.world.set_tiletype( 158, 8 );
			DATA.world.set_tiletype( 170, 8 );
			DATA.world.set_tiletype( 171, 8 );
			DATA.world.set_tiletype( 172, 8 );
			DATA.world.set_tiletype( 173, 8 );
			DATA.world.set_tiletype( 174, 8 );

		//DATA.world.print_tiletype();
		DATA.world.set_mapobj();

	// setting player + enemies
		//DATA.entity.add( new AElisa( WIDTH / 2, HEIGHT / 2 ), EnGroup::PLAYER );
		DATA.entity.add( new ABat( WIDTH / 2, HEIGHT / 2 ), EnGroup::PLAYER );
}
STGDemo::~STGDemo()
{
	// cleanup loaded files
		DATA.file.unload("map", "twilight-tiles.png");
		DATA.file.unload("map", "debugtile-16x16.png");
		DATA.file.unload("map", "exterior-parallaxBG1.png");
		DATA.file.unload("map", "exterior-parallaxBG2.png");
		DATA.file.unload("map", "twilight-wall-200x14.map");
}

void STGDemo::updates( sf::RenderWindow &screen )
{
	if ( DATA.cmd.is_pressed('L') )
		DATA.camera.mv_left();
	if ( DATA.cmd.is_pressed('R') )
		DATA.camera.mv_right();
	if ( DATA.cmd.is_holded('a', 60) )
		DATA.camera.set_pos(0, 0);

	STGDemo::layer1.draw( Display::preScreen_n );
	STGDemo::layer2.draw( Display::preScreen_n );
	DATA.world.draw_map( Display::preScreen_n, Display::preScreen_d );

}

SCRStartMenu::SCRStartMenu() {}
SCRStartMenu::~SCRStartMenu() {}
