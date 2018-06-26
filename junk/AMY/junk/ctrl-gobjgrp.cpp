#include "ctrl-gobjgrp.hpp"

namespace rfs
{
	GObjGroup::GObjGroup()  { GObjGroup::init(); }
	GObjGroup::~GObjGroup() {}

	void GObjGroup::init()
	{
		GObjGroup::id = 0;
		GObjGroup::gobj[0] = new GObject;
	}

	bool GObjGroup::add( rfs::GObject *gobj )
	{
		GObjGroup::id++;
		GObjGroup::gobj[ GObjGroup::id ] = gobj;
		return GObjGroup::id;
	}

	bool GObjGroup::del( uint id )
	{
		// delete if the id is valid
		if ( GObjGroup::gobj.find( id ) == GObjGroup::gobj.end() )
			return false;
		else
		{
			delete GObjGroup::gobj[ id ];
			GObjGroup::gobj.erase( id );
			return true;
		}
	}
}
