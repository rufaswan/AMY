#!/bin/bash

exe='compiled.exe'
obj='built'
gcc_flag='-g -Wall'
inc_path='-I./hpp'
lib_path='-L/lib -L/usr/lib'

ld_theora='-ltheoradec -ltheoraenc -ltheora'
ld_sfml='-lsfml-network -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system'

[ $# = 0 ] && { echo "test certain cpp error"; exit; }
[ ! -d "$obj" ] && mkdir "$obj"

# to test the exe
if [[ "$1" == "-b" ]]; then

	reset
	g++ $gcc_flag $lib_path \
		-o "$exe" "$obj"/*.o \
		$ld_theora \
		$ld_sfml \
		-lvorbisenc \
		-logg \

	chmod 777 "$exe"
	./"$exe"

# to test the cpp errors
else

	clear
	for i in "$@"; do
		g++ $gcc_flag $inc_path -c "$i"
	done

	mv -vf *.o "$obj"

fi
