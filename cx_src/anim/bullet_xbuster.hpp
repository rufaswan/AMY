class XBuster : public EntityData {
	public:
		void ai_update()
		{
			if ( m_dir == 'l' )   move( m_vel_x*-1 , 0, 'l' );
			if ( m_dir == 'r' )   move( m_vel_x* 1 , 0, 'r' );

			if ( ! is_onscreen() )
				m_is_dead = true;
		}

		XBuster()
		{
			m_class_name = "XBuster";
			m_anim.load_def("cx_data/anim/bullet_xbuster/xb.def");
			m_vel_x =  8;
		}
}; // class XBuster


