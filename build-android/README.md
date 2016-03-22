### Binary ###

Download the latest [build for Android](http://d.etpan.org/mailcore2-deps/mailcore2-android/)

### Build for Android ###

```
$ export ANDROID_SDK=/path/to/android-sdk
$ export ANDROID_NDK=/path/to/android-ndk
$ cd build-android
$ ./build.sh
```

It will produce the following binaries:

- mailcore2-android-*version*.aar

### Running example ###

Copy the binary result of the build (mailcore2-android-*version*.aar) to `mailcore2/example/android/AndroidExample/app/libs`.

- Open the example in Android Studio
- Tweaks the login and password in the class `MessagesSyncManager`
- Then, run it.
