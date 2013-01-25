#!/bin/sh

arch="x86_64"
sysrootpath="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk"
url="https://github.com/dinhviethoa/libetpan.git"

arch_flags=""
for current_arch in $arch ; do
	arch_flags="$arch_flags -arch $current_arch"
done

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
if test -d libetpan ; then
	cd libetpan
	git pull --rebase
else
	git clone $url
	cd libetpan
fi
version=`git rev-parse HEAD | cut -c1-10`

if test -f "$resultdir/libetpan-$version.zip" ; then
	echo install from cache
	popd >/dev/null
	rm -rf ../Externals/libetpan
	mkdir -p ../Externals/tmp
	unzip -q "$resultdir/libetpan-$version.zip" -d ../Externals/tmp
	mv "../Externals/tmp/libetpan-$version/libetpan" ../Externals
	rm -rf ../Externals/tmp
	exit 0
fi
popd >/dev/null

pushd . >/dev/null

cp -R "$builddir/downloads/libetpan" "$srcdir/libetpan"
echo building libetpan
cd "$srcdir/libetpan"
./autogen.sh
cd "$srcdir/libetpan/build-mac"
./update.sh

xcodebuild -project libetpan.xcodeproj -target "static libetpan" -configuration Release SYMROOT="$tmpdir/bin" OBJROOT="$tmpdir/obj"
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
