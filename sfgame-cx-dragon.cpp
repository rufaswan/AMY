/*
 * Simple Platformer
 */
#include "zsf-common.hpp"
#include "res-ttf.inc"
#include "class_entitydata.hpp"
#include "class_levelmap.hpp"
#include "class_tiledata.hpp"
#include "class_animdata.hpp"
#include "class_entitytools.hpp"

sf::Font     G_SFFONT;
sf::Color    G_BGCOLOR;
sf::Texture  G_BGIMG;
int  G_CAM_ID;
int  G_ANIM_WATER;
int  G_SCRLOCK;
int  G_BRIGHT;
int  G_EVENT;
bool G_EV1_DONE;
bool G_FLOOR1;

EntityList  G_PLAYER_BULLET;
EntityList  G_ENEMY_BULLET;
EntityList  G_ENEMY_LIST;
EntityList  G_EFFECTS;
EntityList  G_DRAGON;

//////////////////////////////////////////////////
#include "cx/cpp_misc.hpp"
#include "cx/cpp_rockx.hpp"

class drgn1 : public EntityData {
	public:
		void collide( EntityData* entity )
		{

			if ( m_rect_x1 > entity->m_rect_x2 )
			{
				G_BRIGHT = rfs::clamp( (m_rect_x1 - entity->m_rect_x2) / 2, 0, 255 );
				return;
			}
			if ( m_rect_x2 < entity->m_rect_x1 )
			{
				G_BRIGHT = rfs::clamp( (entity->m_rect_x1 - m_rect_x2) / 2, 0, 255 );
				return;
			}

			//printf("drgn1 approached\n");
			G_BRIGHT = 0;
		}

		void ai_update()
		{
			move( m_vel_x, 0, 'r' );
			if ( m_rect_x1 > (m_ref_map->m_map_w * 16) )
			{
				if ( G_BRIGHT > 255 )
				{
					m_is_dead = true;
					G_BRIGHT = 255;
				}
			}
		}

		drgn1()
		{
			m_class_name = "drgn1";
			m_anim.load_def("cx/x1_dragon/x1_drgn1.def");
			m_vel_x =  10;
		}
}; // class drgn1

class INV_floor : public EntityData {
	public:
		void ai_update()
		{
			if ( G_FLOOR1 )  return;

			int tx = m_pos_x / 16;
			int ty = m_pos_y / 16;
			int tid = (ty * m_ref_map->m_map_w) + tx;
			m_ref_map->m_map_col[ tid ] = 0;
			m_ref_map->m_map_fg [ tid ] = 0;
			m_ref_map->m_map_fg [ tid + m_ref_map->m_map_w ] = 0;

			G_EFFECTS.push_back( new Explode );
			G_EFFECTS.back()->m_id = -1;
			G_EFFECTS.back()->m_ref_map  = m_ref_map;
			G_EFFECTS.back()->m_ref_tile = m_ref_tile;
			G_EFFECTS.back()->set( m_pos_x, m_pos_y, 'l' );
			m_is_dead = true;
		}

		INV_floor()
		{
			m_class_name = "INV_floor";
			m_anim.load_def("cx/blocks/inv.def");
		}
}; // class BLK_pushL

class BLK_pushL : public EntityData {
	public:
		bool m_start;

		void collide( EntityData* entity )
		{
			if ( m_rect_y1 > entity->m_rect_y2 )  return;
			if ( m_rect_y2 < entity->m_rect_y1 )  return;
			printf("BLK_pushL activated\n");
			m_start = true;

			while ( is_intersect(entity) )
				entity->left(1);
		}

		void ai_update()
		{
			if ( ! m_start )  return;
			left( m_vel_x, 'l' );
		}

		BLK_pushL()
		{
			m_class_name = "BLK_pushL";
			m_anim.load_def("cx/blocks/x3_pushL.def");
			m_vel_x =  6;
			m_start = false;
		}
}; // class BLK_pushL

class BLK_pushR : public EntityData {
	public:
		bool m_start;

		void collide( EntityData* entity )
		{
			if ( m_rect_y1 > entity->m_rect_y2 )  return;
			if ( m_rect_y2 < entity->m_rect_y1 )  return;
			printf("BLK_pushR activated\n");
			m_start = true;

			while ( is_intersect(entity) )
				entity->right(1);
		}

		void ai_update()
		{
			if ( ! m_start )  return;
			right( m_vel_x, 'l' );
		}

		BLK_pushR()
		{
			m_class_name = "BLK_pushR";
			m_anim.load_def("cx/blocks/x3_pushR.def");
			m_vel_x =  6;
			m_start = false;
		}
}; // class BLK_pushL

class waterL : public EntityData {
	public:
		bool m_start;
		int  m_limit;

		void collide( EntityData* entity )
		{
			if ( m_rect_y1 > entity->m_rect_y2 )  return;
			if ( m_rect_y2 < entity->m_rect_y1 )  return;
			printf("waterL activated\n");
			m_start = true;

			if ( is_intersect(entity) )
				entity->m_is_dead = true;
		}

		void ai_update()
		{
			if ( ! m_start )  return;
			if ( m_limit < 0 )   return;

			move( m_vel_x * -1, 0, 'l' );
			m_limit -= m_vel_x;
		}

		waterL()
		{
			m_class_name = "waterL";
			m_anim.load_def("cx/blocks/x3_waterL.def");
			m_vel_x =  10;
			m_start = false;
			m_limit = 256;
		}
}; // class BLK_pushL

class waterR : public EntityData {
	public:
		bool m_start;
		int  m_limit;

		void collide( EntityData* entity )
		{
			if ( m_rect_y1 > entity->m_rect_y2 )  return;
			if ( m_rect_y2 < entity->m_rect_y1 )  return;
			printf("waterR activated\n");
			m_start = true;

			if ( is_intersect(entity) )
				entity->m_is_dead = true;
		}

		void ai_update()
		{
			if ( ! m_start )  return;
			if ( m_limit < 0 )   return;

			move( m_vel_x, 0, 'l' );
			m_limit -= m_vel_x;
		}

		waterR()
		{
			m_class_name = "waterR";
			m_anim.load_def("cx/blocks/x3_waterR.def");
			m_vel_x =  12;
			m_start = false;
			m_limit = 256;
		}
}; // class BLK_pushL

//////////////////////////////////////////////////
int tile_event( LevelMap &map, int pos_x, int pos_y )
{
	int mid = (pos_x / 16) + (pos_y / 16) * map.m_map_w;
	int tid = map.m_map_col[ mid ];
	if ( tid > 0 )
	{
		if ( tid == 4 )   { G_SCRLOCK = 4; } // vlock
		if ( tid == 5 )   { G_SCRLOCK = 5; } // hlock
		if ( tid == 6 )   { G_SCRLOCK = 6; } // room lock
		if ( tid == 7 )   { G_SCRLOCK = 7; } // no lock

		if ( tid == 21 )   { G_EVENT = ( G_EV1_DONE ) ? -1 : 1; }
		if ( tid == 22 )   { G_EVENT = 2; }
		if ( tid == 23 )   { G_EVENT = 3; }
		if ( tid == 24 )   { G_EVENT = 4; }
		if ( tid == 25 )   { G_EVENT = 5; }
		if ( tid == 26 )   { G_EVENT = 6; }
		if ( tid == 27 )   { G_EVENT = 7; }
		if ( tid == 28 )   { G_EVENT = 8; }
		if ( tid == 29 )   { G_EVENT = 9; }

	}

	switch ( G_SCRLOCK )
	{
		case 4:
			map.cam_lock_x( pos_x );
			map.cam_focus_y( pos_y );
			break;
		case 5:
			map.cam_focus_x( pos_x );
			map.cam_lock_y( pos_y );
			break;
		case 6:
			map.cam_lock_x( pos_x );
			map.cam_lock_y( pos_y );
			break;
		case 7:
			map.cam_focus_x( pos_x );
			map.cam_focus_y( pos_y );
			break;
	}
	return tid;
}

void gen_enlist( LevelMap &map, TileData &tile )
{
	if ( map.m_map_obj.empty() )   return;

	int n = map.m_obj_list.size();
	while ( n > 0 )
	{
		n--;
		if ( has_enlist_id( map.m_obj_list[n].id, G_ENEMY_LIST) )
		{
			map.m_obj_list.erase( map.m_obj_list.begin() + n );
		}
		else
		{
			switch( map.m_obj_list[n].ob )
			{
				case 10:
					printf("new BLK_pushL x %d y %d\n", map.m_obj_list[n].x, map.m_obj_list[n].y );
					G_ENEMY_LIST.push_back( new BLK_pushL );
					G_ENEMY_LIST.back()->m_id = map.m_obj_list[n].id;
					G_ENEMY_LIST.back()->m_ref_map  = &map;
					G_ENEMY_LIST.back()->m_ref_tile = &tile;
					G_ENEMY_LIST.back()->set( map.m_obj_list[n].x, map.m_obj_list[n].y, 'l' );
					break;
				case 11:
					printf("new BLK_pushR x %d y %d\n", map.m_obj_list[n].x, map.m_obj_list[n].y );
					G_ENEMY_LIST.push_back( new BLK_pushR );
					G_ENEMY_LIST.back()->m_id = map.m_obj_list[n].id;
					G_ENEMY_LIST.back()->m_ref_map  = &map;
					G_ENEMY_LIST.back()->m_ref_tile = &tile;
					G_ENEMY_LIST.back()->set( map.m_obj_list[n].x, map.m_obj_list[n].y, 'l' );
					break;
				case 13:
					printf("new waterL x %d y %d\n", map.m_obj_list[n].x, map.m_obj_list[n].y );
					G_ENEMY_LIST.push_back( new waterL );
					G_ENEMY_LIST.back()->m_id = map.m_obj_list[n].id;
					G_ENEMY_LIST.back()->m_ref_map  = &map;
					G_ENEMY_LIST.back()->m_ref_tile = &tile;
					G_ENEMY_LIST.back()->set( map.m_obj_list[n].x, map.m_obj_list[n].y, 'l' );
					break;
				case 14:
					printf("new waterR x %d y %d\n", map.m_obj_list[n].x, map.m_obj_list[n].y );
					G_ENEMY_LIST.push_back( new waterR );
					G_ENEMY_LIST.back()->m_id = map.m_obj_list[n].id;
					G_ENEMY_LIST.back()->m_ref_map  = &map;
					G_ENEMY_LIST.back()->m_ref_tile = &tile;
					G_ENEMY_LIST.back()->set( map.m_obj_list[n].x, map.m_obj_list[n].y, 'l' );
					break;

				case 12:
					if ( G_EV1_DONE )
						break;
					printf("new INV_floor x %d y %d\n", map.m_obj_list[n].x, map.m_obj_list[n].y );
					G_ENEMY_LIST.push_back( new INV_floor );
					G_ENEMY_LIST.back()->m_id = map.m_obj_list[n].id;
					G_ENEMY_LIST.back()->m_ref_map  = &map;
					G_ENEMY_LIST.back()->m_ref_tile = &tile;
					G_ENEMY_LIST.back()->set( map.m_obj_list[n].x, map.m_obj_list[n].y, 'l' );
					break;
			}
		}
	} // while ( n > 0 )
}
//////////////////////////////////////////////////
void clear_all()
{
	clear_entities( G_PLAYER_BULLET, true );
	clear_entities( G_ENEMY_BULLET,  true );
	clear_entities( G_ENEMY_LIST,    true );
	clear_entities( G_EFFECTS,       true );
	clear_entities( G_DRAGON,        true );
}

void draw_bgimg( gamesys* sys )
{
	sf::Sprite spr;
		spr.setTexture( G_BGIMG );
		spr.setPosition( 0, -39 );
	sys->m_win.draw(spr);
}

void draw_bg( gamesys* sys, LevelMap &map )
{
	map.change_map(1); // bg
	map.update_map( G_BRIGHT );
	sys->m_win.draw(map);
}

void draw_fg( gamesys* sys, LevelMap &map )
{
	map.change_map(3); // fg
		G_ANIM_WATER = (G_ANIM_WATER + 1) % 9;
		map.anim_fg(  1, 100 + G_ANIM_WATER );
		map.anim_fg(  2, 110 + G_ANIM_WATER );
		map.anim_fg(  3, 120 + G_ANIM_WATER );
		map.anim_fg(  4, 130 + G_ANIM_WATER );
		map.anim_fg(  5, 140 + G_ANIM_WATER );
		map.anim_fg(  6, 150 + G_ANIM_WATER );
		map.anim_fg(  7, 160 + G_ANIM_WATER );
		map.anim_fg(  8, 170 + G_ANIM_WATER );
		map.anim_fg(  9, 180 + G_ANIM_WATER );
		map.anim_fg( 10, 190 + G_ANIM_WATER );
	map.update_map( G_BRIGHT );
	sys->m_win.draw(map);
}

void drgn_charge( gamesys* sys, LevelMap &map, RockX &rock )
{
	if ( G_EV1_DONE )   return;

	printf("drgn_charge()\n");

	map.cam_lock_x( rock.m_pos_x );
	map.cam_lock_y( rock.m_pos_y );

	sf::Texture  drgn;
		drgn.loadFromFile("cx/x1_dragon/x1_d1ver.png");

	sf::Sprite  spr;
		spr.setTexture( drgn );
		spr.setOrigin ( 20, 30 );

	int i;
	for ( i=0; i < 50; i += 1 )
	{
		sys->input_handler();
		sys->event_handler();
		//sys->m_win.clear( G_BGCOLOR );
		draw_bgimg( sys );

		spr.setScale(0.1*i, 0.1*i);
		spr.setPosition( 128, 128 );
		sys->m_win.draw(spr);

		draw_bg( sys, map );

		rock.ai_update();
		rock.draw( sys );

		draw_fg( sys, map );

		sys->m_win.display();
	}

	int camx = map.m_cam_x;
	int camy = map.m_cam_y;
	sf::RectangleShape   rect;
		rect.setSize    ( sf::Vector2f(sys->m_w , sys->m_h) );
		rect.setPosition( 0, 0 );

	for ( i=255; i > 0; i -= 5 )
	{
		sys->input_handler();
		sys->event_handler();
		//sys->m_win.clear( G_BGCOLOR );
		draw_bgimg( sys );

		map.m_cam_x = camx + (2 - (i%3));
		//map.m_cam_y = camy + 0;

		draw_bg( sys, map );

		rock.ai_update();
		rock.draw( sys );

		draw_fg( sys, map );

		rect.setFillColor( sf::Color(255,255,255,i) );
		sys->m_win.draw(rect);

		sys->m_win.display();
	}

	G_FLOOR1 = false;
	printf("DONE drgn_charge()\n");
	G_EV1_DONE = true;
}

void run_event( gamesys* sys, LevelMap &map, RockX &rock )
{
	if ( G_EVENT < 0 )  return;

	printf("run_event( %d )\n", G_EVENT);
	switch ( G_EVENT )
	{
		case 1:
			drgn_charge( sys, map, rock );
			break;
	}
}

void ready_teleport( gamesys* sys, LevelMap &map, RockX &rock )
{
	if ( ! rock.m_is_dead )
		return;

	printf("ready_teleport()\n");
	clear_all();

	int camx = 0;
	int camy = 0;

	map.cam_lock_x( camx+16 );
	map.cam_lock_y( camy+16 );

	READY ready;
		ready.m_ref_map = &map;
		ready.set( camx + sys->m_half_w + 16, camy + sys->m_half_h + 16, 'l' );

	while ( ! ready.is_anim_done() )
	{
		sys->input_handler();
		sys->event_handler();

		//sys->m_win.clear( G_BGCOLOR );

		draw_bgimg( sys );
		draw_bg( sys, map );
		draw_fg( sys, map );

		ready.draw( sys );

		sys->m_win.display();
	}

	rock.m_is_dead = false;
	rock.m_act_fin = false;
	rock.set( camx + sys->m_half_w + 16 , camy , 'r' );

	while( ! rock.m_act_fin )
	{
		sys->input_handler();
		sys->event_handler();
		rock.act_tele_in();

		//sys->m_win.clear( G_BGCOLOR );
		draw_bgimg( sys );
		draw_bg( sys, map );

		rock.draw( sys );

		draw_fg( sys, map );
		sys->m_win.display();
	}

	G_DRAGON.push_back( new drgn1 );
		G_DRAGON.back()->m_ref_map  = &map;
		G_DRAGON.back()->set( -1000, 128+16, 'r' );
}

void game_update( gamesys* sys, LevelMap &map, TileData &tile, RockX &rock )
{
	int n;
	//sys->m_win.clear( G_BGCOLOR );


	int tid = tile_event( map, rock.m_pos_x, rock.m_pos_y );
		if ( tid == 2 )   rock.m_is_dead = true;
		if ( tid == 3 )   rock.m_is_dead = true;
	rock.pl_update( sys );

	draw_bgimg( sys );

	for ( n=0; n < G_DRAGON.size(); n++ )
	{
		G_DRAGON[n]->ai_update();
		G_DRAGON[n]->draw( sys );
	}
	loop_entity( G_DRAGON, &rock );

	draw_bg( sys, map );
	rock.draw( sys );

	for ( n=0; n < G_PLAYER_BULLET.size(); n++ )
	{
		G_PLAYER_BULLET[n]->ai_update();
		G_PLAYER_BULLET[n]->draw( sys );
	}

	int cam_id = map.get_cam_id();
	if ( G_CAM_ID != cam_id )
	{
		map.get_map_obj();
		gen_enlist( map, tile );
		G_CAM_ID = cam_id;
	}

	for ( n=0; n < G_ENEMY_LIST.size(); n++ )
	{
		G_ENEMY_LIST[n]->ai_update();
		G_ENEMY_LIST[n]->draw( sys );
	}
	loop_entity( G_ENEMY_LIST, &rock );
	loop_entity( G_ENEMY_LIST, G_PLAYER_BULLET );

	for ( n=0; n < G_EFFECTS.size(); n++ )
	{
		G_EFFECTS[n]->ai_update();
		G_EFFECTS[n]->draw( sys );
	}

	clear_entities( G_PLAYER_BULLET );
	clear_entities( G_ENEMY_LIST );
	clear_entities( G_EFFECTS );

	draw_fg( sys, map );
}

int main(int argc, char* argv[])
{
	gamesys* sys = new gamesys;
		sys->create_win( 224, 224, "cx/cx_dragon" );

	LevelMap map;
		map.load_file("cx/cx_dragon");
		map.set_pad(-16, -16);

	if ( ! map.m_has_def )  return 0;
	if ( ! map.m_has_lvl )  return 0;

	TileData tile;
		tile.set_wall (1);
		tile.set_spike(2);
		tile.set_pit  (3);
		tile.set_wall (18);

	RockX rock;
		rock.m_ref_map  = &map;
		rock.m_ref_tile = &tile;
		rock.m_is_dead = true;
		//rock.set( sys->m_half_w , 0 , 'r' );

	G_SFFONT.loadFromMemory( RES_VERAMONO_TTF, RES_VERAMONO_TTF_SIZE );
	G_BGIMG.loadFromFile( "cx/x2_bg_crab.png" );
	G_BGCOLOR = sf::Color(16,41,82);
	G_CAM_ID = -1;
	G_ANIM_WATER = 0;
	G_SCRLOCK = 4;
	G_EVENT   = -1;
	G_BRIGHT  = 255;
	G_FLOOR1  = true;
	G_EV1_DONE = false;

	while( sys->m_win.isOpen() )
	{
		sys->input_handler();
		sys->event_handler();

		if ( ! sys->m_paused )
		{
			run_event( sys, map, rock );
			ready_teleport( sys, map, rock );
			game_update( sys, map, tile, rock );
		}

		draw_player_pos( sys, G_SFFONT, &rock );
		//draw_fps_no( sys, G_SFFONT );
		sys->m_win.display();
		sys->m_fps++;
	}

	clear_all();

	printf("Bye!\n");
	return 0;
}

