#include "data-fcount.hpp"

namespace rfs
{
	FCount::FCount()
	{
		FCount::vfps  = 0;
		FCount::vno   = 0;
		FCount::clock = std::clock();
	}
	FCount::~FCount() {}

	void FCount::tick()
	{
		clock_t  now = std::clock();
		int     diff = ( now - FCount::clock ) * 1000 / CLOCKS_PER_SEC;

		FCount::vno++;

		// record and reset vfps on every second
		if ( diff > 1000 )
		{
			FCount::vfps  = FCount::vno;
			FCount::vno   = 0;
			FCount::clock = now;
		}

	}
}
