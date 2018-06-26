#ifndef AMY_INTERFACE
#define AMY_INTERFACE

#include "common.hpp"
#include "in-input.hpp"
#include "in-display.hpp"

namespace amy
{
	class Interface
	{
		public:
			Interface();
			~Interface();

			amy::Input    input;
			amy::Display  display;
	};
}

#endif
