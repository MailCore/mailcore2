#!/bin/sh
if test ! -d ../Externals/tidy-html5-ios ; then
  ./prepare-tidy-ios.sh
fi
if test ! -d ../Externals/icu4c-ios ; then
  ./prepare-icu4c-ios.sh
fi
if test ! -d ../Externals/libetpan-ios ; then
  ./prepare-libetpan-ios.sh
fi
if test ! -d ../Externals/ctemplate-ios ; then
  ./prepare-ctemplate-ios.sh
fi
