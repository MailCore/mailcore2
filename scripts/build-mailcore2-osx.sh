#!/bin/sh

pushd "`dirname "$0"`" > /dev/null
scriptpath="`pwd`"
popd > /dev/null

. "$scriptpath/include.sh/build-dep.sh"

url="https://github.com/mailcore/mailcore2.git"
rev=c519d475178d09651271913f3453a4820fdd82b4
name="mailcore2-osx"
xcode_target="static mailcore2 osx"
xcode_project="mailcore2.xcodeproj"
library="libMailCore.a"
embedded_deps="ctemplate-osx libetpan-osx"
build_mailcore=1

build_git_osx
