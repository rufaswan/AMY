namespace amy
{
	class Input
	{
		public:
			Input();
			~Input();

			void raw_input      ( sf::RenderWindow&, sf::RenderTexture& );
			void handle_input   ();
			void keyboard_handle( sf::RenderWindow&, sf::RenderTexture&, amy::KeyData& );
			void ev_sshot       ( sf::RenderTexture& );
			void ev_close       ( sf::RenderWindow& );
			void set_key( bool& );
			bool is_nopress( uint pos=0 )
			bool is_pressed( const char, uint pos=0 );
			bool is_holded ( const char, uint dur=1, uint pos=0 );
		private:
			bool focused;
			bool joystick;
			uint joystick_id;
			uint idle;
			uint kb_scheme;
			uint sshot_no;
			amy::KeyData key;
		public:
			std::string sshot_name;
			std::deque < uint > keylist;
	};
}

