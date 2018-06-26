#ifndef AMY_PLAYLIST
#define AMY_PLAYLIST

#include "common.hpp"

namespace amy
{
	class Playlist
	{
		public:
			Playlist();
			~Playlist();

			void tick();
			void play();
			void stop();

		private:
			void state_wait();
			void state_play();

			bool to_play;
			bool is_play;
			uint volumn;
			uint to_remove;
			uint now_playing;
	};
}

#endif

/*
- if no song added, play() wont set the required flags
- if stop(), reduce the volume by 1 on each tick()
- when fully silence, stop completely
- the songs will be played in the order of the playlist
- when play(), tick() will check if it is the last song
- the last song will repeat FOREVER, all other will play only ONCE


 */
