namespace amy
{
	class Utils
	{
		public:
			Utils();
			~Utils();

			// for using C style rb, wb, ab with C++ fstream
			void fopen( std::fstream &fp, const std::string &file, const std::string &mode );

			// load an image data and return them as a vector of texture
			void set_tileset( sf::Image &sfimg, int tw, int th, vect_tex &vect );

			// padding zeroes to int for use with filenames
			std::string int2str( int, uint=0);
	};
}
