namespace amy
{
	class Entity
	{
		public:
			Entity(int, int, bool=false);
			~Entity();

			void set_tileset( const std::string&, int, int );
			virtual void update_ctrl();
			virtual void update_npc ( bool=false );
			virtual void draw( sf::RenderTexture&, sf::RenderTexture& );

			int  base_x; // base point is center
			int  base_y; // base point is bottom
			bool is_left;
			int velocity_x;
			int velocity_y;
			std::vector < sf::Texture >  tileset;

			int  pos_x;
			int  pos_y;
			amy::RectData hitbox;
			amy::RectData pushbox;
			amy::RectData atkbox;
			//std::vector < amy::EnData >  parts;


			sf::Vector2i  draw_adjust;
			amy::Timer    timer;
			std::deque  < uint >        animation_queue;
			std::deque  < uint >        action_queue;
	};
}

