namespace amy
{
	class Display
	{
		public:
			Display();
			~Display();

			void running   ( sf::RenderWindow&, uint msec );
			void raw_input ( sf::RenderWindow& );
			void handles   ( sf::RenderWindow& );
			void display   ( sf::RenderWindow&, uint alpha=255 );
			void draw_debug( sf::RenderWindow& );
			void fadein    ( sf::RenderWindow& );
			void fadeout   ( sf::RenderWindow& );
			virtual void on_enter( sf::RenderWindow& );
			virtual void updates ( sf::RenderWindow& );
			virtual void on_exit ( sf::RenderWindow& );
			std::string  next();

			bool is_running;
			std::string       next_screen;
			sf::RenderTexture preScreen;
			sf::RenderTexture preScreen_n;
			sf::RenderTexture preScreen_d;
			amy::Timer  timer;
	};
}

/*
	void init()
	void reinit()
	uint checkpoint = 0
*/