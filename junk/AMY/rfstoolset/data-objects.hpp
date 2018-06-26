#ifndef RFS_OBJECTS
#define RFS_OBJECTS

#include "defs.hpp"

namespace rfs
{
	class Objects
	{
		public:
			Objects();
			~Objects();

			int zindex;
			virtual void update( rfs::Objects *obj = NULL );

		protected:
		private:
	};

	// SORTING FUNCS
	bool azsort_zindex( rfs::Objects *a, rfs::Objects *b );
	bool zasort_zindex( rfs::Objects *a, rfs::Objects *b );

	// Child class
	class ObjCtrl : public Objects
	{
		public:
			ObjCtrl();
			~ObjCtrl();

			void update( rfs::Objects *obj = NULL );

			std::vector < rfs::Objects* >  list;
		protected:
		private:
	}

}

#endif
