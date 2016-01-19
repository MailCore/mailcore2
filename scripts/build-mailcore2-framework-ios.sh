#!/bin/sh

pushd "`dirname "$0"`" > /dev/null
scriptpath="`pwd`"
popd > /dev/null

. "$scriptpath/include.sh/build-dep.sh"

url="https://github.com/mailcore/mailcore2.git"
rev=26b78229f7b09fa3f358c0ab096d6d9504ca56ba
name="mailcore2-framework-ios"
xcode_target="mailcore ios"
xcode_project="mailcore2.xcodeproj"
library="MailCore.framework"
embedded_deps="ctemplate-ios libetpan-ios tidy-html5-ios"

build_git_ios
