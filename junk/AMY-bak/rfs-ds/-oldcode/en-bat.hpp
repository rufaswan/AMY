class ABat : public amy::Entity
{
	public:
		ABat( int x, int y );
		~ABat();

		void update( amy::World& );
		void update_npc( amy::World& );
		void draw( sf::RenderTexture&, sf::RenderTexture& );
	private:
};
