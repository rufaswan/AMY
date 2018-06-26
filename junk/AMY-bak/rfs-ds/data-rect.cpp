#include "header.hpp"
using namespace amy;

RectData::RectData() {}
RectData::~RectData() {}

void RectData::setsize(uint w, uint h)
{
	RectData::width  = w;
	RectData::height = h;
}

void RectData::tl_setsize(int x, int y, uint w, uint h)
{
	RectData::top    = y;
	RectData::bottom = y + h;
	RectData::left   = x;
	RectData::right  = x + w;
	RectData::setsize( w, h);
}

void RectData::tr_setsize(int x, int y, uint w, uint h)
{
	RectData::top    = y;
	RectData::bottom = y + h;
	RectData::left   = x - w;
	RectData::right  = x;
	RectData::setsize( w, h);
}

void RectData::bc_setsize(int x, int y, uint w, uint h)
{
	RectData::top    = y - h;
	RectData::bottom = y;
	RectData::left   = x - ( w / 2 );
	RectData::right  = x + ( w / 2 );
	RectData::setsize( w, h);
}

void RectData::cc_setsize(int x, int y, uint w, uint h)
{
	RectData::top    = y - ( h / 2 );
	RectData::bottom = y + ( h / 2 );
	RectData::left   = x - ( w / 2 );
	RectData::right  = x + ( w / 2 );
	RectData::setsize( w, h);
}

bool RectData::within( int x, int y )
{
	// there are no rect
	if ( RectData::width  < 1 ) return false;
	if ( RectData::height < 1 ) return false;

	bool ct = ( RectData::top    < y ) ? true : false;
	bool cb = ( RectData::bottom > y ) ? true : false;
	bool cl = ( RectData::left   < x ) ? true : false;
	bool cr = ( RectData::right  > x ) ? true : false;

	if ( cl && cr )
	{
		if ( ct && cb )
			return true;
	}

	return false;
}

bool RectData::within( amy::RectData &rect )
{
	// there are no rect
	if ( RectData::width  < 1 ) return false;
	if ( RectData::height < 1 ) return false;

	// this rect is at least SAME SIZE or BIGGER than the rect passed
	if ( rect.width  > RectData::width  ) return false;
	if ( rect.height > RectData::height ) return false;

	if ( rect.top >= RectData::top && rect.left >= RectData::left )
	{
		if ( RectData::bottom >= rect.bottom && RectData::right >= rect.right )
			return true;
	}
	return false;
}

bool RectData::contact( amy::RectData &rect )
{
	// there are no rect
	if ( RectData::width  < 1 ) return false;
	if ( RectData::height < 1 ) return false;

	// +---+       +---+
	// |   |       |   |
	// | +---+   +---+ |
	// | |   |   |   | |
	// | +---+   +---+ |
	// |   |       |   |
	// +---+       +---+
	//
	bool cl = ( rect.right       > RectData::left ) ? true : false;
	bool cr = ( RectData::right  > rect.left   )    ? true : false;
	if ( cl && cr )
	{
		bool ct = ( rect.bottom      > RectData::top )  ? true : false;
		bool cb = ( RectData::bottom > rect.top    )    ? true : false;
		if ( ct && cb )
			return true;
	}
	return false;
}
