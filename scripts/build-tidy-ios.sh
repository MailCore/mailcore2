#!/bin/sh

pushd "`dirname "$0"`" > /dev/null
scriptpath="`pwd`"
popd > /dev/null

. "$scriptpath/include.sh/build-dep.sh"

url="https://github.com/dinhviethoa/tidy-html5"
rev=7361665fe46de3ec86dc508876866614f7053160
name="tidy-html5-ios"
xcode_target="tidy"
xcode_project="Tidy.xcodeproj"
library="libtidy.a"
embedded_deps=""

build_git_ios
