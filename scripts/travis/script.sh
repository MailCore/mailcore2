#!/bin/sh
set -e

echo Building library for iPhoneOS
xctool -project build-mac/mailcore2.xcodeproj -sdk iphoneos7.1 -scheme "static mailcore2 ios" build ARCHS="armv7 armv7s arm64"
echo Building library for iPhoneSimulator
xctool -project build-mac/mailcore2.xcodeproj -sdk iphonesimulator7.1 -scheme "static mailcore2 ios" build ARCHS="i386 x86_64"
#echo Link test for iPhoneOS
#xcodebuild -project build-mac/mailcore2.xcodeproj -sdk iphoneos7.1 -target "test-ios" CODE_SIGN_IDENTITY="" build
echo Link test for iPhoneSimulator
xctool -project build-mac/mailcore2.xcodeproj -sdk iphonesimulator7.1 -scheme "test-ios" build ARCHS="i386 x86_64"

echo Building library for Mac
xctool -project build-mac/mailcore2.xcodeproj -sdk macosx10.8 -scheme "static mailcore2 osx" build
echo Building framework for Mac
xctool -project build-mac/mailcore2.xcodeproj -sdk macosx10.8 -scheme "mailcore osx" build
echo Link test for Mac
xctool -project build-mac/mailcore2.xcodeproj -sdk macosx10.8 -scheme "tests" build

echo Testing CMake build
mkdir -p build-cmake
cd build-cmake
cmake ..
make
