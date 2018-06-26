#ifndef RFS_GOBJGRP
#define RFS_GOBJGRP

#include "defs.hpp"
#include "base-gobject.hpp"

namespace rfs
{
	class GObjGroup
	{
		public:
			GObjGroup();
			~GObjGroup();

			void init();
			bool add( rfs::GObject *gobj );
			bool del( uint id );

			uint id;
			std::map < uint, rfs::GObject* >  gobj;
			intdeq  grp_eff;  // effects & particles, no rect & no check
			intdeq  grp_plat; // platforms, has rect but no check
			intdeq  grp_mov;  // moving objects
			intdeq  grp_inv;  // invalids, to be del
	};
}

#endif
