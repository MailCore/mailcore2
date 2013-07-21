#!/bin/sh
set -e

xctool -project build-mac/mailcore2.xcodeproj -scheme "static mailcore ios" build
xctool -project build-mac/mailcore2.xcodeproj -scheme "static mailcore osx" build
