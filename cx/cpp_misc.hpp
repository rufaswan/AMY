class Explode : public EntityData {
	public:
		void ai_update()
		{
			if ( is_anim_done() )
				m_is_dead = true;
		}
		Explode()
		{
			m_class_name = "Explode";
			m_anim.load_def("cx/008_explode/exp.def");
		}
}; // class Explode

class READY : public EntityData {
	public:
		READY()
		{
			m_class_name = "READY";
			m_anim.load_def("cx/019_ready/ready.def");
		}
}; // class READY

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
			m_anim.load_def("cx/xbuster/xb.def");
			m_vel_x =  8;
		}
}; // class XBuster3

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
			m_anim.load_def("cx/xbuster/c3.def");
			m_vel_x =  8;
		}
}; // class XBuster3

