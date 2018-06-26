// One Header to Include Them All!!!
#include "define.h"

amy::State STATE;

int switch_level( int lvl )
{
	//printf("%i\n", lvl);
	switch ( lvl )
	{
		case STG_SECRET:
		{
			STATE.STAGE = new StageSEC;
			delete STATE.STAGE;
			return STG_MA;
			break;
		}
		case STG_MA:
		{
			STATE.STAGE = new StageMARS;
			delete STATE.STAGE;
			return STG_TEST;
			break;
		}
		default: break;
	}
	return STG_MA;
}

int main()
{
	STATE.SCREEN.create(sf::VideoMode( WIDTH, HEIGHT ), "testing");
	STATE.SCREEN.setMouseCursorVisible(false);
	STATE.SCREEN.setPosition( sf::Vector2i(0,0) );
	STATE.SCREEN.setFramerateLimit(FPS);
	//STATE.SCREEN.setVerticalSyncEnabled(true);
	//------------------------------------------------------------
	STATE.PLAYER = new RockX;
	STATE.PLAYER->is_player = true;
	STATE.PLAYER->is_control = true;

	int result, screen_now;
	//printf("%i\n", ME);
	screen_now = STG_MA;

	while ( STATE.SCREEN.isOpen() )
	{
		result = switch_level( screen_now );
		screen_now = result;
		/*switch ( result )
		{
			default:
				screen_now = result;
		}*/
	}
	return 0;
}
