#!/bin/bash

pid=$(pidof geany)
[[ "$pid" == '' ]] && { xterm --hold -e echo "Please start a copy of Geany first"; exit; }
find . -type f -iname "*.cpp" -exec geany {} \;
find . -type f -iname "*.hpp" -exec geany {} \;
find . -type f -iname "*.h" -exec geany {} \;
