#ifndef RFS_FRAME
#define RFS_FRAME

#include "defs.hpp"

namespace rfs
{
	class Frame
	{
		public:
			Frame();
			~Frame();
			void setframes( bool repeat, uint no, ... );
			int  play();
			void stop();

			uint    size;
			uint    now;
			bool    repeat;
			intvect frame;
	};
}

#endif