#!/bin/sh

url_prefix="https://github.com/MailCore/mailcore2-deps/raw/master"

if test x$1 != xskipprebuilt ; then
  file_timestamp=0
  if test -f prebuilt.list ; then
    file_timestamp=`stat -f '%m' prebuilt.list`
  fi
  timestamp=`ruby -e 'puts Time.now.to_i'`
  age=$((($timestamp-$file_timestamp)/3600)) # in hours
  if test $age -gt 0 ; then
    networkerror=no
    #echo "$url_prefix/prebuilt.list"
    curl -s -L "$url_prefix/prebuilt.list" > prebuilt.list.tmp
    if test x$? != x0 ; then
      networkerror=yes
    fi
    
    if test x$networkerror = xyes ; then
      echo WARNING: could not get prebuilt.list from repository
    fi
    
    mv prebuilt.list.tmp prebuilt.list
  fi
  
  if test -f prebuilt.list ; then
    files=`cat prebuilt.list`
    mkdir -p ../Externals/builds/builds
    mkdir -p ../Externals/prebuilt/mailcore2-deps
    pushd ../Externals/prebuilt/mailcore2-deps
    rm -rf .git
    for filename in $files ; do
      if test ! -f $filename ; then
        echo get $filename
        curl -O "$url_prefix/$filename"
      fi
    done
    rsync --exclude=.git -av ./ ../../builds/builds/
    popd
  fi
fi
