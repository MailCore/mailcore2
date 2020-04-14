LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := tidy-html5

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../src $(LOCAL_PATH)/../include

FILE_LIST := $(wildcard $(LOCAL_PATH)/../src/access.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/parser.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/lexer.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/attrs.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/istack.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/tags.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/entities.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/pprint.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/charsets.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/clean.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/message.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/config.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/alloc.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/attrdict.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/buffio.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/fileio.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/streamio.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/tagask.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/tmbstr.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/utf8.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/tidylib.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/mappedio.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/gdoc.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/language.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/messageobj.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/sprtf.c)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

TARGET_OUT=$(LOCAL_PATH)/artifacts/$(TARGET_ARCH_ABI)

include $(BUILD_STATIC_LIBRARY)


    