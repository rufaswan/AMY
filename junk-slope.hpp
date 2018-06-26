/*
 *  FROM RKS FS , stage 013 (twins) ,
 *  slopes are objects , like moving platforms
 */

	bool is_slope;
	int  slope_type;
		enum SLOPE {
			SLOPE_1_U,
			SLOPE_1_D,
			SLOPE_2_U_1, SLOPE_2_U_2,
			SLOPE_2_D_1, SLOPE_2_D_2,
			SLOPE_4_U_1, SLOPE_4_U_2, SLOPE_4_U_3, SLOPE_4_U_4,
			SLOPE_4_D_1, SLOPE_4_D_2, SLOPE_4_D_3, SLOPE_4_D_4
		};
				t.is_slope   = false;
				t.slope_type = 0;
		int  m_slope_h[17];

		void set_slope_h( int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15 )
		{
			m_slope_h[0] = a0;
			m_slope_h[1] = a1;
			m_slope_h[2] = a2;
			m_slope_h[3] = a3;
			m_slope_h[4] = a4;
			m_slope_h[5] = a5;
			m_slope_h[6] = a6;
			m_slope_h[7] = a7;
			m_slope_h[8] = a8;
			m_slope_h[9] = a9;
			m_slope_h[10] = a10;
			m_slope_h[11] = a11;
			m_slope_h[12] = a12;
			m_slope_h[13] = a13;
			m_slope_h[14] = a14;
			m_slope_h[15] = a15;
		}

		int get_slope_h( int id )
		{
			switch ( m_tile[id].slope_type )
			{
				// based on top-left corner - the sky
				case SLOPE_2_U_1:
					set_slope_h( 15,15,14,14,13,13,12,12,11,11,10,10,9,9,8,8 );
					return ( m_slope_h[x1] ); break;
				case SLOPE_2_U_2:
					set_slope_h( 7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0 );
					return ( m_slope_h[x1] ); break;

				case SLOPE_2_D_1:
					set_slope_h( 0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7 );
					return ( m_slope_h[x1] ); break;
				case SLOPE_2_D_2:
					set_slope_h( 8,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15 );
					return ( m_slope_h[x1] ); break;


				case SLOPE_4_U_1:
					set_slope_h( 15,15,15,15,14,14,14,14,13,13,13,13,12,12,12,12 );
					return ( m_slope_h[x1] ); break;
				case SLOPE_4_U_2:
					set_slope_h( 11,11,11,11,10,10,10,10,9,9,9,9,8,8,8,8 );
					return ( m_slope_h[x1] ); break;
				case SLOPE_4_U_3:
					set_slope_h( 7,7,7,7,6,6,6,6,5,5,5,5,4,4,4,4 );
					return ( m_slope_h[x1] ); break;
				case SLOPE_4_U_4:
					set_slope_h( 3,3,3,3,2,2,2,2,1,1,1,1,0,0,0,0 );
					return ( m_slope_h[x1] ); break;

				case SLOPE_4_D_1:
					set_slope_h( 0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3 );
					return ( m_slope_h[x1] ); break;
				case SLOPE_4_D_2:
					set_slope_h( 4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7 );
					return ( m_slope_h[x1] ); break;
				case SLOPE_4_D_3:
					set_slope_h( 8,8,8,8,9,9,9,9,10,10,10,10,11,11,11,11 );
					return ( m_slope_h[x1] ); break;
				case SLOPE_4_D_4:
					set_slope_h( 12,12,12,12,13,13,13,13,14,14,14,14,15,15,15,15 );
					return ( m_slope_h[x1] ); break;


				default:
					return false; break;
			} // switch ( m_tile[id].slope_type )
		}

		void set_slope4u( int id1 , int id2, int id3 , int id4 )
		{
			tile_id( id1 , id2 , id3 , id4 );
			m_tile[id1].is_slope = true;
			m_tile[id2].is_slope = true;
			m_tile[id3].is_slope = true;
			m_tile[id4].is_slope = true;
			m_tile[id1].slope_type = SLOPE_4_U_1;
			m_tile[id2].slope_type = SLOPE_4_U_2;
			m_tile[id3].slope_type = SLOPE_4_U_3;
			m_tile[id4].slope_type = SLOPE_4_U_4;
		}
		void set_slope4d( int id1 , int id2, int id3 , int id4 )
		{
			tile_id( id1 , id2 , id3 , id4 );
			m_tile[id1].is_slope = true;
			m_tile[id2].is_slope = true;
			m_tile[id3].is_slope = true;
			m_tile[id4].is_slope = true;
			m_tile[id1].slope_type = SLOPE_4_D_1;
			m_tile[id2].slope_type = SLOPE_4_D_2;
			m_tile[id3].slope_type = SLOPE_4_D_3;
			m_tile[id4].slope_type = SLOPE_4_D_4;
		}

		void set_slope2u( int id1 , int id2 )
		{
			tile_id( id1 , id2 );
			m_tile[id1].is_slope = true;
			m_tile[id2].is_slope = true;
			m_tile[id1].slope_type = SLOPE_2_U_1;
			m_tile[id2].slope_type = SLOPE_2_U_2;
		}
		void set_slope2d( int id1 , int id2 )
		{
			tile_id( id1 , id2 );
			m_tile[id1].is_slope = true;
			m_tile[id2].is_slope = true;
			m_tile[id1].slope_type = SLOPE_2_D_1;
			m_tile[id2].slope_type = SLOPE_2_D_2;
		}

				if ( m_ref_tile->m_tile[ ttype ].is_slope )
				{

				}

		bool check_slope()
		{
			int t1, t2, t3, t4;
			t1 = m_ref_map->get_map_id( m_pos_x , m_pos_y );
			t2 = m_ref_map->m_map_col[ t1 ];
			if ( m_ref_tile->m_tile[ t2 ].is_slope )
			{
				t3 = m_pos_y % 16;
				t4 = m_ref_tile->get_slope_h( t2, m_pos_x , m_pos_y );
				//printf("%s.slope_h %d @ %d\n", m_class_name.c_str(), t4, t3);
				if ( t3 > (16 - t4) )
				{
					//m_pos_y = (m_pos_y - t3) + (16 - t4);
					move(0, t4*-1 );
				}
			}
		}

