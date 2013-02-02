#!/bin/sh

version='2.2'
url="http://ctemplate.googlecode.com/files/ctemplate-$version.tar.gz"
package_filename="ctemplate-$version.tar.gz"

arch="i386 x86_64"
sysrootpath="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk"

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

if test -f "$resultdir/ctemplate-$version.zip" ; then
	echo install from cache
	rm -rf ../Externals/ctemplate
	mkdir -p ../Externals/tmp
	unzip -q "$resultdir/ctemplate-$version.zip" -d ../Externals/tmp
	mv "../Externals/tmp/ctemplate-$version/ctemplate" ../Externals
	rm -rf ../Externals/tmp
	exit 0
fi

mkdir -p "$resultdir"
mkdir -p "$logdir"
mkdir -p "$tmpdir"
mkdir -p "$srcdir"

pushd . >/dev/null

echo get CTemplate
cd "$srcdir"
if test -f "$builddir/downloads/$package_filename" ; then
	cp "$builddir/downloads/$package_filename" .
else
	curl -O "$url"
	if test x$? != x0 ; then
		echo fetch of ctemplate failed
		exit 1
	fi
	mkdir -p "$builddir/downloads"
	cp "$package_filename" "$builddir/downloads"
fi

tar xf "$package_filename"

echo building CTemplate
cd "$srcdir/ctemplate-$version"
export CFLAGS="$arch_flags -isysroot $sysrootpath -mfix-and-continue -mmacosx-version-min=10.7"
export CXXFLAGS="$arch_flags -isysroot $sysrootpath -mfix-and-continue -mmacosx-version-min=10.7"
export LDLAGS="$arch_flags -isysroot $sysrootpath -mfix-and-continue -mmacosx-version-min=10.7"
./configure --disable-shared --disable-dependency-tracking >> "$logdir/ctemplate-build.log"
make libctemplate.la >> "$logdir/ctemplate-build.log"
make libctemplate_nothreads.la >> "$logdir/ctemplate-build.log"
make install-libLTLIBRARIES "prefix=$tmpdir/bin/ctemplate" >> "$logdir/ctemplate-build.log"
if test x$? != x0 ; then
	echo build of ctemplate failed
	exit 1
fi

make install-nodist_ctemplateincludeHEADERS "prefix=$tmpdir/bin/ctemplate" >> "$logdir/ctemplate-build.log"
if test x$? != x0 ; then
	echo install of ctemplate failed
	exit 1
fi

cd "$tmpdir/bin"
mkdir -p "ctemplate-$version"
mv ctemplate "ctemplate-$version"
rm -f "$resultdir/ctemplate-$version.zip"
zip -qry "$resultdir/ctemplate-$version.zip" "ctemplate-$version"
rm -f "$resultdir/ctemplate-latest.zip"
cd "$resultdir"
ln -s "ctemplate-$version.zip" "ctemplate-latest.zip"

echo build of ctemplate-$version done

popd

rm -rf ../Externals/ctemplate
mkdir -p ../Externals/tmp
unzip -q "$resultdir/ctemplate-$version.zip" -d ../Externals/tmp
mv "../Externals/tmp/ctemplate-$version/ctemplate" ../Externals
rm -rf ../Externals/tmp

echo cleaning
rm -rf "$tempbuilddir"
