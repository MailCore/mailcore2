#!/bin/sh

url="https://github.com/dinhviethoa/ctemplate"

if xcodebuild -showsdks|grep iphoneos8.1 >/dev/null ; then
	sdkversion=8.1
    MARCHS="armv7 armv7s arm64"
elif xcodebuild -showsdks|grep iphoneos8.0 >/dev/null ; then
	sdkversion=8.0
    MARCHS="armv7 armv7s arm64"
elif xcodebuild -showsdks|grep iphoneos7.1 >/dev/null ; then
	sdkversion=7.1
    MARCHS="armv7 armv7s arm64"
elif xcodebuild -showsdks|grep iphoneos7.0 >/dev/null ; then
 	sdkversion=7.0
    MARCHS="armv7 armv7s arm64"
elif xcodebuild -showsdks|grep iphoneos6.1 >/dev/null ; then
	sdkversion=6.1
    MARCHS="armv7 armv7s"
else
	echo SDK not found
	exit 1
fi	

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
if test -d ctemplate ; then
	cd ctemplate
	git pull --rebase
else
	git clone $url
	cd ctemplate
fi
version=`git rev-parse HEAD | cut -c1-10`
build_version="$version~2"

if test -f "$resultdir/ctemplate-ios-$build_version.zip" ; then
	echo install from cache
	popd >/dev/null
	rm -rf ../Externals/ctemplate-ios
	mkdir -p ../Externals/tmp
	unzip -q "$resultdir/ctemplate-ios-$build_version.zip" -d ../Externals/tmp
	mv "../Externals/tmp/ctemplate-ios-$build_version/ctemplate-ios" ../Externals
  mkdir -p ../Externals/installed
  ln -sf "$resultdir/ctemplate-ios-$build_version.zip" ../Externals/installed
	rm -rf ../Externals/tmp
	exit 0
fi
popd >/dev/null

pushd . >/dev/null

cp -R "$builddir/downloads/ctemplate" "$srcdir/ctemplate"
echo building ctemplate
cd "$srcdir/ctemplate"

TOOLCHAIN=`xcode-select -p`/Toolchains/XcodeDefault.xctoolchain/usr/bin
export CC=$TOOLCHAIN/clang
export CXX=$TOOLCHAIN/clang++

sdk="iphoneos$sdkversion"
sysroot="`xcode-select -p`/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$sdkversion.sdk"

ARCH=arm
for MARCH in $MARCHS; do
  echo build for $MARCH
  echo "$logdir/ctemplate-build.log"
  export CPPFLAGS="-arch ${MARCH} -isysroot $sysroot -miphoneos-version-min=$sdkversion"
  export CFLAGS="$CPPFLAGS"
  export CXXFLAGS="$CFLAGS -stdlib=libc++"
  export LDFLAGS="-lstdc++ -stdlib=libc++"
  
  ./configure --host=${ARCH} --disable-shared --disable-dependency-tracking >> "$logdir/ctemplate-build.log"
  make >> "$logdir/ctemplate-build.log"
  make install-libLTLIBRARIES "prefix=$tmpdir/bin/ctemplate-ios/$sdk/$MARCH" >> "$logdir/ctemplate-build.log"
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
sysroot="`xcode-select -p`/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator$sdkversion.sdk"

ARCH=i386
MARCHS="i386 x86_64"

for MARCH in $MARCHS; do
  echo build for $MARCH
  echo "$logdir/ctemplate-build.log"
  export CPPFLAGS="-arch ${MARCH} -isysroot $sysroot -miphoneos-version-min=$sdkversion"
  export CFLAGS="$CPPFLAGS"
  export CXXFLAGS="$CFLAGS -stdlib=libc++"
  export LDFLAGS="-lstdc++ -stdlib=libc++"
  
  ./configure --host=${MARCH} --disable-shared --disable-dependency-tracking >> "$logdir/ctemplate-build.log"
  make >> "$logdir/ctemplate-build.log"
  make install-libLTLIBRARIES "prefix=$tmpdir/bin/ctemplate-ios/$sdk/$MARCH" >> "$logdir/ctemplate-build.log"
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
mkdir -p "ctemplate-ios-$build_version/ctemplate-ios/lib"
mv ctemplate-ios/include "ctemplate-ios-$build_version/ctemplate-ios"
lipo -create ctemplate-ios/iphonesimulator$sdkversion/i386/lib/libctemplate.a \
  ctemplate-ios/iphonesimulator$sdkversion/x86_64/lib/libctemplate.a \
  ctemplate-ios/iphoneos$sdkversion/armv7/lib/libctemplate.a \
  ctemplate-ios/iphoneos$sdkversion/armv7s/lib/libctemplate.a \
  ctemplate-ios/iphoneos$sdkversion/arm64/lib/libctemplate.a \
  -output "ctemplate-ios-$build_version/ctemplate-ios/lib/libctemplate-ios.a"
rm -f "$resultdir/ctemplate-ios-$build_version.zip"
zip -qry "$resultdir/ctemplate-ios-$build_version.zip" "ctemplate-ios-$build_version"
rm -f "$resultdir/ctemplate-ios-latest.zip"
cd "$resultdir"
ln -s "ctemplate-ios-$build_version.zip" "ctemplate-ios-latest.zip"

echo build of ctemplate-ios-$build_version done
echo $tmpdir

popd >/dev/null

rm -rf ../Externals/ctemplate-ios
mkdir -p ../Externals/tmp
unzip -q "$resultdir/ctemplate-ios-$build_version.zip" -d ../Externals/tmp
mv "../Externals/tmp/ctemplate-ios-$build_version/ctemplate-ios" ../Externals
rm -rf ../Externals/tmp

echo cleaning
rm -rf "$tempbuilddir"
echo "$tempbuilddir"
