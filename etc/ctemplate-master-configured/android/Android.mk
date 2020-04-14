LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := ctemplate

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../src

FILE_LIST := $(wildcard $(LOCAL_PATH)/../src/base/arena.cc)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/per_expand_data.cc)

FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/template.cc)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/template_annotator.cc)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/template_cache.cc)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/template_dictionary.cc)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/template_modifiers.cc)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/template_namelist.cc)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/template_pathops.cc)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/template_string.cc)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/htmlparser/htmlparser.cc)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/htmlparser/jsparser.cc)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/htmlparser/statemachine.cc)

FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/make_tpl_varnames_h.cc)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/diff_tpl_auto_escape.cc)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/template_dictionary.cc)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/htmlparser/statemachine.cc)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

TARGET_OUT=$(LOCAL_PATH)/artifacts/$(TARGET_ARCH_ABI)

include $(BUILD_STATIC_LIBRARY)


    