// Copyright (c) 2011, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ---
//
// Some generically useful utility routines that in google-land would
// be their own projects.  We make a shortened version here.

#ifndef TEMPLATE_UTIL_H_
#define TEMPLATE_UTIL_H_

#include <config.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// -- utility macros ---------------------------------------------------------

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

// Starting with Visual C++ 2005, WinNT.h includes ARRAYSIZE.
#if !defined(_MSC_VER) || _MSC_VER < 1400
#define ARRAYSIZE(a) \
  ((sizeof(a) / sizeof(*(a))) / \
   static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
#endif

template<typename To, typename From>     // use like this: down_cast<T*>(foo);
inline To down_cast(From* f) {                   // so we only accept pointers
  return static_cast<To>(f);
}

// -- CHECK macros ---------------------------------------------------------

// CHECK dies with a fatal error if condition is not true.  It is *not*
// controlled by NDEBUG, so the check will be executed regardless of
// compilation mode.  Therefore, it is safe to do things like:
//    CHECK(fp->Write(x) == 4)
// We allow stream-like objects after this for debugging, but they're ignored.
#define CHECK(condition)                                        \
  if (true) {                                                   \
    if (!(condition)) {                                         \
      fprintf(stderr, "Check failed: %s\n", #condition);        \
      exit(1);                                                  \
    }                                                           \
  } else std::cerr << ""

#define CHECK_OP(op, val1, val2)                                        \
  if (true) {                                                           \
    if (!((val1) op (val2))) {                                          \
      fprintf(stderr, "Check failed: %s %s %s\n", #val1, #op, #val2);   \
      exit(1);                                                          \
    }                                                                   \
  } else std::cerr << ""

#define CHECK_EQ(val1, val2) CHECK_OP(==, val1, val2)
#define CHECK_NE(val1, val2) CHECK_OP(!=, val1, val2)
#define CHECK_LE(val1, val2) CHECK_OP(<=, val1, val2)
#define CHECK_LT(val1, val2) CHECK_OP(< , val1, val2)
#define CHECK_GE(val1, val2) CHECK_OP(>=, val1, val2)
#define CHECK_GT(val1, val2) CHECK_OP(> , val1, val2)
// Synonyms for CHECK_* that are used in some unittests.
#define EXPECT_EQ(val1, val2) CHECK_EQ(val1, val2)
#define EXPECT_NE(val1, val2) CHECK_NE(val1, val2)
#define EXPECT_LE(val1, val2) CHECK_LE(val1, val2)
#define EXPECT_LT(val1, val2) CHECK_LT(val1, val2)
#define EXPECT_GE(val1, val2) CHECK_GE(val1, val2)
#define EXPECT_GT(val1, val2) CHECK_GT(val1, val2)
#define EXPECT_TRUE(cond)     CHECK(cond)
#define EXPECT_FALSE(cond)    CHECK(!(cond))
#define EXPECT_STREQ(a, b)    CHECK(strcmp(a, b) == 0)
#define ASSERT_TRUE(cond)     EXPECT_TRUE(cond)
// LOG(FATAL) is an alias for CHECK(FALSE).  We define FATAL, but no
// other value that is reasonable inside LOG(), so the compile will
// fail if someone tries to use LOG(DEBUG) or the like.
#define LOG(x)                INTERNAL_DO_LOG_ ## x
#define INTERNAL_DO_LOG_FATAL CHECK(false)

// These are used only in debug mode.
#ifdef NDEBUG
#define DCHECK(condition)     CHECK(condition)
#define DCHECK_EQ(val1, val2) CHECK_EQ(val1, val2)
#define DCHECK_NE(val1, val2) CHECK_NE(val1, val2)
#define DCHECK_LE(val1, val2) CHECK_LE(val1, val2)
#define DCHECK_LT(val1, val2) CHECK_LT(val1, val2)
#define DCHECK_GE(val1, val2) CHECK_GE(val1, val2)
#define DCHECK_GT(val1, val2) CHECK_GT(val1, val2)
#else
#define DCHECK(condition)     if (true) {} else std::cerr << ""
#define DCHECK_EQ(val1, val2) if (true) {} else std::cerr << ""
#define DCHECK_NE(val1, val2) if (true) {} else std::cerr << ""
#define DCHECK_LE(val1, val2) if (true) {} else std::cerr << ""
#define DCHECK_LT(val1, val2) if (true) {} else std::cerr << ""
#define DCHECK_GE(val1, val2) if (true) {} else std::cerr << ""
#define DCHECK_GT(val1, val2) if (true) {} else std::cerr << ""
#endif

#define PCHECK(cond)  CHECK(cond) << ": " << strerror(errno)
#define PFATAL(s)     do { perror(s); exit(1); } while (0)

// -- testing-related macros --------------------------------------------------

// Call this in a .cc file where you will later call RUN_ALL_TESTS in main().
#define TEST_INIT                                                       \
  static std::vector<void (*)()> g_testlist;  /* the tests to run */    \
  static int RUN_ALL_TESTS() {                                          \
    std::vector<void (*)()>::const_iterator it;                         \
    for (it = g_testlist.begin(); it != g_testlist.end(); ++it) {       \
      (*it)();   /* The test will error-exit if there's a problem. */   \
    }                                                                   \
    fprintf(stderr, "\nPassed %d tests\n\nPASS\n",                      \
            static_cast<int>(g_testlist.size()));                       \
    return 0;                                                           \
  }

#define TEST(a, b)                                      \
  class Test_##a##_##b {                                \
   public:                                              \
    Test_##a##_##b() { g_testlist.push_back(&Run); }    \
    static void Run();                                  \
  };                                                    \
  static Test_##a##_##b g_test_##a##_##b;               \
  void Test_##a##_##b::Run()

// This is a dummy class that eases the google->opensource transition.
namespace testing {
class Test {};
}

// -- template-related macros ----------------------------------------------

#ifndef DEFAULT_TEMPLATE_ROOTDIR
# define DEFAULT_TEMPLATE_ROOTDIR  "."
#endif

// -- string-related functions ----------------------------------------------

inline bool safe_strto32(const std::string& s, int* i) {
  char* error_pos;
  if (s.empty()) return false;    // no input at all
  errno = 0;                      // just to be sure
  *i = strtol(s.c_str(), &error_pos, 10);
  return *error_pos == '\0' && errno == 0;
}

inline int atoi32(const char* s) {
  return atoi(s);
}

inline void StripWhiteSpace(std::string* str) {
  int str_length = str->length();

  // Strip off leading whitespace.
  int first = 0;
  while (first < str_length && isspace(str->at(first))) {
    ++first;
  }
  // If entire string is white space.
  if (first == str_length) {
    str->clear();
    return;
  }
  if (first > 0) {
    str->erase(0, first);
    str_length -= first;
  }

  // Strip off trailing whitespace.
  int last = str_length - 1;
  while (last >= 0 && isspace(str->at(last))) {
    --last;
  }
  if (last != (str_length - 1) && last >= 0) {
    str->erase(last + 1, std::string::npos);
  }
}

inline void SplitStringIntoKeyValuePairs(
    const std::string& s,
    const char* kv_split,    // For instance: "="
    const char* pair_split,  // For instance: ","
    std::vector< std::pair<std::string, std::string> > *pairs) {
  std::string key, value;
  std::string* add_to = &key;
  for (std::string::size_type i = 0; i < s.length(); ++i) {
    if (s[i] == kv_split[0]) {
      add_to = &value;
    } else if (s[i] == pair_split[0]) {
      if (!key.empty())
        pairs->push_back(std::pair<std::string, std::string>(key, value));
      key.clear();
      value.clear();
      add_to = &key;
    } else {
      *add_to += s[i];
    }
  }
  if (!key.empty())
    pairs->push_back(std::pair<std::string, std::string>(key, value));
}

#endif  // TEMPLATE_UTIL_H_
