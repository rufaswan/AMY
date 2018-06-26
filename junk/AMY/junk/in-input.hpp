#ifndef AMY_INPUT
#define AMY_INPUT

#include "common.hpp"
#include "in-display.hpp"
#include "rfstoolset/data-struct.hpp"

namespace amy
{
	class Input
	{
		public:
			Input();
			~Input();

			void reg_kb( uchar key, sf::Keyboard::Key  sfkey );
			void tick   ( amy::Display &display );
			void tick_ev( amy::Display &display );
			void tick_kb();

			std::map < uchar, amy::Key* >  kbmap;

		protected:
		private:
	};
}

#endif
