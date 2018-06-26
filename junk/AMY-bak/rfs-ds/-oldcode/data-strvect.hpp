#ifndef RFS_STRVECT
#define RFS_STRVECT

#include "defs.hpp"

namespace rfs
{
	class StrVect
	{
		public:
			StrVect();
			~StrVect();

			void insert( const ascii &d, uint offset=0 );
			void insert( strvect &d, uint offset=0 );
			void insert( StrVect &d, uint offset=0 );
			void append( const ascii &d );
			void append( strvect &d );
			void append( StrVect &d );
			bool del( uint index );
			void clear();
			int  find( const ascii &d ); // to find duplicates
			strvect  getdata(); // returned for foreach loop
		private:
			std::deque < ascii >  data;
	};
}

#endif
