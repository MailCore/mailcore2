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

#include "config_for_unittests.h"
#include <ctemplate/template.h>
#include <assert.h>      // for assert()
#if defined(HAVE_PTHREAD) && !defined(NO_THREADS)
# include <pthread.h>
#endif     // for pthread_t, pthread_create(), etc
#include <stddef.h>      // for size_t
#include <stdio.h>       // for printf(), FILE, snprintf(), fclose(), etc
#include <stdlib.h>      // for exit()
#include <string.h>      // for strcmp(), memchr(), strlen(), strstr()
#include <sys/types.h>   // for mode_t
#include <time.h>        // for time_t, time()
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif      // for link(), unlink()
#include <list>          // for list<>::size_type
#include <vector>        // for vector<>
#include <ctemplate/per_expand_data.h>  // for PerExpandData
#include <ctemplate/template_annotator.h>  // for TextTemplateAnnotator
#include <ctemplate/template_dictionary.h>  // for TemplateDictionary
#include <ctemplate/template_emitter.h>  // for ExpandEmitter
#include <ctemplate/template_enums.h>  // for STRIP_WHITESPACE, Strip, etc
#include <ctemplate/template_modifiers.h>  // for AddModifier(), HtmlEscape, etc
#include <ctemplate/template_namelist.h>  // for TemplateNamelist, etc
#include <ctemplate/template_pathops.h>  // for PathJoin(), IsAbspath(), etc
#include <ctemplate/template_string.h>  // for TemplateString, StringHash, etc
#include "tests/template_test_util.h"  // for StringToTemplate(), etc
#include "base/util.h"
TEST_INIT   // defines RUN_ALL_TESTS()

using std::vector;
using std::string;
using ctemplate::FLAGS_test_tmpdir;

using ctemplate::AssertExpandIs;
using ctemplate::AssertExpandWithDataIs;
using ctemplate::CreateOrCleanTestDir;
using ctemplate::CreateOrCleanTestDirAndSetAsTmpdir;
using ctemplate::DO_NOT_STRIP;
using ctemplate::ExpandEmitter;
using ctemplate::IsAbspath;
using ctemplate::Now;
using ctemplate::PathJoin;
using ctemplate::PerExpandData;
using ctemplate::STRIP_BLANK_LINES;
using ctemplate::STRIP_WHITESPACE;
using ctemplate::StaticTemplateString;
using ctemplate::StringToFile;
using ctemplate::StringToTemplate;
using ctemplate::StringToTemplateFile;
using ctemplate::Strip;
using ctemplate::TC_CSS;
using ctemplate::TC_HTML;
using ctemplate::TC_JS;
using ctemplate::TC_JSON;
using ctemplate::TC_MANUAL;
using ctemplate::TC_UNUSED;
using ctemplate::TC_XML;
using ctemplate::Template;
using ctemplate::TemplateContext;
using ctemplate::TemplateDictionary;
using ctemplate::TemplateNamelist;
using ctemplate::TemplateString;
using ctemplate::kRootdir;

using ctemplate::ExpandTemplate;
using ctemplate::ExpandWithData;
using ctemplate::StringToTemplateCache;

static const StaticTemplateString kHello = STS_INIT(kHello, "Hello");
static const StaticTemplateString kWorld = STS_INIT(kWorld, "World");

static const char* kPragmaHtml = "{{%AUTOESCAPE context=\"HTML\"}}\n";
static const char* kPragmaJs   = "{{%AUTOESCAPE context=\"JAVASCRIPT\"}}\n";
static const char* kPragmaCss  = "{{%AUTOESCAPE context=\"CSS\"}}\n";
static const char* kPragmaXml  = "{{%AUTOESCAPE context=\"XML\"}}\n";
static const char* kPragmaJson = "{{%AUTOESCAPE context=\"JSON\"}}\n";

// How many threads to use for our threading test.
// This is a #define instead of a const int so we can use it in array-sizes
// even on c++ compilers that don't support var-length arrays.
#define kNumThreads  10

#define PFATAL(s)  do { perror(s); exit(1); } while (0)

// TODO(csilvers): rewrite to be more gunit-like: use expectations
// instead of asserts, and move assert-checking out of helper routines
// and into tests proper.  Ideally, replace AssertExpandIs() with
// VerifyExpandIs().
#define ASSERT(cond)  do {                                      \
  if (!(cond)) {                                                \
    printf("ASSERT FAILED, line %d: %s\n", __LINE__, #cond);    \
    assert(cond);                                               \
    exit(1);                                                    \
  }                                                             \
} while (0)

#define ASSERT_STREQ_EXCEPT(a, b, except)  ASSERT(StreqExcept(a, b, except))
#define ASSERT_STREQ(a, b)   ASSERT(strcmp(a, b) == 0)
#define ASSERT_NOT_STREQ(a, b)             ASSERT(strcmp(a, b) != 0)
#define ASSERT_STREQ_VERBOSE(a, b, c)      ASSERT(StrEqVerbose(a, b, c))
#define ASSERT_INTEQ(a, b)                 ASSERT(IntEqVerbose(a, b))

namespace {

// First, (conceptually) remove all chars in "except" from both a and b.
// Then return true iff munged_a == munged_b.
bool StreqExcept(const char* a, const char* b, const char* except) {
  const char* pa = a, *pb = b;
  const size_t exceptlen = strlen(except);
  while (1) {
    // Use memchr isntead of strchr because memchr(foo, '\0') always fails
    while (memchr(except, *pa, exceptlen))  pa++;  // ignore "except" chars in a
    while (memchr(except, *pb, exceptlen))  pb++;  // ignore "except" chars in b
    if ((*pa == '\0') && (*pb == '\0'))
      return true;
    if (*pa++ != *pb++)                  // includes case where one is at \0
      return false;
  }
}

// If a and b do not match, print their values and that of text
// and return false.
bool StrEqVerbose(const string& a, const string& b,
                         const string& text) {
  if (a != b) {
    printf("EXPECTED: %s\n", a.c_str());
    printf("ACTUAL: %s\n", b.c_str());
    printf("TEXT: %s\n", text.c_str());
    return false;
  }
  return true;
}

bool IntEqVerbose(int a, int b) {
  if (a != b) {
    printf("EXPECTED: %d\n", a);
    printf("ACTUAL: %d\n", b);
    return false;
  }
  return true;
}

// This test emitter writes to a string, but writes X's of the right
// length, rather than the actual content passed in.
class SizeofEmitter : public ExpandEmitter {
  string* const outbuf_;
 public:
  SizeofEmitter(string* outbuf) : outbuf_(outbuf) {}
  virtual void Emit(char c) { Emit(&c, 1); }
  virtual void Emit(const string& s) { Emit(s.data(), s.length()); }
  virtual void Emit(const char* s) { Emit(s, strlen(s)); }
  virtual void Emit(const char*, size_t slen) { outbuf_->append(slen, 'X'); }
};

}  // unnamed namespace

RegisterTemplateFilename(VALID1_FN, "template_unittest_test_valid1.in");
RegisterTemplateFilename(INVALID1_FN, "template_unittest_test_invalid1.in");
RegisterTemplateFilename(INVALID2_FN, "template_unittest_test_invalid2.in");
RegisterTemplateFilename(NONEXISTENT_FN, "nonexistent__file.tpl");

// Returns the proper AUTOESCAPE pragma that corresponds to the
// given TemplateContext.
static string GetPragmaForContext(TemplateContext context) {
  switch(context) {
    case TC_HTML:
      return kPragmaHtml;
    case TC_JS:
      return kPragmaJs;
    case TC_CSS:
      return kPragmaCss;
    case TC_JSON:
      return kPragmaJson;
    case TC_XML:
      return kPragmaXml;
    case TC_MANUAL:
      return "";  // No AUTOESCAPE pragma.
    case TC_UNUSED:
      ASSERT(false);  // Developer error, this TC is not to be used.
  }
  ASSERT(false);  // Developer error - invalid TemplateContext.
  return "";
}

// This writes s to a file with the AUTOESCAPE pragma corresponding
// to the given TemplateContext and then loads it into a template object.
static Template* StringToTemplateWithAutoEscaping(const string& s,
                                                  Strip strip,
                                                  TemplateContext context) {
  string text = GetPragmaForContext(context) + s;
  return Template::GetTemplate(StringToTemplateFile(text), strip);
}

// A helper method used by TestCorrectModifiersForAutoEscape.
// Populates out with lines of the form:
// VARNAME:mod1[=val1][:mod2[=val2]]...\n from the dump of the template
// and compares against the expected string.
static void AssertCorrectModifiersInTemplate(Template* tpl,
                                             const string& text,
                                             const string& expected_out) {
  ASSERT(tpl);
  string dump_out, out;
  tpl->DumpToString("bogus_filename", &dump_out);
  string::size_type i, j;
  i = 0;
  while ((i = dump_out.find("Variable Node: ", i)) != string::npos) {
    i += strlen("Variable Node: ");
    j = dump_out.find("\n", i);
    out.append(dump_out.substr(i, j - i));   // should be safe.
    out.append("\n");
  }
  ASSERT_STREQ_VERBOSE(expected_out, out, text);
}

// Wrapper on top of AssertCorrectModifiersInTemplate which first
// obtains a template from the given contents and template context.
static void AssertCorrectModifiers(TemplateContext template_type,
                                   const string& text,
                                   const string& expected_out) {
  Strip strip = STRIP_WHITESPACE;
  Template *tpl = StringToTemplateWithAutoEscaping(text, strip, template_type);
  AssertCorrectModifiersInTemplate(tpl, text, expected_out);
}

// A helper method used by TestCorrectModifiersForAutoEscape.
// Initializes the template in the Auto Escape mode with the
// given TemplateContext, expands it with the given dictionary
// and checks that the output matches the expected value.
static void AssertCorrectEscaping(TemplateContext template_type,
                                  const TemplateDictionary& dict,
                                  const string& text,
                                  const string& expected_out) {
  Strip strip = STRIP_WHITESPACE;
  Template *tpl = StringToTemplateWithAutoEscaping(text, strip, template_type);
  string outstring;
  tpl->Expand(&outstring, &dict);
  ASSERT_STREQ_VERBOSE(expected_out, outstring, text);
}

class DynamicModifier : public ctemplate::TemplateModifier {
 public:
  void Modify(const char* in, size_t inlen,
              const PerExpandData* per_expand_data,
              ExpandEmitter* outbuf, const string& arg) const {
    assert(arg.empty());    // we don't take an argument
    assert(per_expand_data);
    const char* value = per_expand_data->LookupForModifiersAsString("value");
    if (value)
      outbuf->Emit(value);
  }
};

class EmphasizeTemplateModifier : public ctemplate::TemplateModifier {
 public:
  EmphasizeTemplateModifier(const string& match)
      : match_(match) {
  }

  bool MightModify(const PerExpandData* per_expand_data,
                   const string& arg) const {
    return strstr(arg.c_str(), match_.c_str());
  }

  void Modify(const char* in, size_t inlen,
              const PerExpandData* per_expand_data,
              ExpandEmitter* outbuf, const string& arg) const {
    outbuf->Emit(">>");
    outbuf->Emit(in, inlen);
    outbuf->Emit("<<");
  }

 private:
  string match_;
};

// This is used by TestAnnotation().  It behaves like
// TextTemplateAnnotator but just to test our ability to customize
// annotation, and with stateful one, it prefixes each text annotation
// with an event (call) count.
class CustomTestAnnotator : public ctemplate::TextTemplateAnnotator {
 public:
  CustomTestAnnotator() : event_count_(0) { }
  void Reset() { event_count_ = 0; }

  virtual void EmitOpenInclude(ExpandEmitter* emitter, const string& value) {
    EmitTestPrefix(emitter);
    ctemplate::TextTemplateAnnotator::EmitOpenInclude(emitter, value);
  }
  virtual void EmitCloseInclude(ExpandEmitter* emitter) {
    EmitTestPrefix(emitter);
    ctemplate::TextTemplateAnnotator::EmitCloseInclude(emitter);
  }
  virtual void EmitOpenFile(ExpandEmitter* emitter, const string& value) {
    EmitTestPrefix(emitter);
    ctemplate::TextTemplateAnnotator::EmitOpenFile(emitter, value);
  }
  virtual void EmitCloseFile(ExpandEmitter* emitter) {
    EmitTestPrefix(emitter);
    ctemplate::TextTemplateAnnotator::EmitCloseFile(emitter);
  }
  virtual void EmitOpenSection(ExpandEmitter* emitter, const string& value) {
    EmitTestPrefix(emitter);
    ctemplate::TextTemplateAnnotator::EmitOpenSection(emitter, value);
  }
  virtual void EmitCloseSection(ExpandEmitter* emitter) {
    EmitTestPrefix(emitter);
    ctemplate::TextTemplateAnnotator::EmitCloseSection(emitter);
  }
  virtual void EmitOpenVariable(ExpandEmitter* emitter, const string& value) {
    EmitTestPrefix(emitter);
    ctemplate::TextTemplateAnnotator::EmitOpenVariable(emitter, value);
  }
  virtual void EmitCloseVariable(ExpandEmitter* emitter) {
    EmitTestPrefix(emitter);
    ctemplate::TextTemplateAnnotator::EmitCloseVariable(emitter);
  }
  virtual void EmitFileIsMissing(ExpandEmitter* emitter,
                                    const string& value) {
    EmitTestPrefix(emitter);
    ctemplate::TextTemplateAnnotator::EmitFileIsMissing(emitter, value);
  }

 private:
  void EmitTestPrefix(ExpandEmitter* emitter) {
    char buf[128];
    snprintf(buf, sizeof(buf), "{{EVENT=%d}}", ++event_count_);
    emitter->Emit(buf);
  }
  int event_count_;
  DISALLOW_COPY_AND_ASSIGN(CustomTestAnnotator);
};

class TemplateForTest : public Template {
 public:
  using Template::kSafeWhitelistedVariables;
  using Template::kNumSafeWhitelistedVariables;
 private:
  // This quiets gcc3, which otherwise complains: "base `Template'
  // with only non-default constructor in class without a constructor".
  TemplateForTest();
};

// Tests annotation, in particular inheriting annotation among children
// This should be called first, so the filenames don't change as we add
// more tests.
static void TestAnnotation() {
  string incname = StringToTemplateFile("include {{#ISEC}}file{{/ISEC}}\n");
  string incname2 = StringToTemplateFile("include #2\n");
  Template* tpl = StringToTemplate(
      "boo!\n{{>INC}}\nhi {{#SEC}}lo{{#SUBSEC}}jo{{/SUBSEC}}{{/SEC}} bar "
      "{{VAR:x-foo}}",
      DO_NOT_STRIP);
  TemplateDictionary dict("dict");
  PerExpandData per_expand_data;

  dict.ShowSection("SEC");
  TemplateDictionary* incdict = dict.AddIncludeDictionary("INC");
  incdict->SetFilename(incname);
  incdict->ShowSection("ISEC");
  dict.AddIncludeDictionary("INC")->SetFilename(incname2);
  dict.SetValue("VAR", "var");

  // This string is equivalent to "/template." (at least on unix)
  string slash_tpl(PathJoin(kRootdir, "template."));
  per_expand_data.SetAnnotateOutput("");
  char expected[10240];           // 10k should be big enough!
  snprintf(expected, sizeof(expected),
           "{{#FILE=%s003}}{{#SEC=__{{MAIN}}__}}boo!\n"
           "{{#INC=INC}}{{#FILE=%s001}}"
           "{{#SEC=__{{MAIN}}__}}include {{#SEC=ISEC}}file{{/SEC}}\n"
           "{{/SEC}}{{/FILE}}{{/INC}}"
           "{{#INC=INC}}{{#FILE=%s002}}"
           "{{#SEC=__{{MAIN}}__}}include #2\n{{/SEC}}{{/FILE}}{{/INC}}"
           "\nhi {{#SEC=SEC}}lo{{/SEC}} bar "
           "{{#VAR=VAR:x-foo<not registered>}}var{{/VAR}}{{/SEC}}{{/FILE}}",
           (FLAGS_test_tmpdir + slash_tpl).c_str(),
           (FLAGS_test_tmpdir + slash_tpl).c_str(),
           (FLAGS_test_tmpdir + slash_tpl).c_str());
  AssertExpandWithDataIs(tpl, &dict, &per_expand_data, expected, true);

  // Test ability to set custom annotator.
  CustomTestAnnotator custom_annotator;
  per_expand_data.SetAnnotator(&custom_annotator);
  snprintf(expected, sizeof(expected),
           "{{EVENT=1}}{{#FILE=%s003}}"
           "{{EVENT=2}}{{#SEC=__{{MAIN}}__}}boo!\n"
           "{{EVENT=3}}{{#INC=INC}}"
           "{{EVENT=4}}{{#FILE=%s001}}"
           "{{EVENT=5}}{{#SEC=__{{MAIN}}__}}include "
           "{{EVENT=6}}{{#SEC=ISEC}}file"
           "{{EVENT=7}}{{/SEC}}\n"
           "{{EVENT=8}}{{/SEC}}"
           "{{EVENT=9}}{{/FILE}}"
           "{{EVENT=10}}{{/INC}}"
           "{{EVENT=11}}{{#INC=INC}}"
           "{{EVENT=12}}{{#FILE=%s002}}"
           "{{EVENT=13}}{{#SEC=__{{MAIN}}__}}include #2\n"
           "{{EVENT=14}}{{/SEC}}"
           "{{EVENT=15}}{{/FILE}}"
           "{{EVENT=16}}{{/INC}}\nhi "
           "{{EVENT=17}}{{#SEC=SEC}}lo"
           "{{EVENT=18}}{{/SEC}} bar "
           "{{EVENT=19}}{{#VAR=VAR:x-foo<not registered>}}var"
           "{{EVENT=20}}{{/VAR}}"
           "{{EVENT=21}}{{/SEC}}"
           "{{EVENT=22}}{{/FILE}}",
           (FLAGS_test_tmpdir + slash_tpl).c_str(),
           (FLAGS_test_tmpdir + slash_tpl).c_str(),
           (FLAGS_test_tmpdir + slash_tpl).c_str());
  // We can't use AssertExpandWithDataIs() on our deliberately stateful
  // test annotator because it internally does a second expansion
  // assuming no state change between calls.
  string custom_outstring;
  ASSERT(tpl->ExpandWithData(&custom_outstring, &dict, &per_expand_data));
  ASSERT_STREQ(custom_outstring.c_str(), expected);

  // Unset annotator and continue with next test as test of ability
  // to revert to built-in annotator.
  per_expand_data.SetAnnotator(NULL);

  per_expand_data.SetAnnotateOutput(slash_tpl.c_str());
  snprintf(expected, sizeof(expected),
           "{{#FILE=%s003}}{{#SEC=__{{MAIN}}__}}boo!\n"
           "{{#INC=INC}}{{#FILE=%s001}}"
           "{{#SEC=__{{MAIN}}__}}include {{#SEC=ISEC}}file{{/SEC}}\n"
           "{{/SEC}}{{/FILE}}{{/INC}}"
           "{{#INC=INC}}{{#FILE=%s002}}"
           "{{#SEC=__{{MAIN}}__}}include #2\n{{/SEC}}{{/FILE}}{{/INC}}"
           "\nhi {{#SEC=SEC}}lo{{/SEC}} bar "
           "{{#VAR=VAR:x-foo<not registered>}}var{{/VAR}}{{/SEC}}{{/FILE}}",
           (slash_tpl).c_str(),
           (slash_tpl).c_str(),
           (slash_tpl).c_str());
  AssertExpandWithDataIs(tpl, &dict, &per_expand_data, expected, true);

  per_expand_data.SetAnnotateOutput(NULL);   // should turn off annotations
  AssertExpandWithDataIs(tpl, &dict, &per_expand_data,
                         "boo!\ninclude file\ninclude #2\n\nhi lo bar var",
                         true);

  // Test that even if we set an annotator we shouldn't get annotation
  // if it is not turned on with SetAnnotateOutput().
  per_expand_data.SetAnnotator(&custom_annotator);
  AssertExpandWithDataIs(tpl, &dict, &per_expand_data,
                         "boo!\ninclude file\ninclude #2\n\nhi lo bar var",
                         true);

  // Test annotation of "missing include" condition.
  Template* one_inc_tpl =
      StringToTemplate("File contents: {{>INC}}\n", DO_NOT_STRIP);
  TemplateDictionary dict_missing_file("dict_with_missing_file");
  dict_missing_file.AddIncludeDictionary("INC")->SetFilename("missing.tpl");

  per_expand_data.SetAnnotateOutput("");
  per_expand_data.SetAnnotator(NULL);
  snprintf(expected, sizeof(expected),
           "{{#FILE=%s004}}{{#SEC=__{{MAIN}}__}}File contents: "
           "{{#INC=INC}}{{MISSING_FILE=missing.tpl}}{{/INC}}\n"
           "{{/SEC}}{{/FILE}}",
           (FLAGS_test_tmpdir + slash_tpl).c_str());
  // We expect a false return value because of the missing file.
  AssertExpandWithDataIs(one_inc_tpl, &dict_missing_file, &per_expand_data,
                         expected, false);

  // Same missing include test with custom annotator
  custom_annotator.Reset();
  per_expand_data.SetAnnotator(&custom_annotator);
  snprintf(expected, sizeof(expected),
           "{{EVENT=1}}{{#FILE=%s004}}"
           "{{EVENT=2}}{{#SEC=__{{MAIN}}__}}File contents: "
           "{{EVENT=3}}{{#INC=INC}}"
           "{{EVENT=4}}{{MISSING_FILE=missing.tpl}}"
           "{{EVENT=5}}{{/INC}}\n"
           "{{EVENT=6}}{{/SEC}}"
           "{{EVENT=7}}{{/FILE}}",
           (FLAGS_test_tmpdir + slash_tpl).c_str());
  // See comment above on why we can't use AssertExpandWithDataIs() for
  // our stateful test annotator.
  custom_outstring.clear();
  ASSERT(!one_inc_tpl->ExpandWithData(&custom_outstring,
                                      &dict_missing_file,
                                      &per_expand_data));
  ASSERT_STREQ(custom_outstring.c_str(), expected);
}

TEST(Template, CheckWhitelistedVariablesSorted) {
  // NOTE(williasr): kSafeWhitelistedVariables must be sorted, it's accessed
  // using binary search.
  for (size_t i = 1; i < TemplateForTest::kNumSafeWhitelistedVariables; i++) {
    assert(strcmp(TemplateForTest::kSafeWhitelistedVariables[i-1],
                  TemplateForTest::kSafeWhitelistedVariables[i]) < 0);
  }
}


// The following tests test various aspects of how Expand() should behave.
TEST(Template, WeirdSyntax) {
  TemplateDictionary dict("dict");

  // When we see {{{, we should match the second {{, not the first.
  Template* tpl1 = StringToTemplate("hi {{{! VAR {{!VAR} }} lo",
                                    STRIP_WHITESPACE);
  AssertExpandIs(tpl1, &dict, "hi { lo", true);

  // Likewise for }}}
  Template* tpl2 = StringToTemplate("fn(){{{BI_NEWLINE}} x=4;{{BI_NEWLINE}}}",
                                    DO_NOT_STRIP);
  AssertExpandIs(tpl2, &dict, "fn(){\n x=4;\n}", true);

  // Try lots of {'s!
  Template* tpl3 = StringToTemplate("{{{{{{VAR}}}}}}}}", DO_NOT_STRIP);
  AssertExpandIs(tpl3, &dict, "{{{{}}}}}}", true);
}

TEST(Template, Comment) {
  TemplateDictionary dict("dict");
  Template* tpl1 = StringToTemplate("hi {{!VAR}} lo",
                                    STRIP_WHITESPACE);
  AssertExpandIs(tpl1, &dict, "hi  lo", true);

  Template* tpl2 = StringToTemplate("hi {{!VAR {VAR} }} lo",
                                    STRIP_WHITESPACE);
  AssertExpandIs(tpl2, &dict, "hi  lo", true);

  Template* tpl3 = StringToTemplate("hi {{! VAR {{!VAR} }} lo",
                                    STRIP_WHITESPACE);
  AssertExpandIs(tpl3, &dict, "hi  lo", true);
}

TEST(Template, SetMarkerDelimiters) {
  TemplateDictionary dict("dict");
  dict.SetValue("VAR", "yo");
  Template* tpl1 = StringToTemplate("{{=| |=}}\nhi |VAR| {{lo}}",
                                    STRIP_WHITESPACE);
  AssertExpandIs(tpl1, &dict, "hi yo {{lo}}", true);

  Template* tpl2 = StringToTemplate("{{=| |=}}hi |VAR| {{lo}}",
                                    STRIP_WHITESPACE);
  AssertExpandIs(tpl2, &dict, "hi yo {{lo}}", true);

  Template* tpl3 = StringToTemplate("{{=| ||=}}hi ||VAR|||VAR|| {{lo}}",
                                    STRIP_WHITESPACE);
  AssertExpandIs(tpl3, &dict, "hi |yoyo {{lo}}", true);

  Template* tpl4 = StringToTemplate("{{=< >=}}hi <<VAR>> {{lo}}",
                                    STRIP_WHITESPACE);
  AssertExpandIs(tpl4, &dict, "hi <yo> {{lo}}", true);

  Template* tpl4b = StringToTemplate("{{=<< >>=}}hi <<VAR>> {{lo}}",
                                     STRIP_WHITESPACE);
  AssertExpandIs(tpl4b, &dict, "hi yo {{lo}}", true);

  Template* tpl4c = StringToTemplate("{{=<< <<=}}hi <<VAR<< {{lo}}",
                                     STRIP_WHITESPACE);
  AssertExpandIs(tpl4c, &dict, "hi yo {{lo}}", true);

  Template* tpl5 = StringToTemplate("hi {{VAR}} lo\n{{=< >=}}\n"
                                    "hi {{VAR}} lo\n"
                                    "hi <VAR> lo\n<={ }=>\n"
                                    "hi {{VAR}} lo\n{={{ }}=}\n"
                                    "hi {{VAR}} lo\n",
                                    STRIP_WHITESPACE);
  AssertExpandIs(tpl5, &dict,
                 "hi yo lohi {{VAR}} lohi yo lohi {yo} lohi yo lo",
                 true);

  Template* tpl6 = StringToTemplate("hi {{VAR}} lo\n{{=< >}}\n",
                                    STRIP_WHITESPACE);
  ASSERT(tpl6 == NULL);

  Template* tpl7 = StringToTemplate("hi {{VAR}} lo\n{{=<>}}\n",
                                    STRIP_WHITESPACE);
  ASSERT(tpl7 == NULL);

  Template* tpl8 = StringToTemplate("hi {{VAR}} lo\n{{=<  >=}}\n",
                                    STRIP_WHITESPACE);
  ASSERT(tpl8 == NULL);

  Template* tpl9 = StringToTemplate("hi {{VAR}} lo\n{{==}}\n",
                                    STRIP_WHITESPACE);
  ASSERT(tpl9 == NULL);

  Template* tpl10 = StringToTemplate("hi {{VAR}} lo\n{{=}}\n",
                                     STRIP_WHITESPACE);
  ASSERT(tpl10 == NULL);

  // Test that {{= =}} is a "removable" marker.
  Template* tpl11 = StringToTemplate("line\n  {{=| |=}} \nhi |VAR| {{lo}}\n",
                                     STRIP_BLANK_LINES);
  AssertExpandIs(tpl11, &dict, "line\nhi yo {{lo}}\n", true);

  // Test that "removable" markers survive marker-modification.
  Template* tpl12 = StringToTemplate("  {{#SEC1}}  \n"
                                     "{{=| |=}}    |VAR|\n"
                                     "  |/SEC1|\ntada! |VAR|\n"
                                     "hello|=<< >>=|\n"
                                     "   <<! a blank line>>  \n"
                                     "done",
                                     STRIP_BLANK_LINES);
  AssertExpandIs(tpl12, &dict, "tada! yo\nhello\ndone", true);
}

TEST(Template, Variable) {
  Template* tpl = StringToTemplate("hi {{VAR}} lo", STRIP_WHITESPACE);
  TemplateDictionary dict("dict");
  AssertExpandIs(tpl, &dict, "hi  lo", true);
  dict.SetValue("VAR", "yo");
  AssertExpandIs(tpl, &dict, "hi yo lo", true);
  dict.SetValue("VAR", "yoyo");
  AssertExpandIs(tpl, &dict, "hi yoyo lo", true);
  dict.SetValue("VA", "noyo");
  dict.SetValue("VAR ", "noyo2");
  dict.SetValue("var", "noyo3");
  AssertExpandIs(tpl, &dict, "hi yoyo lo", true);

  // Sanity check string template behaviour while we're at it.
  Template* tpl2 = Template::StringToTemplate("hi {{VAR}} lo",
                                              STRIP_WHITESPACE);
  TemplateDictionary dict2("dict");
  AssertExpandIs(tpl2, &dict2, "hi  lo", true);
  dict2.SetValue("VAR", "yo");
  AssertExpandIs(tpl2, &dict2, "hi yo lo", true);
  dict2.SetValue("VAR", "yoyo");
  AssertExpandIs(tpl2, &dict2, "hi yoyo lo", true);
  dict2.SetValue("VA", "noyo");
  dict2.SetValue("VAR ", "noyo2");
  dict2.SetValue("var", "noyo3");
  AssertExpandIs(tpl2, &dict2, "hi yoyo lo", true);
  delete tpl2;   // You have to delete StringToTemplate strings
}

TEST(Template, VariableWithModifiers) {
  Template* tpl = StringToTemplate("hi {{VAR:html_escape}} lo",
                                   STRIP_WHITESPACE);
  TemplateDictionary dict("dict");

  // Test with no modifiers.
  dict.SetValue("VAR", "yo");
  AssertExpandIs(tpl, &dict, "hi yo lo", true);
  dict.SetValue("VAR", "yo&yo");
  AssertExpandIs(tpl, &dict, "hi yo&amp;yo lo", true);

  // Test with URL escaping.
  tpl = StringToTemplate("<a href=\"/servlet?param={{VAR:u}}\">",
                         STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "<a href=\"/servlet?param=yo%26yo\">", true);
  tpl = StringToTemplate("<a href='/servlet?param={{VAR:url_query_escape}}'>",
                         STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "<a href='/servlet?param=yo%26yo'>", true);

  // Test with multiple URL escaping.
  tpl = StringToTemplate("<a href=\"/servlet?param={{VAR:u:u}}\">",
                         STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "<a href=\"/servlet?param=yo%2526yo\">", true);

  // Test HTML escaping.
  tpl = StringToTemplate("hi {{VAR:h}} lo", STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi yo&amp;yo lo", true);

  tpl = StringToTemplate("hi {{VAR:h:h}} lo", STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi yo&amp;amp;yo lo", true);

  // Test special HTML escaping
  dict.SetValue("URL_VAR", "javascript:void");
  dict.SetValue("SNIPPET_VAR", "<b>foo & bar</b>");
  tpl = StringToTemplate("hi {{VAR:H=attribute}} {{URL_VAR:H=url}} "
                         "{{SNIPPET_VAR:H=snippet}} lo", STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi yo_yo # <b>foo & bar</b> lo", true);

  // Test with custom modifiers [regular or XssSafe should not matter].
  ASSERT(ctemplate::AddModifier("x-test",
                                &ctemplate::html_escape));
  ASSERT(ctemplate::AddModifier("x-test-arg=",
                                &ctemplate::html_escape));
  ASSERT(ctemplate::AddXssSafeModifier("x-test-arg=snippet",
                                       &ctemplate::snippet_escape));

  tpl = StringToTemplate("hi {{VAR:x-test}} lo", STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi yo&amp;yo lo", true);
  tpl = StringToTemplate("hi {{SNIPPET_VAR:x-test-arg=snippet}} lo",
                         STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi <b>foo & bar</b> lo", true);
  tpl = StringToTemplate("hi {{VAR:x-unknown}} lo", STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi yo&yo lo", true);

  // Test with a modifier taking per-expand data
  DynamicModifier dynamic_modifier;
  ASSERT(ctemplate::AddModifier("x-dynamic", &dynamic_modifier));
  PerExpandData per_expand_data;
  tpl = StringToTemplate("hi {{VAR:x-dynamic}} lo", STRIP_WHITESPACE);
  AssertExpandWithDataIs(tpl, &dict, &per_expand_data, "hi  lo", true);
  per_expand_data.InsertForModifiers("value", "foo");
  AssertExpandWithDataIs(tpl, &dict, &per_expand_data, "hi foo lo", true);
  per_expand_data.InsertForModifiers("value", "bar");
  AssertExpandWithDataIs(tpl, &dict, &per_expand_data, "hi bar lo", true);
  per_expand_data.InsertForModifiers("value", NULL);
  AssertExpandWithDataIs(tpl, &dict, &per_expand_data, "hi  lo", true);

  // Test with no modifiers.
  tpl = StringToTemplate("hi {{VAR}} lo", STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi yo&yo lo", true);

  // Check that ordering is right
  dict.SetValue("VAR", "yo\nyo");
  tpl = StringToTemplate("hi {{VAR:h}} lo", STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi yo yo lo", true);
  tpl = StringToTemplate("hi {{VAR:p}} lo", STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi yo\nyo lo", true);
  tpl = StringToTemplate("hi {{VAR:j}} lo", STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi yo\\nyo lo", true);
  tpl = StringToTemplate("hi {{VAR:h:j}} lo", STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi yo yo lo", true);
  tpl = StringToTemplate("hi {{VAR:j:h}} lo", STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi yo\\nyo lo", true);

  // Check more complicated modifiers using fullname
  tpl = StringToTemplate("hi {{VAR:javascript_escape:h}} lo",
                         STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi yo\\nyo lo", true);
  tpl = StringToTemplate("hi {{VAR:j:html_escape}} lo",
                         STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi yo\\nyo lo", true);
  tpl = StringToTemplate("hi {{VAR:pre_escape:j}} lo",
                         STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict, "hi yo\\nyo lo", true);

  // Check that illegal modifiers are rejected
  tpl = StringToTemplate("hi {{VAR:j:h2}} lo", STRIP_WHITESPACE);
  ASSERT(tpl == NULL);
  tpl = StringToTemplate("hi {{VAR:html_ecap}} lo", STRIP_WHITESPACE);
  ASSERT(tpl == NULL);
  tpl = StringToTemplate("hi {{VAR:javascript_escaper}} lo",
                         STRIP_WHITESPACE);
  ASSERT(tpl == NULL);
  tpl = StringToTemplate("hi {{VAR:js:j}} lo", STRIP_WHITESPACE);
  ASSERT(tpl == NULL);
  tpl = StringToTemplate("hi {{VAR:}} lo", STRIP_WHITESPACE);
  ASSERT(tpl == NULL);

  // Check we reject modifier-values when we ought to
  tpl = StringToTemplate("hi {{VAR:j=4}} lo", STRIP_WHITESPACE);
  ASSERT(tpl == NULL);
  tpl = StringToTemplate("hi {{VAR:html_escape=yes}} lo", STRIP_WHITESPACE);
  ASSERT(tpl == NULL);
  tpl = StringToTemplate("hi {{VAR:url_query_escape=wombats}} lo",
                         STRIP_WHITESPACE);
  ASSERT(tpl == NULL);

  // Check we don't allow modifiers on sections
  tpl = StringToTemplate("hi {{#VAR:h}} lo {{/VAR}}", STRIP_WHITESPACE);
  ASSERT(tpl == NULL);

  // Test when expanded grows by more than 12% per modifier.
  dict.SetValue("VAR", "http://a.com?b=c&d=e&f=g&q=a>b");
  tpl = StringToTemplate("{{VAR:u:j:h}}",
                         STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict,
                 "http%3A//a.com%3Fb%3Dc%26d%3De%26f%3Dg%26q%3Da%3Eb",
                 true);

  // As above with 4 modifiers.
  dict.SetValue("VAR", "http://a.com?b=c&d=e&f=g&q=a>b");
  tpl = StringToTemplate("{{VAR:u:j:h:h}}",
                         STRIP_WHITESPACE);
  AssertExpandIs(tpl, &dict,
                 "http%3A//a.com%3Fb%3Dc%26d%3De%26f%3Dg%26q%3Da%3Eb",
                 true);
}

TEST(Template, Section) {
  Template* tpl = StringToTemplate(
      "boo!\nhi {{#SEC}}lo{{#SUBSEC}}jo{{/SUBSEC}}{{/SEC}} bar",
      STRIP_WHITESPACE);
  TemplateDictionary dict("dict");
  AssertExpandIs(tpl, &dict, "boo!hi  bar", true);
  dict.ShowSection("SEC");
  AssertExpandIs(tpl, &dict, "boo!hi lo bar", true);
  dict.ShowSection("SEC");
  AssertExpandIs(tpl, &dict, "boo!hi lo bar", true);
  // This should work even though subsec isn't a child of the main dict
  dict.ShowSection("SUBSEC");
  AssertExpandIs(tpl, &dict, "boo!hi lojo bar", true);

  TemplateDictionary dict2("dict2");
  dict2.AddSectionDictionary("SEC");
  AssertExpandIs(tpl, &dict2, "boo!hi lo bar", true);
  dict2.AddSectionDictionary("SEC");
  AssertExpandIs(tpl, &dict2, "boo!hi lolo bar", true);
  dict2.AddSectionDictionary("sec");
  AssertExpandIs(tpl, &dict2, "boo!hi lolo bar", true);
  dict2.ShowSection("SUBSEC");
  AssertExpandIs(tpl, &dict2, "boo!hi lojolojo bar", true);
}


TEST(Template, SectionSeparator) {
  Template* tpl = StringToTemplate(
      "hi {{#SEC}}lo{{#SEC_separator}}jo{{JO}}{{/SEC_separator}}{{/SEC}} bar",
      STRIP_WHITESPACE);
  TemplateDictionary dict("dict");
  AssertExpandIs(tpl, &dict, "hi  bar", true);
  // Since SEC is only expanded once, the separator section shouldn't show.
  dict.ShowSection("SEC");
  AssertExpandIs(tpl, &dict, "hi lo bar", true);
  dict.ShowSection("SEC");
  AssertExpandIs(tpl, &dict, "hi lo bar", true);
  // This should work even though SEC_separator isn't a child of the
  // main dict.  It verifies SEC_separator is just a normal section, too.
  dict.ShowSection("SEC_separator");
  AssertExpandIs(tpl, &dict, "hi lojo bar", true);

  TemplateDictionary dict2("dict2");
  dict2.AddSectionDictionary("SEC");
  AssertExpandIs(tpl, &dict2, "hi lo bar", true);
  dict2.AddSectionDictionary("SEC");
  AssertExpandIs(tpl, &dict2, "hi lojolo bar", true);
  // This is a weird case: using separator and specifying manually.
  dict2.ShowSection("SEC_separator");
  AssertExpandIs(tpl, &dict2, "hi lojojolojo bar", true);

  TemplateDictionary dict3("dict3");
  TemplateDictionary* sec1 = dict3.AddSectionDictionary("SEC");
  TemplateDictionary* sec2 = dict3.AddSectionDictionary("SEC");
  TemplateDictionary* sec3 = dict3.AddSectionDictionary("SEC");
  dict3.SetValue("JO", "J");
  AssertExpandIs(tpl, &dict3, "hi lojoJlojoJlo bar", true);
  sec1->SetValue("JO", "JO");
  AssertExpandIs(tpl, &dict3, "hi lojoJOlojoJlo bar", true);
  sec2->SetValue("JO", "JOO");
  AssertExpandIs(tpl, &dict3, "hi lojoJOlojoJOOlo bar", true);
  dict3.AddSectionDictionary("SEC");
  AssertExpandIs(tpl, &dict3, "hi lojoJOlojoJOOlojoJlo bar", true);
  sec3->AddSectionDictionary("SEC_separator");
  AssertExpandIs(tpl, &dict3, "hi lojoJOlojoJOOlojoJjoJlo bar", true);

  // Make sure we don't do anything special with var or include names
  Template* tpl2 = StringToTemplate(
      "hi {{#SEC}}lo{{>SEC_separator}}{{/SEC}} bar",
      STRIP_WHITESPACE);
  AssertExpandIs(tpl2, &dict2, "hi lolo bar", true);

  Template* tpl3 = StringToTemplate(
      "hi {{#SEC}}lo{{SEC_separator}}{{/SEC}} bar",
      STRIP_WHITESPACE);
  dict2.SetValue("SEC_separator", "-");
  AssertExpandIs(tpl3, &dict2, "hi lo-lo- bar", true);
}

TEST(Template, Include) {
  string incname = StringToTemplateFile("include file\n");
  string incname2 = StringToTemplateFile("inc2a\ninc2b\n");
  string incname_bad = StringToTemplateFile("{{syntax_error");
  Template* tpl = StringToTemplate("hi {{>INC}} bar\n", STRIP_WHITESPACE);
  TemplateDictionary dict("dict");
  AssertExpandIs(tpl, &dict, "hi  bar", true);
  dict.AddIncludeDictionary("INC");
  AssertExpandIs(tpl, &dict, "hi  bar", true);   // noop: no filename was set
  dict.AddIncludeDictionary("INC")->SetFilename("/notarealfile ");
  AssertExpandIs(tpl, &dict, "hi  bar", false);   // noop: illegal filename
  dict.AddIncludeDictionary("INC")->SetFilename(incname);
  AssertExpandIs(tpl, &dict, "hi include file bar", false);
  dict.AddIncludeDictionary("INC")->SetFilename(incname_bad);
  AssertExpandIs(tpl, &dict, "hi include file bar",
                 false);  // noop: syntax error
  dict.AddIncludeDictionary("INC")->SetFilename(incname);
  AssertExpandIs(tpl, &dict, "hi include fileinclude file bar", false);
  dict.AddIncludeDictionary("inc")->SetFilename(incname);
  AssertExpandIs(tpl, &dict, "hi include fileinclude file bar", false);
  dict.AddIncludeDictionary("INC")->SetFilename(incname2);
  AssertExpandIs(tpl, &dict,
                 "hi include fileinclude fileinc2ainc2b bar", false);

  // Now test that includes preserve Strip
  Template* tpl2 = StringToTemplate("hi {{>INC}} bar", DO_NOT_STRIP);
  AssertExpandIs(tpl2, &dict,
                 "hi include file\ninclude file\ninc2a\ninc2b\n bar", false);

  // Test that if we indent the include, every line on the include
  // is indented.
  Template* tpl3 = StringToTemplate("hi\n  {{>INC}} bar", DO_NOT_STRIP);
  AssertExpandIs(tpl3, &dict,
                 "hi\n  include file\n  include file\n"
                 "  inc2a\n  inc2b\n   bar",
                 false);
  // But obviously, if we strip leading whitespace, no indentation.
  Template* tpl4 = StringToTemplate("hi\n  {{>INC}} bar", STRIP_WHITESPACE);
  AssertExpandIs(tpl4, &dict,
                 "hiinclude fileinclude fileinc2ainc2b bar", false);
  // And if it's not a whitespace indent, we don't indent either.
  Template* tpl5 = StringToTemplate("hi\n - {{>INC}} bar", DO_NOT_STRIP);
  AssertExpandIs(tpl5, &dict,
                 "hi\n - include file\ninclude file\n"
                 "inc2a\ninc2b\n bar",
                 false);
  // Make sure we indent properly at the beginning.
  Template* tpl6 = StringToTemplate("  {{>INC}}\nbar", DO_NOT_STRIP);
  AssertExpandIs(tpl6, &dict,
                 "  include file\n  include file\n"
                 "  inc2a\n  inc2b\n  \nbar",
                 false);
  // And deal correctly when we include twice in a row.
  Template* tpl7 = StringToTemplate("  {{>INC}}-{{>INC}}", DO_NOT_STRIP);
  AssertExpandIs(tpl7, &dict,
                 "  include file\n  include file\n  inc2a\n  inc2b\n  "
                 "-include file\ninclude file\ninc2a\ninc2b\n",
                 false);
}

TEST(Template, IncludeWithModifiers) {
  string incname = StringToTemplateFile("include & print file\n");
  string incname2 = StringToTemplateFile("inc2\n");
  string incname3 = StringToTemplateFile("yo&yo");
  // Note this also tests that html-escape, but not javascript-escape or
  // pre-escape, escapes \n to <space>
  Template* tpl1 = StringToTemplate("hi {{>INC:h}} bar\n", DO_NOT_STRIP);
  Template* tpl2 = StringToTemplate("hi {{>INC:javascript_escape}} bar\n",
                                    DO_NOT_STRIP);
  Template* tpl3 = StringToTemplate("hi {{>INC:pre_escape}} bar\n",
                                    DO_NOT_STRIP);
  Template* tpl4 = StringToTemplate("hi {{>INC:u}} bar\n", DO_NOT_STRIP);
  // Test that if we include the same template twice, once with a modifer
  // and once without, they each get applied properly.
  Template* tpl5 = StringToTemplate("hi {{>INC:h}} bar {{>INC}} baz\n",
                                    DO_NOT_STRIP);

  TemplateDictionary dict("dict");
  AssertExpandIs(tpl1, &dict, "hi  bar\n", true);
  dict.AddIncludeDictionary("INC")->SetFilename(incname);
  AssertExpandIs(tpl1, &dict, "hi include &amp; print file  bar\n", true);
  dict.AddIncludeDictionary("INC")->SetFilename(incname2);
  AssertExpandIs(tpl1, &dict, "hi include &amp; print file inc2  bar\n",
                 true);
  AssertExpandIs(tpl2, &dict, "hi include \\x26 print file\\ninc2\\n bar\n",
                 true);
  AssertExpandIs(tpl3, &dict, "hi include &amp; print file\ninc2\n bar\n",
                 true);
  dict.AddIncludeDictionary("INC")->SetFilename(incname3);
  AssertExpandIs(tpl4, &dict,
                 "hi include+%26+print+file%0Ainc2%0Ayo%26yo bar\n",
                 true);
  AssertExpandIs(tpl5, &dict,
                 "hi include &amp; print file inc2 yo&amp;yo bar "
                 "include & print file\ninc2\nyo&yo baz\n",
                 true);

  // Don't test modifier syntax here; that's in TestVariableWithModifiers()
}

// Make sure we don't deadlock when a template includes itself.
// This also tests we handle recursive indentation properly.
TEST(Template, RecursiveInclude) {
  string incname = StringToTemplateFile("hi {{>INC}} bar\n  {{>INC}}!");
  Template* tpl = Template::GetTemplate(incname, DO_NOT_STRIP);
  TemplateDictionary dict("dict");
  dict.AddIncludeDictionary("INC")->SetFilename(incname);
  // Note the last line is indented 4 spaces instead of 2.  This is
  // because the last sub-include is indented.
  AssertExpandIs(tpl, &dict, "hi hi  bar\n  ! bar\n  hi  bar\n    !!", true);
}

// Tests that vars inherit/override their parents properly
TEST(Template, Inheritence) {
  Template* tpl = StringToTemplate("{{FOO}}{{#SEC}}{{FOO}}{{#SEC}}{{FOO}}{{/SEC}}{{/SEC}}",
                                   STRIP_WHITESPACE);
  TemplateDictionary dict("dict");
  dict.SetValue("FOO", "foo");
  dict.ShowSection("SEC");
  AssertExpandIs(tpl, &dict, "foofoofoo", true);

  TemplateDictionary dict2("dict2");
  dict2.SetValue("FOO", "foo");
  TemplateDictionary* sec = dict2.AddSectionDictionary("SEC");
  AssertExpandIs(tpl, &dict2, "foofoofoo", true);
  sec->SetValue("FOO", "bar");
  AssertExpandIs(tpl, &dict2, "foobarbar", true);
  TemplateDictionary* sec2 = sec->AddSectionDictionary("SEC");
  AssertExpandIs(tpl, &dict2, "foobarbar", true);
  sec2->SetValue("FOO", "baz");
  AssertExpandIs(tpl, &dict2, "foobarbaz", true);

  // Now test an include template, which shouldn't inherit from its parents
  tpl = StringToTemplate("{{FOO}}{{#SEC}}hi{{/SEC}}\n{{>INC}}",
                         STRIP_WHITESPACE);
  string incname = StringToTemplateFile(
      "include {{FOO}}{{#SEC}}invisible{{/SEC}}file\n");
  TemplateDictionary incdict("dict");
  incdict.ShowSection("SEC");
  incdict.SetValue("FOO", "foo");
  incdict.AddIncludeDictionary("INC")->SetFilename(incname);
  AssertExpandIs(tpl, &incdict, "foohiinclude file", true);
}

TEST(Template, TemplateString) {
  // Make sure using TemplateString and StaticTemplateString for the
  // dictionary expands the same as using char*'s.
  Template* tpl = StringToTemplate("hi {{VAR}} lo", STRIP_WHITESPACE);
  TemplateDictionary dict("dict");
  dict.SetValue("VAR", TemplateString("short-lived", strlen("short")));
  AssertExpandIs(tpl, &dict, "hi short lo", true);
  dict.SetValue("VAR", kHello);
  AssertExpandIs(tpl, &dict, "hi Hello lo", true);
}

// Tests that we append to the output string, rather than overwrite
TEST(Template, Expand) {
  Template* tpl = StringToTemplate("hi", STRIP_WHITESPACE);
  TemplateDictionary dict("test_expand");
  string output("premade");
  ASSERT(tpl->Expand(&output, &dict));
  ASSERT_STREQ(output.c_str(), "premadehi");

  tpl = StringToTemplate("   lo   ", STRIP_WHITESPACE);
  ASSERT(tpl->Expand(&output, &dict));
  ASSERT_STREQ(output.c_str(), "premadehilo");
}

TEST(Template, ExpandTemplate) {
  string filename = StringToTemplateFile("  hi {{THERE}}");
  TemplateDictionary dict("test_expand");
  dict.SetValue("THERE", "test");
  string output;
  ASSERT(ExpandTemplate(filename, STRIP_WHITESPACE, &dict, &output));
  ASSERT_STREQ(output.c_str(), "hi test");

  // This will append to output, so we see both together.
  ASSERT(ExpandWithData(filename, DO_NOT_STRIP, &dict, NULL, &output));
  ASSERT_STREQ(output.c_str(), "hi test  hi test");

  ASSERT(!ExpandTemplate(filename + " not found", DO_NOT_STRIP, &dict,
                         &output));
}

TEST(Template, ExpandWithCustomEmitter) {
  Template* tpl = StringToTemplate("{{VAR}} {{VAR}}", STRIP_WHITESPACE);
  TemplateDictionary dict("test_expand");
  dict.SetValue("VAR", "this song is just six words long");
  string output;
  SizeofEmitter e(&output);
  ASSERT(tpl->Expand(&e, &dict));
  ASSERT_STREQ("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
               output.c_str());
}

TEST(Template, TemplateExpansionModifier) {
  string parent_tpl_name = StringToTemplateFile("before {{>INC}} after");
  string child_tpl_name1 = StringToTemplateFile("child1");
  string child_tpl_name2 = StringToTemplateFile("child2");
  Template* tpl = Template::GetTemplate(parent_tpl_name, DO_NOT_STRIP);

  TemplateDictionary dict("parent dict");
  dict.AddIncludeDictionary("INC")->SetFilename(child_tpl_name1);
  dict.AddIncludeDictionary("INC")->SetFilename(child_tpl_name2);

  PerExpandData per_expand_data;

  EmphasizeTemplateModifier modifier1(child_tpl_name1);
  per_expand_data.SetTemplateExpansionModifier(&modifier1);
  AssertExpandWithDataIs(tpl, &dict, &per_expand_data,
                         "before >>child1<<child2 after", true);

  EmphasizeTemplateModifier modifier2(child_tpl_name2);
  per_expand_data.SetTemplateExpansionModifier(&modifier2);
  AssertExpandWithDataIs(tpl, &dict, &per_expand_data,
                         "before child1>>child2<< after", true);

  EmphasizeTemplateModifier modifier3(parent_tpl_name);
  per_expand_data.SetTemplateExpansionModifier(&modifier3);
  AssertExpandWithDataIs(tpl, &dict, &per_expand_data,
                         ">>before child1child2 after<<", true);

  per_expand_data.SetTemplateExpansionModifier(NULL);
  AssertExpandWithDataIs(tpl, &dict, &per_expand_data,
                         "before child1child2 after", true);
}

TEST(Template, GetTemplate) {
  // Tests the cache
  string filename = StringToTemplateFile("{This is perfectly valid} yay!");
  Template* tpl1 = Template::GetTemplate(filename, DO_NOT_STRIP);
  Template* tpl2 = Template::GetTemplate(filename.c_str(), DO_NOT_STRIP);
  Template* tpl3 = Template::GetTemplate(filename, STRIP_WHITESPACE);
  ASSERT(tpl1 && tpl2 && tpl3);
  ASSERT(tpl1 == tpl2);
  ASSERT(tpl1 != tpl3);

  // Tests that a nonexistent template returns NULL
  Template* tpl4 = Template::GetTemplate("/yakakak", STRIP_WHITESPACE);
  ASSERT(!tpl4);

  // Tests that syntax errors cause us to return NULL
  Template* tpl5 = StringToTemplate("{{This has spaces in it}}", DO_NOT_STRIP);
  ASSERT(!tpl5);
  Template* tpl6 = StringToTemplate("{{#SEC}}foo", DO_NOT_STRIP);
  ASSERT(!tpl6);
  Template* tpl7 = StringToTemplate("{{#S1}}foo{{/S2}}", DO_NOT_STRIP);
  ASSERT(!tpl7);
  Template* tpl8 = StringToTemplate("{{#S1}}foo{{#S2}}bar{{/S1}{{/S2}",
                                    DO_NOT_STRIP);
  ASSERT(!tpl8);
  Template* tpl9 = StringToTemplate("{{noend", DO_NOT_STRIP);
  ASSERT(!tpl9);
}

TEST(Template, StringCacheKey) {
  // If you use these same cache keys somewhere else,
  // call Template::ClearCache first.
  const string cache_key_a = "cache key a";
  const string text = "Test template 1";
  TemplateDictionary empty_dict("dict");

  // When a string template is registered via StringToTemplateCache,
  // we can use GetTemplate for that same cache-key under any other
  // Strip because we cache the contents.
  Template *tpl1, *tpl2;
  ASSERT(Template::StringToTemplateCache(cache_key_a, text));
  tpl1 = Template::GetTemplate(cache_key_a, DO_NOT_STRIP);
  AssertExpandIs(tpl1, &empty_dict, text, true);

  // Different strip.
  ASSERT(tpl2 = Template::GetTemplate(cache_key_a, STRIP_BLANK_LINES));
  ASSERT(tpl2 != tpl1);
  AssertExpandIs(tpl2, &empty_dict, text, true);

  Template::ClearCache();
}

TEST(Template, StringGetTemplate) {
  TemplateDictionary dict("dict");

  // Test cache lookups
  const char* const tpltext = "{This is perfectly valid} yay!";
  ASSERT(Template::StringToTemplateCache("tgt", tpltext));

  Template* tpl1 = Template::GetTemplate("tgt", DO_NOT_STRIP);
  Template* tpl2 = Template::GetTemplate("tgt", STRIP_WHITESPACE);
  ASSERT(tpl1 && tpl2);
  ASSERT(tpl1 != tpl2);
  AssertExpandIs(tpl1, &dict, tpltext, true);
  AssertExpandIs(tpl2, &dict, tpltext, true);

  // If we register a new string under the same text, it should be
  // ignored.
  ASSERT(!Template::StringToTemplateCache("tgt", tpltext));
  ASSERT(!Template::StringToTemplateCache("tgt", "new text"));
  Template* tpl3 = Template::GetTemplate("tgt", DO_NOT_STRIP);
  ASSERT(tpl3 == tpl1);
  AssertExpandIs(tpl3, &dict, tpltext, true);

  // Tests that syntax errors cause us to return NULL
  ASSERT(!Template::StringToTemplateCache("tgt2", "{{This has spaces}}"));
  ASSERT(!Template::StringToTemplateCache("tgt3", "{{#SEC}}foo"));
  ASSERT(!Template::StringToTemplateCache("tgt4", "{{#S1}}foo{{/S2}}"));
  ASSERT(!Template::StringToTemplateCache("tgt5",
                                          "{{#S1}}foo{{#S2}}bar{{/S1}{{/S2}"));
  ASSERT(!Template::StringToTemplateCache("tgt6", "{{noend"));
  // And that we didn't cache them by mistake
  ASSERT(!Template::GetTemplate("tgt2", STRIP_WHITESPACE));

  Template::ClearCache();
}

TEST(Template, StringTemplateInclude) {
  Template::ClearCache();   // just for exercise.
  const string cache_key = "TestStringTemplateInclude";
  const string cache_key_inc = "TestStringTemplateInclude-inc";
  const string cache_key_indent = "TestStringTemplateInclude-indent";
  const string text = "<html>{{>INC}}</html>";
  const string text_inc = "<div>\n<p>\nUser {{USER}}\n</div>";
  const string text_indent = "<html>\n  {{>INC}}</html>";

  ASSERT(Template::StringToTemplateCache(cache_key, text));
  ASSERT(Template::StringToTemplateCache(cache_key_inc, text_inc));
  ASSERT(Template::StringToTemplateCache(cache_key_indent, text_indent));

  Template *tpl = Template::GetTemplate(cache_key, DO_NOT_STRIP);
  ASSERT(tpl);

  TemplateDictionary dict("dict");
  TemplateDictionary* sub_dict = dict.AddIncludeDictionary("INC");
  sub_dict->SetFilename(cache_key_inc);

  sub_dict->SetValue("USER", "John<>Doe");
  string expected = "<html><div>\n<p>\nUser John<>Doe\n</div></html>";
  AssertExpandIs(tpl, &dict, expected, true);

  // Repeat the same except that now the parent has a template-level
  // directive (by way of the automatic-line-indenter).
  tpl = Template::GetTemplate(cache_key_indent, DO_NOT_STRIP);
  ASSERT(tpl);
  expected =
      "<html>\n"
      "  <div>\n"
      "  <p>\n"
      "  User John<>Doe\n"
      "  </div>"
      "</html>";
  AssertExpandIs(tpl, &dict, expected, true);

  Template::ClearCache();
}

TEST(Template, TemplateSearchPath) {
  const string pathA = PathJoin(FLAGS_test_tmpdir, "a/");
  const string pathB = PathJoin(FLAGS_test_tmpdir, "b/");
  CreateOrCleanTestDir(pathA);
  CreateOrCleanTestDir(pathB);

  TemplateDictionary dict("");
  Template::SetTemplateRootDirectory(pathA);
  Template::AddAlternateTemplateRootDirectory(pathB);

  // 1. Show that a template in the secondary path can be found.
  const string path_b_bar = PathJoin(pathB, "template_bar");
  StringToFile("b/template_bar", path_b_bar);
  ASSERT_STREQ(path_b_bar.c_str(),
               Template::FindTemplateFilename("template_bar").c_str());
  Template* b_bar = Template::GetTemplate("template_bar", DO_NOT_STRIP);
  ASSERT(b_bar);
  AssertExpandIs(b_bar, &dict, "b/template_bar", true);

  // 2. Show that the search stops once the first match is found.
  //    Create two templates in separate directories with the same name.
  const string path_a_foo = PathJoin(pathA, "template_foo");
  StringToFile("a/template_foo", path_a_foo);
  StringToFile("b/template_foo", PathJoin(pathB, "template_foo"));
  ASSERT_STREQ(path_a_foo.c_str(),
               Template::FindTemplateFilename("template_foo").c_str());
  Template* a_foo = Template::GetTemplate("template_foo", DO_NOT_STRIP);
  ASSERT(a_foo);
  AssertExpandIs(a_foo, &dict, "a/template_foo", true);

  // 3. Show that attempting to find a non-existent template gives an
  //    empty path.
  ASSERT(Template::FindTemplateFilename("baz").empty());

  CreateOrCleanTestDir(pathA);
  CreateOrCleanTestDir(pathB);
}

TEST(Template, RemoveStringFromTemplateCache) {
  Template::ClearCache();   // just for exercise.
  const string cache_key = "TestRemoveStringFromTemplateCache";
  const string text = "<html>here today...</html>";

  TemplateDictionary dict("test");
  ASSERT(Template::StringToTemplateCache(cache_key, text));
  Template* tpl = Template::GetTemplate(cache_key, DO_NOT_STRIP);
  ASSERT(tpl);
  AssertExpandIs(tpl, &dict, text, true);
  tpl = Template::GetTemplate(cache_key, STRIP_WHITESPACE);
  ASSERT(tpl);
  AssertExpandIs(tpl, &dict, text, true);

  Template::RemoveStringFromTemplateCache(cache_key);
  tpl = Template::GetTemplate(cache_key, DO_NOT_STRIP);
  ASSERT(!tpl);
  tpl = Template::GetTemplate(cache_key, STRIP_WHITESPACE);
  ASSERT(!tpl);
  tpl = Template::GetTemplate(cache_key, STRIP_BLANK_LINES);
  ASSERT(!tpl);
}

TEST(Template, TemplateCache) {
  const string filename_a = StringToTemplateFile("Test template 1");
  const string filename_b = StringToTemplateFile("Test template 2.");

  Template *tpl, *tpl2;
  ASSERT(tpl = Template::GetTemplate(filename_a, DO_NOT_STRIP));

  ASSERT(tpl2 = Template::GetTemplate(filename_b, DO_NOT_STRIP));
  ASSERT(tpl2 != tpl);  // different filenames.
  ASSERT(tpl2 = Template::GetTemplate(filename_a, STRIP_BLANK_LINES));
  ASSERT(tpl2 != tpl);  // different strip.
  ASSERT(tpl2 = Template::GetTemplate(filename_b, STRIP_BLANK_LINES));
  ASSERT(tpl2 != tpl);  // different filenames and strip.
  ASSERT(tpl2 = Template::GetTemplate(filename_a, DO_NOT_STRIP));
  ASSERT(tpl2 == tpl);  // same filename and strip.
}

// Tests that the various strip values all do the expected thing.
TEST(Template, Strip) {
  TemplateDictionary dict("dict");
  dict.SetValue("FOO", "foo");

  const char* tests[][4] = {  // 0: in, 1: do-not-strip, 2: blanklines, 3: ws
    {"hi!\n", "hi!\n", "hi!\n", "hi!"},
    {"hi!", "hi!", "hi!", "hi!"},
    // These test strip-blank-lines, primarily
    {"{{FOO}}\n\n{{FOO}}", "foo\n\nfoo", "foo\nfoo", "foofoo"},
    {"{{FOO}}\r\n\r\n{{FOO}}", "foo\r\n\r\nfoo", "foo\r\nfoo", "foofoo"},
    {"{{FOO}}\n   \n{{FOO}}\n", "foo\n   \nfoo\n", "foo\nfoo\n", "foofoo"},
    {"{{FOO}}\n{{BI_NEWLINE}}\nb", "foo\n\n\nb", "foo\n\n\nb", "foo\nb"},
    {"{{FOO}}\n{{!comment}}\nb", "foo\n\nb", "foo\nb", "foob"},
    {"{{FOO}}\n{{!comment}}{{!comment2}}\nb", "foo\n\nb", "foo\n\nb", "foob"},
    {"{{FOO}}\n{{>ONE_INC}}\nb", "foo\n\nb", "foo\nb", "foob"},
    {"{{FOO}}\n\t{{>ONE_INC}}  \nb", "foo\n\t  \nb", "foo\nb", "foob"},
    {"{{FOO}}\n{{>ONE_INC}}{{>TWO_INC}}\nb", "foo\n\nb", "foo\n\nb", "foob"},
    {"{{FOO}}\n  {{#SEC}}\ntext \n  {{/SEC}}\n", "foo\n  \n", "foo\n", "foo"},
    {"{{%AUTOESCAPE context=\"HTML\"}}\nBLA", "\nBLA", "BLA", "BLA"},
    // These test strip-whitespace
    {"foo\nbar\n", "foo\nbar\n", "foo\nbar\n", "foobar"},
    {"{{FOO}}\nbar\n", "foo\nbar\n", "foo\nbar\n", "foobar"},
    {"  {{FOO}}  {{!comment}}\nb", "  foo  \nb", "  foo  \nb", "foo  b"},
    {"  {{FOO}}  {{BI_SPACE}}\n", "  foo   \n", "  foo   \n", "foo   "},
    {"  \t \f\v  \n\r\n  ", "  \t \f\v  \n\r\n  ", "", ""},
  };

  for (int i = 0; i < sizeof(tests)/sizeof(*tests); ++i) {
    Template* tpl1 = StringToTemplate(tests[i][0], DO_NOT_STRIP);
    Template* tpl2 = StringToTemplate(tests[i][0], STRIP_BLANK_LINES);
    Template* tpl3 = StringToTemplate(tests[i][0], STRIP_WHITESPACE);
    AssertExpandIs(tpl1, &dict, tests[i][1], true);
    AssertExpandIs(tpl2, &dict, tests[i][2], true);
    AssertExpandIs(tpl3, &dict, tests[i][3], true);
  }
}

TEST(Template, TemplateRootDirectory) {
  string filename = StringToTemplateFile("Test template");
  ASSERT(IsAbspath(filename));
  Template* tpl1 = Template::GetTemplate(filename, DO_NOT_STRIP);
  Template::SetTemplateRootDirectory(kRootdir);  // "/"
  // template-root shouldn't matter for absolute directories
  Template* tpl2 = Template::GetTemplate(filename, DO_NOT_STRIP);
  Template::SetTemplateRootDirectory("/sadfadsf/waerfsa/safdg");
  Template* tpl3 = Template::GetTemplate(filename, DO_NOT_STRIP);
  ASSERT(tpl1 != NULL);
  ASSERT(tpl1 == tpl2);
  ASSERT(tpl1 == tpl3);

  // Now test it actually works by breaking the abspath in various places.
  // We do it twice, since we don't know if the path-sep is "/" or "\".
  // NOTE: this depends on filename not using "/" or "\" except as a
  //       directory separator (so nothing like "/var/tmp/foo\a/weirdfile").
  const char* const kPathSeps = "/\\";
  for (const char* path_sep = kPathSeps; *path_sep; path_sep++) {
    for (string::size_type sep_pos = filename.find(*path_sep, 0);
         sep_pos != string::npos;
         sep_pos = filename.find(*path_sep, sep_pos + 1)) {
      Template::SetTemplateRootDirectory(filename.substr(0, sep_pos + 1));
      Template* tpl = Template::GetTemplate(filename.substr(sep_pos + 1),
                                            DO_NOT_STRIP);
      ASSERT(string(tpl->template_file()) == tpl1->template_file());
    }
  }
}

#if defined(HAVE_PTHREAD) && !defined(NO_THREADS)
struct ThreadReturn {
  Template* file_template;
  bool string_to_template_cache_return;
  Template* string_template;
};

// RunThread returns a ThreadReturn* that should be deleted.
static void* RunThread(void* vfilename) {
  const char* filename = reinterpret_cast<const char*>(vfilename);
  ThreadReturn* ret = new ThreadReturn;
  ret->file_template = Template::GetTemplate(filename, DO_NOT_STRIP);
  ASSERT(ret->file_template != NULL);
  const char* const key = "RunThread key";
  ret->string_to_template_cache_return =
      StringToTemplateCache(key, " RunThread text ", STRIP_WHITESPACE);
  ret->string_template = Template::GetTemplate(key, STRIP_WHITESPACE);
  ASSERT(ret->string_template != NULL);
  return ret;
}

TEST(Template, ThreadSafety) {
  string filename = StringToTemplateFile("(testing thread-safety)");

  // GetTemplate() is the most thread-contended routine.  We get a
  // template in many threads, and assert we get the same template
  // from each.
  pthread_t thread_ids[kNumThreads];
  for (int i = 0; i < kNumThreads; ++i) {
    ASSERT(pthread_create(thread_ids+i, NULL, RunThread,
                          (void*)filename.c_str())
           == 0);
  }

  // Wait for all the threads to terminate (should be very quick!)
  ThreadReturn* first_thread_return = NULL;
  int num_times_string_to_template_cache_returned_true = 0;
  for (int i = 0; i < kNumThreads; ++i) {
    void* vthread_return;
    ASSERT(pthread_join(thread_ids[i], &vthread_return) == 0);
    ThreadReturn* thread_return =
        reinterpret_cast<ThreadReturn*>(vthread_return);
    if (thread_return->string_to_template_cache_return) {
      ++num_times_string_to_template_cache_returned_true;
    }
    if (first_thread_return == NULL) {   // we're the first thread
      first_thread_return = thread_return;
    } else {
      ASSERT(thread_return->file_template ==
             first_thread_return->file_template);
      ASSERT(thread_return->string_template ==
             first_thread_return->string_template);
      delete thread_return;
    }
  }
  delete first_thread_return;
  ASSERT_INTEQ(1, num_times_string_to_template_cache_returned_true);
  Template::ClearCache();
}
#endif  // #if defined(HAVE_PTHREAD) && !defined(NO_THREADS)

// Tests all the static methods in TemplateNamelist
TEST(Template, TemplateNamelist) {
  time_t before_time = Now();   // in template_test_util.cc
  string f1 = StringToTemplateFile("{{This has spaces in it}}");
  string f2 = StringToTemplateFile("{{#SEC}}foo");
  string f3 = StringToTemplateFile("{This is ok");
  // Where we'll copy f1 - f3 to: these are names known at compile-time
  string f1_copy = PathJoin(FLAGS_test_tmpdir, INVALID1_FN);
  string f2_copy = PathJoin(FLAGS_test_tmpdir, INVALID2_FN);
  string f3_copy = PathJoin(FLAGS_test_tmpdir, VALID1_FN);
  Template::SetTemplateRootDirectory(FLAGS_test_tmpdir);
  time_t after_time = Now();   // f1, f2, f3 all written by now

  TemplateNamelist::NameListType names = TemplateNamelist::GetList();
  ASSERT(names.size() == 4);
  ASSERT(names.count(NONEXISTENT_FN));
  ASSERT(names.count(INVALID1_FN));
  ASSERT(names.count(INVALID2_FN));
  ASSERT(names.count(VALID1_FN));

  // Before creating the files INVALID1_FN, etc., all should be missing.
  for (int i = 0; i < 3; ++i) {   // should be consistent all 3 times
    const TemplateNamelist::MissingListType& missing =
        TemplateNamelist::GetMissingList(false);
    ASSERT(missing.size() == 4);
  }
  // Everyone is missing, but nobody should have bad syntax
  ASSERT(!TemplateNamelist::AllDoExist());
  ASSERT(TemplateNamelist::IsAllSyntaxOkay(DO_NOT_STRIP));

  // Now create those files
  ASSERT(link(f1.c_str(), f1_copy.c_str()) == 0);
  ASSERT(link(f2.c_str(), f2_copy.c_str()) == 0);
  ASSERT(link(f3.c_str(), f3_copy.c_str()) == 0);
  // We also have to clear the template cache, since we created a new file.
  // ReloadAllIfChanged() would probably work, too.
  Template::ClearCache();

  // When GetMissingList is false, we don't reload, so you still get all-gone
  TemplateNamelist::MissingListType missing =
      TemplateNamelist::GetMissingList(false);
  ASSERT(missing.size() == 4);
  // But with true, we should have a different story
  missing = TemplateNamelist::GetMissingList(true);
  ASSERT(missing.size() == 1);
  missing = TemplateNamelist::GetMissingList(false);
  ASSERT(missing.size() == 1);
  ASSERT(missing[0] == NONEXISTENT_FN);
  ASSERT(!TemplateNamelist::AllDoExist());

  // IsAllSyntaxOK did a badsyntax check, before the files were created.
  // So with a false arg, should still say everything is ok
  TemplateNamelist::SyntaxListType badsyntax =
      TemplateNamelist::GetBadSyntaxList(false, DO_NOT_STRIP);
  ASSERT(badsyntax.size() == 0);
  // But IsAllSyntaxOK forces a refresh
  ASSERT(!TemplateNamelist::IsAllSyntaxOkay(DO_NOT_STRIP));
  badsyntax = TemplateNamelist::GetBadSyntaxList(false, DO_NOT_STRIP);
  ASSERT(badsyntax.size() == 2);
  ASSERT(badsyntax[0] == INVALID1_FN || badsyntax[1] == INVALID1_FN);
  ASSERT(badsyntax[0] == INVALID2_FN || badsyntax[1] == INVALID2_FN);
  ASSERT(!TemplateNamelist::IsAllSyntaxOkay(DO_NOT_STRIP));
  badsyntax = TemplateNamelist::GetBadSyntaxList(true, DO_NOT_STRIP);
  ASSERT(badsyntax.size() == 2);

  time_t modtime = TemplateNamelist::GetLastmodTime();
  ASSERT(modtime >= before_time && modtime <= after_time);
  // Now update a file and make sure lastmod time is updated.
  // Note that since TemplateToFile uses "fake" timestamps way
  // in the past, this append should definitely give a time
  // that's after after_time.
  FILE* fp = fopen(f1_copy.c_str(), "ab");
  ASSERT(fp);
  fwrite("\n", 1, 1, fp);
  fclose(fp);
  modtime = TemplateNamelist::GetLastmodTime();
  ASSERT(modtime > after_time);

  // Checking if we can register templates at run time.
  string f4 = StringToTemplateFile("{{ONE_GOOD_TEMPLATE}}");
  TemplateNamelist::RegisterTemplate(f4.c_str());
  names = TemplateNamelist::GetList();
  ASSERT(names.size() == 5);

  string f5 = StringToTemplateFile("{{ONE BAD TEMPLATE}}");
  TemplateNamelist::RegisterTemplate(f5.c_str());
  names = TemplateNamelist::GetList();
  ASSERT(names.size() == 6);
  badsyntax = TemplateNamelist::GetBadSyntaxList(false, DO_NOT_STRIP);
  ASSERT(badsyntax.size() == 2);  // we did not refresh the bad syntax list
  badsyntax = TemplateNamelist::GetBadSyntaxList(true, DO_NOT_STRIP);
  // After refresh, the file we just registerd also added in bad syntax list
  ASSERT(badsyntax.size() == 3);

  TemplateNamelist::RegisterTemplate("A_non_existant_file.tpl");
  names = TemplateNamelist::GetList();
  ASSERT(names.size() == 7);
  missing = TemplateNamelist::GetMissingList(false);
  ASSERT(missing.size() == 1);  // we did not refresh the missing list
  missing = TemplateNamelist::GetMissingList(true);
  // After refresh, the file we just registerd also added in missing list
  ASSERT(missing.size() == 2);
}

// This test is not "end-to-end", it doesn't use a dictionary
// and only outputs what the template system thinks is the
// correct modifier for variables.
TEST(Template, CorrectModifiersForAutoEscape) {
  string text, expected_out;

  // template with no variable, nothing to emit.
  text = "Static template.";
  AssertCorrectModifiers(TC_HTML, text, "");

  // Simple templates with one variable substitution.

  // 1. No in-template modifiers. Auto Escaper sets correct ones.
  text = "Hello {{USER}}";
  AssertCorrectModifiers(TC_HTML, text, "USER:h\n");

  // Complete URLs in different attributes that take URLs.
  text = "<a href=\"{{URL}}\">bla</a>";
  AssertCorrectModifiers(TC_HTML, text, "URL:U=html\n");
  text = "<script src=\"{{URL}}\"></script>";
  AssertCorrectModifiers(TC_HTML, text, "URL:U=html\n");
  text = "<img src=\"{{URL}}\">";
  AssertCorrectModifiers(TC_HTML, text, "URL:U=html\n");
  // URL fragment only so just html_escape.
  text = "<img src=\"/bla?q={{QUERY}}\">";
  AssertCorrectModifiers(TC_HTML, text, "QUERY:h\n");
  // URL fragment not quoted, so url_escape.
  text = "<img src=/bla?q={{QUERY}}>";
  AssertCorrectModifiers(TC_HTML, text, "QUERY:u\n");

  text = "<br class=\"{{CLASS}}\">";
  AssertCorrectModifiers(TC_HTML, text, "CLASS:h\n");
  text = "<br class={{CLASS}}>";
  AssertCorrectModifiers(TC_HTML, text, "CLASS:H=attribute\n");
  text = "<br {{CLASS}}>";   // CLASS here is name/value pair.
  AssertCorrectModifiers(TC_HTML, text, "CLASS:H=attribute\n");
  text = "<br style=\"display:{{DISPLAY}}\">";   // Style attribute.
  AssertCorrectModifiers(TC_HTML, text, "DISPLAY:c\n");

  // Content inside a style tag should have :c regardless of quoting.
  text = "<style>color:{{COLOR}}; font:\"{{FONT}}\"</style>";
  AssertCorrectModifiers(TC_HTML, text, "COLOR:c\nFONT:c\n");

  // onMouseEvent and onKeyUp accept javascript.
  text = "<a href=\"url\" onkeyup=\"doX('{{ID}}');\">";  // ID quoted
  AssertCorrectModifiers(TC_HTML, text, "ID:j\n");
  text = "<a href=\"url\" onclick=\"doX({{ID}});\">";    // ID not quoted
  AssertCorrectModifiers(TC_HTML, text, "ID:J=number\n");
  text = "<a href=\"url\" onclick=\"'{{ID}}'\">";        // not common
  AssertCorrectModifiers(TC_HTML, text, "ID:j\n");
  // If ID is javascript code, J=number  will break it, for good and bad.
  text = "<a href=\"url\" onclick=\"{{ID}}\">";
  AssertCorrectModifiers(TC_HTML, text, "ID:J=number\n");

  // Target just needs html escaping.
  text = "<a href=\"url\" target=\"{{TARGET}}\">";
  AssertCorrectModifiers(TC_HTML, text, "TARGET:h\n");

  // Test a parsing corner case which uses TemplateDirective
  // call in the parser to change state properly. To reproduce
  // both variables should be unquoted and the first should
  // have no value except the variable substitution.
  text = "<img class={{CLASS}} src=/bla?q={{QUERY}}>";
  AssertCorrectModifiers(TC_HTML, text, "CLASS:H=attribute\nQUERY:u\n");

  // TODO(jad): Once we have a fix for it in code, fix me.
  // Javascript URL is not properly supported, we currently
  // apply :h which is not sufficient.
  text = "<a href=\"javascript:foo('{{VAR}}')>bla</a>";
  AssertCorrectModifiers(TC_HTML, text, "VAR:h\n");

  // Special handling for BI_SPACE and BI_NEWLINE.
  text = "{{BI_SPACE}}";
  AssertCorrectModifiers(TC_HTML, text, "BI_SPACE\n");      // Untouched.
  text = "{{BI_NEWLINE}}";
  AssertCorrectModifiers(TC_HTML, text, "BI_NEWLINE\n");    // Untouched.
  // Check that the parser is parsing BI_SPACE, if not, it would have failed.
  text = "<a href=/bla{{BI_SPACE}}style=\"{{VAR}}\">text</a>";
  AssertCorrectModifiers(TC_HTML, text, "BI_SPACE\nVAR:c\n");


  // XML and JSON modes.
  text = "<PARAM name=\"{{VAL}}\">{{DATA}}";
  AssertCorrectModifiers(TC_XML, text, "VAL:xml_escape\nDATA:xml_escape\n");
  text = "{ x = \"{{VAL}}\"}";
  AssertCorrectModifiers(TC_JSON, text, "VAL:j\n");

  // 2. Escaping modifiers were set, handle them.

  // 2a: Modifier :none is honored whether the escaping is correct or not.
  text = "Hello {{USER:none}}";                   // :none on its own.
  AssertCorrectModifiers(TC_HTML, text, "USER:none\n");
  text = "Hello {{USER:h:none}}";                 // correct escaping.
  AssertCorrectModifiers(TC_HTML, text, "USER:h:none\n");
  text = "Hello {{USER:j:none}}";                 // incorrect escaping.
  AssertCorrectModifiers(TC_HTML, text, "USER:j:none\n");
  text = "<a href=\"url\" onkeyup=\"doX('{{ID:none}}');\">";
  AssertCorrectModifiers(TC_HTML, text, "ID:none\n");

  // 2b: Correct modifiers, nothing to change.
  text = "Hello {{USER:h}}";
  AssertCorrectModifiers(TC_HTML, text, "USER:h\n");
  text = "Hello {{USER:U=html}}";  // :U=html is a valid replacement for .h
  AssertCorrectModifiers(TC_HTML, text, "USER:U=html\n");
  text = "Hello {{USER:H=url}}";   // :H=url (a.k.a. U=html) is valid too
  AssertCorrectModifiers(TC_HTML, text, "USER:H=url\n");
  text = "Hello {{USER:h:j}}";   // Extra :j, honor it.
  AssertCorrectModifiers(TC_HTML, text, "USER:h:j\n");
  text = "<a href=\"{{URL:U=html}}\">bla</a>";
  AssertCorrectModifiers(TC_HTML, text, "URL:U=html\n");
  text = "<a href=\"/bla?q={{QUERY:h}}\">bla</a>";  // :h is valid.
  AssertCorrectModifiers(TC_HTML, text, "QUERY:h\n");
  text = "<a href=\"/bla?q={{QUERY:u}}\">bla</a>";  // so is :u.
  AssertCorrectModifiers(TC_HTML, text, "QUERY:u\n");
  text = "<a href=\"url\" onclick=\"doX('{{ID:j}}');\">";
  AssertCorrectModifiers(TC_HTML, text, "ID:j\n");
  text = "<a href=\"url\" onclick=\"doX({{ID:J=number}});\">";
  AssertCorrectModifiers(TC_HTML, text, "ID:J=number\n");
  text = "<style>@import url(\"{{URL:U=css}}\")</style>";  // correct :U=css
  AssertCorrectModifiers(TC_HTML, text, "URL:U=css\n");

  // 2c: Incorrect modifiers, add our own.
  text = "Hello {{USER:j}}";                          // Missing :h
  AssertCorrectModifiers(TC_HTML, text, "USER:j:h\n");
  text = "Hello {{USER:c:c:c:c:c:j}}";                // Still missing :h
  AssertCorrectModifiers(TC_HTML, text, "USER:c:c:c:c:c:j:h\n");
  text = "<script>var a = \"{{VAR:h}}\";</script>";   // Missing :j
  AssertCorrectModifiers(TC_HTML, text, "VAR:h:j\n");
  text = "<script>var a = \"{{VAR:j:h:j}}\";</script>";   // Extra :h:j
  AssertCorrectModifiers(TC_HTML, text, "VAR:j:h:j\n");
  text = "<a href=\"url\" onclick=\"doX({{ID:j}});\">";   // Unquoted
  AssertCorrectModifiers(TC_HTML, text, "ID:j:J=number\n");

  // 2d: Custom modifiers are maintained.
  text = "Hello {{USER:x-bla}}";                  // Missing :h
  AssertCorrectModifiers(TC_HTML, text, "USER:x-bla:h\n");
  text = "Hello {{USER:x-bla:h}}";                // Correct, accept it.
  AssertCorrectModifiers(TC_HTML, text, "USER:x-bla:h\n");
  text = "Hello {{USER:x-bla:x-foo}}";            // Missing :h
  AssertCorrectModifiers(TC_HTML, text, "USER:x-bla:x-foo:h\n");
  text = "Hello {{USER:x-bla:none}}";             // Complete due to :none
  AssertCorrectModifiers(TC_HTML, text, "USER:x-bla:none\n");
  text = "Hello {{USER:h:x-bla}}";                // Still missing :h.
  AssertCorrectModifiers(TC_HTML, text, "USER:h:x-bla:h\n");
  text = "Hello {{USER:x-bla:h:x-foo}}";          // Still missing :h
  AssertCorrectModifiers(TC_HTML, text, "USER:x-bla:h:x-foo:h\n");
  text = "Hello {{USER:x-bla:h:x-foo:h}}";        // Valid, accept it.
  AssertCorrectModifiers(TC_HTML, text, "USER:x-bla:h:x-foo:h\n");

  // 2e: Equivalent modifiers are honored. All HTML Escapes.
  text = "Hello {{USER:p}}";
  AssertCorrectModifiers(TC_HTML, text, "USER:p\n");
  text = "Hello {{USER:H=attribute}}";
  AssertCorrectModifiers(TC_HTML, text, "USER:H=attribute\n");
  text = "Hello {{USER:H=snippet}}";
  AssertCorrectModifiers(TC_HTML, text, "USER:H=snippet\n");
  text = "Hello {{USER:H=pre}}";
  AssertCorrectModifiers(TC_HTML, text, "USER:H=pre\n");
  // All URL + HTML Escapes.
  text = "<a href=\"{{URL:H=url}}\">bla</a>";
  AssertCorrectModifiers(TC_HTML, text, "URL:H=url\n");
  text = "<a href=\"{{URL:U=html}}\">bla</a>";
  AssertCorrectModifiers(TC_HTML, text, "URL:U=html\n");

  // 2f: Initialize template in Javascript Context.
  text = "var a = '{{VAR}}'";                     // Escaping not given.
  AssertCorrectModifiers(TC_JS, text, "VAR:j\n");
  text = "var a = '{{VAR:none}}'";                // Variable safe.
  AssertCorrectModifiers(TC_JS, text, "VAR:none\n");
  text = "var a = '{{VAR:j}}'";                   // Escaping correct.
  AssertCorrectModifiers(TC_JS, text, "VAR:j\n");
  text = "var a = '{{VAR:h}}'";                   // Escaping incorrect.
  AssertCorrectModifiers(TC_JS, text, "VAR:h:j\n");
  text = "var a = '{{VAR:J=number}}'";            // Not considered equiv.
  AssertCorrectModifiers(TC_JS, text, "VAR:J=number:j\n");

  // 2g: Honor any modifiers for BI_SPACE and BI_NEWLINE.
  text = "{{BI_NEWLINE:j}}";     // An invalid modifier for the context.
  AssertCorrectModifiers(TC_HTML, text, "BI_NEWLINE:j\n");
  text = "{{BI_SPACE:h}}";       // An otherwise valid modifier.
  AssertCorrectModifiers(TC_HTML, text, "BI_SPACE:h\n");
  text = "{{BI_SPACE:x-bla}}";   // Also support custom modifiers.
  AssertCorrectModifiers(TC_HTML, text, "BI_SPACE:x-bla\n");

  // 2h: TC_CSS, TC_XML and TC_JSON
  text = "H1{margin-{{START_EDGE}}:0;\n text-align:{{END_EDGE}}\n}";
  AssertCorrectModifiers(TC_CSS, text, "START_EDGE:c\nEND_EDGE:c\n");
  text = "body{background:url('{{URL:U=css}}')}";  // :U=css valid substitute
  AssertCorrectModifiers(TC_CSS, text, "URL:U=css\n");
  text = "body{background:url('{{URL:U=html}}')}";  // Not valid, will add :c.
  AssertCorrectModifiers(TC_CSS, text, "URL:U=html:c\n");
  text = "<PARAM name=\"{{VAL:xml_escape}}\">";   // Correct escaping
  AssertCorrectModifiers(TC_XML, text, "VAL:xml_escape\n");
  text = "<PARAM name=\"{{VAL:H=attribute}}\">";   // XSS equivalent
  AssertCorrectModifiers(TC_XML, text, "VAL:H=attribute\n");
  text = "<PARAM name=\"{{VAL:h}}\">";   // XSS equivalent
  AssertCorrectModifiers(TC_XML, text, "VAL:h\n");
  text = "<PARAM name=\"{{VAL:H=pre}}\">";   // Not XSS equivalent
  AssertCorrectModifiers(TC_XML, text, "VAL:H=pre:xml_escape\n");
  text = "<PARAM name=\"{{VAL:c}}\">";   // Not XSS equivalent
  AssertCorrectModifiers(TC_XML, text, "VAL:c:xml_escape\n");
  text = "{user={{USER:j}}";   // Correct escaping
  AssertCorrectModifiers(TC_JSON, text, "USER:j\n");
  text = "{user={{USER:o}}";   // json_escape is XSS equivalent
  AssertCorrectModifiers(TC_JSON, text, "USER:o\n");
  text = "{user={{USER:h}}";   // but html_escape is not
  AssertCorrectModifiers(TC_JSON, text, "USER:h:j\n");

  // 2i: Variables with XssSafe Custom modifiers are untouched.
  ASSERT(ctemplate::AddXssSafeModifier("x-test-cm",
                                       &ctemplate::html_escape));
  text = "Hello {{USER:x-test-cm}}";              // Missing :h
  AssertCorrectModifiers(TC_HTML, text, "USER:x-test-cm\n");
  text = "Hello {{USER:x-test-cm:j}}";            // Extra :j
  AssertCorrectModifiers(TC_HTML, text, "USER:x-test-cm:j\n");
  text = "Hello {{USER:x-test-cm:x-foo}}";        // Non-safe modifier
  AssertCorrectModifiers(TC_HTML, text, "USER:x-test-cm:x-foo\n");
  text = "Hello {{USER:x-foo:x-test-cm}}";        // Non-safe modifier
  AssertCorrectModifiers(TC_HTML, text, "USER:x-foo:x-test-cm\n");
  text = "Hello {{USER:x-test-cm:none}}";         // Complete due to :none
  AssertCorrectModifiers(TC_HTML, text, "USER:x-test-cm:none\n");
  text = "Hello {{USER:h:x-test-cm}}";            // Prior escaping
  AssertCorrectModifiers(TC_HTML, text, "USER:h:x-test-cm\n");

  // 3. Larger test with close to every escaping case.

  text = "<html><head>\n"
      "<style>\n"
      "@import url(\"{{CSS_URL:U=css}}\");\n"
      "color:{{COLOR}}</style></head><body>\n"
      "<h1>{{TITLE}}</h1>\n"
      "<img src=\"{{IMG_URL}}\">\n"
      "<form action=\"/search\">\n"
      "  <input name=\"hl\" value={{HL}}>\n"
      "  <input name=\"m\" value=\"{{FORM_MSG}}\">\n"
      "</form>\n"
      "<div style=\"background:{{BG_COLOR}}\">\n"
      "</div>\n"
      "<script>\n"
      "  var msg_text = '{{MSG_TEXT}}';\n"
      "</script>\n"
      "<a href=\"url\" onmouseover=\"'{{MOUSE}}'\">bla</a>\n"
      "Goodbye friend {{USER}}!\n</body></html>\n";
  expected_out = "CSS_URL:U=css\n"
      "COLOR:c\n"
      "TITLE:h\n"
      "IMG_URL:U=html\n"
      "HL:H=attribute\n"
      "FORM_MSG:h\n"
      "BG_COLOR:c\n"
      "MSG_TEXT:j\n"
      "MOUSE:j\n"   // :j also escapes html entities
      "USER:h\n";
  AssertCorrectModifiers(TC_HTML, text, expected_out);
}

// More "end-to-end" test to ensure that variables are
// escaped as expected with auto-escape mode enabled.
// Obviously there is a lot more we can test.
TEST(Template, VariableWithAutoEscape) {
  string text, expected_out;
  TemplateDictionary dict("dict");
  string good_url("http://www.google.com/");
  string bad_url("javascript:alert();");

  text = "hi {{VAR}} lo";
  dict.SetValue("VAR", "<bad>yo");
  AssertCorrectEscaping(TC_HTML, dict, text, "hi &lt;bad&gt;yo lo");

  text = "<a href=\"{{URL}}\">bla</a>";
  dict.SetValue("URL", good_url);
  expected_out = "<a href=\"" + good_url + "\">bla</a>";
  AssertCorrectEscaping(TC_HTML, dict, text, expected_out);
  dict.SetValue("URL", bad_url);
  expected_out = "<a href=\"#\">bla</a>";
  AssertCorrectEscaping(TC_HTML, dict, text, expected_out);

  text = "<br style=\"display:{{DISPLAY}}\">";
  dict.SetValue("DISPLAY", "none");
  expected_out = "<br style=\"display:none\">";
  AssertCorrectEscaping(TC_HTML, dict, text, expected_out);
  // Bad characters are simply removed in CleanseCss.
  dict.SetValue("URL", "!#none_ ");
  expected_out = "<br style=\"display:none\">";
  AssertCorrectEscaping(TC_HTML, dict, text, expected_out);

  text = "<a href=\"url\" onkeyup=\"'{{EVENT}}'\">";
  dict.SetValue("EVENT", "safe");
  expected_out = "<a href=\"url\" onkeyup=\"'safe'\">";
  AssertCorrectEscaping(TC_HTML, dict, text, expected_out);
  dict.SetValue("EVENT", "f = 'y';");
  expected_out = "<a href=\"url\" onkeyup=\"'f \\x3d \\x27y\\x27;'\">";

  // Check special handling of BI_SPACE and BI_NEWLINE.
  text = "Hello\n{{BI_SPACE}}bla{{BI_NEWLINE}}foo.";
  expected_out = "Hello bla\nfoo.";
  AssertCorrectEscaping(TC_HTML, dict, text, expected_out);

  // TC_CSS
  text = "H1{margin-{{EDGE}}:0; text-align:{{BAD_EDGE}}}";
  dict.SetValue("EDGE", "left");
  dict.SetValue("BAD_EDGE", "$$center()!!");  // Bad chars are removed.
  AssertCorrectEscaping(TC_CSS, dict, text,
                        "H1{margin-left:0; text-align:center!!}");

  // TC_XML and TC_JSON
  text = "<Q>{{DATA}}</Q>";
  dict.SetValue("DATA", "good-data");
  AssertCorrectEscaping(TC_XML, dict, text, "<Q>good-data</Q>");
  dict.SetValue("DATA", "<BAD>FOO</BAD>");
  AssertCorrectEscaping(TC_XML, dict, text,
                        "<Q>&lt;BAD&gt;FOO&lt;/BAD&gt;</Q>");
  text = "{user = \"{{USER}}\"}";
  dict.SetValue("USER", "good-user");
  AssertCorrectEscaping(TC_JSON, dict, text, "{user = \"good-user\"}");
  dict.SetValue("USER", "evil'<>\"");
  AssertCorrectEscaping(TC_JSON, dict, text,
                        "{user = \"evil\\x27\\x3c\\x3e\\x22\"}");
}

// Test that the template initialization fails in auto-escape
// mode if the parser failed to parse.
TEST(Template, FailedInitWithAutoEscape) {
  Strip strip = STRIP_WHITESPACE;
  // Taken from HTML Parser test suite.
  string bad_html = "<a href='http://www.google.com' ''>\n";
  ASSERT(NULL == StringToTemplateWithAutoEscaping(bad_html, strip, TC_HTML));

  // Missing quotes around URL, not accepted in URL-taking attributes.
  bad_html = "<a href={{URL}}>bla</a>";
  ASSERT(NULL == StringToTemplateWithAutoEscaping(bad_html, strip, TC_HTML));

  // Missing quotes around STYLE, not accepted in style-taking attributes.
  bad_html = "<div style={{STYLE}}>";
  ASSERT(NULL == StringToTemplateWithAutoEscaping(bad_html, strip, TC_HTML));
}

TEST(Template, AutoEscaping) {
  Strip strip = STRIP_WHITESPACE;
  Template *tpl;
  string filename;
  string text;
  string user = "John<>Doe";
  string user_esc = "John&lt;&gt;Doe";

  // Positive test cases -- template initialization succeeds.
  // We also check that modifiers that were missing or given incorrect
  // have been updated as expected.
  // TODO(jad): Cut-down redundancy by merging with
  //            TestCorrectModifiersForAutoEscape.
  text = "{{%AUTOESCAPE context=\"HTML\"}}"         // HTML
      "{{USER:o}}<a href=\"{{URL}}\" class={{CLASS:h}}</a>";
  ASSERT(tpl = StringToTemplate(text, strip));
  string expected_mods = "USER:o:h\nURL:U=html\nCLASS:h:H=attribute\n";
  AssertCorrectModifiersInTemplate(tpl, text, expected_mods);

  text = "{{%AUTOESCAPE context=\"HTML\" state=\"IN_TAG\"}}"  // HTML in tag
      "href=\"{{URL}}\" class={{CLASS:h}} style=\"font:{{COLOR}}\"";
  ASSERT(tpl = StringToTemplate(text, strip));
  expected_mods = "URL:U=html\nCLASS:h:H=attribute\nCOLOR:c\n";
  AssertCorrectModifiersInTemplate(tpl, text, expected_mods);

  text = "{{%AUTOESCAPE context=\"HTML\" state=\"in_tag\"}}"  // lowercase ok
      "href=\"{{URL}}\" class={{CLASS:h}} style=\"font:{{COLOR}}\"";
  ASSERT(tpl = StringToTemplate(text, strip));
  expected_mods = "URL:U=html\nCLASS:h:H=attribute\nCOLOR:c\n";
  AssertCorrectModifiersInTemplate(tpl, text, expected_mods);

  // Repeat the test with trailing HTML that closes the tag. This is
  // undefined behavior. We test it to ensure the parser does not choke.
  text += ">Hello</a><span>Some text</span></body></html>";
  ASSERT(tpl = StringToTemplate(text, strip));
  expected_mods = "URL:U=html\nCLASS:h:H=attribute\nCOLOR:c\n";
  AssertCorrectModifiersInTemplate(tpl, text, expected_mods);

  text = "{{%AUTOESCAPE context=\"JAVASCRIPT\"}}"   // JAVASCRIPT
      "var a = {{A}}; var b = '{{B:h}}';";
  ASSERT(tpl = StringToTemplate(text, strip));
  expected_mods = "A:J=number\nB:h:j\n";
  AssertCorrectModifiersInTemplate(tpl, text, expected_mods);

  text = "{{%AUTOESCAPE context=\"CSS\"}}"          // CSS
      "body {color:\"{{COLOR}}\"; font-size:{{SIZE:j}}";
  ASSERT(tpl = StringToTemplate(text, strip));
  expected_mods = "COLOR:c\nSIZE:j:c\n";
  AssertCorrectModifiersInTemplate(tpl, text, expected_mods);

  text = "{{%AUTOESCAPE context=\"JSON\"}}"         // JSON
      "{ 'id': {{ID:j}}, 'value': {{VALUE:h}} }";
  ASSERT(tpl = StringToTemplate(text, strip));
  expected_mods = "ID:j\nVALUE:h:j\n";
  AssertCorrectModifiersInTemplate(tpl, text, expected_mods);

  text = "{{%AUTOESCAPE context=\"XML\"}}"          // XML
      "<PARAM name=\"{{VAL}}\">{{DATA:h}}";
  ASSERT(tpl = StringToTemplate(text, strip));
  expected_mods = "VAL:xml_escape\nDATA:h\n";
  AssertCorrectModifiersInTemplate(tpl, text, expected_mods);

  text = "{{%AUTOESCAPE context=\"xml\"}}"          // lower-case XML
      "<PARAM name=\"{{VAL}}\">{{DATA:h}}";
  ASSERT(tpl = StringToTemplate(text, strip));
  expected_mods = "VAL:xml_escape\nDATA:h\n";
  AssertCorrectModifiersInTemplate(tpl, text, expected_mods);

  text = "{{!bla}}{{%AUTOESCAPE context=\"HTML\"}}";  // after comment
  ASSERT(tpl = StringToTemplate(text, strip));
  text = "{{%AUTOESCAPE context=\"HTML\" state=\"default\"}}";
  ASSERT(tpl = StringToTemplate(text, strip));        // adding state

  // Negative test cases - template initialization fails due to errors
  // in the marker. Also checks that our parsing is defensive.
  text = "{{%AUTOESCAPE}}";                                 // missing context
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPER context=\"HTML\"}}";               // invalid id
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%}}";                                           // missing id
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{% }}";                                          // missing id
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{% =}}";                                         // missing id
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE =\"HTML\"}}";                       // missing name
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE foo=\"HTML\"}}";                    // bogus name
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE =}}";                               // lone '='
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE context=HTML}}";                    // val not quoted
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE context=\"HTML}}";                  // no end quotes
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE context=\"\\\"HTML\"}}";            // Unescape val
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE context=\"\\\"HT\\\"\\\"ML\\\"\"}}";   // more complex
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE context=\"\"HTML\"}}";              // Unescape val
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE context=\"JAVASCRIPT\" bla}}";      // extra attr
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE context=\"JAVASCRIPT\"bla}}";       // invalid value
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE context=\"JAVASCRIPT\" foo=bla}}";  // extra attr/val
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE  context=\"HTML\"}}";               // extra whitesp
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE context =\"HTML\"}}";               // extra whitesp
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE context= \"HTML\"}}";               // extra whitesp
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE context=\"HTML\" }}";               // extra whitesp
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE context=\"Xml\"}}";                 // mixed-case xml
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE context=\"HTML\" state=\"tag\"}}";  // bad state
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{%AUTOESCAPE context=\"CSS\" state=\"IN_TAG\"}}";  // invalid state
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "Hello{{%AUTOESCAPE context=\"HTML\"}}";           // after text
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{USER}}{{%AUTOESCAPE context=\"HTML\"}}";        // after variable
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);
  text = "{{#SEC}}{{%AUTOESCAPE context=\"HTML\"}}{{/SEC}}";  // not in MAIN
  ASSERT((tpl = StringToTemplate(text, strip)) == NULL);

  string kAutoescapeHtmlPragma = "{{%AUTOESCAPE context=\"HTML\"}}";

  // Check that Selective Auto-Escape does not auto-escape included templates
  // unless these are also marked for auto-escape. To attest that,
  // we check that when no escaping was given in the included template, none
  // will be applied to it. USER will not get html-escaped.
  text = kAutoescapeHtmlPragma + "{{>INC}}";
  tpl = StringToTemplate(text, strip);
  ASSERT(tpl);
  string inc_text = "{{USER}}";    // missing :h escaping.
  TemplateDictionary dict("dict");
  TemplateDictionary *inc_dict = dict.AddIncludeDictionary("INC");
  inc_dict->SetFilename(StringToTemplateFile(inc_text));
  inc_dict->SetValue("USER", user);
  AssertExpandIs(tpl, &dict, user, true);

  // Add AUTOESCAPE pragma to included template and check that it works.
  inc_text = kAutoescapeHtmlPragma + inc_text;
  filename = StringToTemplateFile(inc_text);
  inc_dict->SetFilename(filename);
  AssertExpandIs(tpl, &dict, user_esc, true);

  // Check that Selective Auto-Escape works with Template::StringToTemplate.
  tpl = Template::StringToTemplate(inc_text, strip);
  ASSERT(tpl);
  TemplateDictionary dict2("dict2");
  dict2.SetValue("USER", user);
  AssertExpandIs(tpl, &dict2, user_esc, true);
  delete tpl;

  // Test that Selective AutoEscape follows included templates: Included
  // templates 2 and 4 are registered for auto-escaping but not included
  // templates 1 and 3. Check that only templates 2 and 4 get escaped.
  text = "Parent: {{USER}}; {{>INCONE}}";
  string text_inc1 = "INC1: {{USER1}}; {{>INCTWO}}";
  string text_inc2 = kAutoescapeHtmlPragma + "INC2: {{USER2}}; {{>INCTHREE}}";
  string text_inc3 = "INC3: {{USER3}}; {{>INCFOUR}}";
  string text_inc4 = kAutoescapeHtmlPragma + "INC4: {{USER4}}";
  dict.SetValue("USER", user);

  TemplateDictionary *dict_inc1 = dict.AddIncludeDictionary("INCONE");
  dict_inc1->SetFilename(StringToTemplateFile(text_inc1));
  dict_inc1->SetValue("USER1", user);

  TemplateDictionary *dict_inc2 = dict_inc1->AddIncludeDictionary("INCTWO");
  filename = StringToTemplateFile(text_inc2);
  dict_inc2->SetFilename(filename);
  dict_inc2->SetValue("USER2", user);

  TemplateDictionary *dict_inc3 = dict_inc2->AddIncludeDictionary("INCTHREE");
  dict_inc3->SetFilename(StringToTemplateFile(text_inc3));
  dict_inc3->SetValue("USER3", user);

  TemplateDictionary *dict_inc4 = dict_inc3->AddIncludeDictionary("INCFOUR");
  filename = StringToTemplateFile(text_inc4);
  dict_inc4->SetFilename(filename);
  dict_inc4->SetValue("USER4", user);

  tpl = StringToTemplate(text, strip);
  string expected_out = "Parent: " + user + "; INC1: " + user +
      "; INC2: " + user_esc + "; INC3: " + user + "; INC4: " + user_esc;
  AssertExpandIs(tpl, &dict, expected_out, true);

  // Check that we do not modify template-includes.
  // Here, xml_escape would have been changed to :h:xml_escape
  // causing a double-escaping of the USER.
  text = kAutoescapeHtmlPragma + "{{>INC:xml_escape}}";
  inc_text = "{{USER}}";
  tpl = StringToTemplate(text, strip);
  ASSERT(tpl);
  TemplateDictionary dict3("dict");
  inc_dict = dict3.AddIncludeDictionary("INC");
  inc_dict->SetFilename(StringToTemplateFile(inc_text));
  inc_dict->SetValue("USER", user);
  AssertExpandIs(tpl, &dict3, user_esc, true);

  // Test that {{%...}} is a "removable" marker. A related test is
  // also added to TestStrip().
  tpl = StringToTemplate("{{%AUTOESCAPE context=\"HTML\"}}\nText\n Text",
                         STRIP_BLANK_LINES);
  AssertExpandIs(tpl, &dict, "Text\n Text", true);
}

TEST(Template, RegisterString) {
  ASSERT(Template::StringToTemplateCache("file1", "Some text"));
  Template* tpl = Template::GetTemplate("file1", STRIP_WHITESPACE);
  ASSERT(tpl);
  ASSERT(Template::GetTemplate("file1", STRIP_WHITESPACE) == tpl);

  ASSERT(Template::StringToTemplateCache("file2", "Top {{>INC}}"));

  TemplateDictionary dict("dict");
  string expected = "Some text";
  AssertExpandIs(tpl, &dict, expected, true);

  TemplateDictionary* sub_dict = dict.AddIncludeDictionary("INC");
  sub_dict->SetFilename("file1");
  tpl = Template::GetTemplate("file2", STRIP_WHITESPACE);
  expected = "Top Some text";
  AssertExpandIs(tpl, &dict, expected, true);
}

// This tests that StaticTemplateString is sufficiently initialized at
// static-initialization time (as opposed to dynamic-initialization
// time, which comes later), that we can safely expand templates
// during dynamic initialization.  This is worth testing, because some
// parts of a StaticTemplateString -- especially the hash value, *do*
// get computed later at dynamic-initialization time, and we want to
// make sure that things still work properly even if we access the
// StaticTemplateString before that happens.
extern const StaticTemplateString kLateDefine;
class DynamicInitializationTemplateExpander {
 public:
  DynamicInitializationTemplateExpander() {
    Template* tpl = Template::StringToTemplate("hi {{VAR}} lo",
                                               STRIP_WHITESPACE);
    TemplateDictionary dict("dict");
    dict.SetValue("VAR", TemplateString("short-lived", strlen("short")));
    AssertExpandIs(tpl, &dict, "hi short lo", true);
    dict.SetValue("VAR", kHello);
    AssertExpandIs(tpl, &dict, "hi Hello lo", true);
    dict.SetValue("VAR", kLateDefine);
    AssertExpandIs(tpl, &dict, "hi laterz lo", true);
    delete tpl;
  }
};
DynamicInitializationTemplateExpander sts_tester;  // this runs before main()
const StaticTemplateString kLateDefine = STS_INIT(kLateDefine, "laterz");

int main(int argc, char** argv) {

  CreateOrCleanTestDirAndSetAsTmpdir(FLAGS_test_tmpdir);

  // This goes first so that future tests don't mess up the filenames.
  // So we make it a normal function rather than using TEST() on it.
  TestAnnotation();
  return RUN_ALL_TESTS();
}
