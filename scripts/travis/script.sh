#!/bin/sh
set -e

echo Building library for iPhoneOS
xcodebuild -project build-mac/mailcore2.xcodeproj -sdk iphoneos6.1 -target "static mailcore2 ios" build > /dev/null
echo Building library for iPhoneSimulator
xcodebuild -project build-mac/mailcore2.xcodeproj -sdk iphonesimulator6.1 -target "static mailcore2 ios" build > /dev/null
#echo Link test for iPhoneOS
#xcodebuild -project build-mac/mailcore2.xcodeproj -sdk iphoneos6.1 -target "test-ios" CODE_SIGN_IDENTITY="" build
echo Link test for iPhoneSimulator
xcodebuild -project build-mac/mailcore2.xcodeproj -sdk iphonesimulator6.1 -target "test-ios" build > /dev/null

echo Building library for Mac
xcodebuild -project build-mac/mailcore2.xcodeproj -sdk macosx10.8 -target "static mailcore2 osx" build > /dev/null
echo Building framework for Mac
xcodebuild -project build-mac/mailcore2.xcodeproj -sdk macosx10.8 -target "mailcore osx" build > /dev/null
echo Link test for Mac
xcodebuild -project build-mac/mailcore2.xcodeproj -sdk macosx10.8 -target "tests" build > /dev/null

echo Testing CMake build
mkdir -p build-cmake
cd build-cmake
cmake ..
make
