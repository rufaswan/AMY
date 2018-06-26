/*
 * Level Editor w/.DEF + .LVL files
 */
#include "zsf-common.hpp"
#include "class_levelmap.hpp"
#include "res-ttf.inc"

sf::Font      SFFONT;
sf::Texture   EDMENU;

int g_selected;


void draw_cam_pos( gamesys* sys, sf::Font &font, LevelMap &map )
{
	char cam[32];
		rfs::memset( cam, 32 );
		sprintf(cam, "CAM %d,%d (%d,%d)", map.m_cam_x, map.m_cam_y, map.m_cam_x/16, map.m_cam_y/16 );

	sf::Text   text(cam, font);
		text.setCharacterSize(16);
	sys->m_win.draw(text);
}


void game_update( gamesys* sys, LevelMap &map )
{
	//printf("game_update()\n");
	sys->m_win.clear( sf::Color::White );

	if ( sys->m_Dup    )   map.cam_move(0, -1);
	if ( sys->m_Ddown  )   map.cam_move(0,  1);
	if ( sys->m_Dleft  )   map.cam_move(-1, 0);
	if ( sys->m_Dright )   map.cam_move( 1, 0);

	if ( is_sfkey('1') )   map.change_map(0);
	if ( is_sfkey('2') )   map.change_map(1);
	if ( is_sfkey('3') )   map.change_map(2);
	if ( is_sfkey('4') )   map.change_map(3);

	map.update_map();
	sys->m_win.draw(map);

	sf::Sprite spr;
		spr.setTexture(EDMENU);
		sys->m_win.draw(spr);

	map.update_tileset(288, 32, 128, 240);
	sys->m_win.draw(map);

	return;
}

void area_handler( int tx, int ty, LevelMap &map )
{
	int x1, y1, id;

	// 256+16 , 256+16 = 16+1 x 16+1
	if ( rfs::in_range(tx, 1, 16 ) )
	{
		if ( rfs::in_range(ty, 1, 16 ) )
		{
			x1 = (map.m_cam_x + tx - 1) % map.m_map_w;
			y1 = (map.m_cam_y + ty - 1) % map.m_map_h;
			id = (y1 * map.m_map_w) + x1;
			printf("MapArea : %d,%d [REL %d] , SEL %d\n", tx-1, ty-1, id, g_selected);
			map.set_map_selected( id, g_selected );
		}
	}
	// 128+288 x 240+32 = 8+18 , 15+2
	if ( rfs::in_range(tx, 18, 26) )
	{
		if ( rfs::in_range(ty, 2, 17) )
		{
			x1 = tx - 18;
			y1 = ty - 2;
			id = (y1 * 8) + x1;
			printf("TileArea : %d,%d [REL %d] , SEL %d\n", tx-18, ty-2, id, g_selected);
			g_selected = id;
		}
	}
	// 64+288 x 16+272 = 4+18 , 1+17
	if ( rfs::in_range(tx, 18, 22) )
	{
		if ( rfs::in_range(ty, 17, 18) )
		{
			map.save_lvl();
		}
	}
}

void mouse_handler( gamesys* sys, LevelMap &map )
{
	if ( sf::Mouse::isButtonPressed(sf::Mouse::Left) )
	{
		sf::Vector2i   pos = sf::Mouse::getPosition( sys->m_win );
		int tilesz = 16 * sys->m_zoom;
		area_handler( pos.x / tilesz, pos.y / tilesz, map );
	}
}

int main(int argc, char* argv[])
{
	if ( argc == 1 )   return printf("usage : %s  PREFIX_for_DEF_and_LVL\n", argv[0] );

	LevelMap map;
		map.load_file( argv[1] );
		map.set_pad(16, 16);

	gamesys* sys = new gamesys;
		sys->create_win( 416, 288, argv[0] );

	SFFONT.loadFromMemory( RES_VERAMONO_TTF, RES_VERAMONO_TTF_SIZE );
	g_selected = 0;

	//sf::Thread thd_fps( &fps_watch, op);
		//thd_fps.launch();

	EDMENU.loadFromFile("res-level-editor-16.png");

	if ( ! map.m_has_def )   return printf("FATAL : DEF file not found\n");

	while( sys->m_win.isOpen() )
	{
		sys->input_handler();
		sys->event_handler();

		if ( ! sys->m_paused )
		{
			mouse_handler( sys, map );
			game_update( sys, map );
			draw_cam_pos( sys, SFFONT, map );
		}

		sys->m_win.display();
		//sys->m_fps++;
	}

	printf("Bye!\n");
	return 0;
}

