namespace amy
{
	class Timer
	{
		public:
			Timer();
			~Timer();

			bool can_frame_update( uint ); // by loop
			bool can_msec_update ( uint ); // by time
			bool can_fps_update  ( uint ); // by time
			uint get_fps();
		private:
			uint minisec; // for counting x frames per sec
			uint minifps; // for enforcing custom fps
			uint cnt_fps;     // custom fps
			uint cnt_fps_sec; // actual fps
			uint cnt_frames;  // frame
			uint sec_frames;  // frame, holded for drawing
			sf::Clock clock;
	};
}
