#!/bin/bash

exe='compiled.exe'
obj='built'
gcc_flag='-g -Wall'
inc_path='-I./hpp'
lib_path='-L/lib -L/usr/lib'

ld_theora='-ltheoradec -ltheoraenc -ltheora'
ld_sfml='-lsfml-network -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system'

# clean up previous built
	reset
	find . -iname "*.o" -exec rm -vf {} \;
	rm -vf "$exe"

# process -log option
	log=''
	[[ "$1" == "-log" ]] && { log=1;shift; }
	[[ "$1" == "-op"  ]] && { gcc_flag='-O3';shift; }

# make the $obj dir and move all the *.o there
	[ ! -d "$obj" ] && mkdir "$obj"

	t1=$(ls -1 *.cpp | wc -l)
	t2=0
	for i in $(ls -1 *.cpp); do
		let t2++
		echo "compiling $t2/$t1 $i ..."
		g++ $gcc_flag $inc_path -c "$i"
	done

	mv -vf *.o "$obj"

# compiling...
	g++ $gcc_flag $lib_path \
		-o "$exe" "$obj"/*.o \
		$ld_theora \
		$ld_sfml \
		-lvorbisenc \
		-logg \


# test run the exe
	chmod 777 "$exe"

	if [ $log ]; then
		./"$exe" &> "$exe".log
	else
		./"$exe"
	fi

# notes for debugging
# $ gdb ./compiled	# init
# $ run				# run the exe
# $ bt				# backtrace
