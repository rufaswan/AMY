namespace amy
{
	class World
	{
		public:
			World();
			~World();

			void set_wall ( const std::string&, uint, uint );
			void set_mapbg( const std::string& );
			void set_mapfg( const std::string& );
			void set_tiles( const std::string& );
			bool is_redraw();
			void render_wall( uint, uint );
			void render_bg  ( uint, uint );
			void render_fg  ( uint, uint );
			void draw_wall( sf::RenderTexture& );
			void draw_bg  ( sf::RenderTexture& );
			void draw_fg  ( sf::RenderTexture& );
			void clear();
			int  get_maptile( int, int );
			void set_mapint ( int, int, int );
			void set_mapint ( int, int, int, int, int );
		private:
			bool ready;
			bool wall_ready;
			bool tile_ready;
			bool has_bg;
			bool has_fg;
			int  mapid;
			int  map_w;
			int  map_h;
			std::vector < uint >         map_load;
			std::vector < uint >         map_wall;
			std::vector < uint >         map_bg;
			std::vector < uint >         map_int;
			std::vector < bool >         map_fg;
			std::vector < sf::Texture >  map_tile;
		public:
			sf::RenderTexture  rend_bg;
			sf::RenderTexture  rend_fg;
			sf::RenderTexture  rend_wall;
	};
}

