namespace amy
{
	class GameMaster
	{
		public:
			GameMaster();
			~GameMaster();

			void init(uint w, uint h, const std::string &title);
			void run();
			int run_level( std::string& );
		private:
			bool ready;
			uint msec;
			std::string       now_screen;
			sf::RenderWindow  screen;
		public:
			amy::Display* display;
	};
}

