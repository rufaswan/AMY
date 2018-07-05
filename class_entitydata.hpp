#ifndef ENTITYDATA_HPP
#define ENTITYDATA_HPP

#include "class_gamesys.hpp"
#include "class_levelmap.hpp"
#include "class_tiledata.hpp"
#include "class_animdata.hpp"

class EntityData {
	private:
	protected:
		int  m_vel_x;
		int  m_vel_y;
		int  m_state;
		bool m_flip_left;
		bool m_flip_up;
		std::string  m_class_name;
		AnimData  m_anim;

		void update_rect()
		{
			m_rect_x1 = m_pos_x + m_anim.m_clsn[ m_state ].x1;
			m_rect_y1 = m_pos_y + m_anim.m_clsn[ m_state ].y1;
			m_rect_x2 = m_pos_x + m_anim.m_clsn[ m_state ].x2;
			m_rect_y2 = m_pos_y + m_anim.m_clsn[ m_state ].y2;
		}

		void change_state( int state )
		{
			m_state = m_anim.change_state( m_state, state );
			update_rect();
		}

		void switch_state( int state_new )
		{
			m_state = m_anim.switch_state( state_new );
			update_rect();
		}

		void aware_nearby_walls()
		{
			m_is_wall_up    = ! can_move_ver(-1);
			m_is_wall_down  = ! can_move_ver( 1);
			m_is_wall_left  = ! can_move_hor(-1);
			m_is_wall_right = ! can_move_hor( 1);
		}

		bool move( int x, int y, int dir )
		{
			m_dir = dir;
			if ( dir == 'l' )   m_flip_left = false;
			if ( dir == 'r' )   m_flip_left = true;
			if ( dir == 'u' )   m_flip_up   = false;
			if ( dir == 'd' )   m_flip_up   = true;
			return move( x, y );
		}

		bool move( int x, int y )
		{
			m_pos_x += x;
			m_pos_y += y;
			update_rect();

			if ( x != 0 )  return true;
			if ( y != 0 )  return true;
			return false;
		}

		bool can_move( int x1, int y1 )
		{
			int tid, ttype;
			tid   = (y1 * m_ref_map->m_map_w) + x1;
			ttype = m_ref_map->m_map_col[ tid ];

			if ( m_ref_tile->m_tile[ ttype ].is_wall )
				return false;

			return true;
		}

		bool can_move_hor( int mv )
		{
			int x1 = ( mv > 0 ) ? (m_rect_x2 + mv) / 16 : (m_rect_x1 + mv) / 16;
			int y1 = m_rect_y1 / 16;
			int y2 = m_rect_y2 / 16;

			while ( y1 <= y2 )
			{
				if ( ! can_move( x1, y1 ) )
					return false;
				y1++;
			} // while y1 < y2
			return true;
		}

		bool can_move_ver( int mv )
		{
			int y1 = ( mv > 0 ) ? (m_rect_y2 + mv) / 16 : (m_rect_y1 + mv) / 16;
			int x1 = m_rect_x1 / 16;
			int x2 = m_rect_x2 / 16;

			while ( x1 <= x2 )
			{
				if ( ! can_move( x1, y1 ) )
					return false;
				x1++;
			} // while x1 < x2
			return true;
		}

		void draw_frame( gamesys* sys , bool dbg_rect = false )
		{
			int frame = m_anim.get_frame( m_state );

			sf::Sprite  spr;
				spr.setTexture    ( m_anim.m_frame[ frame ].tex );

			int fr_x = m_anim.m_frame[ frame ].x * -1;
			int fr_y = m_anim.m_frame[ frame ].y * -1;

			int mapx = ((m_ref_map->m_cam_x * 16) - m_ref_map->m_tile_x) - m_ref_map->m_pad_x;
			int mapy = ((m_ref_map->m_cam_y * 16) - m_ref_map->m_tile_y) - m_ref_map->m_pad_y;

			int px = (m_pos_x + fr_x) - mapx;
			int py = (m_pos_y + fr_y) - mapy;
				spr.setPosition   ( px, py );

			sf::IntRect rect = spr.getTextureRect();
				if ( m_flip_left )
				{
					rect.left  = rect.width;
					rect.width *= -1;
				}
				if ( m_flip_up )
				{
					rect.top     = rect.height;
					rect.height *= -1;
				}

				spr.setTextureRect( rect );

			sys->m_win.draw( spr );

			// FOR DEBUG
			if ( ! dbg_rect )
				return;

			sf::RectangleShape sfrect;
				sfrect.setSize(
					sf::Vector2f(
						(m_rect_x2 - m_rect_x1) ,
						(m_rect_y2 - m_rect_y1)
					)
				);
				sfrect.setFillColor( sf::Color::Red );
				sfrect.setPosition(
					(m_rect_x1 - mapx) ,
					(m_rect_y1 - mapy)
				);

			sys->m_win.draw( sfrect );
		}

	public:
		int  m_id;
		int  m_dir;
		int  m_pos_x;
		int  m_pos_y;
		int  m_rect_x1;
		int  m_rect_y1;
		int  m_rect_x2;
		int  m_rect_y2;
		int  m_is_hitted;
		bool m_is_boss;
		bool m_is_dead;
		bool m_no_draw;
		bool m_is_wall_up;
		bool m_is_wall_down;
		bool m_is_wall_left;
		bool m_is_wall_right;
		LevelMap*    m_ref_map;
		TileData*    m_ref_tile;
		EntityData*  m_parent;

		bool is_intersect( EntityData* atk )
		{
			if ( m_rect_x1 > atk->m_rect_x2 )  return false;
			if ( m_rect_y1 > atk->m_rect_y2 )  return false;

			if ( m_rect_x2 < atk->m_rect_x1 )  return false;
			if ( m_rect_y2 < atk->m_rect_y1 )  return false;
			return true;
		}

		bool is_onscreen()
		{
			int cam_x = m_ref_map->m_cam_x * 16;
			int cam_y = m_ref_map->m_cam_y * 16;

			if ( m_rect_x1 > (cam_x + 256) )  return false;
			if ( m_rect_y1 > (cam_y + 256) )  return false;

			if ( m_rect_x2 < (cam_x) )  return false;
			if ( m_rect_y2 < (cam_y) )  return false;
			return true;
		}

		bool is_onmap()
		{
			int map_w = m_ref_map->m_map_w * 16;
			int map_h = m_ref_map->m_map_h * 16;

			if ( m_rect_x1 > map_w )  return false;
			if ( m_rect_y1 > map_h )  return false;

			if ( m_rect_x2 < 0 )  return false;
			if ( m_rect_y2 < 0 )  return false;
			return true;
		}

		bool is_anim_done()
		{
			return m_anim.m_is_done;
		}

		bool up( int mv, int dir = 'u' )
		{
			while ( mv > 0 )
			{
				if ( can_move_ver(mv*-1) )
					return move(0, mv*-1, dir);
				mv--;
			} // while mv
			return false;
		}

		bool down( int mv, int dir = 'd' )
		{
			while ( mv > 0 )
			{
				if ( can_move_ver(mv*1) )
					return move(0, mv*1, dir);
				mv--;
			} // while mv
			return false;
		}

		bool left( int mv, int dir = 'l' )
		{
			while ( mv > 0 )
			{
				if ( can_move_hor(mv*-1) )
					return move(mv*-1, 0, dir);
				mv--;
			} // while mv
			return false;
		}

		bool right( int mv, int dir = 'r' )
		{
			while ( mv > 0 )
			{
				if ( can_move_hor(mv*1) )
					return move(mv*1, 0, dir);
				mv--;
			} // while mv
			return false;
		}

		void set( int x, int y, int dir = 'd' )
		{
			m_pos_x = x;
			m_pos_y = y;
			m_dir = dir;
			printf("%s.set( %d, %d, %c )\n", m_class_name.c_str(), x, y, dir);
			change_state(0);

			if ( m_ref_map == NULL )
				printf("ERROR [ %s.m_ref_map is NULL ]\n", m_class_name.c_str() );

			if ( m_anim.m_anim[0].empty() )
				printf("ERROR [ %s.m_anim.m_anim[0] is empty ]\n", m_class_name.c_str() );
		}

		EntityData()
		{
			m_class_name = "EntityData";
			m_id     = -1;
			m_vel_x  = m_vel_y = 0;
			m_state  = 0;
			m_parent   = NULL;
			m_ref_map  = NULL;
			m_ref_tile = NULL;
			m_is_hitted     = -1;
			m_is_boss       = false;
			m_is_dead       = false;
			m_no_draw       = false;
			m_flip_left     = m_flip_up       = false;
			m_is_wall_up    = m_is_wall_down  = false;
			m_is_wall_left  = m_is_wall_right = false;
		}

		virtual void draw( gamesys* sys , bool dbg_rect = false )
		{
			if ( m_is_dead || m_no_draw || m_anim.m_frame.empty() )
				return;
			if ( (m_is_hitted > 0) && (m_is_hitted % 2) )
				return;

			draw_frame( sys, dbg_rect );
		}
		virtual void pl_update( gamesys* sys ) {}
		virtual void ai_update() {}
		virtual void collide( EntityData* entity ) {}
}; // EntityData

#endif //  ENTITYDATA_HPP
