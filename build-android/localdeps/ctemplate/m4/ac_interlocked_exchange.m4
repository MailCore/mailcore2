# Check whether InterlockedExchange() takes a LONG or a volatile LONG
# as its first argument.  InterlockedCompareExchange is a windows
# function; obviously, this macro is useful only for cygwin and mingw,
# and other systems that compile against the windows API.
#
# Apparently the interface for this function is a bit inconsistent.
# Windows likes volatile LONG, but mingw and cygwin don't, at least
# for the versions I'm using.  But rather than try to guess who
# supports what, let's just check at configure time.  (Note: this
# is an error in C++ but only a warning in C, so we test in the former.)
#
# This function returns 'yes' if the type does not need volatile,
# and defines the symbol INTERLOCKED_EXCHANGE_NONVOLATILE.  (This
# is the expected case for mingw and cygwin).  It returns 'no',
# and defines no symbol, otherwise.  (This is the expected case for
# MSVC.)  The return value was sset this way so that we don't need
# to define any symbols on windows, which doesn't run configure.

AC_DEFUN([AC_INTERLOCKED_EXCHANGE_NONVOLATILE],
[
AC_MSG_CHECKING(whether first argument to InterlockedExchange omits volatile)
AC_CACHE_VAL(ac_cv_interlocked_exchange_nonvolatile,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <windows.h>],
                [volatile LONG once; InterlockedExchange(&once,  1);],
                ac_cv_interlocked_exchange_nonvolatile="no",
                ac_cv_interlocked_exchange_nonvolatile="yes")
 AC_LANG_RESTORE
])
if test "$ac_cv_interlocked_exchange_nonvolatile" = "yes"; then
  AC_DEFINE(INTERLOCKED_EXCHANGE_NONVOLATILE, 1,
            [define if first argument to InterlockedExchange is just LONG])
fi
AC_MSG_RESULT($ac_cv_interlocked_exchange_nonvolatile)
])
