#!/bin/bash

[ $# = 0 ] && exit

for i in "$@"; do

	# data-vmem => DATA_VMEM
	up=$(printf "%s" "$i" | tr [a-z] [A-Z] | tr -c [0-9A-Z] '_' )

	[ -f "$i".hpp ] || touch "$i".hpp
	cat << _HPP > "$i".hpp
#ifndef $up
#define $up

#include ""

namespace ###
{
	class @@@
	{
		public:
			@@@();
			~@@@();
		protected:
		private:
	};
}

#endif
_HPP

	[ -f "$i".cpp ] || touch "$i".cpp
	cat << _CPP >> "$i".cpp
#include "$i.hpp"

namespace ###
{
	@@@::@@@()  {}
	@@@::~@@@() {}
}
_CPP

done
