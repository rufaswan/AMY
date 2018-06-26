#include "define.h"
using namespace amy;
extern State STATE;

StageMARS::~StageMARS() {}
StageMARS::StageMARS()
{
	STATE.CAMERA.set_pos(1270, 200);
	STATE.CAMERA.set_range(0, 0, 15, 15);
	STATE.CAMERA.set_speed(10, 10);
	Stage::set_tileset("map/stg-mars.png", 16, 16);
	Stage::set_map_size(143, 234);
	Stage::set_mapdata_file("map/stg-mars-wall-143x234.map");
	Stage::set_mapdata_file("map/stg-mars-tile-bg-143x234.map", true);
	Stage::set_mapevent( 10,  10,  11,  11, 1); // 1st before
	Stage::set_mapevent(  3, 113,   4, 123, 1); // 1st after
	Stage::set_mapevent(  5, 113,   6, 123, 2); // 2nd before
	Stage::set_mapevent( 71, 117,  72, 119, 2); // 2nd after
	Stage::set_mapevent( 73, 117,  74, 125, 3); // 3rd before
	Stage::set_mapevent( 65, 221,  66, 230, 3); // 3rd after
	Stage::set_mapevent( 67, 221,  68, 230, 4); // 4th before
	Stage::set_mapevent(111, 224, 124, 226, 5); // pre-boss // checkpoint
	Stage::set_mapevent(127, 220, 140, 231, 6); // boss
	Stage::set_mapevent(109, 224, 110, 226, 7); // pre-boss door
	Stage::set_mapevent(125, 224, 126, 226, 8); // boss door

	Stage::set_mapevent(  7, 126,   8, 127, 21); // hidden area in - 1st
	Stage::set_mapevent(  4, 164,   5, 167, 22); // hidden area 2nd in
	Stage::set_mapevent( 59, 167,  60, 170, 23); // hidden area 3rd in
	Stage::set_mapevent( 61, 228,  64, 230, 24); // hidden area out

	STATE.PLAYER->center2pos( HALF_WIDTH, 3*TILE_H );
	run_level();
}

Tile* StageMARS::mapobj_list(int lst, int x, int y)
{
	int l = ( lst == 0 ) ? 0 : lst - 1;

	if      ( STATE.within(l,  0, 15) )		return new Tile(l, x, y, 0);
	else if ( STATE.within(l, 16, 18) )		return new Tile(l, x, y, 0); // door
	else if ( STATE.within(l, 19, 22) )		return new Tile(l, x, y, 1); // spike
	else if ( STATE.within(l, 32, 127) )	return new Tile(l, x, y, 1); // wall area
	else									return new Tile(0, x, y, 0);
}

void StageMARS::event_list(int lst, int x, int y)
{
	switch ( lst )
	{
		case 0: break;
		case 1: STATE.CAMERA.set_range(  0,  0,   15, 124); break; // 1st area, vertical
		case 2: STATE.CAMERA.set_range(  0, 111,  78, 124); break; // 2nd area, horizontal
		case 3: STATE.CAMERA.set_range( 63, 111,  78, 232); break; // 3rd area, vertical
		case 4: STATE.CAMERA.set_range( 63, 219, 109, 232); break; // 4th area, horizontal
		case 5: STATE.CAMERA.set_range(110, 219, 125, 232); break; // pre-boss
		case 6: STATE.CAMERA.set_range(126, 219, 141, 232); break; // boss
		case 7: {
			STATE.CAMERA.set_range(63, 219, 125, 232);
			STATE.PLAYER->force_mv_right(20);
			break; } // pre-boss door
		case 8: {
			STATE.CAMERA.set_range(110, 219, 141, 232);
			STATE.PLAYER->force_mv_right(20);
			break; } // boss door

		case 21: STATE.CAMERA.set_range( 0, 125,  15, 169); break; // hidden area 1st, vertical
		case 22: STATE.CAMERA.set_range( 0, 156,  62, 169); break; // hidden area 2nd, horizontal
		case 23: STATE.CAMERA.set_range(47, 142,  62, 232); break; // hidden area 3rd, vertical
		case 24: {
			STATE.CAMERA.set_range(47, 219, 109, 232);
			STATE.PLAYER->force_mv_right(20);
			break; } // hidden area out

		default: break;
	}
}

//----------------------------------------------------------------
