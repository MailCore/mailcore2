#!/bin/sh

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

if test -f "$resultdir/libetpan-$version.zip" ; then
	echo install from cache
	popd >/dev/null
	rm -rf ../Externals/libetpan
	mkdir -p ../Externals/tmp
	unzip -q "$resultdir/libetpan-$version.zip" -d ../Externals/tmp
	mv "../Externals/tmp/libetpan-$version/libetpan" ../Externals
  mkdir -p ../Externals/installed
  ln -sf "$resultdir/libetpan-$version.zip" ../Externals/installed
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
xcodebuild -project libetpan.xcodeproj -target "static libetpan" -configuration Release SYMROOT="$tmpdir/bin" OBJROOT="$tmpdir/obj"
if test x$? != x0 ; then
  echo failed
  exit 1
fi
echo finished

cd "$tmpdir/bin"
mkdir -p "libetpan-$version/libetpan"
mkdir -p "libetpan-$version/libetpan/lib"
mv Release/include "libetpan-$version/libetpan"
mv Release/libetpan.a "libetpan-$version/libetpan/lib"
zip -qry "$resultdir/libetpan-$version.zip" "libetpan-$version"
rm -f "$resultdir/libetpan-latest.zip"
cd "$resultdir"
ln -s "libetpan-$version.zip" "libetpan-latest.zip"

echo build of libetpan-$version done

popd >/dev/null

rm -rf ../Externals/libetpan
mkdir -p ../Externals/tmp
unzip -q "$resultdir/libetpan-$version.zip" -d ../Externals/tmp
mv "../Externals/tmp/libetpan-$version/libetpan" ../Externals
rm -rf ../Externals/tmp

echo cleaning
rm -rf "$tempbuilddir"
echo "$tempbuilddir"
