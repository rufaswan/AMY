#include "define.h"
using namespace amy;
extern State STATE;

baseScreen::baseScreen()
{
	baseScreen::running = true;
}
baseScreen::~baseScreen() {}

void baseScreen::keyinput()
{
	// EVENT LOOP, KEYBOARD Input Delay applied
	sf::Event EV;
	while ( STATE.SCREEN.pollEvent(EV) )
	{
		switch(EV.type)
		{
			case sf::Event::Closed:			STATE.SCREEN.close();			break;
			default:						break;
		}
	}

	// REAL TIME INPUT, no delay
	// keyboard related
	if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) )	STATE.SCREEN.close();

	baseScreen::set_kb(sf::Keyboard::Up,	KEY_UP,		KEY_DN);
	baseScreen::set_kb(sf::Keyboard::Down,	KEY_DN,		KEY_UP);
	baseScreen::set_kb(sf::Keyboard::Left,	KEY_LF,		KEY_RT);
	baseScreen::set_kb(sf::Keyboard::Right,	KEY_RT,		KEY_LF);
	baseScreen::set_kb(sf::Keyboard::Q,		KEY_L_TR,	KEY_R_TR);
	baseScreen::set_kb(sf::Keyboard::W,		KEY_R_TR,	KEY_L_TR);
	baseScreen::set_kb(sf::Keyboard::A,		KEY_SHT,	KEY_RPD);
	baseScreen::set_kb(sf::Keyboard::S,		KEY_RPD,	KEY_SHT);

	baseScreen::set_kb(sf::Keyboard::Z,		KEY_JMP);
	baseScreen::set_kb(sf::Keyboard::X,		KEY_DSH);
	baseScreen::set_kb(sf::Keyboard::C,		KEY_JMP);
	baseScreen::set_kb(sf::Keyboard::C,		KEY_DSH);
	baseScreen::set_kb(sf::Keyboard::Space,		KEY_STR);
	baseScreen::set_kb(sf::Keyboard::LShift,	KEY_SEL);

	// joystick related - to-do

	//baseScreen::add_keydata();
}

void baseScreen::set_kb(sf::Keyboard::Key key, int stkey, int unstkey)
{
	if ( sf::Keyboard::isKeyPressed( key ) )
	{
		//printf( "get key: %s unset %s\n", stkey.c_str(), unstkey.c_str() );
		STATE.KEY_PRESS = true;
		STATE.KEYS[ stkey ] = true;
		STATE.KEYS[ unstkey ] = false;
	}
}

void baseScreen::add_keydata()
{
	int k = 0;
	if ( STATE.KEYS[ KEY_UP ] )		k += KEY_UP;
	if ( STATE.KEYS[ KEY_DN ] )		k += KEY_DN;
	if ( STATE.KEYS[ KEY_LF ] )		k += KEY_LF;
	if ( STATE.KEYS[ KEY_RT ] )		k += KEY_RT;
	if ( STATE.KEYS[ KEY_SHT ] )	k += KEY_SHT;
	if ( STATE.KEYS[ KEY_RPD ] )	k += KEY_RPD;
	if ( STATE.KEYS[ KEY_JMP ] )	k += KEY_JMP;
	if ( STATE.KEYS[ KEY_DSH ] )	k += KEY_DSH;
	if ( STATE.KEYS[ KEY_L_TR ] )	k += KEY_L_TR;
	if ( STATE.KEYS[ KEY_R_TR ] )	k += KEY_R_TR;
	if ( STATE.KEYS[ KEY_SEL ] )	k += KEY_SEL;
	if ( STATE.KEYS[ KEY_STR ] )	k += KEY_STR;

	// add key to key data (for command trigger, like a fighting game)
	if ( STATE.KEYSDATA.empty() )
		STATE.KEYSDATA.push_back( k );
	else
	{
		// if same key, ignore
		if ( STATE.KEYSDATA.back() & k )
			STATE.KEY_HOLD = true;
		else
		{
			STATE.KEY_HOLD = false;
			STATE.KEYSDATA.push_back( k );
		}
	}

	/*
		Actor::action_queue.push_back( action );

		first = Actor::action_queue.front();
		last =  Actor::action_queue.back();

		push_front(a) = add a before the first item
		push_back(a)  = add a after the last item

		pop_front() = remove the first item
		pop_back()  = remove the last item

		listsize = size();
		// size 10, array 0-9

		if ( Actor::action_queue.empty() )
			Actor::is_action = false;
		else
		{
			Actor::is_action = true;
			Actor::exec_action( Actor::action_queue.first() );
			Actor::action_queue.pop_front();
		}
	*/
}
