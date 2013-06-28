#!/bin/sh
./get-prebuilt.sh

missing=no
for filename in `cat prebuilt.list|grep -- -ios` ; do
  if test ! -f "../Externals/installed/$filename" ; then
    missing=yes
  fi
done
if test $missing = yes ; then
  ./prepare-tidy-ios.sh
  ./prepare-icu4c-ios.sh
  ./prepare-libetpan-ios.sh
  ./prepare-ctemplate-ios.sh
fi
