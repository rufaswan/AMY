#include "define.h"
using namespace amy;
extern State STATE;

Tile::Tile() {}
Tile::~Tile() {}

Tile::Tile(int tile, int x, int y, int type)
{
	Tile::event_no = 0;
	Tile::is_wall	= false;
	Tile::is_spike	= false;
	Tile::set_tile(tile, x, y, type);
}

void Tile::set_tile(int tile, int x, int y, int type)
{
	switch (type)
	{
		// 0 - normal walkable area, fake walls
		// 1 - normal non-walkable area
		// 2 - normal spike / instant death area
		// 3 - slope up   ( / ) 2 of 1
		// 4 - slope up   ( / ) 2 of 2
		// 5 - slope down ( \ ) 2 of 1
		// 6 - slope down ( \ ) 2 of 2
		// 7 - convey belt, not walkable, push nearby tile, direction left
		// 8 - convey belt, not walkable, push nearby tile, direction right
		// 9 - waterway, walkable, push direction left
		// 10 - waterway, walkable, push direction right
		// 11 - gravity half,   walkable, jump *2 (underwater)
		// 12 - gravity double, walkable, jump /2 (quicksand)
		case 0:		break;
		case 1:		{ Tile::is_wall = true;		break; }
		case 2:		{ Tile::is_wall = true;		Tile::is_spike = true;		break; }
	}

	Tile::tile = tile;
	Tile::map_x = x;
	Tile::map_y = y;
}

void Tile::draw()
{
	// tile 0 is blank, no need to draw, just skip it
	if ( Tile::tile > 0 )
	{
		sf::Sprite spr;
		int x = ( Tile::map_x * TILE_W ) - STATE.CAMERA.cam_x;
		int y = ( Tile::map_y * TILE_H ) - STATE.CAMERA.cam_y;

		spr.setTexture( STATE.TILESET[ STATE.TILEIND[ Tile::tile ] ] );
		spr.setPosition(x, y);
		STATE.SCREEN.draw( spr );
	}
}

void Tile::set_event(int ev)	{ Tile::event_no = ev; }
int  Tile::get_event()			{ return Tile::event_no; }
