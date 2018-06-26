#include "header.hpp"
using namespace amy;
extern PlayData DATA;

World::World() {}
World::~World() {}

void World::clear()
{
	World::ready      = false;
	World::wall_ready = false;
	World::tile_ready = false;
	World::has_bg     = false;
	World::has_fg     = false;
	World::mapid = -1;
	World::map_w = 0;
	World::map_h = 0;
}

void World::set_wall( const std::string &file, uint map_w, uint map_h )
{
	printf(">> World::set_wall( %s, %i, %i )\n", file.c_str(), map_w, map_h );
	World::map_w = map_w;
	World::map_h = map_h;

	// unlike drawing tiles, wall map is ONE BYTE per tile
	uint size = map_w * map_h;
	uint i;
	World::map_wall.clear();
	World::map_wall.resize( size );
	for ( i=0; i < size; i++ )
		World::map_wall[i] = DATA.file.data[ file ].buffer[i];

	printf("World::map_wall EXPECTED : %i LOADED : %i\n", DATA.file.data[ file ].buffer.size(), World::map_wall.size() );
	World::wall_ready = true;
	World::rend_wall.create( WIDTH + TILE_W, HEIGHT + TILE_H );
}

void World::set_mapbg( const std::string &file )
{
	printf(">> World::set_mapbg( %s )\n", file.c_str() );
	if ( ! World::wall_ready )  return;

	// unlike wall tiles, drawing map is TWO BYTE per tile
	uint size = World::map_w * World::map_h;
	uint i, b1, b2;
	World::map_bg.clear();
	World::map_bg.resize( size );
	World::map_int.clear();
	World::map_int.resize( size );
	for ( i=0; i < size; i++ )
	{
		b1 = ( DATA.file.data[ file ].buffer[ i*2+0 ] ) << 0;
		b2 = ( DATA.file.data[ file ].buffer[ i*2+1 ] ) << 8;
		World::map_bg[i]  = b1 + b2;
		World::map_int[i] = i;
	}

	printf("World::map_bg EXPECTED : %i LOADED : %i\n", DATA.file.data[ file ].buffer.size() / 2, World::map_bg.size() );
	World::has_bg = true;
	World::rend_bg.create( WIDTH + TILE_W, HEIGHT + TILE_H );
}

void World::set_mapfg( const std::string &file )
{
	printf(">> World::set_mapfg( %s )\n", file.c_str() );
	if ( ! World::wall_ready )  return;

	// unlike wall tiles, drawing map is TWO BYTE per tile
	uint size = World::map_w * World::map_h;
	uint i;
	World::map_fg.clear();
	World::map_fg.resize( size );
	for ( i=0; i < size; i++ )
		World::map_fg[i] = ( DATA.file.data[ file ].buffer[i] > 0 ) ? true : false;

	printf("World::map_fg EXPECTED : %i LOADED : %i\n", DATA.file.data[ file ].buffer.size(), World::map_fg.size() );
	World::has_fg = true;
	World::rend_fg.create( WIDTH + TILE_W, HEIGHT + TILE_H );
}

void World::set_tiles( const std::string &file )
{
	printf(">> World::set_tiles( %s )\n", file.c_str() );

	DATA.sfimage = DATA.file.data[ file ].get_sfimage();
	DATA.utils.set_tileset( DATA.sfimage, TILE_W, TILE_H, World::map_tile );

	printf("World::map_tile.size() : %i\n", World::map_tile.size() );
	World::tile_ready = true;
}

bool World::is_redraw()
{
	World::map_load.clear();

	uint cam_x = DATA.camera.pos_x / TILE_W;
	uint cam_y = DATA.camera.pos_y / TILE_H;

	int mapid = World::get_maptile( cam_x, cam_y );
	if ( World::mapid != mapid )
	{
		//printf("redraw map %i\n", mapid);
		// check which new tiles are loaded

		// re-render wall + bg
		// fg is animated, so it is re-render on every frame
		World::render_wall( cam_x, cam_y );
		World::render_bg  ( cam_x, cam_y );

		// update map id
		World::mapid = mapid;
		return true;
	}
	return false;
}

void World::render_wall( uint cam_x, uint cam_y )
{
	if ( ! World::wall_ready ) return;

	World::rend_wall.clear( sf::Color(0,0,0,0) );
	uint i, j, k;
	uint col = WIDTH / TILE_W + 1;
	uint row = WIDTH / TILE_H + 1;
	for ( i=0; i < row; i++ )
	{
		for ( j=0; j < col; j++ )
		{
			k = World::get_maptile( cam_x + j, cam_y + i );
			k = ( k < World::map_bg.size() ) ? k : 0;

			DATA.tile.draw( World::rend_wall, World::map_wall[k], j*TILE_W, i*TILE_H );
		}
	}
	World::rend_wall.display();
}

void World::render_bg( uint cam_x, uint cam_y )
{
	if ( ! World::has_bg )     return;
	if ( ! World::tile_ready ) return;

	World::rend_bg.clear( sf::Color(0,0,0,0) );
	uint i, j, k;
	uint col = WIDTH / TILE_W + 1;
	uint row = WIDTH / TILE_H + 1;
	for ( i=0; i < row; i++ )
	{
		for ( j=0; j < col; j++ )
		{
			k = World::get_maptile( cam_x + j, cam_y + i );
			k = ( k < World::map_bg.size() ) ? k : 0;
			sf::Sprite spr;

			spr.setTexture( World::map_tile[ World::map_bg[ k ] ] );
			spr.setPosition( j*TILE_W, i*TILE_H );

			World::rend_bg.draw( spr );
		}
	}
	World::rend_bg.display();
}

void World::render_fg( uint cam_x, uint cam_y )
{
	if ( ! World::has_bg )     return;
	if ( ! World::has_fg )     return;
	if ( ! World::tile_ready ) return;

	World::rend_fg.clear( sf::Color(0,0,0,0) );
	uint i, j, k;
	uint col = WIDTH / TILE_W + 1;
	uint row = WIDTH / TILE_H + 1;
	for ( i=0; i < row; i++ )
	{
		for ( j=0; j < col; j++ )
		{
			k = World::get_maptile( cam_x + j, cam_y + i );
			k = ( k < World::map_fg.size() ) ? k : 0;

			// draw only the required
			if ( World::map_fg[k] )
			{
				sf::Sprite spr;

				spr.setTexture( World::map_tile[ World::map_int[ World::map_bg[k] ] ] );
				spr.setPosition( j*TILE_W, i*TILE_H );

				World::rend_fg.draw( spr );
			}
		}
	}
	World::rend_fg.display();
}

void World::draw_wall( sf::RenderTexture &screen )
{
	if ( ! World::wall_ready ) return;

	int pos_x = ( DATA.camera.pos_x % TILE_W ) * -1;
	int pos_y = ( DATA.camera.pos_y % TILE_H ) * -1;

	// actual drawing to screen
	sf::Sprite bgspr;
	bgspr.setTexture( World::rend_wall.getTexture() );
	bgspr.setPosition( pos_x, pos_y );
	screen.draw( bgspr );
}

void World::draw_bg( sf::RenderTexture &screen )
{
	if ( ! World::has_bg )     return;
	if ( ! World::tile_ready ) return;

	int pos_x = ( DATA.camera.pos_x % TILE_W ) * -1;
	int pos_y = ( DATA.camera.pos_y % TILE_H ) * -1;

	// actual drawing to screen
	sf::Sprite bgspr;
	bgspr.setTexture( World::rend_bg.getTexture() );
	bgspr.setPosition( pos_x, pos_y );
	screen.draw( bgspr );
}

void World::draw_fg( sf::RenderTexture &screen )
{
	if ( ! World::has_fg ) return;
	if ( ! World::tile_ready ) return;

	int pos_x = ( DATA.camera.pos_x % TILE_W ) * -1;
	int pos_y = ( DATA.camera.pos_y % TILE_H ) * -1;

	// actual drawing to screen
	sf::Sprite fgspr;
	fgspr.setTexture( World::rend_fg.getTexture() );
	fgspr.setPosition( pos_x, pos_y );
	screen.draw( fgspr );
}

int World::get_maptile( int mx, int my )
{
	if ( ! World::wall_ready ) return 0;
	if ( mx > World::map_w ) return 0;
	if ( my > World::map_h ) return 0;

	int mt = ( my * World::map_w ) + mx;
	return mt;
}

void World::set_mapint( int t1, int t2, int frame )
{
	if ( ! World::has_bg )  return;
	World::map_int[ t1 ] = t2 + frame;
}

void World::set_mapint( int x1, int y1, int x2, int y2, int frame )
{
	int t1 = World::get_maptile( x1, y1 );
	int t2 = World::get_maptile( x2, y2 );
	World::set_mapint( t1, t2, frame );
}
