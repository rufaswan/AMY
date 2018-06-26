#include "define.h"
using namespace amy;

Rect::Rect()
{
	Rect::collided	= false;
	Rect::set_rect(0, 0, TILE_W, TILE_H);
}
Rect::~Rect() {}

void Rect::set_rect(int x, int y, int w, int h)
{
	Rect::x			= x;
	Rect::y			= y;
	Rect::width		= w;
	Rect::height	= h;
	Rect::top		= Rect::y;
	Rect::bottom	= Rect::y + Rect::height - 1;
	Rect::left		= Rect::x;
	Rect::right		= Rect::x + Rect::width - 1;
}

// check if pixel is within rect
bool Rect::within(int x, int y)
{
	bool cl = ( Rect::left   < x) ? true : false;
	bool cr = ( Rect::right  > x) ? true : false;
	bool ct = ( Rect::top    < y) ? true : false;
	bool cb = ( Rect::bottom > y) ? true : false;

	if ( cl && cr )
		if ( ct && cb )
		{
			Rect::collided = true;
			return true;
		}

	return false;
}

// check for collision
// if collide, return true & the sides collided
// if not, return false on everything.
bool Rect::collision( const Rect &rect )
{
	Rect::collide_top		= false;
	Rect::collide_bottom	= false;
	Rect::collide_left		= false;
	Rect::collide_right		= false;

	// +---+       +---+
	// |   |       |   |
	// | +---+   +---+ |
	// | |   |   |   | |
	// | +---+   +---+ |
	// |   |       |   |
	// +---+       +---+
	//
	bool collide = false;
	bool cl = ( Rect::left   < rect.right)  ? true : false;
	bool cr = ( Rect::right  > rect.left)   ? true : false;
	bool ct = ( Rect::top    < rect.bottom) ? true : false;
	bool cb = ( Rect::bottom > rect.top)    ? true : false;

	if ( cl || cr )
		if ( ct || cb )
			collide = true;

	if ( collide )
	{
		Rect::collide_left		= cl;
		Rect::collide_right		= cr;
		Rect::collide_top		= ct;
		Rect::collide_bottom	= cb;
		Rect::collided			= true;
		return true;
	}

	return false;
}
