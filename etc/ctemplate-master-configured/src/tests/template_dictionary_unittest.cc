// Copyright (c) 2005, Google Inc.
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
// This code is written to not use the google testing framework
// as much as possible, to make it easier to opensource.

#include "config_for_unittests.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include "base/arena.h"
#include <ctemplate/template_dictionary.h>
#include <ctemplate/template_modifiers.h>
#include <ctemplate/per_expand_data.h>
#include "tests/template_test_util.h"
#include "base/util.h"
TEST_INIT               // defines RUN_ALL_TESTS

using std::string;
using std::vector;
using ctemplate::UnsafeArena;
using ctemplate::DO_NOT_STRIP;
using ctemplate::ExpandEmitter;
using ctemplate::PerExpandData;
using ctemplate::StaticTemplateString;
using ctemplate::StringToTemplateCache;
using ctemplate::TemplateDictionary;
using ctemplate::TemplateDictionaryInterface;
using ctemplate::TemplateDictionaryPeer;
using ctemplate::TemplateString;

#define ASSERT_STRSTR(text, substr)  do {                       \
  if (!strstr((text), (substr))) {                              \
    printf("%s: %d: ASSERT FAILED: '%s' not in '%s'\n",         \
           __FILE__, __LINE__, (substr), (text));               \
    assert(strstr((text), (substr)));                           \
    exit(1);                                                    \
  }                                                             \
} while (0)


// test escape-functor that replaces all input with "foo"
class FooEscaper : public ctemplate::TemplateModifier {
 public:
  void Modify(const char* in, size_t inlen,
              const PerExpandData*,
              ExpandEmitter* outbuf, const string& arg) const {
    assert(arg.empty());    // we don't take an argument
    outbuf->Emit("foo");
  }
};

// test escape-functor that replaces all input with ""
class NullEscaper : public ctemplate::TemplateModifier {
 public:
  void Modify(const char* in, size_t inlen,
              const PerExpandData*,
              ExpandEmitter* outbuf, const string& arg) const {
    assert(arg.empty());    // we don't take an argument
  }
};

// first does javascript-escaping, then html-escaping
class DoubleEscaper : public ctemplate::TemplateModifier {
 public:
  void Modify(const char* in, size_t inlen,
              const PerExpandData* data,
              ExpandEmitter* outbuf, const string& arg) const {
    assert(arg.empty());    // we don't take an argument
    string tmp = ctemplate::javascript_escape(in, inlen);
    ctemplate::html_escape.Modify(tmp.data(), tmp.size(), data, outbuf, "");
  }
};

namespace {

static const TemplateDictionary* GetSectionDict(
    const TemplateDictionary* d, const char* name, int i) {
  TemplateDictionaryPeer peer(d);
  vector<const TemplateDictionary*> dicts;
  EXPECT_GE(peer.GetSectionDictionaries(name, &dicts), i);
  return dicts[i];
}
static const TemplateDictionary* GetIncludeDict(
    const TemplateDictionary* d, const char* name, int i) {
  TemplateDictionaryPeer peer(d);
  vector<const TemplateDictionary*> dicts;
  EXPECT_GE(peer.GetIncludeDictionaries(name, &dicts), i);
  return dicts[i];
}

static void SetUp() {
  TemplateDictionary::SetGlobalValue("GLOBAL", "top");
}

TEST(TemplateDictionary, SetValueAndTemplateStringAndArena) {
  // Try both with the arena, and without.
  UnsafeArena arena(100);
  // We run the test with arena twice to double-check we don't ever delete it
  UnsafeArena* arenas[] = {&arena, &arena, NULL};
  for (int i = 0; i < sizeof(arenas)/sizeof(*arenas); ++i) {
    TemplateDictionary dict(string("test_arena") + char('0'+i), arenas[i]);

    // Test copying char*s, strings, and explicit TemplateStrings
    dict.SetValue("FOO", "foo");
    dict.SetValue(string("FOO2"), TemplateString("foo2andmore", 4));
		dict["FOO3"] = "foo3";
		dict[string("FOO4")] = TemplateString("foo4andmore", 4);
		dict["FOO5"] = string("Olaf");
		dict["FOO6"] = 6;
		dict["FOO7"] = long(7);

    TemplateDictionaryPeer peer(&dict);
    // verify what happened
    EXPECT_TRUE(peer.ValueIs("FOO", "foo"));
    EXPECT_TRUE(peer.ValueIs("FOO2", "foo2"));
    string dump;
    dict.DumpToString(&dump);
    char expected[256];
    snprintf(expected, sizeof(expected),
             ("global dictionary {\n"
              "   BI_NEWLINE: >\n"
              "<\n"
              "   BI_SPACE: > <\n"
              "   GLOBAL: >top<\n"
              "};\n"
              "dictionary 'test_arena%d' {\n"
              "   FOO: >foo<\n"
              "   FOO2: >foo2<\n"
              "   FOO3: >foo3<\n"
              "   FOO4: >foo4<\n"
              "   FOO5: >Olaf<\n"
              "   FOO6: >6<\n"
              "   FOO7: >7<\n"
              "}\n"), i);
    EXPECT_STREQ(dump.c_str(), expected);
  }
}

TEST(TemplateDictionary, SetValueWithoutCopy) {
  UnsafeArena arena(100);
  TemplateDictionary dict("Test arena", &arena);

  char value[32];
  snprintf(value, sizeof(value), "%s", "value");

  const void* const ptr = arena.Alloc(0);
  dict.SetValueWithoutCopy("key", value);
  // We shouldn't have copied the value string.
  EXPECT_EQ(ptr, arena.Alloc(0));

  TemplateDictionaryPeer peer(&dict);
  EXPECT_TRUE(peer.ValueIs("key", "value"));
  // If our content changes, so does what's in the dictionary -- but
  // only the contents of the buffer, not its length!
  snprintf(value, sizeof(value), "%s", "not_value");
  EXPECT_TRUE(peer.ValueIs("key", "not_v"));   // sizeof("not_v") == sizeof("value")
}

TEST(TemplateDictionary, SetIntValue) {
  TemplateDictionary dict("test_SetIntValue", NULL);
  TemplateDictionaryPeer peer(&dict);

  dict.SetIntValue("INT", 5);
  // - is an illegal varname in templates, but perfectly fine in dicts
  dict.SetIntValue("-INT", -5);

  EXPECT_TRUE(peer.ValueIs("INT", "5"));
  EXPECT_TRUE(peer.ValueIs("-INT", "-5"));
  string dump;
  dict.DumpToString(&dump);
  ASSERT_STRSTR(dump.c_str(), "\n   INT: >5<\n");
  ASSERT_STRSTR(dump.c_str(), "\n   -INT: >-5<\n");

}

TEST(TemplateDictionary, SetFormattedValue) {
  TemplateDictionary dict("test_SetFormattedValue", NULL);
  TemplateDictionaryPeer peer(&dict);

  dict.SetFormattedValue(TemplateString("PRINTF", sizeof("PRINTF")-1),
                         "%s test %04d", "template test", 1);

  EXPECT_TRUE(peer.ValueIs("PRINTF", "template test test 0001"));
  string dump;
  dict.DumpToString(&dump);
  ASSERT_STRSTR(dump.c_str(), "\n   PRINTF: >template test test 0001<\n");

  // Now test something of size 4k or so, where we can't use scratchbuf
  dict.SetFormattedValue(TemplateString("PRINTF", sizeof("PRINTF")-1),
                         "%s test %04444d", "template test", 2);
  string expected("template test test ");
  for (int i = 0; i < 4443; ++i)
    expected.append("0");
  expected.append("2");
  EXPECT_TRUE(peer.ValueIs("PRINTF", expected));
  string dump2;
  dict.DumpToString(&dump2);
  expected = string("\n   PRINTF: >") + expected + string("<\n");
  ASSERT_STRSTR(dump2.c_str(), expected.c_str());
}

TEST(TemplateDictionary, SetEscapedValue) {
  TemplateDictionary dict("test_SetEscapedValue", NULL);
  TemplateDictionaryPeer peer(&dict);

  dict.SetEscapedValue("hardest HTML",
                       "<A HREF='foo'\nid=\"bar\t\t&&\vbaz\">",
                       ctemplate::html_escape);
  dict.SetEscapedValue("hardest JS",
                       ("f = 'foo';\r\n\tprint \"\\&foo = \b\", \"foo\""),
                       ctemplate::javascript_escape);
  dict.SetEscapedValue("query escape 0", "",
                       ctemplate::url_query_escape);

  EXPECT_TRUE(peer.ValueIs("hardest HTML",
                           "&lt;A HREF=&#39;foo&#39; id=&quot;bar  &amp;&amp; "
                           "baz&quot;&gt;"));
  EXPECT_TRUE(peer.ValueIs("hardest JS",
                           "f \\x3d \\x27foo\\x27;\\r\\n\\tprint \\x22\\\\\\x26"
                           "foo \\x3d \\b\\x22, \\x22foo\\x22"));
  EXPECT_TRUE(peer.ValueIs("query escape 0", ""));

  // Test using hand-made modifiers.
  FooEscaper foo_escaper;
  dict.SetEscapedValue("easy foo", "hello there!",
                       FooEscaper());
  dict.SetEscapedValue("harder foo", "so much to say\nso many foos",
                       foo_escaper);
  DoubleEscaper double_escaper;
  dict.SetEscapedValue("easy double", "doo",
                       double_escaper);
  dict.SetEscapedValue("harder double", "<A HREF='foo'>\n",
                       DoubleEscaper());
  dict.SetEscapedValue("hardest double",
                       "print \"<A HREF='foo'>\";\r\n\\1;",
                       double_escaper);

  EXPECT_TRUE(peer.ValueIs("easy foo", "foo"));
  EXPECT_TRUE(peer.ValueIs("harder foo", "foo"));
  EXPECT_TRUE(peer.ValueIs("easy double", "doo"));
  EXPECT_TRUE(peer.ValueIs("harder double",
                           "\\x3cA HREF\\x3d\\x27foo\\x27\\x3e\\n"));
  EXPECT_TRUE(peer.ValueIs("hardest double",
                           "print \\x22\\x3cA HREF\\x3d\\x27foo\\x27\\x3e\\x22;"
                           "\\r\\n\\\\1;"));
}

TEST(TemplateDictionary, SetEscapedFormattedValue) {
  TemplateDictionary dict("test_SetEscapedFormattedValue", NULL);
  TemplateDictionaryPeer peer(&dict);

  dict.SetEscapedFormattedValue("HTML", ctemplate::html_escape,
                                "This is <%s> #%.4f", "a & b", 1.0/3);
  dict.SetEscapedFormattedValue("PRE", ctemplate::pre_escape,
                                "if %s x = %.4f;", "(a < 1 && b > 2)\n\t", 1.0/3);
  dict.SetEscapedFormattedValue("URL", ctemplate::url_query_escape,
                                "pageviews-%s", "r?egex");
  dict.SetEscapedFormattedValue("XML", ctemplate::xml_escape,
                                "This&is%s -- ok?", "just&");

  EXPECT_TRUE(peer.ValueIs("HTML",
                           "This is &lt;a &amp; b&gt; #0.3333"));
  EXPECT_TRUE(peer.ValueIs("PRE",
                           "if (a &lt; 1 &amp;&amp; b &gt; 2)\n\t x = 0.3333;"));
  EXPECT_TRUE(peer.ValueIs("URL", "pageviews-r%3Fegex"));

  EXPECT_TRUE(peer.ValueIs("XML", "This&amp;isjust&amp; -- ok?"));
}

static const StaticTemplateString kSectName =
    STS_INIT(kSectName, "test_SetAddSectionDictionary");

TEST(TemplateDictionary, AddSectionDictionary) {
  // For fun, we'll make this constructor take a static template string.
  TemplateDictionary dict(kSectName, NULL);
  TemplateDictionaryPeer peer(&dict);
  dict.SetValue("TOPLEVEL", "foo");
  dict.SetValue("TOPLEVEL2", "foo2");

  TemplateDictionary* subdict_1a = dict.AddSectionDictionary("section1");
  // This is the same dict, but name is specified a different way.
  TemplateDictionary* subdict_1b = dict.AddSectionDictionary(
      TemplateString("section1__ignored__", strlen("section1")));
  TemplateDictionaryPeer subdict_1a_peer(subdict_1a);
  TemplateDictionaryPeer subdict_1b_peer(subdict_1b);
  subdict_1a->SetValue("SUBLEVEL", "subfoo");
  subdict_1b->SetValue("SUBLEVEL", "subbar");

  TemplateDictionary* subdict_2 = dict.AddSectionDictionary("section2");
  TemplateDictionaryPeer subdict_2_peer(subdict_2);
  subdict_2->SetValue("TOPLEVEL", "bar");    // overriding top dict
  TemplateDictionary* subdict_2_1 = subdict_2->AddSectionDictionary("sub");
  TemplateDictionaryPeer subdict_2_1_peer(subdict_2_1);
  subdict_2_1->SetIntValue("GLOBAL", 21);    // overrides value in setUp()

  // Verify that all variables that should be look-up-able are, and that
  // we have proper precedence.
  EXPECT_TRUE(peer.ValueIs("GLOBAL", "top"));
  EXPECT_TRUE(peer.ValueIs("TOPLEVEL", "foo"));
  EXPECT_TRUE(peer.ValueIs("TOPLEVEL2", "foo2"));
  EXPECT_TRUE(peer.ValueIs("SUBLEVEL", ""));

  EXPECT_TRUE(subdict_1a_peer.ValueIs("GLOBAL", "top"));
  EXPECT_TRUE(subdict_1a_peer.ValueIs("TOPLEVEL", "foo"));
  EXPECT_TRUE(subdict_1a_peer.ValueIs("TOPLEVEL2", "foo2"));
  EXPECT_TRUE(subdict_1a_peer.ValueIs("SUBLEVEL", "subfoo"));

  EXPECT_TRUE(subdict_1b_peer.ValueIs("GLOBAL", "top"));
  EXPECT_TRUE(subdict_1b_peer.ValueIs("TOPLEVEL", "foo"));
  EXPECT_TRUE(subdict_1b_peer.ValueIs("TOPLEVEL2", "foo2"));
  EXPECT_TRUE(subdict_1b_peer.ValueIs("SUBLEVEL", "subbar"));

  EXPECT_TRUE(subdict_2_peer.ValueIs("GLOBAL", "top"));
  EXPECT_TRUE(subdict_2_peer.ValueIs("TOPLEVEL", "bar"));
  EXPECT_TRUE(subdict_2_peer.ValueIs("TOPLEVEL2", "foo2"));
  EXPECT_TRUE(subdict_2_peer.ValueIs("SUBLEVEL", ""));

  EXPECT_TRUE(subdict_2_1_peer.ValueIs("GLOBAL", "21"));
  EXPECT_TRUE(subdict_2_1_peer.ValueIs("TOPLEVEL", "bar"));
  EXPECT_TRUE(subdict_2_1_peer.ValueIs("TOPLEVEL2", "foo2"));
  EXPECT_TRUE(subdict_2_1_peer.ValueIs("SUBLEVEL", ""));

  // Verify that everyone knows about its sub-dictionaries, and also
  // that these go 'up the chain' on lookup failure
  EXPECT_FALSE(peer.IsHiddenSection("section1"));
  EXPECT_FALSE(peer.IsHiddenSection("section2"));
  EXPECT_TRUE(peer.IsHiddenSection("section3"));
  EXPECT_TRUE(peer.IsHiddenSection("sub"));
  EXPECT_FALSE(subdict_1a_peer.IsHiddenSection("section1"));
  EXPECT_TRUE(subdict_1a_peer.IsHiddenSection("sub"));
  EXPECT_FALSE(subdict_2_peer.IsHiddenSection("sub"));
  EXPECT_FALSE(subdict_2_1_peer.IsHiddenSection("sub"));

  // We should get the dictionary-lengths right as well
  vector<const TemplateDictionary*> dummy;
  EXPECT_EQ(2, peer.GetSectionDictionaries("section1", &dummy));
  EXPECT_EQ(1, peer.GetSectionDictionaries("section2", &dummy));
  EXPECT_EQ(1, subdict_2_peer.GetSectionDictionaries("sub", &dummy));
  // Test some of the values
  EXPECT_TRUE(TemplateDictionaryPeer(GetSectionDict(&dict, "section1", 0))
              .ValueIs("SUBLEVEL", "subfoo"));
  EXPECT_TRUE(TemplateDictionaryPeer(GetSectionDict(&dict, "section1", 1))
              .ValueIs("SUBLEVEL", "subbar"));
  EXPECT_TRUE(TemplateDictionaryPeer(GetSectionDict(&dict, "section2", 0))
              .ValueIs("TOPLEVEL", "bar"));
  EXPECT_TRUE(TemplateDictionaryPeer(
      GetSectionDict(GetSectionDict(&dict, "section2", 0), "sub", 0))
              .ValueIs("TOPLEVEL", "bar"));
  EXPECT_TRUE(TemplateDictionaryPeer(
      GetSectionDict(GetSectionDict(&dict, "section2", 0), "sub", 0))
              .ValueIs("GLOBAL", "21"));

  // Make sure we're making descriptive names
  EXPECT_STREQ(dict.name().c_str(),
               "test_SetAddSectionDictionary");
  EXPECT_STREQ(subdict_1a->name().c_str(),
               "test_SetAddSectionDictionary/section1#1");
  EXPECT_STREQ(subdict_1b->name().c_str(),
               "test_SetAddSectionDictionary/section1#2");
  EXPECT_STREQ(subdict_2->name().c_str(),
               "test_SetAddSectionDictionary/section2#1");
  EXPECT_STREQ(subdict_2_1->name().c_str(),
               "test_SetAddSectionDictionary/section2#1/sub#1");

  // Finally, we can test the whole kit and kaboodle
  string dump;
  dict.DumpToString(&dump);
  const char* const expected =
    ("global dictionary {\n"
     "   BI_NEWLINE: >\n"
     "<\n"
     "   BI_SPACE: > <\n"
     "   GLOBAL: >top<\n"
     "};\n"
     "dictionary 'test_SetAddSectionDictionary' {\n"
     "   TOPLEVEL: >foo<\n"
     "   TOPLEVEL2: >foo2<\n"
     "   section section1 (dict 1 of 2) -->\n"
     "     dictionary 'test_SetAddSectionDictionary/section1#1' {\n"
     "       SUBLEVEL: >subfoo<\n"
     "     }\n"
     "   section section1 (dict 2 of 2) -->\n"
     "     dictionary 'test_SetAddSectionDictionary/section1#2' {\n"
     "       SUBLEVEL: >subbar<\n"
     "     }\n"
     "   section section2 (dict 1 of 1) -->\n"
     "     dictionary 'test_SetAddSectionDictionary/section2#1' {\n"
     "       TOPLEVEL: >bar<\n"
     "       section sub (dict 1 of 1) -->\n"
     "         dictionary 'test_SetAddSectionDictionary/section2#1/sub#1' {\n"
     "           GLOBAL: >21<\n"
     "         }\n"
     "     }\n"
     "}\n");
  EXPECT_STREQ(dump.c_str(), expected);
}

TEST(TemplateDictionary, ShowSection) {
  TemplateDictionary dict("test_SetShowSection", NULL);
  // Let's say what filename dict is associated with
  dict.SetFilename("bigmamainclude!.tpl");
  dict.SetValue("TOPLEVEL", "foo");
  dict.SetValue("TOPLEVEL2", "foo2");
  dict.ShowSection("section1");
  dict.ShowSection("section2");
  // Test calling ShowSection twice on the same section
  dict.ShowSection("section2");
  // Test that ShowSection is a no-op if called after AddSectionDictionary()
  TemplateDictionary* subdict = dict.AddSectionDictionary("section3");
  TemplateDictionaryPeer subdict_peer(subdict);
  subdict->SetValue("TOPLEVEL", "bar");
  dict.ShowSection("section3");

  EXPECT_TRUE(subdict_peer.ValueIs("TOPLEVEL", "bar"));

  // Since ShowSection() doesn't return a sub-dict, the only way to
  // probe what the dicts look like is via Dump()
  string dump;
  dict.DumpToString(&dump);
  const char* const expected =
    ("global dictionary {\n"
     "   BI_NEWLINE: >\n"
     "<\n"
     "   BI_SPACE: > <\n"
     "   GLOBAL: >top<\n"
     "};\n"
     "dictionary 'test_SetShowSection (intended for bigmamainclude!.tpl)' {\n"
     "   TOPLEVEL: >foo<\n"
     "   TOPLEVEL2: >foo2<\n"
     "   section section1 (dict 1 of 1) -->\n"
     "     dictionary 'empty dictionary' {\n"
     "     }\n"
     "   section section2 (dict 1 of 1) -->\n"
     "     dictionary 'empty dictionary' {\n"
     "     }\n"
     "   section section3 (dict 1 of 1) -->\n"
     "     dictionary 'test_SetShowSection/section3#1' {\n"
     "       TOPLEVEL: >bar<\n"
     "     }\n"
     "}\n");
  EXPECT_STREQ(dump.c_str(), expected);
}

TEST(TemplateDictionary, SetValueAndShowSection) {
  TemplateDictionary dict("test_SetValueAndShowSection");
  TemplateDictionaryPeer peer(&dict);
  dict.SetValue("TOPLEVEL", "foo");

  dict.SetValueAndShowSection("INSEC", "bar", "SEC1");
  dict.SetValueAndShowSection("NOTINSEC", "", "SEC2");
  dict.SetValueAndShowSection("NOTINSEC2", NULL, "SEC3");

  EXPECT_FALSE(peer.IsHiddenSection("SEC1"));
  EXPECT_TRUE(peer.IsHiddenSection("SEC2"));
  EXPECT_TRUE(peer.IsHiddenSection("SEC3"));

  // Again, we don't get subdicts, so we have to dump to check values
  string dump;
  dict.DumpToString(&dump);
  const char* const expected =
    ("global dictionary {\n"
     "   BI_NEWLINE: >\n"
     "<\n"
     "   BI_SPACE: > <\n"
     "   GLOBAL: >top<\n"
     "};\n"
     "dictionary 'test_SetValueAndShowSection' {\n"
     "   TOPLEVEL: >foo<\n"
     "   section SEC1 (dict 1 of 1) -->\n"
     "     dictionary 'test_SetValueAndShowSection/SEC1#1' {\n"
     "       INSEC: >bar<\n"
     "     }\n"
     "}\n");
  EXPECT_STREQ(dump.c_str(), expected);
}

TEST(TemplateDictionary, SetTemplateGlobalValue) {
  // The functionality involving it passing across the included dictionaries
  // is also tested in TestAddIncludeDictionary
  TemplateDictionary dict("test_SetTemplateGlobalValue", NULL);
  TemplateDictionary* subdict = dict.AddSectionDictionary("section1");
  TemplateDictionary* subsubdict =
    subdict->AddSectionDictionary("section1's child");
  TemplateDictionary* includedict = dict.AddIncludeDictionary("include1");

  TemplateDictionaryPeer peer(&dict);
  TemplateDictionaryPeer subdict_peer(subdict);
  TemplateDictionaryPeer subsubdict_peer(subsubdict);
  TemplateDictionaryPeer includedict_peer(includedict);

  // Setting a template value after sub dictionaries are created should
  // affect the sub dictionaries as well.
  dict.SetTemplateGlobalValue("TEMPLATEVAL", "templateval");
  EXPECT_TRUE(peer.ValueIs("TEMPLATEVAL", "templateval"));
  EXPECT_TRUE(subdict_peer.ValueIs("TEMPLATEVAL", "templateval"));
  EXPECT_TRUE(subsubdict_peer.ValueIs("TEMPLATEVAL", "templateval"));
  EXPECT_TRUE(includedict_peer.ValueIs("TEMPLATEVAL", "templateval"));

  // sub dictionaries after you set the template value should also
  // get the template value
  TemplateDictionary* subdict2 = dict.AddSectionDictionary("section2");
  TemplateDictionary* includedict2 = dict.AddIncludeDictionary("include2");
  TemplateDictionaryPeer subdict2_peer(subdict2);
  TemplateDictionaryPeer includedict2_peer(includedict2);

  EXPECT_TRUE(subdict2_peer.ValueIs("TEMPLATEVAL", "templateval"));
  EXPECT_TRUE(includedict2_peer.ValueIs("TEMPLATEVAL", "templateval"));

  // setting a template value on a sub dictionary should affect all the other
  // sub dictionaries and the parent as well
  subdict->SetTemplateGlobalValue("TEMPLATEVAL2", "templateval2");
  EXPECT_TRUE(peer.ValueIs("TEMPLATEVAL2", "templateval2"));
  EXPECT_TRUE(subdict_peer.ValueIs("TEMPLATEVAL2", "templateval2"));
  EXPECT_TRUE(subsubdict_peer.ValueIs("TEMPLATEVAL2", "templateval2"));
  EXPECT_TRUE(includedict_peer.ValueIs("TEMPLATEVAL2", "templateval2"));
  EXPECT_TRUE(subdict2_peer.ValueIs("TEMPLATEVAL2", "templateval2"));
  EXPECT_TRUE(includedict2_peer.ValueIs("TEMPLATEVAL2", "templateval2"));

  includedict->SetTemplateGlobalValue("TEMPLATEVAL3", "templateval3");
  EXPECT_TRUE(peer.ValueIs("TEMPLATEVAL3", "templateval3"));
  EXPECT_TRUE(subdict_peer.ValueIs("TEMPLATEVAL3", "templateval3"));
  EXPECT_TRUE(subsubdict_peer.ValueIs("TEMPLATEVAL3", "templateval3"));
  EXPECT_TRUE(includedict_peer.ValueIs("TEMPLATEVAL3", "templateval3"));
  EXPECT_TRUE(subdict2_peer.ValueIs("TEMPLATEVAL3", "templateval3"));
  EXPECT_TRUE(includedict2_peer.ValueIs("TEMPLATEVAL3", "templateval3"));

  // you should be able to override a template value with a regular value
  // and the overwritten regular value should pass on to its children
  subdict->SetValue("TEMPLATEVAL2", "subdictval");
  includedict->SetValue("TEMPLATEVAL2", "includedictval");
  EXPECT_TRUE(peer.ValueIs("TEMPLATEVAL2", "templateval2"));
  EXPECT_TRUE(subdict_peer.ValueIs("TEMPLATEVAL2", "subdictval"));
  EXPECT_TRUE(subsubdict_peer.ValueIs("TEMPLATEVAL2", "subdictval"));
  EXPECT_TRUE(includedict_peer.ValueIs("TEMPLATEVAL2", "includedictval"));
  EXPECT_TRUE(subdict2_peer.ValueIs("TEMPLATEVAL2", "templateval2"));
  EXPECT_TRUE(includedict2_peer.ValueIs("TEMPLATEVAL2", "templateval2"));

  // A section shown template-globally will be shown in all its children.
  dict.ShowTemplateGlobalSection("ShownTemplateGlobalSection");
  EXPECT_FALSE(peer.IsHiddenSection("ShownTemplateGlobalSection"));

  EXPECT_FALSE(subdict2_peer.IsHiddenSection("ShownTemplateGlobalSection"));
  EXPECT_FALSE(subsubdict_peer.IsHiddenSection("ShownTemplateGlobalSection"));

  // Showing a template-global section in a child will show it in all templates
  // in the tree
  subdict->ShowTemplateGlobalSection("ShownFromAChild");
  EXPECT_FALSE(peer.IsHiddenSection("ShownFromAChild"));
  EXPECT_FALSE(subsubdict_peer.IsHiddenSection("ShownFromAChild"));

  // Asking for a section that doesn't exist shouldn't cause infinite recursion
  peer.IsHiddenSection("NAVBAR_SECTION");
}

TEST(TemplateDictionary, SetTemplateGlobalValueWithoutCopy) {
  UnsafeArena arena(100);
  TemplateDictionary dict("Test arena", &arena);
  TemplateDictionaryPeer peer(&dict);

  char value[32];
  snprintf(value, sizeof(value), "%s", "value");

  const void* const ptr = arena.Alloc(0);
  dict.SetTemplateGlobalValueWithoutCopy("key", value);
  // We shouldn't have copied the value string.
  EXPECT_EQ(ptr, arena.Alloc(0));

  EXPECT_TRUE(peer.ValueIs("key", "value"));
  // If our content changes, so does what's in the dictionary -- but
  // only the contents of the buffer, not its length!
  snprintf(value, sizeof(value), "%s", "not_value");
  EXPECT_TRUE(peer.ValueIs("key", "not_v"));   // "not_v" size == value" size
}

TEST(TemplateDictionary, AddIncludeDictionary) {
  TemplateDictionary dict("test_SetAddIncludeDictionary", NULL);
  TemplateDictionaryPeer peer(&dict);
  dict.SetValue("TOPLEVEL", "foo");
  dict.SetValue("TOPLEVEL2", "foo2");
  dict.SetTemplateGlobalValue("TEMPLATELEVEL", "foo3");

  TemplateDictionary* subdict_1a = dict.AddIncludeDictionary("include1");
  TemplateDictionaryPeer subdict_1a_peer(subdict_1a);
  subdict_1a->SetFilename("incfile1a");
  // This is the same dict, but name is specified a different way.
  TemplateDictionary* subdict_1b = dict.AddIncludeDictionary(
      TemplateString("include1__ignored__", strlen("include1")));
  TemplateDictionaryPeer subdict_1b_peer(subdict_1b);
  // Let's try not calling SetFilename on this one.
  subdict_1a->SetValue("SUBLEVEL", "subfoo");
  subdict_1b->SetValue("SUBLEVEL", "subbar");

  TemplateDictionary* subdict_2 = dict.AddIncludeDictionary("include2");
  TemplateDictionaryPeer subdict_2_peer(subdict_2);
  subdict_2->SetFilename("foo/bar");
  subdict_2->SetValue("TOPLEVEL", "bar");    // overriding top dict
  // overriding template dict
  subdict_2->SetValue("TEMPLATELEVEL", "subfoo3");
  TemplateDictionary* subdict_2_1 = subdict_2->AddIncludeDictionary("sub");
  TemplateDictionaryPeer subdict_2_1_peer(subdict_2_1);
  subdict_2_1->SetFilename("baz");
  subdict_2_1->SetIntValue("GLOBAL", 21);    // overrides value in setUp()

  // Verify that all variables that should be look-up-able are, and that
  // we have proper precedence.  Unlike with sections, includes lookups
  // do not go 'up the chain'.
  EXPECT_TRUE(peer.ValueIs("GLOBAL", "top"));
  EXPECT_TRUE(peer.ValueIs("TOPLEVEL", "foo"));
  EXPECT_TRUE(peer.ValueIs("TOPLEVEL2", "foo2"));
  EXPECT_TRUE(peer.ValueIs("TEMPLATELEVEL", "foo3"));
  EXPECT_TRUE(peer.ValueIs("SUBLEVEL", ""));

  EXPECT_TRUE(subdict_1a_peer.ValueIs("GLOBAL", "top"));
  EXPECT_TRUE(subdict_1a_peer.ValueIs("TOPLEVEL", ""));
  EXPECT_TRUE(subdict_1a_peer.ValueIs("TOPLEVEL2", ""));
  EXPECT_TRUE(subdict_1a_peer.ValueIs("TEMPLATELEVEL", "foo3"));
  EXPECT_TRUE(subdict_1a_peer.ValueIs("SUBLEVEL", "subfoo"));

  EXPECT_TRUE(subdict_1b_peer.ValueIs("GLOBAL", "top"));
  EXPECT_TRUE(subdict_1b_peer.ValueIs("TOPLEVEL", ""));
  EXPECT_TRUE(subdict_1b_peer.ValueIs("TOPLEVEL2", ""));
  EXPECT_TRUE(subdict_1b_peer.ValueIs("SUBLEVEL", "subbar"));

  EXPECT_TRUE(subdict_2_peer.ValueIs("GLOBAL", "top"));
  EXPECT_TRUE(subdict_2_peer.ValueIs("TOPLEVEL", "bar"));
  EXPECT_TRUE(subdict_2_peer.ValueIs("TOPLEVEL2", ""));
  EXPECT_TRUE(subdict_2_peer.ValueIs("TEMPLATELEVEL", "subfoo3"));
  EXPECT_TRUE(subdict_2_peer.ValueIs("SUBLEVEL", ""));

  EXPECT_TRUE(subdict_2_1_peer.ValueIs("GLOBAL", "21"));
  EXPECT_TRUE(subdict_2_1_peer.ValueIs("TOPLEVEL", ""));
  EXPECT_TRUE(subdict_2_1_peer.ValueIs("TOPLEVEL2", ""));
  EXPECT_TRUE(subdict_2_1_peer.ValueIs("SUBLEVEL", ""));

  // Verify that everyone knows about its sub-dictionaries, but that
  // these do not try to go 'up the chain' on lookup failure
  EXPECT_FALSE(peer.IsHiddenTemplate("include1"));
  EXPECT_FALSE(peer.IsHiddenTemplate("include2"));
  EXPECT_TRUE(peer.IsHiddenTemplate("include3"));
  EXPECT_TRUE(peer.IsHiddenTemplate("sub"));
  EXPECT_TRUE(subdict_1a_peer.IsHiddenTemplate("include1"));
  EXPECT_TRUE(subdict_1a_peer.IsHiddenTemplate("sub"));
  EXPECT_FALSE(subdict_2_peer.IsHiddenTemplate("sub"));
  EXPECT_TRUE(subdict_2_1_peer.IsHiddenTemplate("sub"));

  // We should get the dictionary-lengths right as well
  vector<const TemplateDictionary*> dummy;
  EXPECT_EQ(2, peer.GetIncludeDictionaries("include1", &dummy));
  EXPECT_EQ(1, peer.GetIncludeDictionaries("include2", &dummy));
  EXPECT_EQ(1, subdict_2_peer.GetIncludeDictionaries("sub", &dummy));

  // We can also test the include-files are right
  EXPECT_EQ(2, peer.GetIncludeDictionaries("include1", &dummy));
  EXPECT_EQ(1, peer.GetIncludeDictionaries("include2", &dummy));
  EXPECT_EQ(1, subdict_2_peer.GetIncludeDictionaries("sub", &dummy));
  // Test some of the values
  EXPECT_TRUE(TemplateDictionaryPeer(GetIncludeDict(&dict, "include1", 0))
              .ValueIs("SUBLEVEL", "subfoo"));
  EXPECT_TRUE(TemplateDictionaryPeer(GetIncludeDict(&dict, "include1", 1))
              .ValueIs("SUBLEVEL", "subbar"));
  EXPECT_TRUE(TemplateDictionaryPeer(GetIncludeDict(&dict, "include2", 0))
              .ValueIs("TOPLEVEL", "bar"));
  EXPECT_TRUE(TemplateDictionaryPeer(
      GetIncludeDict(GetIncludeDict(&dict, "include2", 0), "sub", 0))
              .ValueIs("TOPLEVEL", ""));
  EXPECT_TRUE(TemplateDictionaryPeer(
      GetIncludeDict(GetIncludeDict(&dict, "include2", 0), "sub", 0))
              .ValueIs("GLOBAL", "21"));
  // We can test the include-names as well
  EXPECT_STREQ(peer.GetIncludeTemplateName("include1", 0), "incfile1a");
  EXPECT_STREQ(peer.GetIncludeTemplateName("include1", 1), "");
  EXPECT_STREQ(peer.GetIncludeTemplateName("include2", 0), "foo/bar");
  EXPECT_STREQ(TemplateDictionaryPeer(GetIncludeDict(&dict, "include2", 0))
               .GetIncludeTemplateName("sub", 0),
               "baz");

  // Make sure we're making descriptive names
  EXPECT_STREQ(dict.name().c_str(),
               "test_SetAddIncludeDictionary");
  EXPECT_STREQ(subdict_1a->name().c_str(),
               "test_SetAddIncludeDictionary/include1#1");
  EXPECT_STREQ(subdict_1b->name().c_str(),
               "test_SetAddIncludeDictionary/include1#2");
  EXPECT_STREQ(subdict_2->name().c_str(),
               "test_SetAddIncludeDictionary/include2#1");
  EXPECT_STREQ(subdict_2_1->name().c_str(),
               "test_SetAddIncludeDictionary/include2#1/sub#1");

  // Finally, we can test the whole kit and kaboodle
  string dump;
  dict.DumpToString(&dump);
  const char* const expected =
    ("global dictionary {\n"
     "   BI_NEWLINE: >\n"
     "<\n"
     "   BI_SPACE: > <\n"
     "   GLOBAL: >top<\n"
     "};\n"
     "template dictionary {\n"
     "   TEMPLATELEVEL: >foo3<\n"
     "};\n"
     "dictionary 'test_SetAddIncludeDictionary' {\n"
     "   TOPLEVEL: >foo<\n"
     "   TOPLEVEL2: >foo2<\n"
     "   include-template include1 (dict 1 of 2, from incfile1a) -->\n"
     "     global dictionary {\n"
     "       BI_NEWLINE: >\n"
     "<\n"
     "       BI_SPACE: > <\n"
     "       GLOBAL: >top<\n"
     "     };\n"
     "     dictionary 'test_SetAddIncludeDictionary/include1#1 (intended for incfile1a)' {\n"
     "       SUBLEVEL: >subfoo<\n"
     "     }\n"
     "   include-template include1 (dict 2 of 2, **NO FILENAME SET; THIS DICT WILL BE IGNORED**) -->\n"
     "     global dictionary {\n"
     "       BI_NEWLINE: >\n"
     "<\n"
     "       BI_SPACE: > <\n"
     "       GLOBAL: >top<\n"
     "     };\n"
     "     dictionary 'test_SetAddIncludeDictionary/include1#2' {\n"
     "       SUBLEVEL: >subbar<\n"
     "     }\n"
     "   include-template include2 (dict 1 of 1, from foo/bar) -->\n"
     "     global dictionary {\n"
     "       BI_NEWLINE: >\n"
     "<\n"
     "       BI_SPACE: > <\n"
     "       GLOBAL: >top<\n"
     "     };\n"
     "     dictionary 'test_SetAddIncludeDictionary/include2#1 (intended for foo/bar)' {\n"
     "       TEMPLATELEVEL: >subfoo3<\n"
     "       TOPLEVEL: >bar<\n"
     "       include-template sub (dict 1 of 1, from baz) -->\n"
     "         global dictionary {\n"
     "           BI_NEWLINE: >\n"
     "<\n"
     "           BI_SPACE: > <\n"
     "           GLOBAL: >top<\n"
     "         };\n"
     "         dictionary 'test_SetAddIncludeDictionary/include2#1/sub#1 (intended for baz)' {\n"
     "           GLOBAL: >21<\n"
     "         }\n"
     "     }\n"
     "}\n");
  EXPECT_STREQ(dump.c_str(), expected);
}

static void TestMakeCopy(bool use_local_arena) {
  UnsafeArena local_arena(1024);
  UnsafeArena* arena = NULL;
  if (use_local_arena)
    arena = &local_arena;

  // First, let's make a non-trivial template dictionary (We use
  // 'new' because later we'll test deleting this dict but keeping
  // around the copy.)
  TemplateDictionary* dict = new TemplateDictionary("testdict", arena);

  dict->SetValue("TOPLEVEL", "foo");

  dict->SetTemplateGlobalValue("TEMPLATELEVEL", "foo3");

  TemplateDictionary* subdict_1a = dict->AddIncludeDictionary("include1");
  subdict_1a->SetFilename("incfile1a");
  subdict_1a->SetValue("SUBLEVEL", "subfoo");
  TemplateDictionary* subdict_1b = dict->AddIncludeDictionary("include1");
  // Let's try not calling SetFilename on this one.
  subdict_1b->SetValue("SUBLEVEL", "subbar");

  TemplateDictionary* subdict_2a = dict->AddSectionDictionary("section1");
  TemplateDictionary* subdict_2b = dict->AddSectionDictionary("section1");
  subdict_2a->SetValue("SUBLEVEL", "subfoo");
  subdict_2b->SetValue("SUBLEVEL", "subbar");
  TemplateDictionary* subdict_3 = dict->AddSectionDictionary("section2");
  subdict_3->SetValue("TOPLEVEL", "bar");    // overriding top dict
  TemplateDictionary* subdict_3_1 = subdict_3->AddSectionDictionary("sub");
  subdict_3_1->SetIntValue("GLOBAL", 21);    // overrides value in setUp()

  string orig;
  dict->DumpToString(&orig);

  // Make a copy
  TemplateDictionary* dict_copy = dict->MakeCopy("testdict", NULL);
  // Make sure it doesn't work to copy a sub-dictionary
  EXPECT_TRUE(subdict_1a->MakeCopy("copy of subdict") == NULL);
  EXPECT_TRUE(subdict_2a->MakeCopy("copy of subdict") == NULL);

  // Delete the original dict, to make sure the copy really is independent
  delete dict;
  dict = NULL;
  string copy;
  dict_copy->DumpToString(&copy);
  delete dict_copy;

  EXPECT_STREQ(orig.c_str(), copy.c_str());
}

TEST(MakeCopy, UseLocalArena) {
  TestMakeCopy(true);
}

TEST(MakeCopy, DoNotUseLocalArena) {
  TestMakeCopy(false);
}

TEST(TemplateDictionary, SetModifierData) {
  PerExpandData per_expand_data;
  const void* data = "test";
  per_expand_data.InsertForModifiers("a", data);
  EXPECT_EQ(data, per_expand_data.LookupForModifiers("a"));
}

TEST(TemplateDictionary, Iterator) {
  // Build up a nice community of TemplateDictionaries.
  TemplateDictionary farm("Farm");
  TemplateDictionaryPeer farm_peer(&farm);
  TemplateDictionaryInterface* grey_barn =
      farm.AddIncludeDictionary("BARN");
  TemplateDictionaryInterface* duck_pond =
      farm.AddIncludeDictionary("POND");
  TemplateDictionaryInterface* cattle_pond =
      farm.AddIncludeDictionary("POND");
  TemplateDictionaryInterface* irrigation_pond =
      farm.AddIncludeDictionary("POND");

  // A section name with repeated sections
  TemplateDictionaryInterface* lillies = farm.AddSectionDictionary("FLOWERS");
  TemplateDictionaryInterface* lilacs = farm.AddSectionDictionary("FLOWERS");
  TemplateDictionaryInterface* daisies = farm.AddSectionDictionary("FLOWERS");
  // A section name with one repeat
  TemplateDictionaryInterface* wheat = farm.AddSectionDictionary("WHEAT");
  // A section name, just shown
  farm.ShowSection("CORN");

  // Check that the iterators expose all of the dictionaries.
  TemplateDictionaryPeer::Iterator* barns =
      farm_peer.CreateTemplateIterator("BARN");
  EXPECT_TRUE(barns->HasNext());
  EXPECT_EQ(&barns->Next(), grey_barn);
  EXPECT_FALSE(barns->HasNext());
  delete barns;

  TemplateDictionaryPeer::Iterator* ponds =
      farm_peer.CreateTemplateIterator("POND");
  EXPECT_TRUE(ponds->HasNext());
  EXPECT_EQ(&ponds->Next(), duck_pond);
  EXPECT_TRUE(ponds->HasNext());
  EXPECT_EQ(&ponds->Next(), cattle_pond);
  EXPECT_TRUE(ponds->HasNext());
  EXPECT_EQ(&ponds->Next(), irrigation_pond);
  EXPECT_FALSE(ponds->HasNext());
  delete ponds;

  TemplateDictionaryPeer::Iterator* flowers =
      farm_peer.CreateSectionIterator("FLOWERS");
  EXPECT_TRUE(flowers->HasNext());
  EXPECT_EQ(&flowers->Next(), lillies);
  EXPECT_TRUE(flowers->HasNext());
  EXPECT_EQ(&flowers->Next(), lilacs);
  EXPECT_TRUE(flowers->HasNext());
  EXPECT_EQ(&flowers->Next(), daisies);
  EXPECT_FALSE(flowers->HasNext());
  delete flowers;

  TemplateDictionaryPeer::Iterator* crop =
      farm_peer.CreateSectionIterator("WHEAT");
  EXPECT_TRUE(crop->HasNext());
  EXPECT_EQ(&crop->Next(), wheat);
  EXPECT_FALSE(crop->HasNext());
  delete crop;

  TemplateDictionaryPeer::Iterator* corn_crop =
      farm_peer.CreateSectionIterator("CORN");
  EXPECT_TRUE(corn_crop->HasNext());
  EXPECT_TRUE(&corn_crop->Next());  // ShowSection doesn't give us the dict back
  EXPECT_FALSE(corn_crop->HasNext());
  delete corn_crop;
}

TEST(TemplateDictionary, IsHiddenSectionDefault) {
  TemplateDictionary dict("dict");
  TemplateDictionaryPeer peer(&dict);
  EXPECT_TRUE(peer.IsHiddenSection("UNDEFINED"));
  EXPECT_FALSE(peer.IsUnhiddenSection("UNDEFINED"));
  dict.ShowSection("VISIBLE");
  EXPECT_FALSE(peer.IsHiddenSection("VISIBLE"));
  EXPECT_TRUE(peer.IsUnhiddenSection("VISIBLE"));
}

// This has to run last, since its SetGlobalValue modifies the global
// state, which can affect other tests (especially given the embedded
// NUL!)  So we don't use the normal TEST() here, and call it manually
// in main().

void TestSetValueWithNUL() {
  TemplateDictionary dict("test_SetValueWithNUL", NULL);
  TemplateDictionaryPeer peer(&dict);

  // Test copying char*s, strings, and explicit TemplateStrings
  dict.SetValue(string("FOO\0BAR", 7), string("QUX\0QUUX", 8));
  dict.SetGlobalValue(string("GOO\0GAR", 7), string("GUX\0GUUX", 8));

  // FOO should not match FOO\0BAR
  EXPECT_TRUE(peer.ValueIs("FOO", ""));
  EXPECT_TRUE(peer.ValueIs("GOO", ""));

  EXPECT_TRUE(peer.ValueIs(string("FOO\0BAR", 7), string("QUX\0QUUX", 8)));
  EXPECT_TRUE(peer.ValueIs(string("GOO\0GAR", 7), string("GUX\0GUUX", 8)));

  string dump;
  dict.DumpToString(&dump);
  // We can't use EXPECT_STREQ here because of the embedded NULs.
  // They also require I count the length of the string by hand. :-(
  string expected(("global dictionary {\n"
                   "   BI_NEWLINE: >\n"
                   "<\n"
                   "   BI_SPACE: > <\n"
                   "   GLOBAL: >top<\n"
                   "   GOO\0GAR: >GUX\0GUUX<\n"
                   "};\n"
                   "dictionary 'test_SetValueWithNUL' {\n"
                   "   FOO\0BAR: >QUX\0QUUX<\n"
                   "}\n"),
                  160);
  EXPECT_EQ(dump, expected);
}

TEST(TemplateDictionary, TestShowTemplateGlobalSection) {
  StringToTemplateCache("test.tpl", "{{#sect}}OK{{/sect}}", DO_NOT_STRIP);

  TemplateDictionary dict("mydict");
  dict.ShowTemplateGlobalSection("sect");

  string out;
  ExpandTemplate("test.tpl", DO_NOT_STRIP, &dict, &out);
}

TEST(TemplateDictionary, TestShowTemplateGlobalSection_Child) {
  // The TemplateDictionary::template_global_dict_ behaves differently for child
  // dictionaries than for the root parent dictionary.
  StringToTemplateCache("test2.tpl",
                        "{{#foo}}{{#sect}}OK{{/sect}}{{/foo}}",
                        DO_NOT_STRIP);

  TemplateDictionary dict("mydict");
  dict.ShowTemplateGlobalSection("sect");

  dict.AddSectionDictionary("foo");

  string out;
  ExpandTemplate("test2.tpl", DO_NOT_STRIP, &dict, &out);
}

TEST(TemplateDictionary, TestShowTemplateGlobalSection_SectionDoesntExist) {
  StringToTemplateCache("test3.tpl",
                        "{{#bad}}bad{{/bad}}",
                        DO_NOT_STRIP);

  TemplateDictionary dict("mydict");

  string out;
  ExpandTemplate("test3.tpl", DO_NOT_STRIP, &dict, &out);
}


}  // unnamed namespace


int main(int argc, char** argv) {

  SetUp();

  const int retval = RUN_ALL_TESTS();

  // This has to run last, so we run it manually
  TestSetValueWithNUL();

  return retval;
}
