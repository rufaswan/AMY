#include "data-struct.hpp"

namespace rfs
{
	Pos::Pos()             { Pos::set(0,0); }
	Pos::Pos(int x, int y) { Pos::set(x,y); }
	Pos::~Pos() {}
	void Pos::set(int x, int y)
	{
		Pos::x = x;
		Pos::y = y;
	}

	Size::Size()               { Size::set(0,0); }
	Size::Size(uint w, uint h) { Size::set(w,h); }
	Size::~Size() {}
	void Size::set_w(uint w)   { Size::set(      w,Size::h); }
	void Size::set_h(uint h)   { Size::set(Size::w,      h); }
	void Size::swap()          { Size::set(Size::h,Size::w); }
	void Size::set(uint w, uint h)
	{
		Size::w = w;
		Size::h = h;
		Size::s = w * h;
	}

	Zoom::Zoom()               { Zoom::set(100,100); }
	Zoom::Zoom(uint x, uint y) { Zoom::set(  x,  y); }
	Zoom::~Zoom() {}
	void Zoom::set_x(uint x)   { Zoom::set(      x,Zoom::y); }
	void Zoom::set_y(uint y)   { Zoom::set(Zoom::x,      y); }
	void Zoom::swap()          { Zoom::set(Zoom::y,Zoom::x); }
	void Zoom::set(uint x, uint y)
	{
		Zoom::x = x;
		Zoom::y = y;
		Zoom::z = ( Zoom::x > y ) ? Zoom::y : Zoom::x;
	}

	Distance::Distance()             { Distance::set(0,0); }
	Distance::Distance(int a,int b)  { Distance::set(a,b); }
	Distance::~Distance() {}
	void Distance::set_a(int a)      { Distance::set(          a,Distance::b); }
	void Distance::set_b(int b)      { Distance::set(Distance::a,          b); }
	void Distance::swap()            { Distance::set(Distance::b,Distance::a); }
	void Distance::set(int a, int b)
	{
		Distance::a = a;
		Distance::b = b;
		Distance::d1 = b - a;
		Distance::d2 = ( Distance::d1 > 0 ) ? Distance::d1 : Distance::d1 * -1;
	}

}
