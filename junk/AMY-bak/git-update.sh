#!/bin/bash

git='git@github.com:rufaswan/AMY.git'

msg='
please type a comment within quotes
type pull to do a git pull
'
[ $# = 0 ] && { echo "$msg"; exit; }

# basic stuff
git config --global user.name "Rufas Wan"
git config --global user.email rufaswan@gmail.com
git config --global pack.deltaCacheSize 0

git remote rm origin
git remote add origin "$git"

if [[ "$1" == "pull" ]]; then
	git pull origin master
else
	git add .
	git ls-files --deleted -z | xargs -0 git rm
	git reflog expire --expire=now --all
	git gc --prune=now
	git commit -m "$1"
	git push origin master
fi
