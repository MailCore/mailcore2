#!/bin/sh
./get-prebuilt.sh

missing=no
for filename in `cat prebuilt.list|grep -v -- -ios` ; do
  if test ! -f "../Externals/installed/$filename" ; then
    missing=yes
  fi
done
if test $missing = yes ; then
  ./prepare-icu4c-macos.sh
  ./prepare-libetpan-macos.sh
  ./prepare-ctemplate-macos.sh
fi

