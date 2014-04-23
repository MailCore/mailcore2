#!/bin/sh

if xcodebuild -showsdks|grep iphoneos6.1 >/dev/null ; then
	sdkversion=6.1
         devicearchs="armv7 armv7s"
elif xcodebuild -showsdks|grep iphoneos7.0 >/dev/null ; then
	sdkversion=7.0
         devicearchs="armv7 armv7s arm64"
elif xcodebuild -showsdks|grep iphoneos7.1 >/dev/null ; then
  sdkversion=7.1
  devicearchs="armv7 armv7s arm64"
else
	echo SDK not found
	exit 1
fi	
url="https://github.com/MailCore/mailcore2.git"

pushd `dirname $0` > /dev/null
scriptpath=`pwd`
popd > /dev/null
builddir="$scriptpath/../Externals/builds"

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
if test -d mailcore2 ; then
	cd mailcore2
	git pull --rebase
else
	git clone $url
	cd mailcore2
fi
version=`git rev-parse HEAD | cut -c1-10`

cp -R "$builddir/downloads/mailcore2" "$srcdir/mailcore2"
echo copying deps

mkdir -p "$srcdir/mailcore2/Externals"
cd "$scriptpath/../Externals"
for filename in * ; do
  if test x$filename != xbuilds ; then
    cp -R "$scriptpath/../Externals/$filename" "$srcdir/mailcore2/Externals"
  fi
done

echo building mailcore2
cd "$srcdir/mailcore2/build-mac"
sdk="iphoneos$sdkversion"
echo building $sdk
xcodebuild -project mailcore2.xcodeproj -sdk $sdk -target "static mailcore2 ios" -configuration Release SYMROOT="$tmpdir/bin" OBJROOT="$tmpdir/obj" ARCHS="$devicearchs"
if test x$? != x0 ; then
  echo failed
  exit 1
fi
sdk="iphonesimulator$sdkversion"
simarchs="i386"
echo building $sdk
xcodebuild -project mailcore2.xcodeproj -sdk $sdk -target "static mailcore2 ios" -configuration Release SYMROOT="$tmpdir/bin" OBJROOT="$tmpdir/obj" ARCHS="$simarchs"
if test x$? != x0 ; then
  echo failed
  exit 1
fi
echo finished

cd "$tmpdir/bin"
mkdir -p "mailcore2-ios-$version/mailcore2-ios"
mkdir -p "mailcore2-ios-$version/mailcore2-ios/lib"
mv Release-iphoneos/include "mailcore2-ios-$version/mailcore2-ios"
lipo -create Release-iphoneos/libMailCore-ios.a \
  Release-iphonesimulator/libMailCore-ios.a \
  -output "mailcore2-ios-$version/mailcore2-ios/lib/libMailCore-ios.a"
zip -qry "$resultdir/mailcore2-ios-$version.zip" "mailcore2-ios-$version"
rm -f "$resultdir/mailcore2-ios-latest.zip"
cd "$resultdir"
ln -s "mailcore2-ios-$version.zip" "mailcore2-ios-latest.zip"

echo build of mailcore2-ios-$version done

popd >/dev/null

rm -rf ../Externals/tmp

echo cleaning
rm -rf "$tempbuilddir"
echo "$tempbuilddir"
