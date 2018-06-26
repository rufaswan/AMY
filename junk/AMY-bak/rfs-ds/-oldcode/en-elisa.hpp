class AElisa : public amy::Entity
{
	public:
		AElisa( int x, int y );
		~AElisa();

		void update( amy::World& );
		void update_npc( amy::World& );
		void draw( sf::RenderTexture&, sf::RenderTexture& );
	private:
};
