#ifndef RFS_RECT
#define RFS_RECT

#include "defs.hpp"
#include "data-struct.hpp"

namespace rfs
{
	class Rect
	{
		public:
			Rect();
			Rect( int w, int h, int x=0, int y=0 );
			~Rect();

			void init();
			void setsize( int w, int h );
			void setpos ( int x, int y );
			bool within ( int x, int y );
			bool within ( rfs::Rect& );
			bool contact( rfs::Rect& );

			rfs::Pos  pos;
			rfs::Size size;
			int vtop;
			int vmid;
			int vbtm;
			int hleft;
			int hmid;
			int hright;
	};
}

#endif
