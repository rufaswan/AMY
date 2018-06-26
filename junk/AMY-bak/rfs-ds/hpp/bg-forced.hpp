namespace amy
{
	class BGForced : public Background
	{
		public:
			BGForced();
			~BGForced();
			void set_texture( const std::string& );
			void set_draw();
			void draw( sf::RenderTexture& );
	};
}

