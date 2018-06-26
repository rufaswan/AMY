#ifndef RFS_TIMER
#define RFS_TIMER

#include "defs.hpp"

namespace rfs
{
	class Timer
	{
		public:
			Timer();
			~Timer();

			void start();
			void stop();
			uint diff_msec();

			bool is_track;
			clock_t  clock1;
			clock_t  clock2;
	};
}

#endif
