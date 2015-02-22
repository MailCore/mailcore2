#!/bin/sh

pushd "`dirname "$0"`" > /dev/null
scriptpath="`pwd`"
popd > /dev/null

. "$scriptpath/include.sh/build-dep.sh"

url="https://github.com/mailcore/mailcore2.git"
rev=2f6f1ca69d59e2692f85d56941e7d996195cd6e8
name="mailcore2-osx"
xcode_target="static mailcore2 osx"
xcode_project="mailcore2.xcodeproj"
library="libMailCore.a"
embedded_deps="ctemplate-osx libetpan-osx uchardet-osx"
build_mailcore=1

build_git_osx
