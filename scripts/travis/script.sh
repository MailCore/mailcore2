#!/bin/sh
set -e

echo '** printenv **'
printenv
echo '** ls **'
ls
echo '** pwd **'
pwd
xctool -project build-mac/mailcore2.xcodeproj -scheme "static mailcore2 ios" build
xctool -project build-mac/mailcore2.xcodeproj -scheme "static mailcore2 osx" build
