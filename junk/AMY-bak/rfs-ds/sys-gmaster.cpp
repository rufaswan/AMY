#include "header.hpp"
using namespace amy;
extern PlayData DATA;

GameMaster::GameMaster()
{
	GameMaster::now_screen = "x3seahorse"
	GameMaster::ready      = false;
}
GameMaster::~GameMaster() {}

void GameMaster::init(uint w, uint h, const std::string &title)
{
	printf(">> GameMaster::init( %i, %i, %s )\n", w, h, title.c_str());

	bool error = false;
	if ( (w % 16) != 0 ) { printf("ERROR : width must be multiply of 16!"); error = true; }
	if ( (h % 16) != 0 ) { printf("ERROR : height must be multiply of 16!"); error = true; }
	if ( error ) return;

	GameMaster::screen.create( sf::VideoMode( w, h ), title );
	GameMaster::screen.setMouseCursorVisible(false);
	GameMaster::screen.setPosition( sf::Vector2i(0,0) );
	GameMaster::screen.setVerticalSyncEnabled(true);
	//GameMaster::screen.setFramerateLimit( fps );

	GameMaster::msec = 1000 / FPS;
	GameMaster::ready = true;
}

void GameMaster::run()
{
	if ( ! GameMaster::ready ) return;

	printf(">> GameMaster::run()\n");
	while ( GameMaster::screen.isOpen() )
		GameMaster::now_screen = GameMaster::run_level( GameMaster::now_screen );
}

int GameMaster::run_level( std::string &lvl)
{
	printf(">> GameMaster::run_level( %s )\n", lvl.c_str() );
	// point to the class that is going to run
	switch (lvl)
	{
		case "x1stgselect":
			GameMaster::display = new X1StgSlt;
			break;
		case "x1intro":
			GameMaster::display = new X1Intro;
			break;
		case "x3seahorse":
			GameMaster::display = new X3Seahorse;
			break;
		default:
			GameMaster::screen.close();
			break;
	}

	GameMaster::display->running( GameMaster::screen, GameMaster::msec );

	// finishing up & prepare for the next loop
	std::string next = GameMaster::display->next();
	delete GameMaster::display;
	GameMaster::display = NULL;
	return next;
}

