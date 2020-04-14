/* A manual version of config.h fit for windows machines. */

/* Sometimes we accidentally #include this config.h instead of the one
   in .. -- this is particularly true for msys/mingw, which uses the
   unix config.h but also runs code in the windows directory.
*/
#if defined(__MINGW32__) || defined(__MING64__)
#define CTEMPLATE_DLL_DECL
// These two lines make sure we read the unix-style config.h, and not the
// windows-style config.h -- it would be bad if we tried to read both!
#include "../config.h"
#define GOOGLE_CTEMPLATE_WINDOWS_CONFIG_H_
#endif

#ifndef GOOGLE_CTEMPLATE_WINDOWS_CONFIG_H_
#define GOOGLE_CTEMPLATE_WINDOWS_CONFIG_H_

/* Namespace for Google classes */
#define GOOGLE_NAMESPACE  ctemplate

/* the location of <unordered_map> or <hash_map> */
#define HASH_MAP_H  <unordered_map>

/* the namespace of hash_map/hash_set */
#define HASH_NAMESPACE  std

/* the location of <unordered_set> or <hash_set> */
#define HASH_SET_H  <unordered_set>

/* Define to 1 if you have the <byteswap.h> header file. */
#undef HAVE_BYTESWAP_H

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#undef HAVE_DIRENT_H

/* Define to 1 if you have the <dlfcn.h> header file. */
#undef HAVE_DLFCN_H

/* Define to 1 if you have the <endian.h> header file. */
#undef HAVE_ENDIAN_H

/* Define to 1 if you have the `getopt' function. */
#undef HAVE_GETOPT

/* Define to 1 if you have the <getopt.h> header file. */
#undef HAVE_GETOPT_H

/* Define to 1 if you have the `getopt_long' function. */
#undef HAVE_GETOPT_LONG

/* define if the compiler has hash_map */
#define HAVE_HASH_MAP  1

/* define if the compiler has hash_set */
#define HAVE_HASH_SET  1

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Define to 1 if you have the <libkern/OSByteOrder.h> header file. */
#undef HAVE_LIBKERN_OSBYTEORDER_H

/* Define to 1 if you have the <machine/endian.h> header file. */
#undef HAVE_MACHINE_ENDIAN_H

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H  1

/* define if the compiler implements namespaces */
#define HAVE_NAMESPACES  1

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
#undef HAVE_NDIR_H

/* Define if you have POSIX threads libraries and header files. */
#undef HAVE_PTHREAD

/* define if the compiler implements pthread_rwlock_* */
#undef HAVE_RWLOCK

/* Define to 1 if you have the <stdint.h> header file. */
#undef HAVE_STDINT_H

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H  1

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H  1

/* Define to 1 if you have the <sys/byteorder.h> header file. */
#undef HAVE_SYS_BYTEORDER_H

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
#undef HAVE_SYS_DIR_H

/* Define to 1 if you have the <sys/endian.h> header file. */
#undef HAVE_SYS_ENDIAN_H

/* Define to 1 if you have the <sys/isa_defs.h> header file. */
#undef HAVE_SYS_ISA_DEFS_H

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
#undef HAVE_SYS_NDIR_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H  1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H  1

/* Define to 1 if the system has the type `uint32_t'. */
#undef HAVE_UINT32_T

/* Define to 1 if the system has the type `uint64_t'. */
#undef HAVE_UINT64_T

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* define if the compiler supports unordered_{map,set} */
#define HAVE_UNORDERED_MAP 1

/* Define to 1 if you have the <utime.h> header file. */
#undef HAVE_UTIME_H

/* Define to 1 if the system has the type `u_int32_t'. */
#undef HAVE_U_INT32_T

/* Define to 1 if the system has the type `u_int64_t'. */
#undef HAVE_U_INT64_T

/* define if your compiler has __attribute__ */
#undef HAVE___ATTRIBUTE__

/* Define to 1 if the system has the type `__uint32. */
#define HAVE___INT32  1

/* Define to 1 if the system has the type `__uint64. */
#define HAVE___INT64  1

/* The namespace to put the htmlparser code. */
#define HTMLPARSER_NAMESPACE  google_ctemplate_streamhtmlparser

/* define if first argument to InterlockedExchange is just LONG */
#undef INTERLOCKED_EXCHANGE_NONVOLATILE

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#undef LT_OBJDIR

/* Name of package */
#undef PACKAGE

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#undef PACKAGE_NAME

/* Define to the full name and version of this package. */
#define PACKAGE_STRING  "ctemplate 1.1"

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the home page for this package. */
#undef PACKAGE_URL

/* Define to the version of this package. */
#undef PACKAGE_VERSION

/* printf format code for printing a size_t and ssize_t */
#define PRIdS  "Id"

/* printf format code for printing a size_t and ssize_t */
#define PRIuS  "Iu"

/* printf format code for printing a size_t and ssize_t */
#define PRIxS  "Ix"

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
#undef PTHREAD_CREATE_JOINABLE

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS  1

/* the namespace where STL code like vector<> is defined */
#define STL_NAMESPACE  std

/* Version number of package */
#undef VERSION

/* Stops putting the code inside the Google namespace */
#define _END_GOOGLE_NAMESPACE_  }

/* Puts following code inside the Google namespace */
#define _START_GOOGLE_NAMESPACE_  namespace ctemplate {

// ---------------------------------------------------------------------
// Extra stuff not found in config.h.in

// This must be defined before anything else in our project: make sure
// that when compiling the dll, we export our functions/classes.  Safe
// to define this here because this file is only used internally, to
// compile the DLL, and every dll source file #includes "config.h"
// before anything else.
#ifndef CTEMPLATE_DLL_DECL
# define CTEMPLATE_DLL_DECL  __declspec(dllexport)
# define CTEMPLATE_DLL_DECL_FOR_UNITTESTS  __declspec(dllimport)
#endif

// TODO(csilvers): include windows/port.h in every relevant source file instead?
#include "windows/port.h"

#endif  /* GOOGLE_CTEMPLATE_WINDOWS_CONFIG_H_ */
