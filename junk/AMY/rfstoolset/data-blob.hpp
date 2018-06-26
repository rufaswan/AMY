#ifndef RFS_BLOB
#define RFS_BLOB

#include "defs.hpp"
#include "funcs.hpp"

namespace rfs
{
	class Blob
	{
		public:
			Blob();
			~Blob();

			bool  loadfile( const ascii &file );
			bool  loadblob( rfs::Blob &bin, uint offset=0, uint size=0 );
			bool  savefile( const ascii &file );
			void  append( rfs::Blob &bin );
			void  update( rfs::Blob &bin, uint offset=0 );
			void  insert( rfs::Blob &bin, uint offset=0 );//
			void* getdataptr();//
			uint  getsize();//
			uint  getint  ( uint offset=0, uint byte=1 );
			ascii getascii( uint offset=0, uint len=0 );
			void  clear();
			void  debug( bool showdata=false );
		private:
			uint size;//
			bool has_data;
		public:
			charvect  data;
	};
}

/*
setint(uint, uint offset, uint size)
setdata( uint size, ... )
padding( uint nearest )
*/

#endif