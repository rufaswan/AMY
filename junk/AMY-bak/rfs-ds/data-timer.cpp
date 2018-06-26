#include "header.hpp"
using namespace amy;

Timer::Timer()
{
	Timer::minisec     = 0;
	Timer::minifps     = 0;
	Timer::cnt_fps     = 0;
	Timer::cnt_fps_sec = 0;
	Timer::cnt_frames  = 0;
	Timer::sec_frames  = 0;
}
Timer::~Timer() {}

bool Timer::can_frame_update( uint frame )
{
	Timer::cnt_frames++;
	uint left = Timer::cnt_frames % frame;
	if ( left == 0 )
		return true;
	return false;
}

bool Timer::can_msec_update( uint minisec )
{
	uint msec = Timer::clock.getElapsedTime().asMilliseconds();
	uint fdif = msec - Timer::minifps;

	if ( fdif > minisec )
	{
		Timer::minifps += fdif;
		return true;
	}
	return false;
}

bool Timer::can_fps_update( uint fps )
{
	uint msec = Timer::clock.getElapsedTime().asMilliseconds();
	uint fdif = msec - Timer::minifps;

	uint fps_sec = 1000 / fps;
	if ( fdif > fps_sec )
	{
		Timer::minifps += fdif;
		return true;
	}
	return false;
}

uint Timer::get_fps()
{
	uint msec = Timer::clock.getElapsedTime().asMilliseconds();
	Timer::cnt_fps_sec++;
	//printf("msec %i mdif %i\n", msec, mdif);

	// if over one second, print result and reset for next loop
	uint mdif = msec - Timer::minisec;
	if ( mdif > 1000 )
	{
		Timer::sec_frames  = Timer::cnt_fps_sec;
		Timer::cnt_fps_sec = 0;
		Timer::minisec += mdif;
	}
	return Timer::sec_frames;
}
