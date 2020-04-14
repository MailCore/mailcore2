// Copyright (c) 2008, Google Inc.
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
// Author: Craig Silverstein
//
// Most other tests use "config_for_unittests.h" to make testing easier.
// This brings in some code most users won't use.  This test is meant
// entirely to use ctemplate as users will, just #including the public
// .h files directly.  It does hardly any work, and is mainly intended
// as a compile check for the .h files.  It will not work if you use
// a non-standard name for the package namespace (via
//     ./configure --enable-namespace=foo
// ), though you can fix that by changing the namespace alias below.

// These are all the .h files that we export
#include <ctemplate/per_expand_data.h>
#include <ctemplate/template.h>
#include <ctemplate/template_dictionary.h>
#include <ctemplate/template_dictionary_interface.h>
#include <ctemplate/template_emitter.h>
#include <ctemplate/template_enums.h>
#include <ctemplate/template_modifiers.h>
#include <ctemplate/template_namelist.h>
#include <ctemplate/template_pathops.h>
#include <ctemplate/template_string.h>
#include <stdio.h>
#include <string>

// If you used ./configure --enable-namespace=foo, replace 'ctemplate'
// here with 'foo'.
namespace template_ns = ctemplate;

int main() {
  template_ns::Template::StringToTemplateCache("key", "example");
  template_ns::Template* tpl = template_ns::Template::GetTemplate(
      "key", template_ns::DO_NOT_STRIP);
  template_ns::TemplateDictionary dict("my dict");
  std::string nothing_will_come_of_nothing;
  tpl->Expand(&nothing_will_come_of_nothing, &dict);

  // Try using a bit more functionality.
  template_ns::PerExpandData data;
  nothing_will_come_of_nothing.clear();
  template_ns::ExpandWithData("key", template_ns::DO_NOT_STRIP, &dict, &data,
                              &nothing_will_come_of_nothing);

  printf("PASS.\n");
  return 0;
}
