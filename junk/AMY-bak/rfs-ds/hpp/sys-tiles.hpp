namespace amy
{
	class Tiles
	{
		public:
			Tiles();
			~Tiles();

			void set_tileset( const std::string& );
			void draw( sf::RenderTexture&, uint, int, int );

			std::vector < sf::Texture >  tiletex;
	};
}

