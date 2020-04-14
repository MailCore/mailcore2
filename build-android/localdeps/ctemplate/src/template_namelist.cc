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

#include <config.h>
#include <stdlib.h>
#include <sys/stat.h>            // for stat()
#include <time.h>                // for time_t
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <algorithm>             // for binary_search
#include HASH_SET_H              // that's NameListType
#include <string>
#include <vector>                // that's MissingListType, SyntaxListType
#include <ctemplate/template_namelist.h>
#include <ctemplate/template_pathops.h>
#include <ctemplate/template.h>   // for Strip, GetTemplate(), etc.
#include <assert.h>
#include <iostream>              // for cerr
#include "base/fileutil.h"

using std::max;
using std::pair;
using std::string;
using std::vector;

#define LOG(level)  std::cerr << #level << ": "

namespace ctemplate {

TemplateNamelist::NameListType *TemplateNamelist::namelist_ = NULL;
TemplateNamelist::MissingListType *TemplateNamelist::missing_list_ = NULL;
TemplateNamelist::SyntaxListType *TemplateNamelist::bad_syntax_list_ = NULL;

// Make sure there is a namelist_ and then insert the name onto it
const char* TemplateNamelist::RegisterTemplate(const char* name) {
  if (!namelist_) {
    namelist_ = new NameListType;
  }
  pair<NameListType::iterator, bool> insert_result = namelist_->insert(name);
  // return a pointer to the entry corresponding to name;
  return insert_result.first->c_str();
}

// GetList
// Make sure there is a namelist_ and return a reference to it.
const TemplateNamelist::NameListType& TemplateNamelist::GetList() {
  if ( !namelist_ ) {
    namelist_ = new NameListType;
  }
  return *namelist_;
}

// GetMissingList
//   On the first invocation, it creates a new missing list and sets
//   refresh to true.
//   If refresh is true, whether from being passed to the function
//   or being set when the list is created the first time, it iterates
//   through the complete list of registered template files
//   and adds to the list any that are missing
//   On subsequent calls, if refresh is false it merely returns the
//   list created in the prior call that refreshed the list.
//   Returns a sorted list of missing templates.
const TemplateNamelist::MissingListType& TemplateNamelist::GetMissingList(
    bool refresh) {
  if (!missing_list_) {
    missing_list_ = new MissingListType;
    refresh = true;  // always refresh the first time
  }

  if (refresh) {
    const NameListType& the_list = TemplateNamelist::GetList();
    missing_list_->clear();

    for (NameListType::const_iterator iter = the_list.begin();
         iter != the_list.end();
         ++iter) {
      const string path = Template::FindTemplateFilename(*iter);
      if (path.empty() || !File::Readable(path.c_str())) {
        missing_list_->push_back(*iter);
        LOG(ERROR) << "Template file missing: " << *iter
                   << " at path: " << (path.empty() ? "(empty path)" : path)
                   << "\n";
      }
    }
  }

  sort(missing_list_->begin(), missing_list_->end());
  return *missing_list_;
}

// GetBadSyntaxList
//   On the first invocation, it creates a new "bad syntax" list and
//   sets refresh to true.
//   If refresh is true, whether from being passed to the function
//   or being set when the list is created the first time, it
//   iterates through the complete list of registered template files
//   and adds to the list any that cannot be loaded. In the process, it
//   calls GetMissingList, refreshing it. It does not include any
//   files in the bad syntax list which are in the missing list.
//   On subsequent calls, if refresh is false it merely returns the
//   list created in the prior call that refreshed the list.
const TemplateNamelist::SyntaxListType& TemplateNamelist::GetBadSyntaxList(
    bool refresh, Strip strip) {
  if (!bad_syntax_list_) {
    bad_syntax_list_ = new SyntaxListType;
    refresh = true;  // always refresh the first time
  }

  if (refresh) {
    const NameListType& the_list = TemplateNamelist::GetList();

    bad_syntax_list_->clear();

    const MissingListType& missing_list = GetMissingList(true);
    for (NameListType::const_iterator iter = the_list.begin();
         iter != the_list.end();
         ++iter) {
      Template *tpl = Template::GetTemplate((*iter), strip);
      if (!tpl) {
        if (!binary_search(missing_list.begin(), missing_list.end(), *iter)) {
          // If it's not in the missing list, then we're here because
          // it caused an error during parsing
          bad_syntax_list_->push_back(*iter);
          LOG(ERROR) << "Error loading template: " << (*iter) << "\n";
        }
      }
    }
  }
  return *bad_syntax_list_;
}

// Look at all the existing template files, and get their lastmod time via stat()
time_t TemplateNamelist::GetLastmodTime() {
  time_t retval = -1;

  const NameListType& the_list = TemplateNamelist::GetList();
  for (NameListType::const_iterator iter = the_list.begin();
       iter != the_list.end();
       ++iter) {
    // Only prepend root_dir if *iter isn't an absolute path:
    const string path = Template::FindTemplateFilename(*iter);
    struct stat statbuf;
    if (path.empty() || stat(path.c_str(), &statbuf) != 0)
      continue;  // ignore files we can't find
    retval = max(retval, statbuf.st_mtime);
  }
  return retval;
}

// AllDoExist
bool TemplateNamelist::AllDoExist() {
  // AllDoExist always refreshes the list, hence the "true"
  const MissingListType& missing_list = TemplateNamelist::GetMissingList(true);
  return missing_list.empty();
}

// IsAllSyntaxOkay
bool TemplateNamelist::IsAllSyntaxOkay(Strip strip) {
  // IsAllSyntaxOkay always refreshes the list, hence the "true"
  const SyntaxListType& bad_syntax_list =
    TemplateNamelist::GetBadSyntaxList(true, strip);
  return bad_syntax_list.empty();
}

}
