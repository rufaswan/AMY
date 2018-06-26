namespace amy
{
	class EnManager
	{
		public:
			EnManager();
			~EnManager();
			enum types
			{
				GENERIC, PLAYER, ENEMY, NPC, PBULLETS, EBULLETS, DEAD, PLATFORM
			};

			void add( amy::Entity*, uint=0 );
			void update();
			void draw( sf::RenderTexture&, sf::RenderTexture& );
			void clear();

			uint get_objectid();
			void player2npc();
			void npc2player( uint id );

			uint objectid;
			bool has_player;
			amy::Entity* player;
			std::map < uint, amy::Entity* >  objects;
			std::map < uint, uint >          npc;
			std::map < uint, uint >          npcbul;
			std::map < uint, uint >          en;
			std::map < uint, uint >          en_bul;
			std::map < uint, uint >          dead;
			std::map < uint, amy::Entity* >::iterator  enit;
			std::map < uint, uint >::iterator          uit;
	};
}

