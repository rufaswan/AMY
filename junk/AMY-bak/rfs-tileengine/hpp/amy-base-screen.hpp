namespace amy
{
	class baseScreen
	{
		public:
			baseScreen();
			virtual ~baseScreen();
			bool running;
			void keyinput();
			void set_kb(sf::Keyboard::Key, int, int = 0);
			void add_keydata();
		protected:
		private:
	};
}

namespace amy
{
	class Stage : public baseScreen
	{
		public:
			Stage();
			virtual ~Stage();
			bool use_bgmap;
			int map_width;  // in tiles
			int map_height;

			void set_tileset(const std::string&, int col, int row);
			void set_map_size (int, int);
			void set_mapdata_file (const std::string&, bool bg = false);
			void set_mapdata_array(int[], int,         bool bg = false);
			void run_level();
			void run_event();
			void draw_map();
			void draw_bgmap();
			void show_map(bool bg = false);
			void   set_mapobject();
			void unset_mapobject();
			void   set_mapevent(int left, int top, int right, int bottom, int ev = 0);
			void unset_mapevent(int left, int top, int right, int bottom);
			void   set_wall    (int left, int top, int right, int bottom);
			void unset_wall    (int left, int top, int right, int bottom);
			virtual amy::Tile* mapobj_list(int lst, int x, int y);
			virtual void event_list(int lst, int x, int y);
		protected:
		private:
	};
}
