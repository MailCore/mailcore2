#!/bin/sh

pushd "`dirname "$0"`" > /dev/null
scriptpath="`pwd`"
popd > /dev/null

. "$scriptpath/include.sh/build-dep.sh"

url="https://github.com/mailcore/mailcore2.git"
rev=da097788fc9d80ed0473dd634b26b3ae281e5618
name="mailcore2-ios"
xcode_target="static mailcore2 ios"
xcode_project="mailcore2.xcodeproj"
library="libMailCore-ios.a"
embedded_deps="ctemplate-ios libetpan-ios tidy-html5-ios uchardet-ios"
flatten_deps=1

build_git_ios
