# Modernized: use the LLVM libc++ STL (gnustl / GCC were removed in NDK r18).
# c++_static: libMailCore.so is the only native lib in the .aar and exposes a JNI/Java
# boundary (no C++ types cross to other libs), so static libc++ is safe and avoids
# shipping a separate libc++_shared.so — which matters for 16KB alignment (below).
APP_STL := c++_static
APP_CPPFLAGS := -frtti
# android-23: the prebuilt deps (openssl/iconv/cyrus-sasl) are built at API 23, where
# Bionic exports stdin/stderr/stdout as real symbols. Linking at a lower API fails with
# "undefined symbol: stdin/stderr".
APP_PLATFORM := android-23
# 16KB page alignment: required to load on 16KB-page devices (Pixel 8/9, Android 15+)
# and by Google Play for new/updated apps from Nov 2025. NDK 26.3 defaults to 4KB.
APP_LDFLAGS := -Wl,-z,max-page-size=16384
