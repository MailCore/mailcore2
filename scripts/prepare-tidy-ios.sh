#!/bin/sh

sdkversion=6.1
url="https://github.com/dinhviethoa/tidy-html5.git"

builddir="$HOME/MailCore-Builds/dependencies"
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
cd "$builddir/downloads"
if test -d tidy-html5 ; then
	cd tidy-html5
	git pull --rebase
else
	git clone $url
	cd tidy-html5
fi
version=`git rev-parse HEAD | cut -c1-10`

if test -f "$resultdir/tidy-html5-$version.zip" ; then
	echo install from cache
	popd >/dev/null
	rm -rf ../Externals/tidy-html5
	mkdir -p ../Externals/tmp
	unzip -q "$resultdir/tidy-html5-$version.zip" -d ../Externals/tmp
	mv "../Externals/tmp/tidy-html5-$version/tidy-html5" ../Externals
	rm -rf ../Externals/tmp
	exit 0
fi
popd >/dev/null

pushd . >/dev/null

cp -R "$builddir/downloads/tidy-html5" "$srcdir/tidy-html5"
echo building tidy-html5

cd "$srcdir/tidy-html5/build-mac"
sdk="iphoneos$sdkversion"
archs="armv7"
echo building $sdk
xcodebuild -project Tidy.xcodeproj -sdk $sdk -target "tidy" -configuration Release SYMROOT="$tmpdir/bin" OBJROOT="$tmpdir/obj" ARCHS="$archs"
if test x$? != x0 ; then
  echo failed
  exit 1
fi
sdk="iphonesimulator$sdkversion"
archs="i386"
echo building $sdk
xcodebuild -project Tidy.xcodeproj -sdk $sdk -target "tidy" -configuration Release SYMROOT="$tmpdir/bin" OBJROOT="$tmpdir/obj" ARCHS="$archs"
if test x$? != x0 ; then
  echo failed
  exit 1
fi
echo finished

cd "$tmpdir/bin"
mkdir -p "tidy-html5-$version/tidy-html5"
mkdir -p "tidy-html5-$version/tidy-html5/lib"
lipo -create Release-iphoneos/libtidy.a \
  Release-iphonesimulator/libtidy.a \
  -output "tidy-html5-$version/tidy-html5/lib/libtidy.a"
mv Release-iphoneos/include "tidy-html5-$version/tidy-html5"
zip -qry "$resultdir/tidy-html5-ios-$version.zip" "tidy-html5-$version"
rm -f "$resultdir/tidy-html5-ios=latest.zip"
cd "$resultdir"
ln -s "tidy-html5-ios-$version.zip" "tidy-html5-ios-latest.zip"

echo build of tidy-html5-$version done

popd >/dev/null

rm -rf ../Externals/tidy-html5
mkdir -p ../Externals/tmp
unzip -q "$resultdir/tidy-html5-ios-$version.zip" -d ../Externals/tmp
mv "../Externals/tmp/tidy-html5-ios-$version/tidy-html5" ../Externals
rm -rf ../Externals/tmp

echo cleaning
rm -rf "$tempbuilddir"
echo "$tempbuilddir"
