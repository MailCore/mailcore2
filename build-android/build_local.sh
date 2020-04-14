#!/bin/sh

find ../src -name "*.h" -type file -exec cp {} ./include/MailCore \;

current_dir="`pwd`"

cd $current_dir/localdeps/ctemplate/android
chmod 777 ./build.sh
./build.sh
cd $current_dir

cd $current_dir/localdeps/icu4c/android
chmod 777 ./build.sh
./build.sh
cd $current_dir

cd $current_dir/localdeps/libxml2/android
chmod 777 ./build.sh
./build.sh
cd $current_dir

cd $current_dir/localdeps/tidy-html5/android
chmod 777 ./build.sh
./build.sh
cd $current_dir

rm -rf "$current_dir/mailcore-deps"

mkdir -p "$current_dir/mailcore-deps"
mkdir -p "$current_dir/mailcore-deps/include"
mkdir -p "$current_dir/mailcore-deps/include/ctemplate"
mkdir -p "$current_dir/mailcore-deps/include/icu4c"
mkdir -p "$current_dir/mailcore-deps/include/icu4c/unicode"
mkdir -p "$current_dir/mailcore-deps/include/icu4c/i18n"
mkdir -p "$current_dir/mailcore-deps/include/icu4c/i18n/unicode"
mkdir -p "$current_dir/mailcore-deps/include/libxml"
mkdir -p "$current_dir/mailcore-deps/include/tidy-html5"

mkdir -p "$current_dir/mailcore-deps/libs"
mkdir -p "$current_dir/mailcore-deps/libs/arm64-v8a"
mkdir -p "$current_dir/mailcore-deps/libs/armeabi-v7a"
mkdir -p "$current_dir/mailcore-deps/libs/x86"
mkdir -p "$current_dir/mailcore-deps/libs/x86_64"

cp "$current_dir/localdeps/ctemplate/android/artifacts/arm64-v8a/libctemplate.a" "$current_dir/mailcore-deps/libs/arm64-v8a"
cp "$current_dir/localdeps/ctemplate/android/artifacts/armeabi-v7a/libctemplate.a" "$current_dir/mailcore-deps/libs/armeabi-v7a"
cp "$current_dir/localdeps/ctemplate/android/artifacts/x86/libctemplate.a" "$current_dir/mailcore-deps/libs/x86"
cp "$current_dir/localdeps/ctemplate/android/artifacts/x86_64/libctemplate.a" "$current_dir/mailcore-deps/libs/x86_64"

cp "$current_dir/localdeps/icu4c/android/artifacts/arm64-v8a/libicui18n_static.a" "$current_dir/mailcore-deps/libs/arm64-v8a"
cp "$current_dir/localdeps/icu4c/android/artifacts/armeabi-v7a/libicui18n_static.a" "$current_dir/mailcore-deps/libs/armeabi-v7a"
cp "$current_dir/localdeps/icu4c/android/artifacts/x86/libicui18n_static.a" "$current_dir/mailcore-deps/libs/x86"
cp "$current_dir/localdeps/icu4c/android/artifacts/x86_64/libicui18n_static.a" "$current_dir/mailcore-deps/libs/x86_64"
cp "$current_dir/localdeps/icu4c/android/artifacts/arm64-v8a/libicuuc_static.a" "$current_dir/mailcore-deps/libs/arm64-v8a"
cp "$current_dir/localdeps/icu4c/android/artifacts/armeabi-v7a/libicuuc_static.a" "$current_dir/mailcore-deps/libs/armeabi-v7a"
cp "$current_dir/localdeps/icu4c/android/artifacts/x86/libicuuc_static.a" "$current_dir/mailcore-deps/libs/x86"
cp "$current_dir/localdeps/icu4c/android/artifacts/x86_64/libicuuc_static.a" "$current_dir/mailcore-deps/libs/x86_64"

cp "$current_dir/localdeps/libxml2/android/artifacts/arm64-v8a/libxml2.a" "$current_dir/mailcore-deps/libs/arm64-v8a"
cp "$current_dir/localdeps/libxml2/android/artifacts/armeabi-v7a/libxml2.a" "$current_dir/mailcore-deps/libs/armeabi-v7a"
cp "$current_dir/localdeps/libxml2/android/artifacts/x86/libxml2.a" "$current_dir/mailcore-deps/libs/x86"
cp "$current_dir/localdeps/libxml2/android/artifacts/x86_64/libxml2.a" "$current_dir/mailcore-deps/libs/x86_64"

cp "$current_dir/localdeps/tidy-html5/android/artifacts/arm64-v8a/libtidy-html5.a" "$current_dir/mailcore-deps/libs/arm64-v8a"
cp "$current_dir/localdeps/tidy-html5/android/artifacts/armeabi-v7a/libtidy-html5.a" "$current_dir/mailcore-deps/libs/armeabi-v7a"
cp "$current_dir/localdeps/tidy-html5/android/artifacts/x86/libtidy-html5.a" "$current_dir/mailcore-deps/libs/x86"
cp "$current_dir/localdeps/tidy-html5/android/artifacts/x86_64/libtidy-html5.a" "$current_dir/mailcore-deps/libs/x86_64"

find ./localdeps/ctemplate/src/ctemplate -name "*.h" -type file -exec cp {} ./mailcore-deps/include/ctemplate \;
find ./localdeps/icu4c/source/common/unicode -name "*.h" -type file -exec cp {} ./mailcore-deps/include/icu4c/unicode \;
find ./localdeps/icu4c/source/i18n/unicode -name "*.h" -type file -exec cp {} ./mailcore-deps/include/icu4c/i18n/unicode \;
find ./localdeps/libxml2/include/libxml -name "*.h" -type file -exec cp {} ./mailcore-deps/include/libxml \;
find ./localdeps/tidy-html5/include -name "*.h" -type file -exec cp {} ./mailcore-deps/include/tidy-html5 \;


#call %LocalAppData%\Android\Sdk\ndk-bundle\ndk-build.cmd -C . NDK_DEBUG=0 DIR=%CD% APP_PLATFORM=android-21 NDKLEVEL=21 ANDROID_NDK=%LocalAppData%\Android\Sdk\ndk-bundle
$ANDROID_NDK/ndk-build -C . NDK_APPLICATION_MK=Application.mk NDK_DEBUG=0 APP_PLATFORM=android-23 NDKLEVEL=23

#REM copy obj\local\arm64-v8a\*.a mailcore-deps\libs\arm64-v8a
#REM copy obj\local\armeabi-v7a\*.a mailcore-deps\libs\armeabi-v7a
#REM copy obj\local\x86\*.a mailcore-deps\libs\x86
#REM copy obj\local\x86_64\*.a mailcore-deps\libs\x86_64

