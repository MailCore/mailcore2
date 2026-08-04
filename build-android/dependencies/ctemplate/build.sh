#!/bin/sh

# ctemplate is a mailcore2 dependency (HTML rendering, src/core/renderer). It is
# the one dependency that exposes C++ STL types (std::string) across its public
# API, so it MUST be compiled with the same STL as mailcore2 — libc++. The
# original d.etpan.org prebuilt was built against gnustl and fails to link
# against an NDK r18+ / libc++ build. This script rebuilds it from source with
# the NDK clang/libc++ toolchain.

build_version=3
package_name=ctemplate-android

# dinhviethoa fork + revision used by mailcore2's own scripts/build-ctemplate-osx.sh
ctemplate_url=https://github.com/dinhviethoa/ctemplate
ctemplate_rev=b50f83f26ce3658889db1ae41b6a2d5874a3a10f

export MIN_SDK_VERSION=23
export HOST_TAG=darwin-x86_64

if test "x$ANDROID_NDK" = x ; then
  echo should set ANDROID_NDK before running this script.
  exit 1
fi

current_dir="`pwd`"
export TOOLCHAIN=$ANDROID_NDK/toolchains/llvm/prebuilt/$HOST_TAG
export AR=$TOOLCHAIN/bin/llvm-ar
export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
PATH=$TOOLCHAIN/bin:$PATH

if test -f "$current_dir/$package_name-$build_version.zip" ; then
  echo "$package_name-$build_version.zip already exists; remove it to rebuild."
  exit 0
fi

rm -rf "$current_dir/src"
mkdir -p "$current_dir/src"
cd "$current_dir/src"

# --- Fetch source -----------------------------------------------------------
git clone "$ctemplate_url" ctemplate
cd ctemplate
git checkout "$ctemplate_rev"

# config.sub/config.guess in this 2014-era fork predate the Android triples;
# fetch current ones so ./configure accepts aarch64/armv7a/i686/x86_64-*-android.
curl -L -o config.sub  "https://git.savannah.gnu.org/cgit/config.git/plain/config.sub"
curl -L -o config.guess "https://git.savannah.gnu.org/cgit/config.git/plain/config.guess"
chmod +x config.sub config.guess

# --- Generate htmlparser finite-state-machine headers -----------------------
# generate_fsm.py (and fsm_config.py) are Python 2; convert them to Python 3,
# then generate the *_fsm.h headers the parser sources #include.
( cd src/htmlparser
  2to3 -w -n generate_fsm.py fsm_config.py >/dev/null 2>&1
  python3 generate_fsm.py htmlparser_fsm.config > htmlparser_fsm.h
  python3 generate_fsm.py jsparser_fsm.config   > jsparser_fsm.h )

CTEMPLATE_SRC="$current_dir/src/ctemplate"

# abi : configure --host triple : NDK clang prefix
build_abi() {
  abi="$1" ; host="$2" ; cc="$3"
  echo "############ ctemplate $abi ############"
  cd "$CTEMPLATE_SRC"
  make distclean >/dev/null 2>&1

  export CC="$TOOLCHAIN/bin/${cc}${MIN_SDK_VERSION}-clang"
  export CXX="$TOOLCHAIN/bin/${cc}${MIN_SDK_VERSION}-clang++"
  export CFLAGS="-fPIC -O2"
  export CXXFLAGS="-fPIC -O2 -std=c++11"

  ./configure --host="$host" --enable-static --disable-shared \
      --disable-dependency-tracking
  # Build only the library (the test/utility binaries can't run when cross-built).
  make -j5 libctemplate.la

  out="$current_dir/$package_name-$build_version"
  mkdir -p "$out/libs/$abi"
  cp .libs/libctemplate.a "$out/libs/$abi/libctemplate.a"

  # Public headers (incl. configure-generated ones) — copy once.
  if test ! -d "$out/include/ctemplate" ; then
    mkdir -p "$out/include/ctemplate"
    cp src/ctemplate/*.h "$out/include/ctemplate/"
  fi
}

build_abi arm64-v8a    aarch64-linux-android      aarch64-linux-android
build_abi armeabi-v7a  armv7a-linux-androideabi   armv7a-linux-androideabi
build_abi x86          i686-linux-android         i686-linux-android
build_abi x86_64       x86_64-linux-android       x86_64-linux-android

cd "$current_dir"
zip -qry "$package_name-$build_version.zip" "$package_name-$build_version"
rm -rf "$current_dir/$package_name-$build_version"
rm -rf "$current_dir/src"
echo "Built $package_name-$build_version.zip"
