#!/bin/sh

# push to epitech repository
echo
git push origin master
if [ $? -eq 0 ]
then
  echo
  echo '√ Pushed to epitech repository'
else
  echo
  echo '✗ Failed to push to epitech repository'
fi

# push to mirror repository (https://github.com/Liroo/Arcade)
# echo
# git push --mirror git@github.com:Liroo/Arcade.git
# if [ $? -eq 0 ]
# then
#  echo
#   echo '√ Pushed to mirror repository'
# else
#   echo
#   echo '✗ Failed to push to mirror repository'
# fi
