# Binary

Download the latest [build for Android](http://d.etpan.org/mailcore2-deps/mailcore2-android/) from 2016. Or build mailcore on Android:

# Build for Android

The below instruction allows to build mailcore with its needed dependencies on macOS (when followed exactly in this order).

## Compile libetpan
- Clone https://github.com/dinhvh/libetpan/releases/tag/1.9.4 in version 1.9.4 (which fixes exception in QUOTA operation): Make sure in this libetpan path are not spaces, this causes random compile errors. Folder should be named `libetpan-master` after cloning.
- Download Android NDK 17
- Download Android SDK 16 and SDK 21
- Set ENV-vars to your NDK/SDK root folder
- Set ENV-vars to your NDK/SDK root folder
```
export ANDROID_NDK=/Users/xxx/Library/Android/sdk/ndk/17.2.4988734
export ANDROID_SDK=/Users/xxx/Library/Android/sdk
```

1. Compile openssl:
  - `cd build-android/dependencies/openssl`
  - build.sh: Change version in line 4 to most recent 1.0.2xxx, see https://ftp.openssl.org/source/old/1.0.2/. e.g. "1.0.2u"
  - `./build.sh` (Ignore that we get several errors regarding "cryptlib.h:62:21: fatal error: stdlib.h: No such file or directory" / "cp: libcrypto.a: No such file or directory" / "cp: libssl.a: No such file or directory". If the generated openssl-android-3.zip has e.g. 408KB and contains some h-files (but no so files), we are fine.)

2. Compile iconv (probably not needed when removing "-DHAVE_ICONV=1" later)
  - `cd build-android/dependencies/iconv`
  - `./build.sh`

3. Compile cyrus-sasl
  - cd build-mac/dependencies
  - prepare-cyrus-sasl.sh: Change version in line 5 to version=2.1.26 (to match version with build-android/dependencies/cyrus-sasl/build.sh)
  - `./prepare-cyrus-sasl.sh` (can be quitted after "building tools" is printed)
  - `cd build-android/dependencies/cyrus-sasl`
  - `./build.sh`

4. Compile libetpan
  - `./autogen && make` to generate libetpan-config.h (Ignore that we error at make, we just need the libetpan-config.h file)
  - `cd build-android`
  - jni/Android.mk: Remove "-DHAVE_ICONV=1" in LOCAL_CFLAGS (line 131) (don't set it to 0 but remove it altogether) to build without iconv
  - `./build.sh`

## Compile libmailcore
- Clone https://github.com/MailCore/mailcore2 (commit fad23d736ed5a63cf8321469d3a98a583f55df97 works definitely with this instruction): Make sure in this mailcore2 path are not spaces, this causes random compile errors. Folder should be named `libmailcore2-master` after cloning and should be next to the `libetpan-master` folder.
- Insert in src/core/sercurity/MCCeritificateUtils.cpp before line `status = X509_verify_cert(storectx);` the following statement to ensure LetsEncrypt compatibiltiy: `X509_STORE_CTX_set_flags(storectx, X509_V_FLAG_TRUSTED_FIRST);`, see https://www.openssl.org/blog/blog/2021/09/13/LetsEncryptRootCertExpire/ and https://community.letsencrypt.org/t/openssl-client-compatibility-changes-for-let-s-encrypt-certificates/143816
- `cd build-android`
- build.sh: Change libetpan version to 7
- build.sh: Change `-source 1.6 -target 1.6` to `-source 1.8 -target 1.8`
- `mkdir third-party`
- Copy libetpan-android-7.zip (from step "Compile libetpan") to third-party folder and extract it
- `mkdir include/MailCore`
- `cd ../src`
- ``cp `find . -name '*.h'` ../build-android/include/MailCore/``
- `cd ../../libetpan-master/build-android`
- ``cp `find . -name '*.h'` ../../mailcore2-master/build-android/include/MailCore``
- `cd ../../mailcore2-master/build-android`
- `./build.sh`
- Make sure that the above command does not contain any `error:` output

## Shrink mailcore (by removing unnecessary files to reduce 50% of file size)
- cd to the directory where `mailcore2-android-4.aar` is located
- Execute
```
unzip mailcore2-android-4.aar -d mailcore-unzipped
rm mailcore2-android-4.aar
cd mailcore-unzipped
unzip classes.jar -d classes-unzipped
rm -rf classes-unzipped/jni
rm classes.jar
jar cvf classes.jar -C classes-unzipped/ .
rm -rf classes-unzipped
cd ..
jar cvf mailcore2-android-4.aar -C mailcore-unzipped/ .
rm -rf mailcore-unzipped
```
- Copy shrinked `mailcore2-android-4.aar` to android project and test it on real devices

## Troubleshooting
- Don't download libetpan / libmailcore as zip, but `git checkout` them. Then seeing all your modifications is way easier when opening both folders in CLion.
- Openssl can't be downloaded: adapt letter in version based on latest release in https://ftp.openssl.org/source/ or https://ftp.openssl.org/source/old/1.0.2/
- Compile openssl 1.0.2u with NDK 17 and libsasl 2.1.26
- Compile openssl 1.1.1k with NDK 20 and libsasl 2.1.27
- openssl compile error "aarch64-linux-android-gcc: No such file or directory": Make sure to use correct NDK
- Android Looper/Handler or `android.os` package not found: Make sure to install all needed Android SDK versions in Android Studio
- "Missing archive cyrus-sasl-2.1.26": Make sure to run `prepare-cyrus-sasl.sh` first
- Build scripts don't always exit on errors: Manually search for "error:" or other errors
- Missing gnustl_shared: Install & use Android NDK 17 (version 18 removed some GCC stuff)
- Error while building libmailcore that some header-files are not found: You forgot to copy libmailcore header files see https://stackoverflow.com/a/31534327/3997741 or libetpan header files (see cp commands above)
- 'libetpan-config.h' file not found: Run `./autogen.sh && make` first
- sasl/sasl.h not found: `cd build-mac/dependencies && ./prepare-cyrus-sasl.sh`
- "Configuring OpenSSL version" missing: make sure to use current 1.1.1xxx/1.0.2xxx openssl version
- "no sysroot" while building openssl: use Android NDK 20 instead of 17
- error: variable has incomplete type 'HMAC_CTX' (aka 'struct hmac_ctx_st'): use sasl 2.1.27 instead of 2.1.26 in build-mac/dependencies/prepare-cyrus-sasl.sh and build-android/dependencies/cyrus-sasl/build.sh
- Buildscripts can't run: Call them with `bash build.sh` (even when starting with "#! /bin/sh")
- iconv stuff not found when linking: you missed the step to remove "-DHAVE_ICONV=1"

### Running example ###

Copy the binary result of the build (mailcore2-android-*version*.aar) to `mailcore2-master/example/android/AndroidExample/app/libs`.

- Open the example in Android Studio
- Tweaks the login and password in the class `MessagesSyncManager`
- Then, run it.
