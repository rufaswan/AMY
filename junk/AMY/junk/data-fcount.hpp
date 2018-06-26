#ifndef RFS_FCOUNT
#define RFS_FCOUNT

#include "defs.hpp"

namespace rfs
{
	class FCount
	{
		public:
			FCount();
			~FCount();

			void tick();
			uint vfps;
		private:
			uint vno;
			clock_t  clock;
	};
}

#endif
