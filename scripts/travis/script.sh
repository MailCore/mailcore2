#!/bin/sh
set -e

IOSSDK="`xcodebuild -showsdks 2>/dev/null | grep iphoneos | sed 's/.*iphoneos\(.*\)/\1/'`"
OSXSDK="`xcodebuild -showsdks 2>/dev/null | grep macosx | sed 's/.*macosx\(.*\)/\1/'`"
IPHONESDK=iphoneos$IOSSDK
SIMULATORSDK=iphonesimulator$IOSSDK
MACSDK=macosx$OSXSDK

export TZ=PST8PDT

echo Operating system:
uname

# mkdir -p Externals/prebuilt
# cd Externals/prebuilt
# git clone --depth=1 https://github.com/MailCore/mailcore2-deps
# cd ../..
# cp Externals/prebuilt/mailcore2-deps/prebuilt.list scripts/prebuilt.list
# mkdir -p Externals/builds/builds
# rsync --exclude=.git -av Externals/prebuilt/mailcore2-deps/ Externals/builds/builds/

if test x"`uname`" = xDarwin ; then
  echo Building library for iPhoneOS
  xctool -project build-mac/mailcore2.xcodeproj -sdk $IPHONESDK -scheme "static mailcore2 ios" build ARCHS="armv7 armv7s arm64"
  echo Building library for iPhoneSimulator
  xctool -project build-mac/mailcore2.xcodeproj -sdk $SIMULATORSDK -scheme "static mailcore2 ios" build ARCHS="i386 x86_64"
  #echo Link test for iPhoneOS
  #xcodebuild -project build-mac/mailcore2.xcodeproj -sdk iphoneos7.1 -target "test-ios" CODE_SIGN_IDENTITY="" build
  echo Link test for iPhoneSimulator
  xctool -project build-mac/mailcore2.xcodeproj -sdk $SIMULATORSDK -scheme "test-ios" build ARCHS="i386 x86_64"

  echo Building library for Mac
  xctool -project build-mac/mailcore2.xcodeproj -sdk $MACSDK -scheme "static mailcore2 osx" build
  echo Building framework for Mac
  xctool -project build-mac/mailcore2.xcodeproj -sdk $MACSDK -scheme "mailcore osx" build
  echo Link test for Mac
  xctool -project build-mac/mailcore2.xcodeproj -sdk $MACSDK -scheme "tests" build
  echo Unit Tests for Mac
  xctool -project build-mac/mailcore2.xcodeproj -sdk $MACSDK -scheme "unittest" test
fi

echo Testing CMake build
mkdir -p build-cmake
cd build-cmake
cmake ..
make
cd unittest
./unittestcpp ../../unittest/data
