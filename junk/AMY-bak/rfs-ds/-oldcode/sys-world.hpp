namespace amy
{
	class World
	{
		public:
			World();
			~World();

			void set_tileset  ( const std::string&, uint, uint, uint, uint );
			void set_tiledebug( const std::string&, uint, uint );
			void set_mapfile  ( const std::string&, uint, uint );
			void set_tiletype( uint k, uint v );
			uint get_tileint ( uint x, uint y );
			void set_mapobj();
			void draw_map( sf::RenderTexture&, sf::RenderTexture& );
			void redraw_map( int id );
			void print_tiletype();
			void clear();
			void set_bglayer( const std::string& );
			void set_fglayer( const std::string& );
			void set_mapdata( const std::string&, uint, uint, uint, uint );
			void set_dbg();
			void draw_bg ( sf::RenderTexture&, sf::RenderTexture& );
			void draw_fg ( sf::RenderTexture& );
			void draw_dbg( sf::RenderTexture& );
		private:
			bool ready;
			bool mapobj_ready;
			bool mapfile_ready;
			bool tileset_ready;
			bool tiledebug_ready;
			int  map_id;
			uint map_x;
			uint map_y;
			uint tile_w;
			uint tile_h;
			std::vector < uint >          mapfile;
			std::vector < amy::TileData > mapobj;
			std::vector < sf::Texture >   tileset;
			std::vector < sf::Texture >   tileset_d;
			std::map    < uint, uint >    tiletype;
			sf::RenderTexture map_n;
			sf::RenderTexture map_d;
			bool bg_ready;
			bool fg_ready;
			bool dbg_ready;
			bool data_ready;
			uint map_x;
			uint map_y;
			uint tile_w;
			uint tile_h;

			sf::Image  bgimg;
			sf::Image  fgimg;

			std::vector < sf::Texture > debugset;
			std::vector < uint >        mapdata;
	};
}

