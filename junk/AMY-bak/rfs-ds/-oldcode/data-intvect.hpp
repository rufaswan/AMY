#ifndef RFS_INTVECT
#define RFS_INTVECT

#include "defs.hpp"

namespace rfs
{
	class IntVect
	{
		public:
			IntVect();
			~IntVect();

			void insert( int d,      uint offset=0 );
			void insert( intvect &d, uint offset=0 );
			void insert( IntVect &d, uint offset=0 );
			void append( int d );
			void append( intvect &d );
			void append( IntVect &d );
			bool del( uint index );
			void clear();
			int  find( int d ); // to find duplicates
			intvect  getdata(); // returned for foreach loop
		private:
			std::deque < int >  data;
	};
}

#endif
