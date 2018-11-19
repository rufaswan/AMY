/*
 * Simple RPG
 */
#include <ctime>
#include "zsf-common.hpp"
#include "res-ttf.inc"
#include "class_entitydata.hpp"
#include "class_levelmap.hpp"
#include "class_tiledata.hpp"
#include "class_animdata.hpp"
#include "class_entitytools.hpp"

sf::Font     G_SFFONT;
int  G_CAM_ID;

//////////////////////////////
class Player : public EntityData {
	private:
		void pl_up( int state, int mv )
		{
			change_state( state );
			up( mv );
		}
		void pl_down( int state, int mv )
		{
			change_state( state );
			down( mv );
		}
		void pl_left( int state, int mv )
		{
			change_state( state );
			left( mv );
		}
		void pl_right( int state, int mv )
		{
			change_state( state );
			right( mv );
		}

	protected:
	public:
		enum STATE { STAND = 0 };

		void pl_update( gamesys* sys )
		{
			aware_nearby_walls();

			if ( sys->m_Dup    )   pl_up   ( STAND, m_vel_y );
			if ( sys->m_Ddown  )   pl_down ( STAND, m_vel_y );
			if ( sys->m_Dleft  )   pl_left ( STAND, m_vel_x );
			if ( sys->m_Dright )   pl_right( STAND, m_vel_x );
		}

		Player()
		{
			m_class_name = "Player";
			m_anim.load_def("castle-platformer/bat.def");
			m_vel_x = 4;
			m_vel_y = 4;
		}
}; // class Player

class Enemy : public EntityData {
	private:
	protected:
		void collide( EntityData* entity )
		{
			if ( ! is_intersect( entity ) )
				return;

			m_is_dead = true;
			//m_is_hitted = 240;
			/*
			bool b1 = true;
			while( b1 )
			{
				entity->up(1);
				if ( entity->m_is_wall_up )
				{
					m_is_wall_up = true;
					down(1);
				}
				b1 = m_rect.intersects( entity->m_rect, result );
			}
			*/
		}

	public:
		enum STATE { STAND = 0 };
		char m_ai_mv;
		std::string  m_ai_dir;

		void en_up( int state, int mv )
		{
			change_state( state );
			up( mv );
		}
		void en_down( int state, int mv )
		{
			change_state( state );
			down( mv );
		}
		void en_left( int state, int mv )
		{
			change_state( state );
			left( mv );
		}
		void en_right( int state, int mv )
		{
			change_state( state );
			right( mv );
		}

		void ai_update()
		{
			if ( m_is_dead )  return;
			aware_nearby_walls();

			if ( m_is_hitted > 0 )
				m_is_hitted--;

			char dir = rand() % 6;
			if ( m_is_wall_up    )   m_ai_mv = m_ai_dir[ dir ];
			if ( m_is_wall_left  )   m_ai_mv = m_ai_dir[ dir ];
			if ( m_is_wall_down  )   m_ai_mv = m_ai_dir[ dir ];
			if ( m_is_wall_right )   m_ai_mv = m_ai_dir[ dir ];

			if ( m_ai_mv == 'l' )  en_left ( STAND, m_vel_y );
			if ( m_ai_mv == 'd' )  en_down ( STAND, m_vel_y );
			if ( m_ai_mv == 'r' )  en_right( STAND, m_vel_y );
			if ( m_ai_mv == 'u' )  en_up   ( STAND, m_vel_y );

			if ( ! is_onscreen() )
				m_is_dead = true;
		}

		Enemy()
		{
			m_class_name = "Enemy";
			m_anim.load_def("castle-platformer/bat.def");
			m_vel_x = 8;
			m_vel_y = 8;
			m_ai_dir = "lrudlr";
			m_ai_mv = 'u';
		}
}; // class Enemy
//////////////////////////////
void gen_enlist( LevelMap &map, TileData &tile, EntityList &EnList )
{
	if ( map.m_map_obj.empty() )   return;

	int n = map.m_obj_list.size();
	while ( n > 0 )
	{
		n--;
		if ( has_enlist_id( map.m_obj_list[n].id, EnList) )
		{
			map.m_obj_list.erase( map.m_obj_list.begin() + n );
		}
		else
		{
			switch( map.m_obj_list[n].ob )
			{
				//case 2:
				default:
					EnList.push_back( new Enemy );
					EnList.back()->m_id = map.m_obj_list[n].id;
					EnList.back()->m_ref_map  = &map;
					EnList.back()->m_ref_tile = &tile;
					EnList.back()->set( map.m_obj_list[n].x, map.m_obj_list[n].y, 'u' );
					break;
			}
		}
	}
}
//////////////////////////////
void game_update( gamesys* sys, LevelMap &map, TileData &tile, Player &hero, EntityList &EnemyList )
{
	sys->m_win.clear( sf::Color::White );

	map.change_map(0);
	map.update_map();
	sys->m_win.draw(map);

	hero.pl_update( sys );
		map.cam_focus_x( hero.m_pos_x );
		map.cam_focus_y( hero.m_pos_y );
	hero.draw( sys, true );

	int cam_id = map.get_cam_id();
	if ( G_CAM_ID != cam_id )
	{
		map.get_map_obj();
		gen_enlist( map, tile, EnemyList );
		G_CAM_ID = cam_id;
	}

	int n;
	for ( n=0; n < EnemyList.size(); n++ )
	{
		EnemyList[n]->ai_update();
		EnemyList[n]->draw( sys, true );
	}

	loop_entity( EnemyList, &hero );
}

int main(int argc, char* argv[])
{
	srand( time(0) );

	gamesys* sys = new gamesys;
		sys->create_win( 224, 224, "level-1rpg" );

	LevelMap map;
		map.load_file("level-1rpg");
		map.set_pad(-16, -16);

	TileData tile;
		tile.set_wall(1);
		tile.set_wall(2);
		tile.set_wall(3);
		tile.set_wall(4);

	Player hero;
		hero.m_ref_map  = &map;
		hero.m_ref_tile = &tile;
		hero.set( sys->m_half_w , sys->m_half_h , 'r' );

	EntityList EnemyList;
		EnemyList.reserve(32);

	G_SFFONT.loadFromMemory( RES_VERAMONO_TTF, RES_VERAMONO_TTF_SIZE );
	G_CAM_ID = -1;

	//sf::Thread thd_fps( &fps_watch, sys );
		//thd_fps.launch();

	while( sys->m_win.isOpen() )
	{
		sys->input_handler();
		sys->event_handler();

		if ( ! sys->m_paused )
		{
			game_update( sys, map, tile, hero, EnemyList );
		}

		//draw_player_pos( sys, G_SFFONT, &hero );
		//draw_fps_no( sys, G_SFFONT );
		sys->m_win.display();
		sys->m_fps++;
	}

	clear_entities( EnemyList, true );

	printf("Bye!\n");
	return 0;
}
