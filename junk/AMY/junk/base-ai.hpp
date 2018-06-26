#ifndef RFS_AI
#define RFS_AI

#include "defs.hpp"
#include "data-struct.hpp"

namespace rfs
{
	class AI
	{
		public:
			AI();
			~AI();


			bool is_done;
			rfs::Pos  target;
	};
}

#endif
