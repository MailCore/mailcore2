#!/bin/sh
set -e

echo Building library for iPhoneOS
pwd
xctool -version
#xcodebuild -project build-mac/mailcore2.xcodeproj -sdk iphoneos6.1 -target "static mailcore2 ios" build > /dev/null
xctool -project build-mac/mailcore2.xcodeproj -sdk iphoneos6.1 -scheme "static mailcore2 ios" build
echo Building library for iPhoneSimulator
xctool -project build-mac/mailcore2.xcodeproj -sdk iphonesimulator6.1 -target "static mailcore2 ios" build
#echo Link test for iPhoneOS
#xcodebuild -project build-mac/mailcore2.xcodeproj -sdk iphoneos6.1 -target "test-ios" CODE_SIGN_IDENTITY="" build
echo Link test for iPhoneSimulator
xctool -project build-mac/mailcore2.xcodeproj -sdk iphonesimulator6.1 -target "test-ios" build

echo Building library for Mac
xctool -project build-mac/mailcore2.xcodeproj -sdk macosx10.8 -target "static mailcore2 osx" build
echo Building framework for Mac
xctool -project build-mac/mailcore2.xcodeproj -sdk macosx10.8 -target "mailcore osx" build
echo Link test for Mac
xctool -project build-mac/mailcore2.xcodeproj -sdk macosx10.8 -target "tests" build

echo Testing CMake build
mkdir -p build-cmake
cd build-cmake
cmake ..
make
