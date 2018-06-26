#ifndef AMY_DISPLAY
#define AMY_DISPLAY

#include "common.hpp"
#include "rfstoolset/data-struct.hpp"

namespace amy
{
	class Display
	{
		public:
			Display();
			~Display();

			void create( uint w, uint h, const ascii &title, uint zoom=100 );
			void draw();

			bool is_created;
			bool is_open;
			rfs::Zoom  zoom;
			rfs::Size  size;
			sf::RenderTexture  sfrend;
			sf::RenderWindow   sfwin;
	};
}

#endif
