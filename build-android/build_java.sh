#!/bin/sh

build_version=5
package_name=mailcore2-android

current_dir="`pwd`"

#find ../src -name "*.h" -type file -exec cp {} ./include/MailCore \;

if test "x$ANDROID_NDK" = x ; then
  echo should set ANDROID_NDK before running this script.
  exit 1
fi

if test "x$ANDROID_SDK" = x ; then
  echo should set ANDROID_SDK before running this script.
  exit 1
fi

ANDROID_PLATFORM=android-28
cd "$current_dir/../src/java"
mkdir -p "$current_dir/bin/jni"
cp -a "$current_dir/libs/" "$current_dir/bin/jni/"
/usr/bin/javac -d "$current_dir/bin" -source 1.7 -target 1.7 -classpath $ANDROID_SDK/platforms/$ANDROID_PLATFORM/android.jar com/libmailcore/*.java
cd "$current_dir/bin"
jar cf classes.jar .
rm -rf "$current_dir/bin/com"
mkdir -p res
sed -e "s/android:versionCode=\"1\"/android:versionCode=\"$build_version\"/" "$current_dir/AndroidManifest.xml" > "$current_dir/bin/AndroidManifest.xml"
zip -r "$current_dir/$package_name-$build_version.aar" .

#rm -rf "$current_dir/bin"
