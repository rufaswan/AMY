#include "define.h"
using namespace amy;
extern State STATE;

Stage::Stage()
{
	Stage::use_bgmap = false;
	STATE.REPLAYDATA.clear();
}
Stage::~Stage()
{
	Stage::unset_mapobject();
}

void Stage::set_tileset( const std::string &file, int col, int row )
{
	if ( DEBUG ) printf(">> setting map file : %s...\n", file.c_str() );
	int x, y;
	int c = 0;
	STATE.add_tileimg(file);
	STATE.TILESET.clear();
	STATE.TILEIND.clear();

	for ( y=0; y < row; y++ )
	{
		for ( x=0; x < col; x++ )
		{
			sf::Texture tmptex;
			tmptex.setSmooth(false);
			tmptex.loadFromImage( STATE.TILEIMG[file], sf::IntRect(x*TILE_W, y*TILE_H, TILE_W, TILE_H) );
			STATE.TILESET.push_back(tmptex);
			STATE.TILEIND.push_back(c);
			c++;
		}
	}
	if ( DEBUG ) printf(">> found %d tile for map\n", c );
}

void Stage::set_map_size(int x, int y)
{
	Stage::map_width = x;
	Stage::map_height = y;
}

void Stage::set_mapdata_file( const std::string &mapfile, bool bg )
{
	if ( DEBUG ) printf(">> setting map data file : %s...\n", mapfile.c_str() );
	int x, y, mapchar;
	FILE *fp;
	if ( bg ) Stage::use_bgmap = true;
	( bg ) ? STATE.MAP_BG_DATA.clear() : STATE.MAP_FG_DATA.clear();

	fp = std::fopen( mapfile.c_str(), "rb" );
	for ( y=0; y < Stage::map_height; y++ )
	{
		std::vector <int> v;
		for ( x=0; x < Stage::map_width; x++ )
		{
			mapchar = std::fgetc(fp);
			v.push_back( mapchar );
			//printf("%i", mapchar );
		}
		//printf("\n");
		( bg ) ? STATE.MAP_BG_DATA.push_back(v) : STATE.MAP_FG_DATA.push_back(v);
	}
	if ( !bg ) Stage::set_mapobject();
}

void Stage::set_mapdata_array( int arr[], int size, bool bg )
{
	if ( DEBUG ) printf(">> setting map object...\n");
	int x, y;
	int c = 0;
	if ( bg ) Stage::use_bgmap = true;
	( bg ) ? STATE.MAP_BG_DATA.clear() : STATE.MAP_FG_DATA.clear();

	for ( y=0; y < Stage::map_height; y++ )
	{
		std::vector <int> v;
		for ( x=0; x < Stage::map_width; x++ )
		{
			//printf("x:%i y:%i c:%i arr[c]:%i\n", x, y, c, arr[c] );
			v.push_back( arr[c] );
			c++;
		}
		( bg ) ? STATE.MAP_BG_DATA.push_back(v) : STATE.MAP_FG_DATA.push_back(v);
	}
	if ( !bg ) Stage::set_mapobject();
}
//----------------------------------------------------------------
void Stage::set_mapobject()
{
	if ( DEBUG ) printf(">> setting map object...\n");
	int x, y, c;
	STATE.MAP_FG_OBJECT.clear();

	for ( y=0; y < Stage::map_height; y++ )
	{
		std::vector <Tile*> v;
		for ( x=0; x < Stage::map_width; x++ )
		{
			c = STATE.MAP_FG_DATA[y][x];
			//printf("x:%i y:%i case:%i\n", x, y, c);
			v.push_back( mapobj_list(c, x, y) );
		}
		STATE.MAP_FG_OBJECT.push_back(v);
	}
}
void Stage::unset_mapobject()
{
	if ( DEBUG ) printf(">> unset map object...\n");
	int x, y;

	for ( y=0; y < Stage::map_height; y++ )
		for ( x=0; x < Stage::map_width; x++ )
			delete STATE.MAP_FG_OBJECT[y][x];

	STATE.MAP_FG_OBJECT.clear();
}
//----------------------------------------------------------------
void Stage::set_mapevent(int left, int top, int right, int bottom, int ev)
{
	if ( DEBUG ) printf(">> set event x: %i-%i y:%i-%i case:%i\n", left, right, top, bottom, ev);
	int x, y;
	for ( y=top; y<=bottom; y++ )
		for ( x=left; x<=right; x++ )
			STATE.MAP_FG_OBJECT[y][x]->set_event(ev);
}
void Stage::unset_mapevent(int left, int top, int right, int bottom)
{
	if ( DEBUG ) printf(">> unset event x: %i-%i y:%i-%i\n", left, right, top, bottom);
	int x, y;
	for ( y=top; y<=bottom; y++ )
		for ( x=left; x<=right; x++ )
			STATE.MAP_FG_OBJECT[y][x]->set_event(0);
}
//----------------------------------------------------------------
void Stage::set_wall(int left, int top, int right, int bottom)
{
	if ( DEBUG ) printf(">> set wall x: %i-%i y:%i-%i\n", left, right, top, bottom);
	int x, y;
	for ( y=top; y<=bottom; y++ )
		for ( x=left; x<=right; x++ )
			STATE.MAP_FG_OBJECT[y][x]->is_wall = true;
}
void Stage::unset_wall(int left, int top, int right, int bottom)
{
	if ( DEBUG ) printf(">> unset wall x: %i-%i y:%i-%i\n", left, right, top, bottom);
	int x, y;
	for ( y=top; y<=bottom; y++ )
		for ( x=left; x<=right; x++ )
			STATE.MAP_FG_OBJECT[y][x]->is_wall = false;
}
//----------------------------------------------------------------
void Stage::draw_map()
{
	int x, y;
	int cam_x = STATE.CAMERA.cam_x / TILE_W;
	int cam_y = STATE.CAMERA.cam_y / TILE_H;

	for ( y=cam_y; y < cam_y+14+1; y++ )
	{
		if ( y > STATE.CAMERA.tile_max_y ) continue;
		for ( x=cam_x; x < cam_x+16+1; x++ )
		{
			if ( x > STATE.CAMERA.tile_max_x ) continue;
			STATE.MAP_FG_OBJECT[y][x]->draw();
		}
	}
}

void Stage::draw_bgmap()
{
	int x, y, t;
	sf::Sprite spr;

	int cam_x = STATE.CAMERA.cam_x / TILE_W;
	int cam_y = STATE.CAMERA.cam_y / TILE_H;

	for ( y=cam_y; y < cam_y+14+1; y++ )
	{
		if ( y > STATE.CAMERA.tile_max_y ) continue;
		for ( x=cam_x; x < cam_x+16+1; x++ )
		{
			if ( x > STATE.CAMERA.tile_max_x ) continue;
			t = STATE.MAP_BG_DATA[y][x];
			if ( t > 0 )
			{
				int mx = ( x * TILE_W ) - STATE.CAMERA.cam_x;
				int my = ( y * TILE_H ) - STATE.CAMERA.cam_y;
				//printf("draw x:%i y:%i = %i to mx:%i my:%i\n", x, y, t, mx, my);

				spr.setTexture( STATE.TILESET[ t-1 ] );
				spr.setPosition(mx, my);
				STATE.SCREEN.draw( spr );
			}
		}
	}
}

void Stage::show_map(bool bg)
{
	int x, y;
	for ( y=0; y < Stage::map_height; y++ )
	{
		for ( x=0; x < Stage::map_width; x++ )
			( bg ) ? printf( "%d ", STATE.MAP_BG_DATA[y][x] ) : printf( "%d ", STATE.MAP_FG_DATA[y][x] );
		printf("\n");
	}
}

void Stage::run_level()
{
	if ( DEBUG )
	{
		printf(">> running level\n");
		if ( Stage::use_bgmap ) printf(">> also use bg object map\n");
		//Stage::show_map();
		//Stage::show_map(true);
	}

	while ( STATE.SCREEN.isOpen() && baseScreen::running )
	{
		// IMPORTANT -MVC CONTROL
		baseScreen::keyinput();			// handle (abstract) keyinput first

		if ( STATE.FRAMECNT_2 )
		{
			// IMPORTANT - MVC MODEL
			Stage::run_event();			// dynamic use child's run_event()
			STATE.PLAYER->update_player();		// update game objects
			STATE.CAMERA.update_player();;

			// IMPORTANT - MVC VIEW
			STATE.SCREEN.clear();		// erase the leftover sprite

			if ( Stage::use_bgmap ) Stage::draw_bgmap();
			STATE.PLAYER->draw();
			Stage::draw_map();
		}

		// IMPORTANT - display and reset for next loop
		// NOTE - display != draw
		STATE.SCREEN.display();
		STATE.update();
	}
}

void Stage::run_event()
{
	int px = STATE.PLAYER->center_x / TILE_W;
	int py = STATE.PLAYER->center_y / TILE_H;
	int ev = STATE.MAP_FG_OBJECT[py][px]->get_event();
	event_list( ev, px, py );
}
//----------------------------------------------------------------
amy::Tile* Stage::mapobj_list(int l, int x, int y)
{
	return new Tile(1, x, y, 1);
}
void Stage::event_list(int l, int x, int y)		{}
