namespace amy
{
	class BGFollow : public Background
	{
		public:
			BGFollow();
			~BGFollow();
			void set_texture( const std::string& );
			void set_draw();
			void draw( sf::RenderTexture& );
	};
}

