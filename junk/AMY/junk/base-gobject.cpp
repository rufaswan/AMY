#include "base-gobject.hpp"

namespace rfs
{
	GObject::GObject()  { GObject::init(); }
	GObject::~GObject() {}

	void GObject::init()
	{
		GObject::pos.set (0,0);
		GObject::size.set(0,0);
	}

	void GObject::setrect( int x, int y, int w, int h )
	{
		GObject::pos.set (x,y);
		GObject::size.set(w,h);
		GObject::rect.setsize( w, h );
		GObject::rect.setpos ( x, y );
	}
}
