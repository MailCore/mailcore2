#!/bin/sh
set -e

if test x"`uname`" = xDarwin ; then
  brew update
#  brew install cmake
elif test x"`uname`" = xLinux ; then
  git clone --depth=1 https://github.com/dinhviethoa/libetpan
  cd libetpan
  ./autogen.sh
  make >/dev/null
  sudo make install >/dev/null
  cd ..

  sudo apt-get install libctemplate-dev
  sudo apt-get install libicu-dev
  sudo apt-get install libsasl2-dev
  sudo apt-get install libtidy-dev
  sudo apt-get install uuid-dev
  sudo apt-get install libxml2-dev
  sudo apt-get install libuchardet-dev
fi

