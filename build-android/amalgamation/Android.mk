AMALG_PATH := $(call my-dir)

OPENSSL_PATH = $(AMALG_PATH)/../../ExternalsSrc/libetpan/build-android/localdeps/openssl
CYRUS_SASL_PATH = $(AMALG_PATH)/../../ExternalsSrc/libetpan/build-android/localdeps/cyrus-sasl
ICONV_PATH = $(AMALG_PATH)/../../ExternalsSrc/libetpan/build-android/localdeps/iconv

include $(AMALG_PATH)/../../ExternalsSrc/libetpan/build-android/localdeps/cyrus-sasl/android/Android.mk
include $(AMALG_PATH)/../../ExternalsSrc/libetpan/build-android/localdeps/iconv/android/Android.mk
include $(AMALG_PATH)/../../ExternalsSrc/libetpan/build-android/jni/Android.mk

include $(AMALG_PATH)/../localdeps/ctemplate/android/Android.mk
include $(AMALG_PATH)/../localdeps/icu4c/android/Android.mk
include $(AMALG_PATH)/../localdeps/libxml2/android/Android.mk
include $(AMALG_PATH)/../localdeps/tidy-html5/android/Android.mk

