#include "define.h"
using namespace amy;

State::State()
{
	State::FRAMECNT		= 0;
	State::FRAMECNT_2R	= 0;
	State::FRAMECNT_3R	= 0;
	State::FRAMECNT_4R	= 0;
	State::FRAMECNT_5R	= 0;
	State::FRAMECNT_6R	= 0;
	State::FRAMECNT_2	= false;
	State::FRAMECNT_3	= false;
	State::FRAMECNT_4	= false;
	State::FRAMECNT_5	= false;
	State::FRAMECNT_6	= false;
	State::reset_keys();
	State::REPLAYDATA.clear();
	State::KEYSDATA.clear();
}
State::~State() {}

void State::update()
{
	State::CAMERA.update();
	State::reset_keys();
	State::FRAMECNT++;
	State::FRAMECNT_2 = ( State::FRAMECNT % 2 == 0 ) ? true : false;
	State::FRAMECNT_3 = ( State::FRAMECNT % 3 == 0 ) ? true : false;
	State::FRAMECNT_4 = ( State::FRAMECNT % 4 == 0 ) ? true : false;
	State::FRAMECNT_5 = ( State::FRAMECNT % 5 == 0 ) ? true : false;
	State::FRAMECNT_6 = ( State::FRAMECNT % 6 == 0 ) ? true : false;
	if ( State::FRAMECNT_2 )	State::FRAMECNT_2R++;
	if ( State::FRAMECNT_3 )	State::FRAMECNT_3R++;
	if ( State::FRAMECNT_4 )	State::FRAMECNT_4R++;
	if ( State::FRAMECNT_5 )	State::FRAMECNT_5R++;
	if ( State::FRAMECNT_6 )	State::FRAMECNT_6R++;
}

void State::reset_keys()
{
	State::KEY_PRESS			= false;
	State::KEYS.clear();
	State::KEYS[ KEY_UP ]		= false;
	State::KEYS[ KEY_DN ]		= false;
	State::KEYS[ KEY_LF ]		= false;
	State::KEYS[ KEY_RT ]		= false;
	State::KEYS[ KEY_SHT ]		= false;
	State::KEYS[ KEY_RPD ]		= false;
	State::KEYS[ KEY_JMP ]		= false;
	State::KEYS[ KEY_DSH ]		= false;
	State::KEYS[ KEY_L_TR ]		= false;
	State::KEYS[ KEY_R_TR ]		= false;
	State::KEYS[ KEY_SEL ]		= false;
	State::KEYS[ KEY_STR ]		= false;
}

void State::add_tileimg(const std::string& file)
{
	// add tileimg only if not found
	if ( State::TILEIMG.find(file) == State::TILEIMG.end() )
	{
		printf(">> adding tileimg to state %s\n", file.c_str() );
		sf::Image char_img;
		char_img.loadFromFile(file);
		State::TILEIMG[file] = char_img;
	}
}

void State::add_replaydata(int key)
{
	State::REPLAYDATA.push_back(key);
}

bool State::within(int x, int min, int max)
{
	if ( x >= min )
		if ( x <= max )
			return true;
	return false;
}