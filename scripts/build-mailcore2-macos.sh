#!/bin/sh

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
xcodebuild -project mailcore2.xcodeproj -target "mailcore osx" -configuration Release SYMROOT="$tmpdir/bin" OBJROOT="$tmpdir/obj"
if test x$? != x0 ; then
  echo failed
  exit 1
fi
echo finished

cd "$tmpdir/bin/Release"
zip -qry "$resultdir/mailcore2-$version.zip" MailCore.framework MailCore.framework.dSYM
cd "$resultdir"
ln -s "mailcore2-$version.zip" "mailcore2-latest.zip"

echo build of mailcore2-$version done

popd >/dev/null

rm -rf ../Externals/tmp

echo cleaning
rm -rf "$tempbuilddir"
echo "$tempbuilddir"
