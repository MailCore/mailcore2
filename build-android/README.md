### Binary ###

Download the latest [build for Android](http://d.etpan.org/mailcore2-deps/mailcore2-android/)

### Build on Mac ###
Tested with macOS 10.13.6, Android Studio 3.1.4

Install Java 6, 8, or 10
* http://www.oracle.com/technetwork/java/javase/downloads/jdk10-downloads-4416644.html
* Building with Java 8 or 10 might throw a few warnings during the build, but it will still build

Download the Required Android SDKs (16, 21, 22)
* Download using Android Studio

Download the Required NDK (13)
* https://dl.google.com/android/repository/android-ndk-r13b-darwin-x86_64.zip

Open terminal and set two temporary export variables
* (Make sure they point to your SDK and NDK folders)
```
export ANDROID_SDK=/Users/YOURNAME/Library/Android/sdk
export ANDROID_NDK=/Users/YOURNAME/Downloads/android-ndk-r13b
```

Clone the repo and goto the `src` folder
```
git clone https://github.com/MailCore/mailcore2.git
cd mailcore2
cd src
```

Copy the headers into the build folder
```
mkdir -p ../build-android/include/MailCore
cp `find . -name '*.h'` ../build-android/include/MailCore/
```

Run the build script
```
cd ../build-android
./build.sh
```

It will generate `mailcore2-android-*version*.aar`

To modify the version number, change the `build_version` in `build.sh` and rerun `./build.sh` (make sure you set your export variables, as listed above)

### Running example ###

Copy the binary result of the build (mailcore2-android-*version*.aar) to `mailcore2/example/android/AndroidExample/app/libs`.

- Open the example in Android Studio
- Tweaks the login and password in the class `MessagesSyncManager`
- Then, run it.
