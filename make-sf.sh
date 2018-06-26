#!/bin/bash

[ $# = 0 ] && exit

GPP=$(which g++)
[ "$GPP" ] || { echo "ERROR : G++ not found!"; exit; }

if [[ $(uname -m) == "x86_64" ]]; then
	FLAG="-O2 -D_OS_LINUX_64"
else
	FLAG="-O2 -D_OS_LINUX_32"
fi
INC="-I. -I./deps/inc -I/inc -I/usr/inc -I$HOME/opt/inc"
LIB="-L. -I./deps/lib -L/lib -L/usr/lib -L$HOME/opt/lib"

while [ "$1" ]; do
	CMD=""

	APP="$1"
	APP=${APP##*/}
	APP=${APP%.*}

	if [ -f "$APP".cpp ]; then
		CMD="$GPP $FLAG $INC $LIB -o /tmp/$APP.out $APP.cpp -lsfml-graphics -lsfml-window -lsfml-system"
		echo "=== $APP ==="
		echo $CMD
		$CMD
		shift
	else
		shift
		continue
	fi

done
