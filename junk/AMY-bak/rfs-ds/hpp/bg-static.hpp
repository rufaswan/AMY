namespace amy
{
	class BGStatic : public Background
	{
		public:
			BGStatic();
			~BGStatic();
			void set_texture( const std::string&, bool=false );
			void set_draw();
			void draw( sf::RenderTexture& );
	};
}

