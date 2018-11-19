class XBuster3 : public EntityData {
	public:
		void ai_update()
		{
			if ( m_dir == 'l' )   move( m_vel_x*-1 , 0, 'l' );
			if ( m_dir == 'r' )   move( m_vel_x* 1 , 0, 'r' );

			if ( ! is_onscreen() )
				m_is_dead = true;
		}

		XBuster3()
		{
			m_class_name = "XBuster3";
			m_anim.load_def("cx_data/anim/bullet_xb3/c3.def");
			m_vel_x =  8;
		}
}; // class XBuster3


