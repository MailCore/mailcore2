LOCAL_PATH:= $(call my-dir)

ICU_COMMON_PATH := ../source/common
ICU_I18N_PATH := ../source/i18n
ICU_STUBDATA_PATH := ../source/stubdata

#------------
# start icuuc
#------------

src_files := \
	$(ICU_COMMON_PATH)/cmemory.c          $(ICU_COMMON_PATH)/cstring.c          \
	$(ICU_COMMON_PATH)/cwchar.c           $(ICU_COMMON_PATH)/locmap.c           \
	$(ICU_COMMON_PATH)/lrucache.cpp \
	$(ICU_COMMON_PATH)/punycode.cpp       $(ICU_COMMON_PATH)/putil.cpp          \
	$(ICU_COMMON_PATH)/sharedobject.cpp \
	$(ICU_COMMON_PATH)/simplepatternformatter.cpp \
	$(ICU_COMMON_PATH)/uarrsort.c         $(ICU_COMMON_PATH)/ubidi.c            \
	$(ICU_COMMON_PATH)/ubidiln.c          $(ICU_COMMON_PATH)/ubidi_props.c      \
	$(ICU_COMMON_PATH)/ubidiwrt.c         $(ICU_COMMON_PATH)/ucase.cpp          \
	$(ICU_COMMON_PATH)/ucasemap.cpp       $(ICU_COMMON_PATH)/ucat.c             \
	$(ICU_COMMON_PATH)/uchar.c            $(ICU_COMMON_PATH)/ucln_cmn.c         \
	$(ICU_COMMON_PATH)/ucmndata.c                            \
	$(ICU_COMMON_PATH)/ucnv2022.cpp       $(ICU_COMMON_PATH)/ucnv_bld.cpp       \
	$(ICU_COMMON_PATH)/ucnvbocu.cpp       $(ICU_COMMON_PATH)/ucnv.c             \
	$(ICU_COMMON_PATH)/ucnv_cb.c          $(ICU_COMMON_PATH)/ucnv_cnv.c         \
	$(ICU_COMMON_PATH)/ucnvdisp.c         $(ICU_COMMON_PATH)/ucnv_err.c         \
	$(ICU_COMMON_PATH)/ucnv_ext.cpp       $(ICU_COMMON_PATH)/ucnvhz.c           \
	$(ICU_COMMON_PATH)/ucnv_io.cpp        $(ICU_COMMON_PATH)/ucnvisci.c         \
	$(ICU_COMMON_PATH)/ucnvlat1.c         $(ICU_COMMON_PATH)/ucnv_lmb.c         \
	$(ICU_COMMON_PATH)/ucnvmbcs.c         $(ICU_COMMON_PATH)/ucnvscsu.c         \
	$(ICU_COMMON_PATH)/ucnv_set.c         $(ICU_COMMON_PATH)/ucnv_u16.c         \
	$(ICU_COMMON_PATH)/ucnv_u32.c         $(ICU_COMMON_PATH)/ucnv_u7.c          \
	$(ICU_COMMON_PATH)/ucnv_u8.c                             \
	$(ICU_COMMON_PATH)/udatamem.c         \
	$(ICU_COMMON_PATH)/udataswp.c         $(ICU_COMMON_PATH)/uenum.c            \
	$(ICU_COMMON_PATH)/uhash.c            $(ICU_COMMON_PATH)/uinit.cpp          \
	$(ICU_COMMON_PATH)/uinvchar.c         $(ICU_COMMON_PATH)/uloc.cpp           \
	$(ICU_COMMON_PATH)/umapfile.c         $(ICU_COMMON_PATH)/umath.c            \
	$(ICU_COMMON_PATH)/umutex.cpp         $(ICU_COMMON_PATH)/unames.cpp         \
	$(ICU_COMMON_PATH)/uresbund.cpp       \
	$(ICU_COMMON_PATH)/ures_cnv.c         $(ICU_COMMON_PATH)/uresdata.c         \
	$(ICU_COMMON_PATH)/usc_impl.c         $(ICU_COMMON_PATH)/uscript.c          \
	$(ICU_COMMON_PATH)/uscript_props.cpp  \
	$(ICU_COMMON_PATH)/ushape.cpp         $(ICU_COMMON_PATH)/ustrcase.cpp       \
	$(ICU_COMMON_PATH)/ustr_cnv.c         $(ICU_COMMON_PATH)/ustrfmt.c          \
	$(ICU_COMMON_PATH)/ustring.cpp        $(ICU_COMMON_PATH)/ustrtrns.cpp       \
	$(ICU_COMMON_PATH)/ustr_wcs.cpp       $(ICU_COMMON_PATH)/utf_impl.c         \
	$(ICU_COMMON_PATH)/utrace.c           $(ICU_COMMON_PATH)/utrie.cpp          \
	$(ICU_COMMON_PATH)/utypes.c           $(ICU_COMMON_PATH)/wintz.c            \
	$(ICU_COMMON_PATH)/utrie2_builder.cpp $(ICU_COMMON_PATH)/icuplug.c          \
	$(ICU_COMMON_PATH)/propsvec.c         $(ICU_COMMON_PATH)/ulist.c            \
	$(ICU_COMMON_PATH)/uloc_tag.c         $(ICU_COMMON_PATH)/ucnv_ct.c

src_files += \
    $(ICU_COMMON_PATH)/bmpset.cpp	$(ICU_COMMON_PATH)/unisetspan.cpp   \
	$(ICU_COMMON_PATH)/brkeng.cpp      $(ICU_COMMON_PATH)/brkiter.cpp      \
	$(ICU_COMMON_PATH)/caniter.cpp     $(ICU_COMMON_PATH)/chariter.cpp     \
	$(ICU_COMMON_PATH)/dictbe.cpp	$(ICU_COMMON_PATH)/locbased.cpp     \
	$(ICU_COMMON_PATH)/locid.cpp       $(ICU_COMMON_PATH)/locutil.cpp      \
	$(ICU_COMMON_PATH)/normlzr.cpp     $(ICU_COMMON_PATH)/parsepos.cpp     \
	$(ICU_COMMON_PATH)/propname.cpp    $(ICU_COMMON_PATH)/rbbi.cpp         \
	$(ICU_COMMON_PATH)/rbbidata.cpp    $(ICU_COMMON_PATH)/rbbinode.cpp     \
	$(ICU_COMMON_PATH)/rbbirb.cpp      $(ICU_COMMON_PATH)/rbbiscan.cpp     \
	$(ICU_COMMON_PATH)/rbbisetb.cpp    $(ICU_COMMON_PATH)/rbbistbl.cpp     \
	$(ICU_COMMON_PATH)/rbbitblb.cpp    $(ICU_COMMON_PATH)/resbund_cnv.cpp  \
	$(ICU_COMMON_PATH)/resbund.cpp     $(ICU_COMMON_PATH)/ruleiter.cpp     \
	$(ICU_COMMON_PATH)/schriter.cpp    $(ICU_COMMON_PATH)/serv.cpp         \
	$(ICU_COMMON_PATH)/servlk.cpp      $(ICU_COMMON_PATH)/servlkf.cpp      \
	$(ICU_COMMON_PATH)/servls.cpp      $(ICU_COMMON_PATH)/servnotf.cpp     \
	$(ICU_COMMON_PATH)/servrbf.cpp     $(ICU_COMMON_PATH)/servslkf.cpp     \
	$(ICU_COMMON_PATH)/ubrk.cpp         \
	$(ICU_COMMON_PATH)/uchriter.cpp    $(ICU_COMMON_PATH)/uhash_us.cpp     \
	$(ICU_COMMON_PATH)/uidna.cpp       $(ICU_COMMON_PATH)/uiter.cpp        \
	$(ICU_COMMON_PATH)/unifilt.cpp     $(ICU_COMMON_PATH)/unifunct.cpp     \
	$(ICU_COMMON_PATH)/uniset.cpp      $(ICU_COMMON_PATH)/uniset_props.cpp \
	$(ICU_COMMON_PATH)/unistr_case.cpp $(ICU_COMMON_PATH)/unistr_cnv.cpp   \
	$(ICU_COMMON_PATH)/unistr.cpp      $(ICU_COMMON_PATH)/unistr_props.cpp \
	$(ICU_COMMON_PATH)/unormcmp.cpp    $(ICU_COMMON_PATH)/unorm.cpp        \
	$(ICU_COMMON_PATH)/uobject.cpp     $(ICU_COMMON_PATH)/uset.cpp         \
	$(ICU_COMMON_PATH)/usetiter.cpp    $(ICU_COMMON_PATH)/uset_props.cpp   \
	$(ICU_COMMON_PATH)/usprep.cpp      $(ICU_COMMON_PATH)/ustack.cpp       \
	$(ICU_COMMON_PATH)/ustrenum.cpp    $(ICU_COMMON_PATH)/utext.cpp        \
	$(ICU_COMMON_PATH)/util.cpp        $(ICU_COMMON_PATH)/util_props.cpp   \
	$(ICU_COMMON_PATH)/uvector.cpp     $(ICU_COMMON_PATH)/uvectr32.cpp     \
	$(ICU_COMMON_PATH)/errorcode.cpp                    \
	$(ICU_COMMON_PATH)/bytestream.cpp  $(ICU_COMMON_PATH)/stringpiece.cpp  \
	$(ICU_COMMON_PATH)/dtintrv.cpp      \
	$(ICU_COMMON_PATH)/ucnvsel.cpp     $(ICU_COMMON_PATH)/uvectr64.cpp     \
	$(ICU_COMMON_PATH)/locavailable.cpp         $(ICU_COMMON_PATH)/locdispnames.cpp   \
	$(ICU_COMMON_PATH)/loclikely.cpp            $(ICU_COMMON_PATH)/locresdata.cpp     \
	$(ICU_COMMON_PATH)/normalizer2impl.cpp      $(ICU_COMMON_PATH)/normalizer2.cpp    \
	$(ICU_COMMON_PATH)/filterednormalizer2.cpp  $(ICU_COMMON_PATH)/ucol_swp.cpp       \
	$(ICU_COMMON_PATH)/uprops.cpp      $(ICU_COMMON_PATH)/utrie2.cpp \
    $(ICU_COMMON_PATH)/charstr.cpp     $(ICU_COMMON_PATH)/uts46.cpp \
    $(ICU_COMMON_PATH)/udata.cpp   $(ICU_COMMON_PATH)/appendable.cpp  $(ICU_COMMON_PATH)/bytestrie.cpp \
    $(ICU_COMMON_PATH)/bytestriebuilder.cpp  $(ICU_COMMON_PATH)/bytestrieiterator.cpp \
    $(ICU_COMMON_PATH)/messagepattern.cpp $(ICU_COMMON_PATH)/patternprops.cpp $(ICU_COMMON_PATH)/stringtriebuilder.cpp \
    $(ICU_COMMON_PATH)/ucharstrie.cpp $(ICU_COMMON_PATH)/ucharstriebuilder.cpp $(ICU_COMMON_PATH)/ucharstrieiterator.cpp \
	$(ICU_COMMON_PATH)/dictionarydata.cpp \
	$(ICU_COMMON_PATH)/ustrcase_locale.cpp $(ICU_COMMON_PATH)/unistr_titlecase_brkiter.cpp \
	$(ICU_COMMON_PATH)/uniset_closure.cpp $(ICU_COMMON_PATH)/ucasemap_titlecase_brkiter.cpp \
	$(ICU_COMMON_PATH)/ustr_titlecase_brkiter.cpp $(ICU_COMMON_PATH)/unistr_case_locale.cpp \
	$(ICU_COMMON_PATH)/listformatter.cpp

# This is the empty compiled-in icu data structure
# that we need to satisfy the linker.
src_files += $(ICU_STUBDATA_PATH)/stubdata.c

c_includes := \
	$(ICU_COMMON_PATH) \
	$(ICU_I18N_PATH)

# We make the ICU data directory relative to $ANDROID_ROOT on Android, so both
# device and sim builds can use the same codepath, and it's hard to break one
# without noticing because the other still works.
local_cflags := '-DICU_DATA_DIR_PREFIX_ENV_VAR="CBL_ICU_PREFIX"'
#local_cflags += '-DICU_DATA_DIR="/data/data/com.couchbase.lite.test/files/icu"'
local_cflags += '-DICU_DATA_DIR="/icu"'

# bionic doesn't have <langinfo.h>.
local_cflags += -DU_HAVE_NL_LANGINFO_CODESET=0

local_cflags += -D_REENTRANT
local_cflags += -DU_COMMON_IMPLEMENTATION
local_cflags += -DU_TIMEZONE=0
local_cflags += -DUCONFIG_ONLY_COLLATION=0
local_cflags += -DUCONFIG_NO_LEGACY_CONVERSION=1

local_cflags += -O3 -fvisibility=hidden

#
# Build for the target (device).
#

#
# Build as a static library against the NDK
#

include $(CLEAR_VARS)
LOCAL_SDK_VERSION := 23
LOCAL_C_INCLUDES := $(c_includes)
LOCAL_EXPORT_C_INCLUDES += $(ICU_COMMON_PATH)
LOCAL_CFLAGS += $(local_cflags) -Os
LOCAL_EXPORT_CFLAGS += -DU_STATIC_IMPLEMENTATION=1
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE    := libicuuc_static
LOCAL_SRC_FILES := $(src_files)
TARGET_OUT=$(LOCAL_PATH)/artifacts/$(TARGET_ARCH_ABI)
include $(BUILD_STATIC_LIBRARY)


#----------
# end icuuc
#----------

#--------------
# start icui18n
#--------------

src_files := \
	$(ICU_I18N_PATH)/ucln_in.c  $(ICU_I18N_PATH)/decContext.c \
	$(ICU_I18N_PATH)/ulocdata.c  $(ICU_I18N_PATH)/utmscale.c $(ICU_I18N_PATH)/decNumber.c

src_files += \
        $(ICU_I18N_PATH)/indiancal.cpp   $(ICU_I18N_PATH)/dtptngen.cpp $(ICU_I18N_PATH)/dtrule.cpp   \
        $(ICU_I18N_PATH)/persncal.cpp    $(ICU_I18N_PATH)/rbtz.cpp     $(ICU_I18N_PATH)/reldtfmt.cpp \
        $(ICU_I18N_PATH)/taiwncal.cpp    $(ICU_I18N_PATH)/tzrule.cpp   $(ICU_I18N_PATH)/tztrans.cpp  \
        $(ICU_I18N_PATH)/udatpg.cpp      $(ICU_I18N_PATH)/vtzone.cpp                \
	$(ICU_I18N_PATH)/anytrans.cpp    $(ICU_I18N_PATH)/astro.cpp    $(ICU_I18N_PATH)/buddhcal.cpp \
	$(ICU_I18N_PATH)/basictz.cpp     $(ICU_I18N_PATH)/calendar.cpp $(ICU_I18N_PATH)/casetrn.cpp  \
	$(ICU_I18N_PATH)/choicfmt.cpp    $(ICU_I18N_PATH)/coleitr.cpp  $(ICU_I18N_PATH)/coll.cpp     \
	$(ICU_I18N_PATH)/collation.cpp \
	$(ICU_I18N_PATH)/collationbasedatabuilder.cpp $(ICU_I18N_PATH)/collationbuilder.cpp \
	$(ICU_I18N_PATH)/collationcompare.cpp $(ICU_I18N_PATH)/collationdata.cpp \
	$(ICU_I18N_PATH)/collationdatabuilder.cpp $(ICU_I18N_PATH)/collationdatareader.cpp \
	$(ICU_I18N_PATH)/collationdatawriter.cpp $(ICU_I18N_PATH)/collationfastlatin.cpp \
	$(ICU_I18N_PATH)/collationfastlatinbuilder.cpp $(ICU_I18N_PATH)/collationfcd.cpp \
	$(ICU_I18N_PATH)/collationiterator.cpp $(ICU_I18N_PATH)/collationkeys.cpp $(ICU_I18N_PATH)/collationroot.cpp \
	$(ICU_I18N_PATH)/collationrootelements.cpp $(ICU_I18N_PATH)/collationruleparser.cpp \
	$(ICU_I18N_PATH)/collationsets.cpp $(ICU_I18N_PATH)/collationsettings.cpp \
	$(ICU_I18N_PATH)/collationtailoring.cpp $(ICU_I18N_PATH)/collationweights.cpp \
	$(ICU_I18N_PATH)/compactdecimalformat.cpp \
	$(ICU_I18N_PATH)/cpdtrans.cpp    $(ICU_I18N_PATH)/csdetect.cpp $(ICU_I18N_PATH)/csmatch.cpp  \
	$(ICU_I18N_PATH)/csr2022.cpp     $(ICU_I18N_PATH)/csrecog.cpp  $(ICU_I18N_PATH)/csrmbcs.cpp  \
	$(ICU_I18N_PATH)/csrsbcs.cpp     $(ICU_I18N_PATH)/csrucode.cpp $(ICU_I18N_PATH)/csrutf8.cpp  \
	$(ICU_I18N_PATH)/curramt.cpp     $(ICU_I18N_PATH)/currfmt.cpp  $(ICU_I18N_PATH)/currunit.cpp \
	$(ICU_I18N_PATH)/dangical.cpp \
	$(ICU_I18N_PATH)/datefmt.cpp     $(ICU_I18N_PATH)/dcfmtsym.cpp $(ICU_I18N_PATH)/decimfmt.cpp \
	$(ICU_I18N_PATH)/decimalformatpattern.cpp \
	$(ICU_I18N_PATH)/digitlst.cpp    $(ICU_I18N_PATH)/dtfmtsym.cpp $(ICU_I18N_PATH)/esctrn.cpp   \
	$(ICU_I18N_PATH)/filteredbrk.cpp \
	$(ICU_I18N_PATH)/fmtable_cnv.cpp $(ICU_I18N_PATH)/fmtable.cpp  $(ICU_I18N_PATH)/format.cpp   \
	$(ICU_I18N_PATH)/funcrepl.cpp    $(ICU_I18N_PATH)/gender.cpp \
	$(ICU_I18N_PATH)/gregocal.cpp $(ICU_I18N_PATH)/gregoimp.cpp \
	$(ICU_I18N_PATH)/hebrwcal.cpp 	$(ICU_I18N_PATH)/identifier_info.cpp \
	$(ICU_I18N_PATH)/inputext.cpp $(ICU_I18N_PATH)/islamcal.cpp \
	$(ICU_I18N_PATH)/japancal.cpp    $(ICU_I18N_PATH)/measfmt.cpp $(ICU_I18N_PATH)/measunit.cpp  \
	$(ICU_I18N_PATH)/measure.cpp  \
	$(ICU_I18N_PATH)/msgfmt.cpp      $(ICU_I18N_PATH)/name2uni.cpp $(ICU_I18N_PATH)/nfrs.cpp     \
	$(ICU_I18N_PATH)/nfrule.cpp      $(ICU_I18N_PATH)/nfsubs.cpp   $(ICU_I18N_PATH)/nortrans.cpp \
	$(ICU_I18N_PATH)/nultrans.cpp    $(ICU_I18N_PATH)/numfmt.cpp   $(ICU_I18N_PATH)/olsontz.cpp  \
	$(ICU_I18N_PATH)/quant.cpp       $(ICU_I18N_PATH)/quantityformatter.cpp \
	$(ICU_I18N_PATH)/rbnf.cpp     $(ICU_I18N_PATH)/rbt.cpp      \
	$(ICU_I18N_PATH)/rbt_data.cpp    $(ICU_I18N_PATH)/rbt_pars.cpp $(ICU_I18N_PATH)/rbt_rule.cpp \
	$(ICU_I18N_PATH)/rbt_set.cpp     $(ICU_I18N_PATH)/regexcmp.cpp $(ICU_I18N_PATH)/regexst.cpp  \
	$(ICU_I18N_PATH)/regeximp.cpp 	$(ICU_I18N_PATH)/region.cpp \
	$(ICU_I18N_PATH)/rematch.cpp     $(ICU_I18N_PATH)/remtrans.cpp $(ICU_I18N_PATH)/repattrn.cpp \
	$(ICU_I18N_PATH)/rulebasedcollator.cpp \
	$(ICU_I18N_PATH)/scriptset.cpp \
	$(ICU_I18N_PATH)/search.cpp      $(ICU_I18N_PATH)/simpletz.cpp $(ICU_I18N_PATH)/smpdtfmt.cpp \
	$(ICU_I18N_PATH)/sortkey.cpp     $(ICU_I18N_PATH)/strmatch.cpp $(ICU_I18N_PATH)/strrepl.cpp  \
	$(ICU_I18N_PATH)/stsearch.cpp    $(ICU_I18N_PATH)/timezone.cpp \
	$(ICU_I18N_PATH)/titletrn.cpp    $(ICU_I18N_PATH)/tolowtrn.cpp $(ICU_I18N_PATH)/toupptrn.cpp \
	$(ICU_I18N_PATH)/translit.cpp    $(ICU_I18N_PATH)/transreg.cpp $(ICU_I18N_PATH)/tridpars.cpp \
	$(ICU_I18N_PATH)/ucal.cpp        \
	$(ICU_I18N_PATH)/ucol.cpp        $(ICU_I18N_PATH)/ucoleitr.cpp \
	$(ICU_I18N_PATH)/ucol_res.cpp    $(ICU_I18N_PATH)/ucol_sit.cpp \
	$(ICU_I18N_PATH)/ucsdet.cpp      $(ICU_I18N_PATH)/ucurr.cpp    $(ICU_I18N_PATH)/udat.cpp     \
	$(ICU_I18N_PATH)/uitercollationiterator.cpp \
	$(ICU_I18N_PATH)/umsg.cpp        $(ICU_I18N_PATH)/unesctrn.cpp $(ICU_I18N_PATH)/uni2name.cpp \
	$(ICU_I18N_PATH)/unum.cpp        $(ICU_I18N_PATH)/uregexc.cpp  $(ICU_I18N_PATH)/uregex.cpp   \
	$(ICU_I18N_PATH)/usearch.cpp     \
	$(ICU_I18N_PATH)/utf16collationiterator.cpp \
	$(ICU_I18N_PATH)/utf8collationiterator.cpp \
	$(ICU_I18N_PATH)/utrans.cpp   $(ICU_I18N_PATH)/windtfmt.cpp \
	$(ICU_I18N_PATH)/winnmfmt.cpp    $(ICU_I18N_PATH)/zonemeta.cpp \
	$(ICU_I18N_PATH)/numsys.cpp      $(ICU_I18N_PATH)/chnsecal.cpp \
	$(ICU_I18N_PATH)/cecal.cpp       $(ICU_I18N_PATH)/coptccal.cpp $(ICU_I18N_PATH)/ethpccal.cpp \
	$(ICU_I18N_PATH)/brktrans.cpp    $(ICU_I18N_PATH)/wintzimpl.cpp $(ICU_I18N_PATH)/plurrule.cpp \
	$(ICU_I18N_PATH)/plurfmt.cpp     $(ICU_I18N_PATH)/dtitvfmt.cpp $(ICU_I18N_PATH)/dtitvinf.cpp \
	$(ICU_I18N_PATH)/tmunit.cpp      $(ICU_I18N_PATH)/tmutamt.cpp  $(ICU_I18N_PATH)/tmutfmt.cpp  \
        $(ICU_I18N_PATH)/currpinf.cpp    $(ICU_I18N_PATH)/uspoof.cpp   $(ICU_I18N_PATH)/uspoof_impl.cpp \
        $(ICU_I18N_PATH)/uspoof_build.cpp     \
        $(ICU_I18N_PATH)/regextxt.cpp    $(ICU_I18N_PATH)/selfmt.cpp   $(ICU_I18N_PATH)/uspoof_conf.cpp \
        $(ICU_I18N_PATH)/uspoof_wsconf.cpp $(ICU_I18N_PATH)/ztrans.cpp $(ICU_I18N_PATH)/zrule.cpp  \
        $(ICU_I18N_PATH)/vzone.cpp       $(ICU_I18N_PATH)/fphdlimp.cpp $(ICU_I18N_PATH)/fpositer.cpp\
        $(ICU_I18N_PATH)/locdspnm.cpp    \
        $(ICU_I18N_PATH)/alphaindex.cpp  $(ICU_I18N_PATH)/bocsu.cpp    $(ICU_I18N_PATH)/decfmtst.cpp \
        $(ICU_I18N_PATH)/smpdtfst.cpp    $(ICU_I18N_PATH)/smpdtfst.h   $(ICU_I18N_PATH)/tzfmt.cpp \
        $(ICU_I18N_PATH)/tzgnames.cpp    $(ICU_I18N_PATH)/tznames.cpp  $(ICU_I18N_PATH)/tznames_impl.cpp \
        $(ICU_I18N_PATH)/udateintervalformat.cpp  $(ICU_I18N_PATH)/upluralrules.cpp

c_includes := \
	$(ICU_COMMON_PATH) \
	$(ICU_I18N_PATH)

local_cflags := -D_REENTRANT
local_cflags += -DU_I18N_IMPLEMENTATION
local_cflags += -DUCONFIG_ONLY_COLLATION=0
local_cflags += -DUCONFIG_NO_LEGACY_CONVERSION=1
local_cflags += -O3 -fvisibility=hidden

#
# Build as a static library against the NDK
#

include $(CLEAR_VARS)
LOCAL_SDK_VERSION := 23
LOCAL_SRC_FILES += $(src_files)
LOCAL_C_INCLUDES += $(c_includes)
LOCAL_STATIC_LIBRARIES += libicuuc_static
LOCAL_EXPORT_C_INCLUDES += $(ICU_I18N_PATH)
# Using -Os over -O3 actually cuts down the final executable size by a few dozen kilobytes
LOCAL_CFLAGS += $(local_cflags) -Os
LOCAL_EXPORT_CFLAGS += -DU_STATIC_IMPLEMENTATION=1
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libicui18n_static
TARGET_OUT=$(LOCAL_PATH)/artifacts/$(TARGET_ARCH_ABI)
include $(BUILD_STATIC_LIBRARY)

#------------
# end icui18n
#------------

#---------------
# start stubdata
#---------------

# Derive a string like 'icudt 48l.dat' from a path like 'external/icu4c/stubdata/icudt 48l.dat'
#stubdata_path:= $(call my-dir)
#dat_file := $(notdir $(wildcard $(stubdata_path)/*.dat))

#include $(CLEAR_VARS)
#LOCAL_MODULE := icu-data
#LOCAL_MODULE_TAGS := optional
#LOCAL_MODULE_CLASS := ETC
#LOCAL_MODULE_PATH := $(TARGET_OUT)/usr/icu
#LOCAL_MODULE_STEM := $(dat_file)
#LOCAL_SRC_FILES := $(dat_file)
#include $(BUILD_PREBUILT)

#include $(CLEAR_VARS)
#LOCAL_MODULE := icu-data-host
#LOCAL_MODULE_TAGS := optional
#LOCAL_MODULE_CLASS := ETC
#LOCAL_MODULE_PATH := $(HOST_OUT)/usr/icu
#LOCAL_MODULE_STEM := $(dat_file)
#LOCAL_SRC_FILES := $(dat_file)
#LOCAL_IS_HOST_MODULE := true
#include $(BUILD_PREBUILT)

#-------------
# end stubdata
#-------------

