#!/bin/sh
if test ! -d ../Externals/icu4c ; then
  ./prepare-icu4c-macos.sh
fi
if test ! -d ../Externals/libetpan ; then
  ./prepare-libetpan-macos.sh
fi
if test ! -d ../Externals/ctemplate ; then
  ./prepare-ctemplate-macos.sh
fi

