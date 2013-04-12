#!/bin/sh

if test x$1 != xskipprebuilt ; then
  if test ! -d ../Externals/prebuilt ; then
    mkdir -p ../Externals/builds/builds
    mkdir -p ../Externals/prebuilt
    pushd ../Externals/prebuilt
    if test -d mailcore2-deps ; then
      cd mailcore2-deps
      git pull --rebase
      cd ..
    else
      git clone https://github.com/dinhviethoa/mailcore2-deps.git
    fi
    cp mailcore2-deps/*.zip ../builds/builds
    popd
  fi
fi
