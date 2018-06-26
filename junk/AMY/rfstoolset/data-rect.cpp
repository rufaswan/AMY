#include "data-rect.hpp"

namespace rfs
{
	Rect::Rect()  { Rect::init(); }
	Rect::~Rect() {}

	Rect::Rect(int w, int h, int x, int y)
	{
		Rect::setsize(w, h);
		Rect::setpos (x, y);
	}

	void Rect::init()
	{
		Rect::pos.set (0,0);
		Rect::size.set(0,0);
		Rect::vtop  = Rect::vmid = Rect::vbtm   = 0;
		Rect::hleft = Rect::hmid = Rect::hright = 0;
	}

	void Rect::setsize(int w, int h)  { Rect::size.set(w,h) ; }
	void Rect::setpos (int x, int y)
	{
		Rect::pos.set(x,y);

		if ( Rect::size.w > 0 )
		{
			Rect::hleft  = x - ( Rect::size.w / 2 );
			Rect::hmid   = x;
			Rect::hright = x + ( Rect::size.w / 2 );
		}

		if ( Rect::size.h > 0 )
		{
			Rect::vtop = y - ( Rect::size.h / 2 );
			Rect::vmid = y;
			Rect::vbtm = y + ( Rect::size.h / 2 );
		}
	}

	bool Rect::within( int x, int y )
	{
		// there are no rect
		if ( Rect::size.w < 1 ) return false;
		if ( Rect::size.h < 1 ) return false;

		bool cl = ( Rect::hleft  < x ) ? true : false;
		bool cr = ( Rect::hright > x ) ? true : false;

		if ( cl && cr )
		{
			bool ct = ( Rect::vtop < y ) ? true : false;
			bool cb = ( Rect::vbtm > y ) ? true : false;
			if ( ct && cb )
				return true;
		}

		return false;
	}

	bool Rect::within( rfs::Rect &r )
	{
		// there are no rect
		if ( Rect::size.w < 1 ) return false;
		if ( Rect::size.h < 1 ) return false;

		// the rect is SAME SIZE or LARGER than this rect
		if ( r.size.w >= Rect::size.w && r.size.h >= Rect::size.h )
		{
			if ( r.vtop >= Rect::vtop && r.hleft >= Rect::hleft )
			{
				if ( Rect::vbtm >= r.vbtm && Rect::hright >= r.hright )
					return true;
			}
		}
		// the rect is SMALLER than this rect
		else
		{
			if ( Rect::vtop >= r.vtop && Rect::hleft >= r.hleft )
			{
				if ( r.vbtm >= Rect::vbtm && r.hright >= Rect::hright )
					return true;
			}
		}
		return false;
	}

	bool Rect::contact( rfs::Rect &r )
	{
		// there are no rect
		if ( Rect::size.w < 1 ) return false;
		if ( Rect::size.h < 1 ) return false;

		// +---+       +---+
		// |   |       |   |
		// | +---+   +-------+
		// +-|   |   |       |
		//   +---+   +-------+
		//
		bool cl = ( r.hright     > Rect::hleft ) ? true : false;
		bool cr = ( Rect::hright > r.hleft     ) ? true : false;
		if ( cl && cr )
		{
			bool ct = ( r.vbtm     > Rect::vtop ) ? true : false;
			bool cb = ( Rect::vbtm > r.vtop     ) ? true : false;
			if ( ct && cb )
				return true;
		}
		return false;
	}
}
