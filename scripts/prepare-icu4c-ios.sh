#!/bin/sh

sdkversion=6.1
versionfolder='51.1'
version='51_1'
build_version="$version~1"
url="http://download.icu-project.org/files/icu4c/$versionfolder/icu4c-$version-src.tgz"
package_filename="icu4c-$version-src.tgz"
sysrootpath="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk"
enable_icu_data=0

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

if test -f "$resultdir/icu4c-ios-$build_version.zip" ; then
	echo install from cache
	rm -rf ../Externals/icu4c-ios
	mkdir -p ../Externals/tmp
	unzip -q "$resultdir/icu4c-ios-$build_version.zip" -d ../Externals/tmp
	mv "../Externals/tmp/icu4c-ios-$build_version/icu4c-ios" ../Externals
  mkdir -p ../Externals/installed
  ln -sf "$resultdir/icu4c-ios-$build_version.zip" ../Externals/installed
	rm -rf ../Externals/tmp
	exit 0
fi

mkdir -p "$resultdir"
mkdir -p "$logdir"
mkdir -p "$tmpdir"
mkdir -p "$srcdir"

pushd . >/dev/null

echo get icu4c
cd "$srcdir"
if test -f "$builddir/downloads/$package_filename" ; then
	cp "$builddir/downloads/$package_filename" .
else
	curl -O "$url"
	if test x$? != x0 ; then
		echo fetch of icu4c failed
		exit 1
	fi
	mkdir -p "$builddir/downloads"
	cp "$package_filename" "$builddir/downloads"
fi

tar xf "$package_filename"

TOOLCHAIN=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
export CC=$TOOLCHAIN/clang
export CXX=$TOOLCHAIN/clang++
export LDLAGS="-lc++ -isysroot $sysrootpath"

echo build x86_64
MARCH=x86_64
mkdir -p "$tmpdir/crossbuild/"
cp -R "$srcdir" "$tmpdir/crossbuild/icu4c-$MARCH"
cd "$tmpdir/crossbuild/icu4c-$MARCH"
"icu/source/configure" --enable-static --disable-shared --with-data-packaging=archive >> "$logdir/icu4c-build.log"
make >> "$logdir/icu4c-build.log"
make install "prefix=$tmpdir/crossbuild/icu4c-$MARCH" >> "$logdir/icu4c-build.log"

ARCH=arm
MARCHS="armv7 armv7s"

iphonesdk="iphoneos$sdkversion"
sysroot="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$sdkversion.sdk"

cd "$srcdir/icu/source"
for MARCH in $MARCHS; do
  export CFLAGS="-arch ${MARCH} -isysroot ${sysroot} -DUCONFIG_NO_FILE_IO=1"
  export CXXFLAGS="$CFLAGS -stdlib=libstdc++ -std=gnu++11"
  export LDFLAGS="-lstdc++ -stdlib=libstdc++"

  echo building $MARCH
  
	mkdir -p "$tmpdir/build/icu4c-$MARCH"
	cd "$tmpdir/build/icu4c-$MARCH"
	"$srcdir/icu/source/configure" --host=$ARCH-apple-darwin10.6.0 --enable-static --disable-shared --with-data-packaging=archive --with-cross-build=$tmpdir/crossbuild/icu4c-x86_64 >> "$logdir/icu4c-build.log"
	make >> "$logdir/icu4c-build.log"
	make install "prefix=$tmpdir/bin/icu4c-$MARCH" >> "$logdir/icu4c-build.log"
	if test x$? != x0 ; then
		echo build of icu4c failed
		exit 1
	fi
done

sdk="iphonesimulator$sdkversion"
sysroot="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator$sdkversion.sdk"

ARCH=i386
MARCHS=i386

cd "$srcdir/icu/source"
for MARCH in $MARCHS; do
  export CFLAGS="-arch ${MARCH} -isysroot ${sysroot} -DUCONFIG_NO_FILE_IO=1"
  export CXXFLAGS="$CFLAGS -stdlib=libstdc++ -std=gnu++11"
  export LDFLAGS="-lstdc++ -stdlib=libstdc++"

  echo building $MARCH
  
	mkdir -p "$tmpdir/build/icu4c-$MARCH"
	cd "$tmpdir/build/icu4c-$MARCH"
	"$srcdir/icu/source/configure" --host=$ARCH-apple-darwin10.6.0 --enable-static --disable-shared --with-data-packaging=archive --with-cross-build=$tmpdir/crossbuild/icu4c-x86_64 >> "$logdir/icu4c-build.log"
	make >> "$logdir/icu4c-build.log"
	make install "prefix=$tmpdir/bin/icu4c-$MARCH" >> "$logdir/icu4c-build.log"
	if test x$? != x0 ; then
		echo build of icu4c failed
		exit 1
	fi
done

mkdir -p "$tmpdir/bin/icu4c-ios"
cp -R $tmpdir/crossbuild/icu4c-x86_64/include "$tmpdir/bin/icu4c-ios"
if test x$enable_icu_data = x1 ; then
  mkdir -p "$tmpdir/bin/icu4c-ios/share/icu"
  cp "$tmpdir/crossbuild/icu4c-x86_64/share/icu/$versionfolder/icudt51l.dat" "$tmpdir/bin/icu4c-ios/share/icu"
fi
mkdir -p "$tmpdir/bin/icu4c-ios/lib"
cd "$tmpdir/bin"

icudata_paths=""
icui18n_paths=""
icuio_paths=""
icule_paths=""
iculx_paths=""
icutest_paths=""
icutu_paths=""
icuuc_paths=""
archs="armv7 armv7s i386"
for cur_arch in $archs ; do
	icudata_paths="$icudata_paths icu4c-$cur_arch/lib/libicudata.a"
	icui18n_paths="$icui18n_paths icu4c-$cur_arch/lib/libicui18n.a"
	icuio_paths="$icuio_paths icu4c-$cur_arch/lib/libicuio.a"
	icule_paths="$icule_paths icu4c-$cur_arch/lib/libicule.a"
	iculx_paths="$iculx_paths icu4c-$cur_arch/lib/libiculx.a"
	icutest_paths="$icutest_paths icu4c-$cur_arch/lib/libicutest.a"
	icutu_paths="$icutu_paths icu4c-$cur_arch/lib/libicutu.a"
	icuuc_paths="$icuuc_paths icu4c-$cur_arch/lib/libicuuc.a"
done
lipo -create $icudata_paths -output icu4c-ios/lib/libicudata.a
lipo -create $icui18n_paths -output icu4c-ios/lib/libicui18n.a
lipo -create $icuio_paths -output icu4c-ios/lib/libicuio.a
lipo -create $icule_paths -output icu4c-ios/lib/libicule.a
lipo -create $iculx_paths -output icu4c-ios/lib/libiculx.a
lipo -create $icutest_paths -output icu4c-ios/lib/libicutest.a
lipo -create $icutu_paths -output icu4c-ios/lib/libicutu.a
lipo -create $icuuc_paths -output icu4c-ios/lib/libicuuc.a

cd "$tmpdir/bin"
mkdir -p "icu4c-ios-$build_version"
mv icu4c-ios "icu4c-ios-$build_version"
zip -qry "$resultdir/icu4c-ios-$build_version.zip" "icu4c-ios-$build_version"
rm -f "$resultdir/icu4c-ios-latest.zip"
cd "$resultdir"
ln -s "icu4c-ios-$build_version.zip" "icu4c-ios-latest.zip"

echo build of icu4c-$build_version done

popd

rm -rf ../Externals/icu4c-ios
mkdir -p ../Externals/tmp
unzip -q "$resultdir/icu4c-ios-$build_version.zip" -d ../Externals/tmp
mv "../Externals/tmp/icu4c-ios-$build_version/icu4c-ios" ../Externals
rm -rf ../Externals/tmp

echo cleaning
rm -rf "$tempbuilddir"
echo "$tempbuilddir"
