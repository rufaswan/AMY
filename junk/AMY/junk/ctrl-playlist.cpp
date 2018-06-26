#include "ctrl-playlist.hpp"

namespace amy
{
	Playlist::Playlist()
	{
		Playlist::to_play = false;
		Playlist::is_play = false;
		Playlist::now_playing = 0;
		Playlist::to_remove   = 0;
	}
	Playlist::~Playlist() {}

	void Playlist::tick()
	{
		if ( Playlist::is_play )
			Playlist::state_play();
		else
			Playlist::state_wait();
	}

	void Playlist::state_play()
	{
		// play() command issued
		if ( Playlist::to_play )
		{
		}
		// stop() command issued
		else
		{
		}
	}

	void Playlist::state_wait()
	{
		// play() command issued
		if ( Playlist::to_play )
		{
		}
		// stop() command issued
		else
		{
		}
	}

	void Playlist::play()  { Playlist::to_play = true;  }
	void Playlist::stop()  { Playlist::to_play = false; }

}
