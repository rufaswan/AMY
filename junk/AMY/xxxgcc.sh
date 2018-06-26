#!/bin/bash

gcc=$(which g++)
[ "$gcc" ] || { echo "GCC/G++ not installed!"; exit; }

cpp=''
ldd=''
type=''
file=''
flag="-O2"
incp="-I/include -I/usr/include -I./extern"
libp="-L/lib     -L/usr/lib     -L$HOME/my-applications/lib"

while [ "$1" ]; do
	case "$1" in
		'-I')     incp=$(echo "$incp -I$2"); shift;;
		'-L')     libp=$(echo "$libp -L$2"); shift;;
		'-c')     type='c';;
		'-out')   type='o'; file="-o $2"; shift;;
		'-so')    type='s'; file="--shared -o $2"; shift;;
		'-sfml')  ldd=$(echo "$ldd -lsfml-network -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system");;
		'-nc')    ldd=$(echo "$ldd -lncurses");;
		'-zlib')  ldd=$(echo "$ldd -lz");;
		'-amy')   ldd=$(echo "$ldd -lamy");;
		'-o2')    flag="-O2";;
		'-g')     flag="-g -Wall";;
		*)        [ -f "$1" ] && cpp=$(echo "$cpp $1");;
	esac

	shift
done

[ "$cpp" ] || { echo "ERROR: no file!"; exit; }
case $type in
	'c')  $gcc $flag $incp $libp -c $cpp;;
	'o')  $gcc $flag $incp $libp $cpp $file $ldd;;
	's')  $gcc $flag $incp $libp $cpp $file $ldd;;
	*)    echo "INVALID [$type] : [$0] [$flag] [$cpp] [$file] [$ldd]";;
esac
