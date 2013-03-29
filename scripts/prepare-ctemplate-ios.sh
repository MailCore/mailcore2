#!/bin/sh

url="https://github.com/dinhviethoa/ctemplate"

sdkversion="6.1"

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
if test -d ctemplate ; then
	cd ctemplate
	git pull --rebase
else
	git clone $url
	cd ctemplate
fi
version=`git rev-parse HEAD | cut -c1-10`

if test -f "$resultdir/ctemplate-ios-$version.zip" ; then
	echo install from cache
	popd >/dev/null
	rm -rf ../Externals/ctemplate-ios
	mkdir -p ../Externals/tmp
	unzip -q "$resultdir/ctemplate-ios-$version.zip" -d ../Externals/tmp
	mv "../Externals/tmp/ctemplate-ios-$version/ctemplate-ios" ../Externals
	rm -rf ../Externals/tmp
	exit 0
fi
popd >/dev/null

pushd . >/dev/null

cp -R "$builddir/downloads/ctemplate" "$srcdir/ctemplate"
echo building ctemplate
cd "$srcdir/ctemplate"

export CC=clang
export CXX=clang++
export LDLAGS="-lc++ $arch_flags -isysroot $sysrootpath"

sdk="iphoneos$sdkversion"
sysroot="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$sdkversion.sdk"

ARCH=arm
MARCHS="armv7"
for MARCH in $MARCHS; do
  export CFLAGS="-arch ${MARCH} -isysroot $sysroot"
  export CXXFLAGS=$CFLAGS
  ./configure --host=${ARCH} --disable-shared --disable-dependency-tracking >> "$logdir/ctemplate-build.log"
  make >> "$logdir/ctemplate-build.log"
  make install-libLTLIBRARIES "prefix=$tmpdir/bin/ctemplate-ios/$sdk" >> "$logdir/ctemplate-build.log"
  if test x$? != x0 ; then
    echo build of ctemplate failed
    exit 1
  fi
  make install-nodist_ctemplateincludeHEADERS "prefix=$tmpdir/bin/ctemplate-ios" >> "$logdir/ctemplate-build.log"
  if test x$? != x0 ; then
  	echo install of ctemplate failed
  	exit 1
  fi
  make distclean
  find . -name *.o | xargs rm
  find . -name config.cache -print0 | xargs -0 rm
done

sdk="iphonesimulator$sdkversion"
sysroot="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator$sdkversion.sdk"

ARCH=i386
MARCHS=i386

for MARCH in $MARCHS; do
  export CFLAGS="-arch ${MARCH} -isysroot $sysroot"
  export CXXFLAGS=$CFLAGS
  ./configure --host=${ARCH} --disable-shared --disable-dependency-tracking >> "$logdir/ctemplate-build.log"
  make >> "$logdir/ctemplate-build.log"
  make install-libLTLIBRARIES "prefix=$tmpdir/bin/ctemplate-ios/$sdk" >> "$logdir/ctemplate-build.log"
  if test x$? != x0 ; then
    echo build of ctemplate failed
    exit 1
  fi
  make install-nodist_ctemplateincludeHEADERS "prefix=$tmpdir/bin/ctemplate-ios" >> "$logdir/ctemplate-build.log"
  if test x$? != x0 ; then
  	echo install of ctemplate failed
  	exit 1
  fi
  make distclean
  find . -name *.o | xargs rm
  find . -name config.cache -print0 | xargs -0 rm
done

echo finished

cd "$tmpdir/bin"
mkdir -p "ctemplate-ios-$version/ctemplate-ios/lib"
mv ctemplate-ios/include "ctemplate-ios-$version/ctemplate-ios"
lipo -create ctemplate-ios/iphonesimulator$sdkversion/lib/libctemplate.a \
  ctemplate-ios/iphoneos$sdkversion/lib/libctemplate.a \
  -output "ctemplate-ios-$version/ctemplate-ios/lib/libctemplate-ios.a"
rm -f "$resultdir/ctemplate-ios-$version.zip"
zip -qry "$resultdir/ctemplate-ios-$version.zip" "ctemplate-ios-$version"
rm -f "$resultdir/ctemplate-ios-latest.zip"
cd "$resultdir"
ln -s "ctemplate-ios-$version.zip" "ctemplate-ios-latest.zip"

echo build of ctemplate-ios-$version done
echo $tmpdir

popd >/dev/null

rm -rf ../Externals/ctemplate-ios
mkdir -p ../Externals/tmp
unzip -q "$resultdir/ctemplate-ios-$version.zip" -d ../Externals/tmp
mv "../Externals/tmp/ctemplate-ios-$version/ctemplate-ios" ../Externals
rm -rf ../Externals/tmp

echo cleaning
rm -rf "$tempbuilddir"
echo "$tempbuilddir"
