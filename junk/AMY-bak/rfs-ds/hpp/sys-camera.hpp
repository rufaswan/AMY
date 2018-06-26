namespace amy
{
	class Camera
	{
		public:
			Camera();
			~Camera();

			void set_pos(int, int);
			void set_rect();
			void set_range    (uint, uint, uint, uint);
			void set_rangetile(uint, uint, uint, uint);
			void reset();
			void mv_up();
			void mv_down();
			void mv_left();
			void mv_right();
		private:
			int min_x;
			int min_y;
			int max_x;
			int max_y;
			uint velocity_x;
			uint velocity_y;
			amy::RectData rect;
		public:
			int pos_x;
			int pos_y;
	};
}

