class RockX : public EntityData {
	private:
		bool m_jumped;
		bool m_jump_hold;
		int  m_jump_cnt;
		int  m_jump_max;
		int  m_noshot;

	protected:
	public:
		enum STATE {
			STAND = 0,
			SHOT  = 49,
			STD_W = 30,
			WALK  = 3,
			JUMP  = 23,
			FALL  = 26,
			LAND  = 28,
			DASH  = 54,
			WALL  = 39,
			CLMB  = 42,
			WALK_SHOT = 13,
			JUMP_SHOT = 32,
			FALL_SHOT = 35,
			LAND_SHOT = 37,
			DASH_SHOT = 57,
			WALL_SHOT = 44,
			CLMB_SHOT = 47,
			TELE     = 60,
			TELE_IN  = 61,
			TELE_OUT = 62,
			VICTORY  = 80
		};
		bool m_act_fin;


		void pl_up( int st, int st_sh, int mv )
		{
			if ( m_state == st || m_state == st_sh )
				switch_state( st );
			else
				change_state( st );
			up( mv, m_dir );
		}
		void pl_down( int st, int st_sh, int mv )
		{
			if ( m_state == st || m_state == st_sh )
				switch_state( st );
			else
				change_state( st );
			down( mv, m_dir );
		}
		void pl_left( int st, int st_sh, int mv )
		{
			if ( m_state == st || m_state == st_sh )
				switch_state( st );
			else
				change_state( st );
			left( mv );
		}
		void pl_right( int st, int st_sh, int mv )
		{
			if ( m_state == st || m_state == st_sh )
				switch_state( st );
			else
				change_state( st );
			right( mv );
		}

		void act_shot()
		{
			switch ( m_state )
			{
				case STAND:   change_state( SHOT ); break;
				case STD_W:   change_state( SHOT ); break;
				case WALK:   switch_state( WALK_SHOT ); break;
				case JUMP:   switch_state( JUMP_SHOT ); break;
				case FALL:   switch_state( FALL_SHOT ); break;
				case LAND:   switch_state( LAND_SHOT ); break;
				case DASH:   switch_state( DASH_SHOT ); break;
				case WALL:   switch_state( WALL_SHOT ); break;
				case CLMB:   switch_state( CLMB_SHOT ); break;
			}

			if ( m_noshot > 0 )
				return;

			int px = ( m_state == SHOT ) ? 0 : 8;
			int py = m_pos_y - 16;

			if ( m_dir == 'l' )
			{
				PLAYER_BULLET.push_back( new XBuster );
				PLAYER_BULLET.back()->m_id = -1;
				PLAYER_BULLET.back()->m_ref_map  = m_ref_map;
				PLAYER_BULLET.back()->m_ref_tile = m_ref_tile;
				PLAYER_BULLET.back()->set( m_rect_x1 - px, py, 'l' );
			}
			if ( m_dir == 'r' )
			{
				PLAYER_BULLET.push_back( new XBuster );
				PLAYER_BULLET.back()->m_id = -1;
				PLAYER_BULLET.back()->m_ref_map  = m_ref_map;
				PLAYER_BULLET.back()->m_ref_tile = m_ref_tile;
				PLAYER_BULLET.back()->set( m_rect_x2 + px, py, 'r' );
			}
			m_noshot = 4;
		}

		void act_victory()
		{
			m_act_fin = false;
			if ( is_anim_done() )
				m_act_fin = true;
		}

		void act_tele_in()
		{
			m_act_fin = false;
			if ( m_state == TELE_IN )
			{
				if ( is_anim_done() )
					m_act_fin = true;
			}
			else
			{
				// out of map , no wall , seg.fault
				if ( m_rect_y1 > 0 )
				{
					aware_nearby_walls();
					pl_down( TELE, TELE, m_vel_y );
					if ( m_is_wall_down )
						change_state( TELE_IN );
				}
				else
				{
					change_state( TELE );
					move( 0, m_vel_y, m_dir );
				}
			}
		}

		void act_fall( gamesys* sys )
		{
			m_jumped    = true;
			m_jump_cnt  = 0;
			pl_down( FALL, FALL_SHOT, m_vel_y );

			if      ( sys->m_Dleft  )  left ( m_vel_x );
			else if ( sys->m_Dright )  right( m_vel_x );
		}

		void act_jump( gamesys* sys )
		{
			if ( m_jumped )
			{
				if ( m_jump_cnt > 0 )
				{
					m_jump_cnt -= 10;
					if ( m_is_wall_up )
						m_jump_cnt = 0;
				}
			}
			else // first press
			{
				m_jumped    = true;
				m_jump_cnt  = m_jump_max;
			}

			int acce_y = (m_vel_y * m_jump_cnt) / m_jump_max;
			pl_up( JUMP, JUMP_SHOT, acce_y );
			if      ( sys->m_Dleft  )  left ( m_vel_x );
			else if ( sys->m_Dright )  right( m_vel_x );
		}

		void act_stand()
		{
			change_state( STAND );
		}

		void ai_update()
		{
			aware_nearby_walls();
			if ( m_is_wall_down )
			{
				m_jumped    = false;
				m_jump_cnt  = 0;
				change_state( STAND );
			}
			else
			{
				pl_down( FALL, FALL_SHOT, m_vel_y );
			}
		}

		void pl_update( gamesys* sys )
		{
			aware_nearby_walls();

			if ( m_noshot > 0 )   m_noshot--;

			// ground control
			if ( m_is_wall_down )
			{
				m_jumped    = false;
				m_jump_cnt  = 0;

				if ( sys->m_Dup  )
					act_jump( sys );

				if ( sys->m_Dleft  )
					pl_left ( WALK, WALK_SHOT, m_vel_x );
				else if ( sys->m_Dright )
					pl_right( WALK, WALK_SHOT, m_vel_x );
				else
					change_state( STAND );
			}
			// air control
			else
			{
				if ( m_jump_cnt > 0 )
					act_jump( sys );
				else
					act_fall( sys );
			}

			if ( sys->m_Ddown  )
				act_shot();

		}

		RockX()
		{
			m_class_name = "RockX";
			m_anim.load_def("cx/000_rock/rock.def");
			m_vel_x =  4;
			m_vel_y = 12;
			m_jumped   = true;
			m_jump_cnt = 0;
			m_jump_max = 100;
			m_noshot   = 0;
			m_act_fin  = false;
		}
}; // class RockX
