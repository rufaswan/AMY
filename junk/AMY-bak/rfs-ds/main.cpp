#include "header.hpp"
amy::PlayData DATA;

// thread wrapper to run in background
// for amy::Recorder to process video frames
void thread_rec()
{
	for (;;)
	{
		DATA.rec->run();
		sf::sleep( sf::milliseconds(100) );
	}
}

// main app
int main()
{
	DATA.file.load( "misc", "mona.ttf" );
	DATA.file.load( "misc", "zekton_free.ttf" );
	DATA.file.load( "map", "debugtile-16x16.png" );

	DATA.tile.set_tileset( "map/debugtile-16x16.png" );

	DATA.debug = true;
	DATA.input.sshot_name = "amy";
	DATA.sffont  = DATA.file.data[ "misc/zekton_free.ttf" ].get_sffont();

	//DATA.rec = new amy::RawRecorder;
	DATA.rec = new amy::OggRecorder;
	//sf::Thread t1( &thread_rec );
	//t1.launch();

	amy::GameMaster game;
	game.init(WIDTH, HEIGHT*2, "Dual Screen");
	game.run();

	DATA.file.unload( "misc", "mona.ttf" );
	DATA.file.unload( "misc", "zekton_free.ttf" );
	DATA.file.unload( "map", "debugtile-16x16.png" );

	return 0;
}
