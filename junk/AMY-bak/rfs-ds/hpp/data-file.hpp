namespace amy
{
	class FileData
	{
		public:
			FileData();
			~FileData();

			bool  loadFromFile( const std::string &file );
			void* get_data();

			sf::Image       get_sfimage();
			sf::Font        get_sffont();
			sf::SoundBuffer get_sfsndbuf();

			bool ready;
			uint size;
			std::vector < uchar > buffer;
	};
}

