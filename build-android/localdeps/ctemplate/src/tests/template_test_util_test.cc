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

#include "config_for_unittests.h"
#include "tests/template_test_util.h"

#include <stdio.h>
#include <string>
#include <vector>

#include "base/arena.h"
#include <ctemplate/template_dictionary.h>
#include <ctemplate/template_string.h>
#include "base/util.h"
TEST_INIT   // defines RUN_ALL_TESTS()

#define ASSERT_EQ(a, b)  EXPECT_EQ(a, b)

using std::vector;
using std::string;
using ctemplate::UnsafeArena;

using ctemplate::TemplateDictionary;
using ctemplate::TemplateDictionaryPeer;
using ctemplate::TemplateString;
using ctemplate::StaticTemplateString;

namespace {

TEST(TemplateTestUtilTest, GetSectionValue) {
  TemplateDictionary dict("test_GetSectionValue");
  dict.SetValue("VALUE", "value");

  TemplateDictionaryPeer peer(&dict);
  EXPECT_STREQ("value", peer.GetSectionValue("VALUE"));
}

TEST(TemplateTestUtilTest, IsHiddenSection) {
  TemplateDictionary dict("test_IsHiddenSection");

  {
    TemplateDictionaryPeer peer(&dict);
    EXPECT_TRUE(peer.IsHiddenSection("SECTION"));
  }

  dict.AddSectionDictionary("SECTION");

  {
    TemplateDictionaryPeer peer(&dict);
    EXPECT_FALSE(peer.IsHiddenSection("SECTION"));
  }
}

TEST(TemplateTestUtilTest, GetSectionDictionaries) {
  TemplateDictionary dict("test_GetSectionDictionaries");

  {
    TemplateDictionaryPeer peer(&dict);
    vector<const TemplateDictionary*> dicts;
    // Add some dummy value into the vector to confirm that the call to
    // GetSectionDictionaries will correctly clear the vector.
    dicts.push_back(NULL);
    EXPECT_EQ(0, peer.GetSectionDictionaries("SECTION", &dicts));
    EXPECT_TRUE(dicts.empty());
  }

  dict.AddSectionDictionary("SECTION")->SetValue("SECTION_VALUE", "0");

  {
    TemplateDictionaryPeer peer(&dict);
    vector<const TemplateDictionary*> dicts;
    ASSERT_EQ(1, peer.GetSectionDictionaries("SECTION", &dicts));

    TemplateDictionaryPeer peer_section(dicts[0]);
    EXPECT_STREQ("0", peer_section.GetSectionValue("SECTION_VALUE"));
  }

  dict.AddSectionDictionary("SECTION")->SetValue("SECTION_VALUE", "1");
  dict.AddSectionDictionary("ANOTHER_SECTION")->SetValue("ANOTHER_VALUE", "2");

  {
    TemplateDictionaryPeer peer(&dict);
    vector<const TemplateDictionary*> dicts;
    ASSERT_EQ(2, peer.GetSectionDictionaries("SECTION", &dicts));

    TemplateDictionaryPeer peer_section0(dicts[0]);
    EXPECT_STREQ("0", peer_section0.GetSectionValue("SECTION_VALUE"));

    TemplateDictionaryPeer peer_section1(dicts[1]);
    EXPECT_STREQ("1", peer_section1.GetSectionValue("SECTION_VALUE"));
  }
}

TEST(TemplateTestUtilTest, GetIncludeDictionaries) {
  TemplateDictionary dict("test_GetIncludeDictionaries");

  {
    TemplateDictionaryPeer peer(&dict);
    vector<const TemplateDictionary*> dicts;
    // Add some dummy value into the vector to confirm that the call to
    // GetSectionDictionaries will correctly clear the vector.
    dicts.push_back(NULL);
    EXPECT_EQ(0, peer.GetIncludeDictionaries("SECTION", &dicts));
    EXPECT_TRUE(dicts.empty());
  }

  dict.AddIncludeDictionary("SECTION")->SetValue("SECTION_VALUE", "0");

  {
    TemplateDictionaryPeer peer(&dict);
    vector<const TemplateDictionary*> dicts;
    ASSERT_EQ(1, peer.GetIncludeDictionaries("SECTION", &dicts));

    TemplateDictionaryPeer peer_section(dicts[0]);
    EXPECT_STREQ("0", peer_section.GetSectionValue("SECTION_VALUE"));
  }

  dict.AddIncludeDictionary("SECTION")->SetValue("SECTION_VALUE", "1");
  dict.AddIncludeDictionary("ANOTHER_SECTION")->SetValue("ANOTHER_VALUE", "2");

  {
    TemplateDictionaryPeer peer(&dict);
    vector<const TemplateDictionary*> dicts;
    ASSERT_EQ(2, peer.GetIncludeDictionaries("SECTION", &dicts));

    TemplateDictionaryPeer peer_section0(dicts[0]);
    EXPECT_STREQ("0", peer_section0.GetSectionValue("SECTION_VALUE"));

    TemplateDictionaryPeer peer_section1(dicts[1]);
    EXPECT_STREQ("1", peer_section1.GetSectionValue("SECTION_VALUE"));
  }
}

TEST(TemplateTestUtilTest, GetIncludeAndSectionDictionaries) {
  TemplateDictionary dict("test_GetIncludeAndSectionDictionaries");

  {
    TemplateDictionaryPeer peer(&dict);
    vector<const TemplateDictionary*> dicts;
    EXPECT_EQ(0, peer.GetIncludeDictionaries("SECTION", &dicts));
    EXPECT_EQ(0, peer.GetSectionDictionaries("SECTION", &dicts));
  }

  dict.AddIncludeDictionary("SECTION")->SetValue("SECTION_VALUE", "0");
  dict.AddSectionDictionary("SECTION")->SetValue("SECTION_VALUE", "1");

  {
    TemplateDictionaryPeer peer(&dict);
    vector<const TemplateDictionary*> include_dicts;
    ASSERT_EQ(1, peer.GetIncludeDictionaries("SECTION", &include_dicts));

    TemplateDictionaryPeer include_peer(include_dicts[0]);
    EXPECT_STREQ("0", include_peer.GetSectionValue("SECTION_VALUE"));

    vector<const TemplateDictionary*> section_dicts;
    ASSERT_EQ(1, peer.GetSectionDictionaries("SECTION", &section_dicts));

    TemplateDictionaryPeer section_peer(section_dicts[0]);
    EXPECT_STREQ("1", section_peer.GetSectionValue("SECTION_VALUE"));
  }

  dict.AddIncludeDictionary("SECTION")->SetValue("SECTION_VALUE", "2");
  dict.AddIncludeDictionary("ANOTHER_SECTION")->SetValue("ANOTHER_VALUE", "3");

  dict.AddSectionDictionary("SECTION")->SetValue("SECTION_VALUE", "4");
  dict.AddSectionDictionary("ONE_MORE_SECTION")->SetValue("ANOTHER_VALUE", "5");

  {
    TemplateDictionaryPeer peer(&dict);
    vector<const TemplateDictionary*> dicts;
    ASSERT_EQ(2, peer.GetIncludeDictionaries("SECTION", &dicts));

    TemplateDictionaryPeer include_peer0(dicts[0]);
    EXPECT_STREQ("0", include_peer0.GetSectionValue("SECTION_VALUE"));

    TemplateDictionaryPeer include_peer1(dicts[1]);
    EXPECT_STREQ("2", include_peer1.GetSectionValue("SECTION_VALUE"));

    EXPECT_EQ(1, peer.GetIncludeDictionaries("ANOTHER_SECTION", &dicts));
    EXPECT_EQ(0, peer.GetIncludeDictionaries("ONE_MORE_SECTION", &dicts));

    vector<const TemplateDictionary*> section_dicts;
    ASSERT_EQ(2, peer.GetSectionDictionaries("SECTION", &section_dicts));

    TemplateDictionaryPeer section_peer0(section_dicts[0]);
    EXPECT_STREQ("1", section_peer0.GetSectionValue("SECTION_VALUE"));

    TemplateDictionaryPeer section_peer1(section_dicts[1]);
    EXPECT_STREQ("4", section_peer1.GetSectionValue("SECTION_VALUE"));

    EXPECT_EQ(0, peer.GetSectionDictionaries("ANOTHER_SECTION", &dicts));
    EXPECT_EQ(1, peer.GetSectionDictionaries("ONE_MORE_SECTION", &dicts));
  }
}

TEST(TemplateTestUtilTest, GetFilename) {
  TemplateDictionary parent("test_GetFilename");
  TemplateDictionary* child = parent.AddIncludeDictionary("INCLUDE_marker");
  child->SetFilename("included_filename");

  TemplateDictionaryPeer parent_peer(&parent);
  EXPECT_EQ(NULL, parent_peer.GetFilename());

  TemplateDictionaryPeer child_peer(child);
  EXPECT_STREQ("included_filename", child_peer.GetFilename());
}

StaticTemplateString GetTestTemplateString(UnsafeArena* arena) {
  string will_go_out_of_scope("VALUE");
  // We want to ensure that the STS_INIT_FOR_TEST macro:
  // - Can produce a StaticTemplateString (guard again its format changing).
  // - Produces a StaticTemplateString that is still valid after the string
  //   used to initialize it goes out-of-scope.
  StaticTemplateString sts = STS_INIT_FOR_TEST(will_go_out_of_scope.c_str(),
                                               will_go_out_of_scope.length(),
                                               arena);
  return sts;
}

TEST(TemplateUtilTest, InitStaticTemplateStringForTest) {
  UnsafeArena arena(1024);
  StaticTemplateString kValue = GetTestTemplateString(&arena);

  TemplateDictionary dict("test_GetSectionValue");
  dict.SetValue(kValue, "value");

  TemplateDictionaryPeer peer(&dict);
  EXPECT_STREQ("value", peer.GetSectionValue(kValue));
}

}  // namespace anonymous

int main(int argc, char **argv) {

  return RUN_ALL_TESTS();
}
