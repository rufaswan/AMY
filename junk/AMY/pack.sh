#!/bin/bash

pwd=$(pwd)
dir=$(basename "$pwd")

# exclude these file types
iname='! -iname "*.o" ! -iname "*.so" ! -iname "*.out" ! -iname "*.dll" ! -iname "*.exe"'

date=$(find . -type f $iname -printf "%TY%Tm%Td-%TH%TM\n" | sort | tail -1)
file="$dir-$date.zip"

cd ..
[ -f "$file" ] || zip -r "$file" "$dir" -x *.o -x *.so -x *.out -x *.dll -x *.exe

#date=$(date +"%Y%m%d-%H%M")

#rm -vf last-*.txt
#echo > last-"$date".txt
