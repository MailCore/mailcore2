# Building MailCore2 for Android

This builds MailCore2 into `mailcore2-android-4.aar` for **NDK 26 / libc++**
(the original 2016 gnustl/NDK-17 chain is gone). The result is a single
self-contained, 16 KB-page-aligned `libMailCore.so` per ABI plus the Java
`com.libmailcore` API.

There is no maintained prebuilt to download — build your own as below.

## Prerequisites

- **Android NDK r26.3+** (r23+ is required on Apple-Silicon Mac hosts). Tested
  with `26.3.11579264`. Set `ANDROID_NDK` to it.
- **Android SDK** with platform **android-16** (used only by the `javac`
  classpath step) and build-tools. Set `ANDROID_SDK`.
- **CMake** (generates the `MailCore/*.h` umbrella headers) and **JDK 17**.
- For the ctemplate build (step 1b): **Python 3** (with `2to3`) and `curl`.

```sh
export ANDROID_NDK=$HOME/Library/Android/sdk/ndk/26.3.11579264
export ANDROID_SDK=$HOME/Library/Android/sdk
```

## Step 1 — build the native dependencies

MailCore links eight native libraries. Five must be **rebuilt** so they match
NDK 26 / libc++ / OpenSSL 1.1.x; three legacy C libraries are still fetched from
`d.etpan.org` automatically and link fine as-is.

**1a. From the libetpan repo** — `libetpan` plus the deps it links. Build them
once in the [libetpan `build-android`](https://github.com/dinhvh/libetpan)
(see its README); reusing libetpan's exact OpenSSL/SASL/iconv keeps the ABI
consistent:

| Zip | Version | Built by |
|-----|---------|----------|
| `libetpan-android-7.zip`     | 1.9.x          | `libetpan/build-android/build.sh` |
| `openssl-android-3.zip`      | OpenSSL 1.1.1w | ↑ (same run) |
| `cyrus-sasl-android-4.zip`   | 2.1.28         | ↑ (same run) |
| `iconv-android-1.zip`        | libiconv 1.15  | ↑ (same run) |

**1b. ctemplate — built here** (it is a MailCore-only dependency; libetpan does
not use it):

```sh
export ANDROID_NDK=$HOME/Library/Android/sdk/ndk/26.3.11579264
cd build-android/dependencies/ctemplate
./build.sh        # → ctemplate-android-3.zip   (needs Python 3 for the fsm headers)
```

> The `d.etpan.org` copies of `ctemplate`, `openssl`, `cyrus-sasl` and
> `libetpan` are the **old gnustl / OpenSSL-1.0.2 builds and will not link** — you
> must supply the rebuilt zips above. `icu4c-android-3`, `libxml2-android-3` and
> `tidy-html5-android-3` are plain C/C++ libs that still work and are downloaded
> for you.

## Step 2 — stage the dependencies

Put all five rebuilt zips into `build-android/third-party/` and unzip them (so
the extracted directories exist before the build runs) — the four from libetpan
plus the `ctemplate-android-3.zip` produced in step 1b:

```sh
cd build-android
mkdir -p third-party
cp dependencies/ctemplate/ctemplate-android-3.zip third-party/
cp /path/to/libetpan/build-android/{libetpan-android-7,\
openssl-android-3,cyrus-sasl-android-4}.zip third-party/
cp /path/to/libetpan/build-android/dependencies/iconv/iconv-android-1.zip third-party/
cd third-party && for z in *.zip ; do unzip -oq "$z" ; done && cd ..
```

`build.sh` skips any dependency whose directory already exists, so it will only
download the three legacy C libs (`icu4c`, `libxml2`, `tidy`).

## Step 3 — build the AAR

```sh
cd build-android
./build.sh
```

Output: `build-android/mailcore2-android-4.aar`, containing
`libMailCore.so` for **armeabi-v7a, arm64-v8a, x86**.

Key build settings (`jni/Application.mk`):
- `APP_STL := c++_static` — libc++ is linked into `libMailCore.so`; there is no
  separate `libc++_shared.so`.
- `APP_PLATFORM := android-23` — the rebuilt deps reference Bionic's API-23
  symbols, so **minSdk 23 is the floor**.
- `-Wl,-z,max-page-size=16384` — 16 KB-page alignment (Pixel 8/9 / Android 15+,
  and a Google Play requirement from Nov 2025).

## Using the AAR

Drop it into an app module and depend on it directly:

```kotlin
// app/build.gradle.kts
android { defaultConfig { minSdk = 23 } }   // >= 23
dependencies { implementation(files("libs/mailcore2-android-4.aar")) }
```

A complete, runnable Kotlin sample (IMAP connect / login / list folders +
recent messages) is in [`../example/android/AndroidExample`](../example/android/AndroidExample).

### Optional: shrink the AAR

The `.aar` bundles the JNI `.so` per ABI. To strip the Java-side `jni/` copy and
keep only what an app needs:

```sh
unzip mailcore2-android-4.aar -d unzipped && rm mailcore2-android-4.aar
cd unzipped && unzip classes.jar -d classes && rm -rf classes/jni classes.jar
jar cf classes.jar -C classes . && rm -rf classes && cd ..
jar cf mailcore2-android-4.aar -C unzipped . && rm -rf unzipped
```

## Notes

- **OpenSSL 1.1.1w is end-of-life (since Sept 2023).** Fine for internal use;
  plan an OpenSSL 3.x dependency rebuild before depending on this long-term.
- `IMAPSession.setConnectionLogger()` is broken in the Android binding (native
  reads the `connectionLogger` field as a `long`, but it is an object) — avoid it.
