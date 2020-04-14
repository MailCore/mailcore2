## Process this file with autoconf to produce configure.
## In general, the safest way to proceed is to run ./autogen.sh

# make sure we're interpreted by some minimal autoconf
AC_PREREQ([2.69])

AC_INIT([ctemplate],[2.3],[google-ctemplate@googlegroups.com])
AM_SILENT_RULES([yes])

# Update this value for every release!  (A:B:C will map to foo.so.(A-C).C.B)
# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
SO_VERSION=3:0:0

# The argument here is just something that should be in the current directory
# (for sanity checking)
AC_CONFIG_SRCDIR(README)
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_HOST
AM_INIT_AUTOMAKE([subdir-objects dist-zip])
AC_CONFIG_HEADERS([src/config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_AR
AM_CONDITIONAL(GCC, test "$GCC" = yes)   # let the Makefile know if we're gcc

# MinGW uses autoconf, but also needs the windows shim routines
# (since it doesn't have its own support for, say, pthreads).
# This requires us to #include a special header file, and also to
# link in some windows versions of .o's instead of the unix versions.
AH_BOTTOM([
#if defined( __MINGW32__) || defined(__MINGW64__)
#include "windows/port.h"
#endif
])
AM_CONDITIONAL(MINGW, expr $host : '.*-mingw' >/dev/null 2>&1)

LT_INIT([disable-fast-install])
AC_SUBST(LIBTOOL_DEPS)
AC_SUBST(SO_VERSION)

AX_C___ATTRIBUTE__

# Defines PRIuS
AC_COMPILER_CHARACTERISTICS

# Here are some examples of how to check for the existence of a fn or file
AC_CHECK_FUNCS([getopt_long getopt])
AC_CHECK_HEADERS([getopt.h])
AC_CHECK_HEADERS([utime.h])           # used by unittests to mock file-times

AC_HEADER_DIRENT               # for template_unittest.cc, template_regtest.cc

# We need to do byte-swapping efficiently to hash efficiently in
# template_string.cc.  Different architectures do this differently.
AC_CHECK_HEADERS(byteswap.h)              # Linux (GNU in general)
AC_CHECK_HEADERS(libkern/OSByteOrder.h)   # OS X
AC_CHECK_HEADERS(sys/byteorder.h)         # Solaris 10
AC_CHECK_HEADERS(endian.h)                # Linux
AC_CHECK_HEADERS(machine/endian.h)        # OS X
AC_CHECK_HEADERS(sys/endian.h)            # FreeBSD
AC_CHECK_HEADERS(sys/isa_defs.h)          # Solaris 10

# A lot of the code in this directory depends on pthreads
AX_PTHREAD

# We'd like to use read/write locks in several places in the code.
# See if our pthreads support extends to that.  Note: for linux, it
# does as long as you define _XOPEN_SOURCE appropriately.
AC_RWLOCK

# For mingw/cygwin, figure out if the mutex code needs to use
# 'volatile' in some places.  They differ from MSVC, and the API is
# unclear, so it's best just to check.
AC_INTERLOCKED_EXCHANGE_NONVOLATILE

# Figures out where hash_map and hash_set live, and what namespace they use
AC_CXX_STL_HASH

AC_SUBST(ac_cv_cxx_hash_namespace)
AC_SUBST(ac_cv_cxx_hash_map)
AC_SUBST(ac_cv_cxx_hash_set)

if test "$ac_cv___attribute__" = "yes"; then
   AC_SUBST(ac_google_attribute, 1)
else
   AC_SUBST(ac_google_attribute, 0)
fi

# On some systems (eg gnu/linux), the linker defines _start and
# data_start to indicate the extent of the .text section.  We can use
# this to know strings are immutable.  In the code, we make the
# variables weak, just in case, but for this check, we only want to
# say "yes" if the linker supports the vars, *and* the compiler supports
# attribute-weak.
AC_LINK_IFELSE([
	AC_LANG_PROGRAM([], [[
	extern char _start[];
	extern char data_start[];
	extern char dummy[]  __attribute__((weak));
	return (_start && data_start && dummy);
	]])
], [
	ac_have_attribute_weak=1
], [
	ac_have_attribute_weak=0
])
AC_SUBST(ac_have_attribute_weak)

# In unix (that is, using this script), this dll-export stuff will always
# be the empty string.  In windows, we'll replace it with windows-specific
# text.
ac_windows_dllexport_defines=
ac_windows_dllexport=
AC_SUBST(ac_windows_dllexport_defines)
AC_SUBST(ac_windows_dllexport)

# Write generated configuration file, and also .h files
AC_CONFIG_FILES([Makefile \
                 src/ctemplate/template_string.h \
                 src/ctemplate/template_enums.h \
                 src/ctemplate/template.h \
                 src/ctemplate/template_cache.h \
                 src/ctemplate/template_modifiers.h \
                 src/ctemplate/template_emitter.h \
                 src/ctemplate/template_annotator.h \
                 src/ctemplate/template_dictionary.h \
                 src/ctemplate/template_pathops.h \
                 src/ctemplate/template_namelist.h \
                 src/ctemplate/find_ptr.h \
                 src/ctemplate/per_expand_data.h \
                 src/ctemplate/str_ref.h \
                 src/ctemplate/template_dictionary_interface.h \
                 ])
AC_OUTPUT
