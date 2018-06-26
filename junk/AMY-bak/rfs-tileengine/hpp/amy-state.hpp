namespace amy
{
	class State
	{
		public:
			State();
			virtual ~State();
			unsigned int		FRAMECNT;
			unsigned int		FRAMECNT_2R;
			unsigned int		FRAMECNT_3R;
			unsigned int		FRAMECNT_4R;
			unsigned int		FRAMECNT_5R;
			unsigned int		FRAMECNT_6R;
			amy::Actor*			PLAYER;
			amy::Stage*			STAGE;
			amy::Camera			CAMERA;
			sf::RenderWindow	SCREEN;
			bool	FRAMECNT_2;
			bool	FRAMECNT_3;
			bool	FRAMECNT_4;
			bool	FRAMECNT_5;
			bool	FRAMECNT_6;
			bool	KEY_PRESS;
			bool	KEY_HOLD;
			bool	NO_PASSWORD;	// for not using password to clear the game
			bool	NO_CONTINUE;	// for not using checkpoints (or die) to clear the game
			bool	NO_HIT;			// for not getting hit to clear the game
			bool	NO_PARTS;		// for not collecting any armor parts to clear the game
			bool	NO_LIFEUP;		// for not collecting any life-ups to clear the game
			bool	NO_ETANK;		// for not collecting any etanks to clear the game
			std::list	< int >							REPLAYDATA;
			std::list	< int >							KEYSDATA;
			std::vector	< sf::Texture >					TILESET;
			std::vector	< int >							TILEIND;
			std::vector < std::vector <int> >			MAP_FG_DATA;
			std::vector < std::vector <amy::Tile*> >	MAP_FG_OBJECT;
			std::vector < std::vector <int> >			MAP_BG_DATA;
			std::map	< std::string, sf::Image >		TILEIMG;
			std::map	< int, bool >					KEYS;

			void reset_keys();
			void update();
			void add_tileimg(const std::string&);
			void add_replaydata(int);
			bool within(int, int, int);
		protected:
		private:
	};
}
