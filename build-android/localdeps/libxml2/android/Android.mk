LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libxml2

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../src $(LOCAL_PATH)/../include $(LOCAL_PATH)/../../icu4c/source/common

FILE_LIST := $(wildcard $(LOCAL_PATH)/../src/access.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../src/parser.c)

FILE_LIST := ../src/SAX.c
FILE_LIST += ../src/entities.c
FILE_LIST += ../src/encoding.c
FILE_LIST += ../src/error.c
FILE_LIST += ../src/parserInternals.c
FILE_LIST += ../src/parser.c
FILE_LIST += ../src/tree.c
FILE_LIST += ../src/hash.c
FILE_LIST += ../src/list.c
FILE_LIST += ../src/xmlIO.c
FILE_LIST += ../src/xmlmemory.c
FILE_LIST += ../src/uri.c
FILE_LIST += ../src/valid.c
FILE_LIST += ../src/xlink.c
FILE_LIST += ../src/debugXML.c
FILE_LIST += ../src/xpath.c
FILE_LIST += ../src/xpointer.c
FILE_LIST += ../src/xinclude.c
FILE_LIST += ../src/DOCBparser.c
FILE_LIST += ../src/catalog.c
FILE_LIST += ../src/globals.c
FILE_LIST += ../src/threads.c
FILE_LIST += ../src/c14n.c
FILE_LIST += ../src/xmlstring.c
FILE_LIST += ../src/buf.c
FILE_LIST += ../src/xmlregexp.c
FILE_LIST += ../src/xmlschemas.c
FILE_LIST += ../src/xmlschemastypes.c
FILE_LIST += ../src/xmlunicode.c
FILE_LIST += ../src/xmlreader.c
FILE_LIST += ../src/relaxng.c
FILE_LIST += ../src/dict.c
FILE_LIST += ../src/SAX2.c
FILE_LIST += ../src/xmlwriter.c
FILE_LIST += ../src/legacy.c
FILE_LIST += ../src/chvalid.c
FILE_LIST += ../src/pattern.c
FILE_LIST += ../src/xmlsave.c
FILE_LIST += ../src/xmlmodule.c
FILE_LIST += ../src/schematron.c
FILE_LIST += ../src/HTMLparser.c
FILE_LIST += ../src/HTMLtree.c

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

TARGET_OUT=$(LOCAL_PATH)/artifacts/$(TARGET_ARCH_ABI)

include $(BUILD_STATIC_LIBRARY)


    