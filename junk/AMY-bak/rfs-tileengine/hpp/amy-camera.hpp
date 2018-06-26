namespace amy
{
	class Camera
	{
		public:
			Camera();
			virtual ~Camera();
			int cam_x; // in pixel, for tracking
			int cam_y;
			int draw_adjust_x; // adjustment for tile pos to screen pos
			int draw_adjust_y;
			int center_x; // map center pos
			int center_y;
			int cam_spd_x;
			int cam_spd_y;
			int cam_min_x; // in pixel
			int cam_min_y;
			int cam_max_x;
			int cam_max_y;
			int tile_min_x; // in tile
			int tile_min_y;
			int tile_max_x;
			int tile_max_y;

			void update();
			void update_player();
			void set_range(int left, int top, int right, int bottom);
			void set_speed(int, int);
			void set_pos  (int, int);
			void mv_pos_x(int);
			void mv_pos_y(int);
			void mv_up   (int dist = 0, int spd = 1); // move camera, all positive int
			void mv_down (int dist = 0, int spd = 1);
			void mv_left (int dist = 0, int spd = 1);
			void mv_right(int dist = 0, int spd = 1);
		protected:
		private:
	};
}
