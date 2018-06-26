#include "header.hpp"
using namespace amy;
extern PlayData DATA;

Background::Background()
{
	Background::tex_w   = 0;
	Background::tex_h   = 0;
	Background::draw_w  = 0;
	Background::draw_h  = 0;
	Background::draw_x  = 0;
	Background::draw_y  = 0;
	Background::speed_x = 0;
	Background::speed_y = 0;
	Background::repeat    = false;
	Background::tex_ready = false;
	Background::pos_ready = false;
	Background::drw_ready = false;
	Background::spd_ready = false;
	Background::ready     = false;
}
Background::~Background() {}

void Background::set_location( uint loc )
{
	if ( Background::drw_ready )
	{
		printf(">> Background::set_location( %i )\n", loc);
		int nt, nc, nb, nl, nm, nr;
		nt = 0;
		nc = ( HEIGHT - (int)Background::draw_h ) / 2;
		nb =   HEIGHT - (int)Background::draw_h;
		nl = 0;
		nm = ( WIDTH - (int)Background::draw_w ) / 2;
		nr =   WIDTH - (int)Background::draw_w;
		printf("draw w %i h %i\n", Background::draw_w, Background::draw_h);
		printf("nt %i nc %i nb %i nl %i nm %i nr %i\n", nt, nc, nb, nl, nm, nr);

		switch( loc )
		{
			case Background::NW:
				Background::set_position( nl, nt );
				break;
			case Background::NN:
				Background::set_position( nm, nt );
				break;
			case Background::NE:
				Background::set_position( nr, nt );
				break;
			case Background::CW:
				Background::set_position( nl, nc );
				break;
			case Background::CC:
				Background::set_position( nm, nc );
				break;
			case Background::CE:
				Background::set_position( nr, nc );
				break;
			case Background::SW:
				Background::set_position( nl, nb );
				break;
			case Background::SS:
				Background::set_position( nm, nb );
				break;
			case Background::SE:
				Background::set_position( nr, nb );
				break;
		}
		printf("draw x %i y %i\n", Background::draw_x, Background::draw_y);
		Background::pos_ready = true;
	}
}

void Background::set_position( int x, int y )
{
	Background::draw_x = x;
	Background::draw_y = y;
}

void Background::set_speed( int x, int y )
{
	Background::speed_x   = x;
	Background::speed_y   = y;
	Background::spd_ready = true;
}
