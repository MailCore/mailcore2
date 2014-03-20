#!/bin/sh

url="https://github.com/MailCore/mailcore2-deps"
url_prefix="$url/raw/master"

if test x$1 != xskipprebuilt ; then
  file_timestamp=0
  if test -f prebuilt.list ; then
    file_timestamp=`stat -f '%m' prebuilt.list`
  fi
  timestamp=`ruby -e 'puts Time.now.to_i'`
  age=$((($timestamp-$file_timestamp)/3600)) # in hours
  if test ! -d ../Externals/prebuilt ; then
    age=1
  fi
  if test $age -gt 0 ; then
    networkerror=no
    echo "$url_prefix/prebuilt.list"
    curl -s -v -L "$url_prefix/prebuilt.list" > prebuilt.list.tmp
    cat prebuilt.list.tmp
    if test x$? != x0 ; then
      networkerror=yes
    fi
    
    if test x$networkerror = xyes ; then
      echo WARNING: could not get prebuilt.list from repository
      exit 1
    fi
    
    mv prebuilt.list.tmp prebuilt.list
  
    if test -f prebuilt.list ; then
      files=`cat prebuilt.list`
      mkdir -p ../Externals/builds/builds
      mkdir -p ../Externals/prebuilt
      pushd ../Externals/prebuilt
      rm -rf .git
      echo Getting prebuilt libraries...
      if test -d mailcore2-deps ; then
        cd mailcore2-deps
        git pull --rebase
        cd ..
      else
        git clone --depth 1 "$url"
      fi
      rsync --exclude=.git -av mailcore2-deps/ ../builds/builds/
      popd
    fi
  fi
fi
