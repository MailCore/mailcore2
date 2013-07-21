#!/bin/sh
set -e

xctool -project build-mac/mailcore2.xcodeproj -scheme "static mailcore2 ios" build
xctool -project build-mac/mailcore2.xcodeproj -scheme "static mailcore2 osx" build
