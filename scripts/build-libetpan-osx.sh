#!/bin/sh

pushd "`dirname "$0"`" > /dev/null
scriptpath="`pwd`"
popd > /dev/null

. "$scriptpath/include.sh/build-dep.sh"

url="https://github.com/dinhviethoa/libetpan.git"
rev=7020007cfcc75b58af4d12248f1e687f21a1986d
name="libetpan-osx"
xcode_target="static libetpan"
xcode_project="libetpan.xcodeproj"
library="libetpan.a"
embedded_deps=""

build_git_osx
