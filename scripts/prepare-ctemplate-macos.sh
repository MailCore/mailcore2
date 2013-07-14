#!/bin/sh

url="https://github.com/dinhviethoa/ctemplate"

arch="x86_64"
#sysrootpath="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk"

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

if test -f "$resultdir/ctemplate-$version.zip" ; then
	echo install from cache
	popd >/dev/null
	rm -rf ../Externals/ctemplate
	mkdir -p ../Externals/tmp
	unzip -q "$resultdir/ctemplate-$version.zip" -d ../Externals/tmp
	mv "../Externals/tmp/ctemplate-$version/ctemplate" ../Externals
  mkdir -p ../Externals/installed
  ln -sf "$resultdir/ctemplate-$version.zip" ../Externals/installed
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
export CFLAGS="$arch_flags"
export CXXFLAGS="-std=c++11 -stdlib=libc++ $arch_flags"
./configure --disable-shared --disable-dependency-tracking >> "$logdir/ctemplate-build.log"
#make libctemplate.la >> "$logdir/ctemplate-build.log"
#make libctemplate_nothreads.la >> "$logdir/ctemplate-build.log"
#make install-libLTLIBRARIES "prefix=$tmpdir/bin/ctemplate" >> "$logdir/ctemplate-build.log"
make >> "$logdir/ctemplate-build.log"
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

echo finished

#mkdir -p "$tmpdir/bin"
cd "$tmpdir/bin"
mkdir -p "ctemplate-$version"
mv ctemplate "ctemplate-$version"
rm -f "$resultdir/ctemplate-$version.zip"
zip -qry "$resultdir/ctemplate-$version.zip" "ctemplate-$version"
rm -f "$resultdir/ctemplate-latest.zip"
cd "$resultdir"
ln -s "ctemplate-$version.zip" "ctemplate-latest.zip"

echo build of ctemplate-$version done

popd >/dev/null

rm -rf ../Externals/ctemplate
mkdir -p ../Externals/tmp
unzip -q "$resultdir/ctemplate-$version.zip" -d ../Externals/tmp
mv "../Externals/tmp/ctemplate-$version/ctemplate" ../Externals
rm -rf ../Externals/tmp

echo cleaning
rm -rf "$tempbuilddir"
echo "$tempbuilddir"
