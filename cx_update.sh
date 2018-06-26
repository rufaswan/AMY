#!/bin/bash
for f in $(find . -type f); do
	[ -f "$HOME/bash/cpp_sf/$f" ] || continue
	cmp=$(cmp $HOME/bash/cpp_sf/$f  $f)
	if [ "$cmp" ]; then
		echo "UPDATE $f"
		cp -vu $HOME/bash/cpp_sf/$f  $f
	fi
done
