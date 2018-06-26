#include "data-timer.hpp"

namespace rfs
{
	Timer::Timer()
	{
		Timer::clock1   = std::clock();
		Timer::clock2   = std::clock();
		Timer::is_track = false;
	}
	Timer::~Timer() {}

	void Timer::start()
	{
		Timer::clock1   = std::clock();
		Timer::is_track = true;
	}

	void Timer::stop()
	{
		Timer::clock2   = std::clock();
		Timer::is_track = false;
	}

	uint Timer::diff_msec()
	{
		int diff = 0;

		// if havent stopped
		if ( Timer::is_track )
		{
			clock_t  now  = std::clock();
			diff = ( now - Timer::clock1 ) * 1000 / CLOCKS_PER_SEC;
		}
		else
			diff = ( Timer::clock2 - Timer::clock1 ) * 1000 / CLOCKS_PER_SEC;

		// return positive value only
		uint t = ( diff > 0 ) ? diff : 0;
		return t;
	}
}
