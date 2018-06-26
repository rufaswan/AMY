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

sf::Font     SFFONT;
sf::Color    BGCOLOR;
int  CAM_ID;
int  ANIM_WATER;
int  SCRLOCK;
int  BRIGHT;
int  BRIGHT_ADJ;

EntityList  PLAYER_BULLET;
EntityList  ENEMY_BULLET;
EntityList  ENEMY_LIST;
EntityList  EFFECTS;

//////////////////////////////////////////////////
#include "cx/cpp_misc.hpp"
#include "cx/cpp_rockx.hpp"

class Ball : public EntityData {
	public:
		enum STATE {
			SHOT        = 1,
			SHOT_UP     = 3,
			END_SHOT    = 2,
			END_SHOT_UP = 4,
			ANM_SHOT    = 8,
			ANM_SHOT_UP = 10,
			HOP = 7,
			WAIT = 5
		};
		void collide( EntityData* entity )
		{
			if ( m_is_dead )
				return;
			if ( ! is_intersect( entity ) )
				return;

			EFFECTS.push_back( new Explode );
			EFFECTS.back()->m_id = -1;
			EFFECTS.back()->m_ref_map  = m_ref_map;
			EFFECTS.back()->m_ref_tile = m_ref_tile;
			EFFECTS.back()->set( m_pos_x, m_pos_y - 16, 'l' );
			m_is_dead = true;
		}
		void ai_update()
		{
			change_state( WAIT );
			if ( ! m_is_wall_down )
				down(4, 'u');
		}
		Ball()
		{
			m_class_name = "Ball";
			m_anim.load_def("cx/004_ball/ball.def");
		}
}; // class Ball

//////////////////////////////////////////////////
int tile_event( LevelMap &map, int pos_x, int pos_y )
{
	int mid = (pos_x / 16) + (pos_y / 16) * map.m_map_w;
	int tid = map.m_map_col[ mid ];
	if ( tid > 0 )
	{
		if ( tid == 4 )   { SCRLOCK = 4; } // vlock
		if ( tid == 5 )   { SCRLOCK = 5; } // hlock
		if ( tid == 6 )   { SCRLOCK = 6; } // room lock
	}

	switch ( SCRLOCK )
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
		if ( has_enlist_id( map.m_obj_list[n].id, ENEMY_LIST) )
		{
			map.m_obj_list.erase( map.m_obj_list.begin() + n );
		}
		else
		{
			switch( map.m_obj_list[n].ob )
			{
				case 4:
					printf("new Ball x %d y %d\n", map.m_obj_list[n].x + 32, map.m_obj_list[n].y + 55 );
					ENEMY_LIST.push_back( new Ball );
					ENEMY_LIST.back()->m_id = map.m_obj_list[n].id;
					ENEMY_LIST.back()->m_ref_map  = &map;
					ENEMY_LIST.back()->m_ref_tile = &tile;
					ENEMY_LIST.back()->set( map.m_obj_list[n].x + 32, map.m_obj_list[n].y + 55, 'l' );
					break;
			}
		}
	} // while ( n > 0 )
}
//////////////////////////////////////////////////
void clear_all()
{
	clear_entities( PLAYER_BULLET, true );
	clear_entities( ENEMY_BULLET,  true );
	clear_entities( ENEMY_LIST,    true );
	clear_entities( EFFECTS,       true );
}

void draw_bg( gamesys* sys, LevelMap &map )
{
	map.change_map(1); // bg
	map.update_map( BRIGHT );
	sys->m_win.draw(map);

	BRIGHT += BRIGHT_ADJ;
	if ( BRIGHT <   0 )   { BRIGHT =   0; BRIGHT_ADJ *= -1; }
	if ( BRIGHT > 255 )   { BRIGHT = 255; BRIGHT_ADJ *= -1; }
}

void draw_fg( gamesys* sys, LevelMap &map )
{
	map.change_map(3); // fg
		ANIM_WATER = (ANIM_WATER + 1) % 9;
		map.anim_fg(  1, 100 + ANIM_WATER );
		map.anim_fg(  2, 110 + ANIM_WATER );
		map.anim_fg(  3, 120 + ANIM_WATER );
		map.anim_fg(  4, 130 + ANIM_WATER );
		map.anim_fg(  5, 140 + ANIM_WATER );
		map.anim_fg(  6, 150 + ANIM_WATER );
		map.anim_fg(  7, 160 + ANIM_WATER );
		map.anim_fg(  8, 170 + ANIM_WATER );
		map.anim_fg(  9, 180 + ANIM_WATER );
		map.anim_fg( 10, 190 + ANIM_WATER );
	map.update_map( BRIGHT );
	sys->m_win.draw(map);
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

		sys->m_win.clear( BGCOLOR );

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

		sys->m_win.clear( BGCOLOR );

		rock.draw( sys );

		draw_bg( sys, map );
		draw_fg( sys, map );
		sys->m_win.display();
	}

}

void game_update( gamesys* sys, LevelMap &map, TileData &tile, RockX &rock )
{
	ready_teleport( sys, map, rock );

	int n;
	sys->m_win.clear( BGCOLOR );

	int tid = tile_event( map, rock.m_pos_x, rock.m_pos_y );
		if ( tid == 2 )   rock.m_is_dead = true;
		if ( tid == 3 )   rock.m_is_dead = true;
	rock.pl_update( sys );
	rock.draw( sys );

	for ( n=0; n < PLAYER_BULLET.size(); n++ )
	{
		PLAYER_BULLET[n]->ai_update();
		PLAYER_BULLET[n]->draw( sys );
	}

	int cam_id = map.get_cam_id();
	if ( CAM_ID != cam_id )
	{
		map.get_map_obj();
		gen_enlist( map, tile );
		CAM_ID = cam_id;
	}

	for ( n=0; n < ENEMY_LIST.size(); n++ )
	{
		ENEMY_LIST[n]->ai_update();
		ENEMY_LIST[n]->draw( sys );
	}
	loop_entity( ENEMY_LIST, PLAYER_BULLET );

	for ( n=0; n < EFFECTS.size(); n++ )
	{
		EFFECTS[n]->ai_update();
		EFFECTS[n]->draw( sys );
	}

	clear_entities( PLAYER_BULLET );
	clear_entities( EFFECTS );

	draw_bg( sys, map );
	draw_fg( sys, map );
}

int main(int argc, char* argv[])
{
	gamesys* sys = new gamesys;
		sys->create_win( 224, 224, "cx/cx_demo" );

	LevelMap map;
		map.load_file("cx/cx_demo");
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

	SFFONT.loadFromMemory( RES_VERAMONO_TTF, RES_VERAMONO_TTF_SIZE );
	BGCOLOR = sf::Color(16,41,82);
	CAM_ID = -1;
	ANIM_WATER = 0;
	SCRLOCK = 0;
	BRIGHT     = 255;
	BRIGHT_ADJ = -4;

	while( sys->m_win.isOpen() )
	{
		sys->input_handler();
		sys->event_handler();

		if ( ! sys->m_paused )
		{
			game_update( sys, map, tile, rock );
		}

		draw_player_pos( sys, SFFONT, &rock );
		//draw_fps_no( sys, SFFONT );
		sys->m_win.display();
		sys->m_fps++;
	}

	clear_all();

	printf("Bye!\n");
	return 0;
}

