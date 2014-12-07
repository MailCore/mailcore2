CURRENT_DIR := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := MailCore

src_dir := $(CURRENT_DIR)/../../src
subdirs = \
	core \
	core/basetypes \
	core/abstract \
	core/imap \
	core/nntp \
	core/pop \
	core/provider \
	core/renderer \
	core/rfc822 \
	core/security \
	core/smtp \
	core/zip \
	core/zip/MiniZip
LOCAL_C_INCLUDES := \
	$(CURRENT_DIR)/../include \
    $(CTEMPLATE_PATH)/include \
    $(ICU4C_PATH)/include \
    $(LIBETPAN_PATH)/include \
    $(LIBXML2_PATH)/include \
    $(UCHARDET_PATH)/include \
    $(TIDY_HTML5_PATH)/include \
    $(OPENSSL_PATH)/include \
    $(ANDROID_NDK)/sources/cxx-stl/llvm-libc++/libcxx/include \
	$(ANDROID_NDK)/sources/cxx-stl/llvm-libc++abi/libcxxabi/include \
	$(addprefix $(src_dir)/, $(subdirs))
core_excludes = MCWin32.cpp MCStringWin32.cpp MCMainThreadWin32.cpp MCMainThreadGTK.cpp
core_src_files := $(filter-out \
	$(addprefix $(src_dir)/core/basetypes/, $(core_excludes)), \
	$(wildcard $(src_dir)/core/basetypes/*.cpp) $(wildcard $(src_dir)/core/basetypes/*.c))
abstract_src_files := $(wildcard $(src_dir)/core/abstract/*.cpp)
imap_src_files := $(wildcard $(src_dir)/core/imap/*.cpp)
nntp_src_files := $(wildcard $(src_dir)/core/nntp/*.cpp)
pop_src_files := $(wildcard $(src_dir)/core/pop/*.cpp)
provider_src_files := $(wildcard $(src_dir)/core/provider/*.cpp)
renderer_src_files := $(wildcard $(src_dir)/core/renderer/*.cpp)
rfc822_src_files := $(wildcard $(src_dir)/core/rfc822/*.cpp)
security_src_files := $(wildcard $(src_dir)/core/security/*.cpp)
smtp_src_files := $(wildcard $(src_dir)/core/smtp/*.cpp)
zip_src_files := $(wildcard $(src_dir)/core/zip/*.cpp)
minizip_src_files := \
	$(src_dir)/core/zip/MiniZip/ioapi.c \
	$(src_dir)/core/zip/MiniZip/unzip.c \
	$(src_dir)/core/zip/MiniZip/zip.c
async_imap_src_files := $(wildcard $(src_dir)/async/imap/*.cpp)
async_nntp_src_files := $(wildcard $(src_dir)/async/nntp/*.cpp)
async_pop_src_files := $(wildcard $(src_dir)/async/pop/*.cpp)
async_smtp_src_files := $(wildcard $(src_dir)/async/smtp/*.cpp)

LOCAL_SRC_FILES := $(core_src_files) $(abstract_src_files) $(imap_src_files) $(nntp_src_files) \
	$(pop_src_files) $(provider_src_files) $(renderer_src_files) $(rfc822_src_files) \
	$(security_src_files) $(smtp_src_files) $(zip_src_files) $(minizip_src_files) \
	$(async_imap_src_files) $(async_nntp_src_files) $(async_pop_src_files) $(async_smtp_src_files)
LOCAL_CPPFLAGS := -frtti
LOCAL_CFLAGS := -DNOCRYPT

include $(BUILD_STATIC_LIBRARY)
