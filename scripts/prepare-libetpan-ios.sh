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

url="https://github.com/dinhviethoa/libetpan.git"
rev=88a41b8a3bb02e0ad4b44975aa03e9e26f738e7d

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
if test -d libetpan ; then
	cd libetpan
	git pull --rebase
else
	git clone $url
	cd libetpan
fi
#version=`git rev-parse HEAD | cut -c1-10`
version=`echo $rev | cut -c1-10`

if test -f "$resultdir/libetpan-ios-$version.zip" ; then
	echo install from cache
	popd >/dev/null
	rm -rf ../Externals/libetpan-ios
	rm -rf ../Externals/libsasl-ios
	mkdir -p ../Externals/tmp
	unzip -q "$resultdir/libetpan-ios-$version.zip" -d ../Externals/tmp
	unzip -q "$resultdir/libsasl-ios-$version.zip" -d ../Externals/tmp
	mv "../Externals/tmp/libetpan-ios-$version/libetpan-ios" ../Externals
	mv "../Externals/tmp/libsasl-ios-$version/libsasl-ios" ../Externals
  mkdir -p ../Externals/installed
  ln -sf "$resultdir/libetpan-ios-$version.zip" ../Externals/installed
  ln -sf "$resultdir/libsasl-ios-$version.zip" ../Externals/installed
	rm -rf ../Externals/tmp
	exit 0
fi
popd >/dev/null

pushd . >/dev/null

cp -R "$builddir/downloads/libetpan" "$srcdir/libetpan"
cd "$srcdir/libetpan"
git checkout $rev
echo building libetpan

cd "$srcdir/libetpan/build-mac"
sdk="iphoneos$sdkversion"
echo building $sdk
xcodebuild -project libetpan.xcodeproj -sdk $sdk -target "libetpan ios" -configuration Release SYMROOT="$tmpdir/bin" OBJROOT="$tmpdir/obj" ARCHS="$devicearchs" IPHONEOS_DEPLOYMENT_TARGET="$sdkversion"
if test x$? != x0 ; then
  echo failed
  exit 1
fi
sdk="iphonesimulator$sdkversion"
simarchs="i386 x86_64"
echo building $sdk
xcodebuild -project libetpan.xcodeproj -sdk $sdk -target "libetpan ios" -configuration Release SYMROOT="$tmpdir/bin" OBJROOT="$tmpdir/obj" ARCHS="$simarchs" IPHONEOS_DEPLOYMENT_TARGET="$sdkversion"
if test x$? != x0 ; then
  echo failed
  exit 1
fi
echo finished

cd "$tmpdir/bin"
mkdir -p "libetpan-ios-$version/libetpan-ios"
mkdir -p "libetpan-ios-$version/libetpan-ios/lib"
mv Release-iphoneos/include "libetpan-ios-$version/libetpan-ios"
lipo -create Release-iphoneos/libetpan-ios.a \
  Release-iphonesimulator/libetpan-ios.a \
  -output "libetpan-ios-$version/libetpan-ios/lib/libetpan-ios.a"
zip -qry "$resultdir/libetpan-ios-$version.zip" "libetpan-ios-$version"
mkdir -p "libsasl-ios-$version"
mv "$srcdir/libetpan/build-mac/libsasl-ios" "libsasl-ios-$version"
zip -qry "$resultdir/libsasl-ios-$version.zip" "libsasl-ios-$version"
rm -f "$resultdir/libetpan-ios-latest.zip"
rm -f "$resultdir/libsasl-ios-latest.zip"
cd "$resultdir"
ln -s "libetpan-ios-$version.zip" "libetpan-ios-latest.zip"
ln -s "libsasl-ios-$version.zip" "libsasl-ios-latest.zip"

echo build of libetpan-ios-$version done

popd >/dev/null

rm -rf ../Externals/libetpan-ios
rm -rf ../Externals/libsasl-ios
mkdir -p ../Externals/tmp
unzip -q "$resultdir/libetpan-ios-$version.zip" -d ../Externals/tmp
unzip -q "$resultdir/libsasl-ios-$version.zip" -d ../Externals/tmp
mv "../Externals/tmp/libetpan-ios-$version/libetpan-ios" ../Externals
mv "../Externals/tmp/libsasl-ios-$version/libsasl-ios" ../Externals
rm -rf ../Externals/tmp

echo cleaning
rm -rf "$tempbuilddir"
echo "$tempbuilddir"
