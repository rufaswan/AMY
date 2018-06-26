#include "define.h"
using namespace amy;
extern State STATE;

StageSEC::StageSEC()
{
	STATE.CAMERA.set_pos(0, 0);
	STATE.CAMERA.set_range(0, 0, 31, 13);
	STATE.CAMERA.set_speed(10, 10);
	Stage::set_tileset("map/rainbow.png", 11, 1);
	Stage::set_map_size(81, 15);
	Stage::set_mapdata_file("map/stg-secret-wall-81x15.map");
	Stage::set_mapdata_file("map/stg-secret-tile-bg-81x15.map", true);

	STATE.PLAYER->center2pos( HALF_WIDTH, 3*TILE_H );
	run_level();

	/*
	trigger start menu any time except cutscenes

	set events
		trigger point
	allow control / cutscene mode?
	oneoff events
		READY and teleport in
		miniboss arena
		boss door
		boss intro
		boss outro
		game over
		cutscenes (actors & position)
	repeat events
		change max & min camera
		navi dialog

	loop all event
		// save rect testing for the last, in favour of much faster test
		if triggerred, skip
			skip
		else
			if repeat
			if within trigger point

	*/

	/*
	enemies
		if isdead
			if outcamera
				if respawn
					re-init (original x, y pos + hp + action)
				else // boss-class
					do nothing, they are already dead
			else // incamera
				wait for camera move
		else // not dead
			if incamera
				random action / attack
			else // outcamera
				do nothing / off-screen attacks (unfair)
	*/
}
StageSEC::~StageSEC() {}

Tile* StageSEC::mapobj_list(int lst, int x, int y)
{
	switch( lst )
	{
		case 0:		return new Tile(0, x, y, 0);	break;
		case 1:		return new Tile(0, x, y, 0);	break;
		case 2:		return new Tile(1, x, y, 1);	break;
		case 3:		return new Tile(2, x, y, 1);	break;
		case 4:		return new Tile(3, x, y, 1);	break;
		case 5:		return new Tile(4, x, y, 1);	break;
		case 6:		return new Tile(5, x, y, 1);	break;
		case 7:		return new Tile(6, x, y, 1);	break;
		case 8:		return new Tile(7, x, y, 1);	break;
		case 9:		return new Tile(8, x, y, 1);	break;
		case 10:	return new Tile(9, x, y, 1);	break;
		case 11:	return new Tile(10, x, y, 1);	break;
		case 12:	return new Tile(11, x, y, 1);	break;
		case 13:	return new Tile(12, x, y, 1);	break;
		case 14:	return new Tile(13, x, y, 1);	break;
		default:	return new Tile(0, x, y, 0);	break;
	}
}

void StageSEC::event_list(int lst, int x, int y)
{
}
//----------------------------------------------------------------
