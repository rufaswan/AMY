#include "header.hpp"
using namespace amy;
extern PlayData DATA;

Events::Events()
{
	Events::is_dialog  = false;
	Events::checkpoint = 1;
}
Events::~Events() {}

void Events::mk_objs( int type, int pos_x, int pos_y, bool is_left, int hp ) {}

void Events::inputs()
{
	if ( Events::is_dialog )
	{

/*
		if ( DATA.dialog.end() )
			Events::is_dialog = false
		else
		{
			if ( DATA.dialog.waitinginput() )
			{
				if ( ! DATA.input.is_nopress() )
					DATA.dialog.nextpage();
			}
			else
				DATA.dialog.update();
		}
*/

	}
	else
	{

/*
		if ( DATA.object.player()->is_action )
			DATA.object.player()->idle();
		else
		{
			if ( DATA.input.is_nopress() )
				DATA.object.player()->idle();
			else
			{
				if ( DATA.input.is_pressed('L') )  DATA.object.mv_left();
				if ( DATA.input.is_pressed('R') )  DATA.object.mv_right();
			}
		}
*/
	}
}

