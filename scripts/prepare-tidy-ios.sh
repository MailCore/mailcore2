#!/bin/sh

if xcodebuild -showsdks|grep iphoneos8.1 >/dev/null ; then
	sdkversion=8.1
    devicearchs="armv7 armv7s arm64"
elif xcodebuild -showsdks|grep iphoneos8.0 >/dev/null ; then
	sdkversion=8.0
    devicearchs="armv7 armv7s arm64"
elif xcodebuild -showsdks|grep iphoneos7.1 >/dev/null ; then
    	sdkversion=7.1
        devicearchs="armv7 armv7s arm64"	 
elif xcodebuild -showsdks|grep iphoneos7.0 >/dev/null ; then
	sdkversion=7.0
    devicearchs="armv7 armv7s arm64"	 
elif xcodebuild -showsdks|grep iphoneos6.1 >/dev/null ; then
	sdkversion=6.1
    devicearchs="armv7 armv7s"
else
	echo SDK not found
	exit 1
fi	
url="https://github.com/dinhviethoa/tidy-html5.git"

pushd `dirname $0` > /dev/null
scriptpath=`pwd`
popd > /dev/null
builddir="$scriptpath/../Externals/builds"

#builddir="$HOME/MailCore-Builds/dependencies"
BUILD_TIMESTAMP=`date +'%Y%m%d%H%M%S'`
tempbuilddir="$builddir/workdir/$BUILD_TIMESTAMP"
mkdir -p "$tempbuilddir"
srcdir="$tempbuilddir/src"
logdir="$tempbuilddir/log"
resultdir="$builddir/builds"
tmpdir="$tempbuilddir/tmp"

mkdir -p "$resultdir"
mkdir -p "$logdir"
mkdir -p "$tmpdir"
mkdir -p "$srcdir"

pushd . >/dev/null
mkdir -p "$builddir/downloads"
cd "$builddir/downloads"
if test -d tidy-html5 ; then
	cd tidy-html5
	git pull --rebase
else
	git clone $url
	cd tidy-html5
fi
version=`git rev-parse HEAD | cut -c1-10`
build_version="$version~1"

if test -f "$resultdir/tidy-html5-ios-$build_version.zip" ; then
	echo install from cache
	popd >/dev/null
	rm -rf ../Externals/tidy-html5-ios
	mkdir -p ../Externals/tmp
	unzip -q "$resultdir/tidy-html5-ios-$build_version.zip" -d ../Externals/tmp
	mv "../Externals/tmp/tidy-html5-ios-$build_version/tidy-html5-ios" ../Externals
  mkdir -p ../Externals/installed
  ln -sf "$resultdir/tidy-html5-ios-$build_version.zip" ../Externals/installed
	rm -rf ../Externals/tmp
	exit 0
fi
popd >/dev/null

pushd . >/dev/null

cp -R "$builddir/downloads/tidy-html5" "$srcdir/tidy-html5"
echo building tidy-html5

cd "$srcdir/tidy-html5/build-mac"
sdk="iphoneos$sdkversion"
echo building $sdk
xcodebuild -project Tidy.xcodeproj -sdk $sdk -target "tidy" -configuration Release SYMROOT="$tmpdir/bin" OBJROOT="$tmpdir/obj" ARCHS="$devicearchs" IPHONEOS_DEPLOYMENT_TARGET="$sdkversion" >/dev/null 2>&1
if test x$? != x0 ; then
  echo failed
  exit 1
fi
sdk="iphonesimulator$sdkversion"
simarchs="i386 x86_64"
echo building $sdk
xcodebuild -project Tidy.xcodeproj -sdk $sdk -target "tidy" -configuration Release SYMROOT="$tmpdir/bin" OBJROOT="$tmpdir/obj" ARCHS="$simarchs" IPHONEOS_DEPLOYMENT_TARGET="$sdkversion" >/dev/null 2>&1
if test x$? != x0 ; then
  echo failed
  exit 1
fi
echo finished

cd "$tmpdir/bin"
mkdir -p "tidy-html5-ios-$build_version/tidy-html5-ios"
mkdir -p "tidy-html5-ios-$build_version/tidy-html5-ios/lib"
lipo -create Release-iphoneos/libtidy.a \
  Release-iphonesimulator/libtidy.a \
  -output "tidy-html5-ios-$build_version/tidy-html5-ios/lib/libtidy.a"
mv Release-iphoneos/include "tidy-html5-ios-$build_version/tidy-html5-ios"
zip -qry "$resultdir/tidy-html5-ios-$build_version.zip" "tidy-html5-ios-$build_version"
rm -f "$resultdir/tidy-html5-ios=latest.zip"
cd "$resultdir"
ln -s "tidy-html5-ios-$build_version.zip" "tidy-html5-ios-latest.zip"

echo build of tidy-html5-ios-$build_version done

popd >/dev/null

rm -rf ../Externals/tidy-html5-ios
mkdir -p ../Externals/tmp
unzip -q "$resultdir/tidy-html5-ios-$build_version.zip" -d ../Externals/tmp
mv "../Externals/tmp/tidy-html5-ios-$build_version/tidy-html5-ios" ../Externals
rm -rf ../Externals/tmp

echo cleaning
rm -rf "$tempbuilddir"
echo "$tempbuilddir"
