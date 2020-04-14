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
// Author: csilvers@google.com (Craig Silverstein)
//
// A tiny wrapper around struct stat and FILE*.

#ifndef TEMPLATE_OPENSOURCE_FILEUTIL_H_
#define TEMPLATE_OPENSOURCE_FILEUTIL_H_

#include <config.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <string>

namespace ctemplate {

class FileStat {
 public:
  time_t mtime;
  off_t length;
  bool IsDirectory() { return S_ISDIR(internal_statbuf.st_mode); }

 private:
  friend class File;
  struct stat internal_statbuf;
};

class File {
 public:
  static bool Stat(const std::string& filename, FileStat* statbuf) {
    if (stat(filename.c_str(), &statbuf->internal_statbuf) != 0)
      return false;
    statbuf->mtime = statbuf->internal_statbuf.st_mtime;
    statbuf->length = statbuf->internal_statbuf.st_size;
    return true;
  }

  static bool Readable(const char* filename) {
    return access(filename, R_OK) == 0;
  }

  static File* Open(const char* filename, const char* mode) {
    char binary_mode[3];
    const char* mode_to_use = mode;
    if ((mode[0] == 'r' || mode[0] == 'w') && mode[1] == '\0') {
      // We add a 'b' to make sure we do the right thing even on
      // Windows.  On unix, this will be a noop.
      binary_mode[0] = mode[0];
      binary_mode[1] = 'b';
      binary_mode[2] = '\0';
      mode_to_use = binary_mode;
    }
    FILE* fp = fopen(filename, mode_to_use);
    if (!fp)  return NULL;
    return new File(fp);
  }

  size_t Read(char* buf, size_t size) {
    return fread(buf, 1, size, fp_);
  }

  void Close() {
    fclose(fp_);
    delete this;   // naughty naughty!
  }

 private:
  explicit File(FILE* fp) : fp_(fp) { }
  FILE* fp_;
};

}

#endif  // TEMPLATE_OPENSOURCE_FILEUTIL_H_
