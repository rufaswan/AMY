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
int  G_CAM_ID;
int  G_ANIM_WATER;
int  G_SCRLOCK;
int  G_BRIGHT;
int  G_BRIGHT_ADJ;

EntityList  G_PLAYER_BULLET;
EntityList  G_ENEMY_BULLET;
EntityList  G_ENEMY_LIST;
EntityList  G_EFFECTS;

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

			G_EFFECTS.push_back( new Explode );
			G_EFFECTS.back()->m_id = -1;
			G_EFFECTS.back()->m_ref_map  = m_ref_map;
			G_EFFECTS.back()->m_ref_tile = m_ref_tile;
			G_EFFECTS.back()->set( m_pos_x, m_pos_y - 16, 'l' );
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
		if ( tid == 4 )   { G_SCRLOCK = 4; } // vlock
		if ( tid == 5 )   { G_SCRLOCK = 5; } // hlock
		if ( tid == 6 )   { G_SCRLOCK = 6; } // room lock
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
				case 4:
					printf("new Ball x %d y %d\n", map.m_obj_list[n].x + 32, map.m_obj_list[n].y + 55 );
					G_ENEMY_LIST.push_back( new Ball );
					G_ENEMY_LIST.back()->m_id = map.m_obj_list[n].id;
					G_ENEMY_LIST.back()->m_ref_map  = &map;
					G_ENEMY_LIST.back()->m_ref_tile = &tile;
					G_ENEMY_LIST.back()->set( map.m_obj_list[n].x + 32, map.m_obj_list[n].y + 55, 'l' );
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
}

void draw_bg( gamesys* sys, LevelMap &map )
{
	map.change_map(1); // bg
	map.update_map( G_BRIGHT );
	sys->m_win.draw(map);

	G_BRIGHT += G_BRIGHT_ADJ;
	if ( G_BRIGHT <   0 )   { G_BRIGHT =   0; G_BRIGHT_ADJ *= -1; }
	if ( G_BRIGHT > 255 )   { G_BRIGHT = 255; G_BRIGHT_ADJ *= -1; }
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

		sys->m_win.clear( G_BGCOLOR );

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

		sys->m_win.clear( G_BGCOLOR );

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
	sys->m_win.clear( G_BGCOLOR );

	int tid = tile_event( map, rock.m_pos_x, rock.m_pos_y );
		if ( tid == 2 )   rock.m_is_dead = true;
		if ( tid == 3 )   rock.m_is_dead = true;
	rock.pl_update( sys );
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
	loop_entity( G_ENEMY_LIST, G_PLAYER_BULLET );

	for ( n=0; n < G_EFFECTS.size(); n++ )
	{
		G_EFFECTS[n]->ai_update();
		G_EFFECTS[n]->draw( sys );
	}

	clear_entities( G_PLAYER_BULLET );
	clear_entities( G_EFFECTS );

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

	G_SFFONT.loadFromMemory( RES_VERAMONO_TTF, RES_VERAMONO_TTF_SIZE );
	G_BGCOLOR = sf::Color(16,41,82);
	G_CAM_ID = -1;
	G_ANIM_WATER = 0;
	G_SCRLOCK = 0;
	G_BRIGHT     = 255;
	G_BRIGHT_ADJ = -4;

	while( sys->m_win.isOpen() )
	{
		sys->input_handler();
		sys->event_handler();

		if ( ! sys->m_paused )
		{
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

