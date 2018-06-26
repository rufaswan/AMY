namespace amy
{
	class RawRecorder : public Recorder
	{
		public:
			RawRecorder();
			~RawRecorder();

			void init();
			void unset();
			void record_video( sf::Image&, int last=0 );
			void record_audio();
			void write_yuv444();
			void write_yuv422();
			void write_yuv420();
	};
}
