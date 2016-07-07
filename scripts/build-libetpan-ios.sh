#!/bin/sh

pushd "`dirname "$0"`" > /dev/null
scriptpath="`pwd`"
popd > /dev/null

. "$scriptpath/include.sh/build-dep.sh"

url="https://github.com/dinhviethoa/libetpan.git"
rev=cc1932c7ca7bb56412c92910a52b8c37a9ded4a8
name="libetpan-ios"
xcode_target="libetpan ios"
xcode_project="libetpan.xcodeproj"
library="libetpan-ios.a"
embedded_deps="libsasl-ios"

build_git_ios
