namespace amy
{
	class Background
	{
		public:
			Background();
			~Background();
			enum location
			{
				NW = 7, NN = 8, NE = 9,
				CW = 4, CC = 5, CE = 6,
				SW = 1, SS = 2, SE = 3
			};
			void set_location( uint );
			void set_position( int, int );
			void set_speed( int, int );

			uint tex_w;
			uint tex_h;
			uint draw_w;
			uint draw_h;
			int  draw_x;
			int  draw_y;
			int  speed_x;
			int  speed_y;
			bool repeat;
			bool tex_ready;
			bool drw_ready;
			bool pos_ready;
			bool spd_ready;
			bool ready;
			sf::Texture texture;
	};
}

