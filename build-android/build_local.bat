

set OLDDIR=%CD%

cd localdeps\ctemplate\android
call build.bat
chdir /d %OLDDIR%

cd localdeps\icu4c\android
call build.bat
chdir /d %OLDDIR%

cd localdeps\libxml2\android
call build.bat
chdir /d %OLDDIR%

cd localdeps\tidy-html5\android
call build.bat
chdir /d %OLDDIR%

RMDIR "mailcore-deps" /S /Q
mkdir mailcore-deps
mkdir mailcore-deps\include
mkdir mailcore-deps\include\ctemplate
mkdir mailcore-deps\include\icu4c
mkdir mailcore-deps\include\icu4c\unicode
mkdir mailcore-deps\include\icu4c\i18n
mkdir mailcore-deps\include\icu4c\i18n\unicode
mkdir mailcore-deps\include\libxml
mkdir mailcore-deps\include\tidy-html5

mkdir mailcore-deps\libs
mkdir mailcore-deps\libs\arm64-v8a
mkdir mailcore-deps\libs\armeabi-v7a
mkdir mailcore-deps\libs\x86
mkdir mailcore-deps\libs\x86_64

copy localdeps\ctemplate\android\artifacts\arm64-v8a\libctemplate.a .\mailcore-deps\libs\arm64-v8a
copy localdeps\ctemplate\android\artifacts\armeabi-v7a\libctemplate.a .\mailcore-deps\libs\armeabi-v7a
copy localdeps\ctemplate\android\artifacts\x86\libctemplate.a .\mailcore-deps\libs\x86
copy localdeps\ctemplate\android\artifacts\x86_64\libctemplate.a .\mailcore-deps\libs\x86_64

copy localdeps\icu4c\android\artifacts\arm64-v8a\libicui18n_static.a .\mailcore-deps\libs\arm64-v8a
copy localdeps\icu4c\android\artifacts\armeabi-v7a\libicui18n_static.a .\mailcore-deps\libs\armeabi-v7a
copy localdeps\icu4c\android\artifacts\x86\libicui18n_static.a .\mailcore-deps\libs\x86
copy localdeps\icu4c\android\artifacts\x86_64\libicui18n_static.a .\mailcore-deps\libs\x86_64
copy localdeps\icu4c\android\artifacts\arm64-v8a\libicuuc_static.a .\mailcore-deps\libs\arm64-v8a
copy localdeps\icu4c\android\artifacts\armeabi-v7a\libicuuc_static.a .\mailcore-deps\libs\armeabi-v7a
copy localdeps\icu4c\android\artifacts\x86\libicuuc_static.a .\mailcore-deps\libs\x86
copy localdeps\icu4c\android\artifacts\x86_64\libicuuc_static.a .\mailcore-deps\libs\x86_64

copy localdeps\libxml2\android\artifacts\arm64-v8a\libxml2.a .\mailcore-deps\libs\arm64-v8a
copy localdeps\libxml2\android\artifacts\armeabi-v7a\libxml2.a .\mailcore-deps\libs\armeabi-v7a
copy localdeps\libxml2\android\artifacts\x86\libxml2.a .\mailcore-deps\libs\x86
copy localdeps\libxml2\android\artifacts\x86_64\libxml2.a .\mailcore-deps\libs\x86_64

copy localdeps\tidy-html5\android\artifacts\arm64-v8a\libtidy-html5.a .\mailcore-deps\libs\arm64-v8a
copy localdeps\tidy-html5\android\artifacts\armeabi-v7a\libtidy-html5.a .\mailcore-deps\libs\armeabi-v7a
copy localdeps\tidy-html5\android\artifacts\x86\libtidy-html5.a .\mailcore-deps\libs\x86
copy localdeps\tidy-html5\android\artifacts\x86_64\libtidy-html5.a .\mailcore-deps\libs\x86_64

copy localdeps\ctemplate\src\ctemplate\*.h mailcore-deps\include\ctemplate
copy localdeps\icu4c\source\common\unicode\*.h mailcore-deps\include\icu4c\unicode
copy localdeps\icu4c\source\i18n\unicode\*.h mailcore-deps\include\icu4c\i18n\unicode
copy localdeps\libxml2\include\libxml\*.h mailcore-deps\include\libxml
copy localdeps\tidy-html5\include\*.h mailcore-deps\include\tidy-html5

call %LocalAppData%\Android\Sdk\ndk-bundle\ndk-build.cmd -C . NDK_DEBUG=0 DIR=%CD% APP_PLATFORM=android-21 NDKLEVEL=21 ANDROID_NDK=%LocalAppData%\Android\Sdk\ndk-bundle

REM copy obj\local\arm64-v8a\*.a mailcore-deps\libs\arm64-v8a
REM copy obj\local\armeabi-v7a\*.a mailcore-deps\libs\armeabi-v7a
REM copy obj\local\x86\*.a mailcore-deps\libs\x86
REM copy obj\local\x86_64\*.a mailcore-deps\libs\x86_64

