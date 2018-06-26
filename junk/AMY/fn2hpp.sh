#!/bin/bash

[ $# = 0 ] && exit

for i in "$@"; do

	if [ -d "$i" ]; then
		fs=$(find "$i" -type f)
		for f in $fs; do
			new=$(echo "$f" | tr '[a-z]' '[A-Z]' | tr -c [0-9A-Z] '_' )
			echo "#define $new \"$f\""
		done
	else
		new=$(echo "$i" | tr '[a-z]' '[A-Z]' | tr -c [0-9A-Z] '_' )
		echo "#define $new \"$i\""
	fi

done
