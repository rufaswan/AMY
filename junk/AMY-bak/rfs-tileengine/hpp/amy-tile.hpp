namespace amy
{
	class Tile
	{
		public:
			Tile();
			Tile(int tile, int x, int y, int type = 0);
			virtual ~Tile();
			int map_x;
			int map_y;
			int tile;
			int event_no;
			bool is_wall;
			bool is_spike;

			void draw();
			void set_tile(int tile, int x, int y, int type = 0);
			void set_event(int);
			int  get_event();
		protected:
		private:
	};
}
