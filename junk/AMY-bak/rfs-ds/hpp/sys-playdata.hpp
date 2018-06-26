namespace amy
{
	class PlayData
	{
		public:
			PlayData();
			~PlayData();

			bool debug;
			sf::Image       sfimage;
			sf::Font        sffont;
			sf::Mutex       sfmutex;
			sf::Clock       sfclock;
			amy::FileLoad   file;
			amy::Recorder*  rec;
			amy::Objects    object;
			amy::Events*    event;
			amy::Input      input;
			amy::Camera     camera;
			amy::Tiles      tile;
			amy::World      world;
			amy::Playlist   playlist;
			amy::Utils      util;

			void savestate( uint );
			void loadstate( uint );
	};
}

