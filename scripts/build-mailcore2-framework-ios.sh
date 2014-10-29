#!/bin/sh

pushd "`dirname "$0"`" > /dev/null
scriptpath="`pwd`"
popd > /dev/null

. "$scriptpath/include.sh/build-dep.sh"

url="https://github.com/mailcore/mailcore2.git"
rev=2f6f1ca69d59e2692f85d56941e7d996195cd6e8
name="mailcore2-framework-ios"
xcode_target="mailcore ios"
xcode_project="mailcore2.xcodeproj"
library="MailCore.framework"
embedded_deps="ctemplate-ios libetpan-ios tidy-html5-ios uchardet-ios"

build_git_ios
