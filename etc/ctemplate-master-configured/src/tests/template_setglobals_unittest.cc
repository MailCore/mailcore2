// Copyright (c) 2002, Google Inc.
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
#include <assert.h>
#include <stdio.h>
#include <ctemplate/template.h>
#include <ctemplate/template_dictionary.h>
#include "tests/template_test_util.h"
#include "base/util.h"
TEST_INIT   // defines RUN_ALL_TESTS()

using ctemplate::Template;
using ctemplate::TemplateDictionary;

TEST(SetGlobalValue, TemplateDictionary) {
  // Test to see that the global dictionary object gets created when you
  // first call the static function TemplateDictionary::SetGlobalValue().
  TemplateDictionary::SetGlobalValue("TEST_GLOBAL_VAR", "test_value");
  TemplateDictionary tpl("empty");
  ctemplate::TemplateDictionaryPeer peer(&tpl);
  EXPECT_STREQ(peer.GetSectionValue("TEST_GLOBAL_VAR"),
               "test_value");

}

TEST(SetGlobalValue, SetRootDirectory) {
  // Test to see that the Template static variables get created when you
  // first call the static function Template::SetRootDirectory().
  Template::SetTemplateRootDirectory("/some/directory/path");
  // We don't know if we appended a / or a \, so we test indirectly
  EXPECT_EQ(strlen("/some/directory/path")+1,   // assert they added a char
            Template::template_root_directory().size());
  EXPECT_EQ(0, memcmp(Template::template_root_directory().c_str(),
                      "/some/directory/path",
                      strlen("/some/directory/path")));
}

int main(int argc, char **argv) {

  return RUN_ALL_TESTS();
}
