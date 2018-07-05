#!/bin/bash

git="git@github.com:rufaswan/CX.git"

msg="
please type a comment to push
  type pull  to do a git pull
  type force to overwrite the repo
"
[ $# = 0 ] && { echo "$msg"; exit; }

git remote rm  origin
git remote add origin "$git"

case "$1" in
	"-pull")
		echo "git pull $git : master"
		git pull origin master
		;;
	"-force")
		[[ "$2" == "i_really_want_to_do_this" ]] || exit
		echo "git push --force $git : master"
		git push --force origin master
		;;
	*)
		echo "git push $git : $@"
		git add .
		git ls-files --deleted -z | xargs -0 git rm
		git reflog expire --expire=now --all
		git gc --prune=now
		git commit -m "$@"
		git push origin master
		;;
esac
