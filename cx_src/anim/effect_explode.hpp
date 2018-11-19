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
			m_anim.load_def("cx_data/anim/effect_explode/exp.def");
		}
}; // class Explode

