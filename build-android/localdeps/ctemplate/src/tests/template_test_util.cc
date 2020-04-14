// Copyright (c) 2006, Google Inc.
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

#include "config_for_unittests.h"
#include "base/mutex.h"  // must come first, for _XOPEN_SOURCE
#include "tests/template_test_util.h"
#include <assert.h>      // for assert()
#ifdef HAVE_DIRENT_H
# include <dirent.h>       // for opendir() etc
#else
# define dirent direct
# ifdef HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# ifdef HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# ifdef HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif      // for DIR, dirent, closedir(), opendir(), etc
#include <stdio.h>       // for printf(), FILE, fclose(), fopen(), etc
#include <stdlib.h>      // for exit()
#include <string.h>      // for strcmp(), strcpy(), strstr()
#include <sys/stat.h>    // for mkdir()
#include <sys/types.h>   // for mode_t
#include <time.h>        // for time_t
#ifdef HAVE_UTIME_H
# include <utime.h>
#endif       // for utime()
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif      // for unlink()
#include <vector>        // for vector<>, vector<>::size_type
#include <ctemplate/template.h>  // for Template
#include <ctemplate/template_dictionary.h>  // for TemplateDictionary
#include <ctemplate/template_dictionary_interface.h>
#include <ctemplate/template_enums.h>  // for Strip
#include <ctemplate/template_namelist.h>  // for TemplateNamelist, etc
#include <ctemplate/template_pathops.h>  // for PathJoin()
#include "base/util.h"   // for down_cast()

using std::string;
using std::vector;

#ifdef ASSERT
# undef ASSERT
#endif
#define ASSERT(cond)  do {                                      \
  if (!(cond)) {                                                \
    printf("ASSERT FAILED, line %d: %s\n", __LINE__, #cond);    \
    assert(cond);                                               \
    exit(1);                                                    \
  }                                                             \
} while (0)

namespace ctemplate {

// Deletes all files named *template* in dir, and sets up dir as the
// place where StringToTemplate writes.
static char* g_tmpdir = NULL;

#ifndef USING_PORT_CC  /* windows defines its own version in windows/port.cc */
void CreateOrCleanTestDir(const string& dirname) {
  DIR* dir = opendir(dirname.c_str());
  if (!dir) {   // directory doesn't exist or something like that
    mkdir(dirname.c_str(), 0755);   // make the dir if we can
    return;
  }
  while (struct dirent* d = readdir(dir)) {
    if (strstr(d->d_name, "template"))
      unlink(PathJoin(dirname, d->d_name).c_str());
  }
  closedir(dir);
}

static string TmpFile(const char* basename) {
  return string("/tmp/") + basename;
}

#endif  // #ifndef USING_PORT_CC

void CreateOrCleanTestDirAndSetAsTmpdir(const string& dirname) {
  CreateOrCleanTestDir(dirname);
  delete[] g_tmpdir;
  g_tmpdir = new char[dirname.length() + 1];
  strcpy(g_tmpdir, dirname.c_str());
}

const string FLAGS_test_tmpdir(TmpFile("template_unittest_dir"));

// This writes s to the given file.  We want to make sure that every
// time we create a file, it has a different mtime (just like would
// be the case in real life), so we use a mock clock.
static Mutex g_time_mutex(base::LINKER_INITIALIZED);
static time_t mock_time = 946713600;   // jan 1, 2000, in california

void StringToFile(const string& s, const string& filename) {
  FILE* fp = fopen(filename.c_str(), "wb");
  ASSERT(fp);
  size_t r = fwrite(s.data(), 1, s.length(), fp);
  ASSERT(r == s.length());
  fclose(fp);

  g_time_mutex.Lock();
  const time_t file_time = mock_time++;
  g_time_mutex.Unlock();
  struct utimbuf timbuf = { file_time, file_time };
  utime(filename.c_str(), &timbuf);
}

time_t Now() {
  g_time_mutex.Lock();
  const time_t now = mock_time;
  g_time_mutex.Unlock();
  return now;
}

// This writes s to a file and returns the filename.
string StringToTemplateFile(const string& s) {
  static int filenum = 0;
  char buf[16];
  snprintf(buf, sizeof(buf), "%03d", ++filenum);
  string filename = PathJoin(g_tmpdir ? g_tmpdir : "",
                             string("template.") + buf);
  StringToFile(s, filename);
  return filename;
}

// This writes s to a file and then loads it into a template object.
Template* StringToTemplate(const string& s, Strip strip) {
  return Template::GetTemplate(StringToTemplateFile(s), strip);
}

// This is esp. useful for calling from within gdb.
// The gdb nice-ness is balanced by the need for the caller to delete the buf.

const char* ExpandIs(const Template* tpl, const TemplateDictionary *dict,
                     PerExpandData* per_expand_data, bool expected) {
  string outstring;
  if (per_expand_data)
    ASSERT(expected == tpl->ExpandWithData(&outstring, dict, per_expand_data));
  else
    ASSERT(expected == tpl->Expand(&outstring, dict));


  char* buf = new char[outstring.size()+1];
  strcpy(buf, outstring.c_str());
  return buf;
}

const char* ExpandWithCacheIs(TemplateCache* cache,
                              const string& filename, Strip strip,
                              const TemplateDictionary *dict,
                              PerExpandData* per_expand_data, bool expected) {
  string outstring;
  ASSERT(expected == cache->ExpandWithData(filename, strip, dict,
                                           per_expand_data, &outstring));


  char* buf = new char[outstring.size()+1];
  strcpy(buf, outstring.c_str());
  return buf;
}

void AssertExpandWithDataIs(const Template* tpl,
                            const TemplateDictionary *dict,
                            PerExpandData* per_expand_data,
                            const string& is, bool expected) {
  const char* buf = ExpandIs(tpl, dict, per_expand_data, expected);
  if (strcmp(buf, is.c_str())) {
    printf("expected = '%s'\n", is.c_str());
    printf("actual   = '%s'\n", buf);
  }
  ASSERT(string(buf) == is);
  delete [] buf;
}

void AssertExpandIs(const Template* tpl, const TemplateDictionary *dict,
                    const string& is, bool expected) {
  AssertExpandWithDataIs(tpl, dict, NULL, is, expected);
}

void AssertExpandWithCacheIs(TemplateCache* cache,
                             const string& filename, Strip strip,
                             const TemplateDictionary *dict,
                             PerExpandData* per_expand_data,
                             const string& is, bool expected) {
  const char* buf = ExpandWithCacheIs(cache, filename, strip, dict,
                                      per_expand_data, expected);
  if (strcmp(buf, is.c_str())) {
    printf("expected = '%s'\n", is.c_str());
    printf("actual   = '%s'\n", buf);
  }
  ASSERT(string(buf) == is);
  delete [] buf;
}

TemporaryRegisterTemplate::TemporaryRegisterTemplate(const char* name) {
  old_namelist_ = TemplateNamelist::namelist_;
  if (old_namelist_) {
    namelist_ = *old_namelist_;
  }

  namelist_.insert(name);
  TemplateNamelist::namelist_ = &namelist_;
}

TemporaryRegisterTemplate::~TemporaryRegisterTemplate() {
  TemplateNamelist::namelist_ = old_namelist_;
}

const char* TemplateDictionaryPeer::GetSectionValue(
    const TemplateString& variable)
    const {
  // Luckily, TemplateDictionary stores all values with a trailing NUL.
  return dict_->GetValue(variable).data();
}

bool TemplateDictionaryPeer::ValueIs(const TemplateString& variable,
                                     const TemplateString& expected) const {
  return dict_->GetValue(variable) == expected;
}

bool TemplateDictionaryPeer::IsHiddenSection(
    const TemplateString& name) const {
  return dict_->IsHiddenSection(name);
}

bool TemplateDictionaryPeer::IsUnhiddenSection(
    const TemplateString& name) const {
  return dict_->IsUnhiddenSection(name);
}

bool TemplateDictionaryPeer::IsHiddenTemplate(
    const TemplateString& name) const {
  return dict_->IsHiddenTemplate(name);
}

int TemplateDictionaryPeer::GetSectionDictionaries(
    const TemplateString& section_name,
    vector<const TemplateDictionary*>* dicts) const {
  dicts->clear();
  if (dict_->IsHiddenSection(section_name))
    return 0;

  TemplateDictionaryInterface::Iterator* di =
      dict_->CreateSectionIterator(section_name);
  while (di->HasNext())
    dicts->push_back(down_cast<const TemplateDictionary*>(&di->Next()));
  delete di;

  return static_cast<int>(dicts->size());
}

int TemplateDictionaryPeer::GetIncludeDictionaries(
    const TemplateString& section_name,
    vector<const TemplateDictionary*>* dicts) const {
  dicts->clear();
  if (dict_->IsHiddenTemplate(section_name))
    return 0;

  TemplateDictionaryInterface::Iterator* di =
      dict_->CreateTemplateIterator(section_name);
  while (di->HasNext())
    dicts->push_back(down_cast<const TemplateDictionary*>(&di->Next()));
  delete di;

  return static_cast<int>(dicts->size());
}

const char* TemplateDictionaryPeer::GetIncludeTemplateName(
    const TemplateString& variable, int dictnum) const {
  return dict_->GetIncludeTemplateName(variable, dictnum);
}

const char* TemplateDictionaryPeer::GetFilename() const {
  return dict_->filename_;
}

}
