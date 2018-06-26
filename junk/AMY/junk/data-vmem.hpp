#ifndef RFS_VMEM
#define RFS_VMEM

#include "defs.hpp"
#include "funcs.hpp"

namespace rfs
{
	class VMem
	{
		public:
			VMem ( uint size );
			~VMem();

			void   pad( uint front, uint back );
			void   dim( uint front, uint back );
			void   pad2fit( uint blksize );
			void   dim2fit();
			bool   set_char( uint pos, uint size, uchar* memblk );
			uchar* get_char( uint pos, uint size );
			bool   set_int ( uint pos, uint size, uint n, bool bigend=false );
			uint   get_int ( uint pos, uint size, bool bigend=false );
			ascii  get_str ( uint pos, uint size=0 );

			uint   size;
			uchar* memblk;
	};
}

#endif
