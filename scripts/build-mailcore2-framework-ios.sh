#!/bin/sh

pushd "`dirname "$0"`" > /dev/null
scriptpath="`pwd`"
popd > /dev/null

. "$scriptpath/include.sh/build-dep.sh"

url="https://github.com/mailcore/mailcore2.git"
rev=da097788fc9d80ed0473dd634b26b3ae281e5618
name="mailcore2-framework-ios"
xcode_target="mailcore ios"
xcode_project="mailcore2.xcodeproj"
library="MailCore.framework"
embedded_deps="ctemplate-ios libetpan-ios tidy-html5-ios uchardet-ios"

build_git_ios
