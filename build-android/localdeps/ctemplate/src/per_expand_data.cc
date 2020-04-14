// Copyright (c) 2009, Google Inc.
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
// This contains some implementation of PerExpandData that is still simple
// but is not conveniently defined in the header file, e.g., because it would
// introduce new include dependencies.

#include <config.h>
#include <ctemplate/find_ptr.h>
#include <ctemplate/per_expand_data.h>
#include <ctemplate/template_annotator.h>

namespace ctemplate {

using std::string;

bool PerExpandData::DataEq::operator()(const char* s1, const char* s2) const {
  return ((s1 == 0 && s2 == 0) ||
          (s1 && s2 && *s1 == *s2 && strcmp(s1, s2) == 0));
}

PerExpandData::~PerExpandData() {
  delete map_;
}

TemplateAnnotator* PerExpandData::annotator() const {
  if (annotator_ != NULL) {
    return annotator_;
  }
  // TextTemplateAnnotator has no static state.  So direct static definition
  // should be safe.
  static TextTemplateAnnotator g_default_annotator;
  return &g_default_annotator;
}

void PerExpandData::InsertForModifiers(const char* key, const void* value) {
  if (!map_)
    map_ = new DataMap;
  (*map_)[key] = value;
}

  // Retrieve data specific to this Expand call. Returns NULL if key
  // is not found.  This should only be used by template modifiers.
const void* PerExpandData::LookupForModifiers(const char* key) const {
  return map_ ? find_ptr2(*map_, key) : NULL;
}

}
