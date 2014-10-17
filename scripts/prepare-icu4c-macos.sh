#!/bin/sh

versionfolder='51.1'
version='51_1'
build_version="$version~1"
url="http://download.icu-project.org/files/icu4c/$versionfolder/icu4c-$version-src.tgz"
package_filename="icu4c-$version-src.tgz"
arch="x86_64"
sysrootpath="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk"
enable_icu_data=0

arch_flags=""
for current_arch in $arch ; do
	arch_flags="$arch_flags -arch $current_arch"
done

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

if test -f "$resultdir/icu4c-$build_version.zip" ; then
	echo install from cache
	rm -rf ../Externals/icu4c
	mkdir -p ../Externals/tmp
	unzip -q "$resultdir/icu4c-$build_version.zip" -d ../Externals/tmp
	mv "../Externals/tmp/icu4c-$build_version/icu4c" ../Externals
  mkdir -p ../Externals/installed
  ln -sf "$resultdir/icu4c-$build_version.zip" ../Externals/installed
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
cd icu
patch -p1 < "$scriptpath/icu4c-ios.patch"
cd ..

echo building icu4c
cd "$srcdir/icu/source"
for cur_arch in $arch ; do
	cur_arch_flags="-arch $cur_arch"
	echo building icu4c for $cur_arch
	export CFLAGS="$cur_arch_flags -isysroot $sysrootpath -mfix-and-continue -mmacosx-version-min=10.7"
	export CXXFLAGS="$cur_arch_flags -isysroot $sysrootpath -mfix-and-continue -mmacosx-version-min=10.7"
	export LDLAGS="$cur_arch_flags -isysroot $sysrootpath -mfix-and-continue -mmacosx-version-min=10.7"
	mkdir -p "$tmpdir/build/icu4c-$cur_arch"
	cd "$tmpdir/build/icu4c-$cur_arch"
	if test "x$cur_arch" = xx86_64 ; then
		"$srcdir/icu/source/configure" --enable-static --disable-shared --with-data-packaging=archive >> "$logdir/icu4c-build.log"
	else
		"$srcdir/icu/source/configure" --host=$cur_arch-apple-darwin10.6.0 --enable-static --disable-shared --with-data-packaging=archive --with-cross-build=$tmpdir/crossbuild/icu4c-x86_64 >> "$logdir/icu4c-build.log"
	fi
	make >> "$logdir/icu4c-build.log"
	make install "prefix=$tmpdir/bin/icu4c-$cur_arch" >> "$logdir/icu4c-build.log"
	if test "x$cur_arch" = xx86_64 ; then
		make install "prefix=$tmpdir/crossbuild/icu4c-$cur_arch" >> "$logdir/icu4c-build.log"
	fi
	if test x$? != x0 ; then
		echo build of icu4c failed
		exit 1
	fi
	#make distclean
done

mkdir -p "$tmpdir/bin/icu4c"
cp -R "$tmpdir/bin/icu4c-x86_64/include" "$tmpdir/bin/icu4c"
if test x$enable_icu_data = x1 ; then
  mkdir -p "$tmpdir/bin/icu4c/share/icu"
  cp "$tmpdir/bin/icu4c-x86_64/share/icu/$versionfolder/icudt51l.dat" "$tmpdir/bin/icu4c/share/icu"
fi
mkdir -p "$tmpdir/bin/icu4c/lib"
cd "$tmpdir/bin"

icudata_paths="" #
icui18n_paths="" #
icuio_paths=""
icule_paths=""
iculx_paths=""
icutest_paths=""
icutu_paths=""
icuuc_paths="" #
for cur_arch in $arch ; do
	icudata_paths="$icudata_paths icu4c-$cur_arch/lib/libicudata.a"
	icui18n_paths="$icui18n_paths icu4c-$cur_arch/lib/libicui18n.a"
	icuio_paths="$icuio_paths icu4c-$cur_arch/lib/libicuio.a"
	icule_paths="$icule_paths icu4c-$cur_arch/lib/libicule.a"
	iculx_paths="$iculx_paths icu4c-$cur_arch/lib/libiculx.a"
	icutest_paths="$icutest_paths icu4c-$cur_arch/lib/libicutest.a"
	icutu_paths="$icutu_paths icu4c-$cur_arch/lib/libicutu.a"
	icuuc_paths="$icuuc_paths icu4c-$cur_arch/lib/libicuuc.a"
done
lipo -create $icudata_paths -output icu4c/lib/libicudata.a
lipo -create $icui18n_paths -output icu4c/lib/libicui18n.a
lipo -create $icuio_paths -output icu4c/lib/libicuio.a
lipo -create $icule_paths -output icu4c/lib/libicule.a
lipo -create $iculx_paths -output icu4c/lib/libiculx.a
lipo -create $icutest_paths -output icu4c/lib/libicutest.a
lipo -create $icutu_paths -output icu4c/lib/libicutu.a
lipo -create $icuuc_paths -output icu4c/lib/libicuuc.a

cd "$tmpdir/bin"
mkdir -p "icu4c-$build_version"
mv icu4c "icu4c-$build_version"
zip -qry "$resultdir/icu4c-$build_version.zip" "icu4c-$build_version"
rm -f "$resultdir/icu4c-latest.zip"
cd "$resultdir"
ln -s "icu4c-$build_version.zip" "icu4c-latest.zip"

echo build of icu4c-$build_version done

popd

rm -rf ../Externals/icu4c
mkdir -p ../Externals/tmp
unzip -q "$resultdir/icu4c-$build_version.zip" -d ../Externals/tmp
mv "../Externals/tmp/icu4c-$build_version/icu4c" ../Externals
rm -rf ../Externals/tmp

echo cleaning
rm -rf "$tempbuilddir"
echo "$tempbuilddir"
