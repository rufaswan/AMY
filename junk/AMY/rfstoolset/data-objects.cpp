#include "data-objects.hpp"

namespace rfs
{
	Objects::Objects()
	{
		Objects::zindex = 0;
	}
	Objects::~Objects() {}
	void Objects::update( rfs::Objects *obj )  {}

	// SORTING FUNCS
	bool azsort_zindex( rfs::Objects *a, rfs::Objects *b )
		{ return ( a->zindex < b->zindex ); }

	bool zasort_zindex( rfs::Objects *a, rfs::Objects *b )
		{ return ( a->zindex > b->zindex ); }

	// Child class
	ObjCtrl::ObjCtrl()  {}
	ObjCtrl::~ObjCtrl() {}
	void ObjCtrl::update( rfs::ObjCtrl *obj )
	{
		if ( ObjCtrl::list.size() < 1 )  return;
		int i;
		for ( i=0; i < ObjCtrl::list.size(); i++ )
			ObjCtrl::list[i]->update( this );
	}

}
