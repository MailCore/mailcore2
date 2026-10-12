# MailCore2 Android example (Kotlin)

A minimal modern Kotlin app that integrates **mailcore2** via the prebuilt
`mailcore2-android-4.aar` and demonstrates a real IMAP sync: connect over TLS,
list folders, and fetch the headers of the most recent INBOX messages.

## Layout
- `app/libs/mailcore2-android-4.aar` — the mailcore2 library (built from `build-android/`).
- `app/src/main/java/com/libmailcore/androidexample/MainActivity.kt` — all the mailcore usage.

## Toolchain
- Gradle 8.7, Android Gradle Plugin 8.5.2, Kotlin 1.9.24, JDK 17.
- `compileSdk 34`, `targetSdk 34`, **`minSdk 23`** (the .aar's native deps are built at API 23).
- ABIs: `armeabi-v7a`, `arm64-v8a`, `x86` (the .aar ships these).

## Build & run
```sh
# point Gradle at your SDK (or rely on $ANDROID_HOME)
echo "sdk.dir=$HOME/Library/Android/sdk" > local.properties

./gradlew :app:assembleDebug
./gradlew :app:installDebug      # with a device/emulator attached
```
Then open the app, edit the host/username/password fields, and tap **Connect & sync**.
The credentials are pre-filled for convenience — replace them with your own.

For headless/automated runs the sync can be triggered without a tap:
```sh
adb shell am start -n com.libmailcore.androidexample/.MainActivity --ez autostart true
adb logcat -s MailCoreExample      # results are mirrored to logcat
```

## Integrating mailcore2 in your own project
1. Drop `mailcore2-android-4.aar` into your module's `libs/` and add:
   ```kotlin
   implementation(files("libs/mailcore2-android-4.aar"))
   ```
2. Use `minSdk 23` or higher.
3. Drive IMAP through `com.libmailcore.IMAPSession` and its `*Operation` objects,
   each started with an `OperationCallback` (`succeeded()` / `failed()`); callbacks
   are delivered on the main thread. See `MainActivity.kt`.

Note: `IMAPSession.setConnectionLogger(...)` is currently broken in the Android
binding (native/Java field-type mismatch) and will abort — avoid it.
