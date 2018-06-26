namespace amy
{
	class Recorder
	{
		public:
			Recorder();
			~Recorder();

			void  trigger ( sf::RenderTexture& );
			void  start   ( sf::RenderTexture& ); // get the first frame for info
			void  stop    ( sf::RenderTexture& ); // get the last frame for EOF header
			void  addframe( sf::RenderTexture& ); // start buffering frames for video
			void  rmframe ();
			void  record  ( sf::RenderTexture& );
			void  run();
			void  binary_write(uchar*, uint);
			void  rgba2yuv420( sf::Image &img );
			void  rgba2yuv422( sf::Image &img );
			void  rgba2yuv444( sf::Image &img );
			uchar clamp( int );
			virtual void init();
			virtual void unset();
			virtual void record_video( sf::Image&, int last=0 );
			virtual void record_audio();
			std::string get_filename( const std::string& ext );

			bool started;
			bool stopped;
			bool done;
			uint width;
			uint height;
			uint mov_no;
			uint enc_no;
			std::fstream wav_fs;
			std::string  wav_file;
			std::fstream mov_fs;
			std::string  mov_file;
			std::string  mov_name;
			std::vector < uchar >       yuv_y;
			std::vector < uchar >       yuv_u;
			std::vector < uchar >       yuv_v;
			std::queue  < sf::Texture > vbuf;
	};
}
