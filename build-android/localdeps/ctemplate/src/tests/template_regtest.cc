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
// This test consists of creating a pretty complicated
// dictionary, and then applying it to a bunch of templates
// (specified in the testdata dir) and making sure the output
// is as expected.  We actually support testing with multiple
// dictionaries.  We glob the testdat dir, so it's possible to
// add a new test just by creating a template and expected-output
// file in the testdata directory.  Files are named
//    template_unittest_testXX.in
//    template_unittest_testXX_dictYY.out
// YY should start with 01 (not 00).  XX can be an arbitrary string.

#include "config_for_unittests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
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
#endif       // for opendir() etc
#include <algorithm>      // for sort() and stable_partition
#include <string>
#include <vector>
#include <ctemplate/template.h>
#include <ctemplate/template_dictionary.h>
#include <ctemplate/template_modifiers.h>
#include <ctemplate/template_pathops.h>
#include "base/util.h"

using std::vector;
using std::string;
using std::sort;

using ctemplate::DO_NOT_STRIP;
using ctemplate::PerExpandData;
using ctemplate::STRIP_BLANK_LINES;
using ctemplate::STRIP_WHITESPACE;
using ctemplate::TC_HTML;
using ctemplate::TC_MANUAL;
using ctemplate::Template;
using ctemplate::TemplateDictionary;

#define ASSERT(cond)  do {                                      \
  if (!(cond)) {                                                \
    printf("%s: %d: ASSERT FAILED: %s\n", __FILE__, __LINE__,   \
           #cond);                                              \
    assert(cond);                                               \
    exit(1);                                                    \
  }                                                             \
} while (0)

#define ASSERT_STRING_EQ(a, b)   do {                   \
    assert(StringEq(a, b, __FILE__, __LINE__, #a, #b)); \
} while (0)

bool StringEq(const string& a, const string& b,
                    const char* filename, int lineno,
                    const char* namea, const char* nameb) {
  if (a != b) {
    printf("%s: %d: ASSERT FAILED: %s == %s:\n", filename, lineno,
           namea, nameb);
    printf("EXPECTED:\n%s\n", a.c_str());
    printf("ACTUAL:\n%s\n", b.c_str());
    return false;
  }
  return true;
}

#define ASSERT_STREQ_EXCEPT(a, b, except)  ASSERT(StreqExcept(a, b, except))
#define ASSERT_STREQ(a, b)                 ASSERT(strcmp(a, b) == 0)
#define ASSERT_NOT_STREQ(a, b)             ASSERT(strcmp(a, b) != 0)

// First, (conceptually) remove all chars in "except" from both a and b.
// Then return true iff munged_a == munged_b.
bool StreqExcept(const char* a, const char* b, const char* except) {
  const char* pa = a, *pb = b;
  const size_t exceptlen = strlen(except);
  while (1) {
    // Use memchr instead of strchr because strchr(foo, '\0') always fails
    while (memchr(except, *pa, exceptlen))  pa++;   // ignore "except" chars in a
    while (memchr(except, *pb, exceptlen))  pb++;   // ignore "except" chars in b
    if ((*pa == '\0') && (*pb == '\0'))
      return true;
    if (*pa++ != *pb++)                  // includes case where one is at \0
      return false;
  }
}


RegisterTemplateFilename(VALID1_FN, "template_unittest_test_valid1.in");
RegisterTemplateFilename(INVALID1_FN, "template_unittest_test_invalid1.in");
RegisterTemplateFilename(INVALID2_FN, "template_unittest_test_invalid2.in");
RegisterTemplateFilename(NONEXISTENT_FN, "nonexistent__file.tpl");

struct Testdata {
  string input_template_name;   // the filename of the input template
  string input_template;        // the contents of the input template
  vector<string> output;        // entry i is the output of using dict i.
  vector<string> annotated_output;  // used to test annotations
};

static void ReadToString(const string& filename, string* s) {
  const int bufsize = 8092;
  char buffer[bufsize];
  size_t n;
  FILE* fp = fopen(filename.c_str(), "rb");
  if (!fp)  PFATAL(filename.c_str());
  while ((n=fread(buffer, 1, bufsize, fp)) > 0) {
    if (ferror(fp))  PFATAL(filename.c_str());
    s->append(string(buffer, n));
  }
  fclose(fp);
}

static bool EndsWith(const string& s, const string& suffix) {
  return (s.length() >= suffix.length() &&
          s.substr(s.length() - suffix.length()) == suffix);
}

#ifndef USING_PORT_CC  /* windows defines its own version in windows/port.cc */
static void GetNamelist(const char* testdata_dir, vector<string>* namelist) {
  DIR* dir = opendir(testdata_dir);
  struct dirent* dir_entry;
  if (dir == NULL) PFATAL("opendir");
  while ( (dir_entry=readdir(dir)) != NULL ) {
    if (!strncmp(dir_entry->d_name, "template_unittest_test",
                 sizeof("template_unittest_test")-1)) {
      namelist->push_back(dir_entry->d_name);    // collect test files
    }
  }
  if (closedir(dir) != 0) PFATAL("closedir");
}
#endif

// expensive to resize this vector and copy it and all, but that's ok
static vector<Testdata> ReadDataFiles(const char* testdata_dir) {
  vector<Testdata> retval;
  vector<string> namelist;

  GetNamelist(testdata_dir, &namelist);
  sort(namelist.begin(), namelist.end());

  for (vector<string>::const_iterator it = namelist.begin();
       it != namelist.end(); ++it) {
    vector<string>* new_output = NULL;
    const string fname = string(testdata_dir) + "/" + it->c_str();
    if (EndsWith(fname, ".in")) {
      retval.push_back(Testdata());
      retval.back().input_template_name = *it;
      ReadToString(fname, &retval.back().input_template);
    } else if (EndsWith(fname, ".out")) {
      new_output = &retval.back().output;
    } else if (EndsWith(fname, ".anno_out")) {
      new_output = &retval.back().annotated_output;
    } else {
      ASSERT(false);  // Filename must end in either .in, .out, or .anno_out.
    }
    if (new_output) {            // the .out and .anno_out cases
      ASSERT(!retval.empty());   // an .out without any corresponding .in?
      ASSERT(it->length() > retval.back().input_template_name.length() + 4);
      // input file is foo.in, and output is foo_dictYY.out.  This gets to YY.
      const char* dictnum_pos = (it->c_str() +
                                 retval.back().input_template_name.length() + 2);
      int dictnum = atoi32(dictnum_pos);   // just ignore chars after the YY
      ASSERT(dictnum);                   // dictnums should start with 01
      while (new_output->size() <
             static_cast<vector<string>::size_type>(dictnum))
        new_output->push_back(string());
      ReadToString(fname, &((*new_output)[dictnum-1]));
    }
  }
  return retval;
}


// Creates a complicated dictionary, using every TemplateDictionary
// command under the sun.  Returns a pointer to the new dictionary-root.
// Should be freed by the caller.
static TemplateDictionary* MakeDict1() {
  TemplateDictionary* dict = new TemplateDictionary("dict1", NULL);
  dict->SetFilename("just used for debugging, so doesn't matter.txt");

  // --- These are used by template_unittest_test_simple.in
  dict->SetValue("HEAD", "   This is the head   ");
  // We leave BODY undefined, to make sure that expansion works properly.

  // --- These are used by template_unittest_test_footer.in
  TemplateDictionary* fbt = dict->AddSectionDictionary("FOOTER_BAR_TEXT");
  fbt->SetValue("BODY", "Should never be shown");  // this is part of simple
  fbt->SetEscapedValue("HOME_LINK", "<b>Time to go home!</b>",
                       ctemplate::html_escape);
  // Note: you should never have code like this in real life!  The <b>
  // and </b> should be part of the template proper.
  fbt->SetFormattedValue("ADVERTISE_LINK", "<b>Be advertiser #%d</b>", 2);
  fbt->SetValue("ABOUT_GOOGLE_LINK", "<A HREF=/>About Google!</A>");

  // We show PROMO_LICENSING_SECTION in the main dict, even though
  // it's defined in the fbt subsection.  This will still work: section
  // showing goes to the parent dict if not found in the current dict.
  dict->ShowSection("PROMO_LICENSING_SECTION");
  dict->SetValue("PROMO_LICENSING_LINK", "<A HREF='foo'>");

  // We don't show the TRIM_LINE section, so these vars shouldn't be seen
  dict->SetValue("TRIM_LINE_COLOR", "Who cares?");
  dict->SetIntValue("TRIM_LINE_HEIGHT", 10);

  dict->SetIntValue("MODIFIED_BY_GOOGLE", 2005);
  dict->SetValue("MSG_copyright", "&copy; Google Inc. (all rights reserved)");
  // We don't set ODP_ATTRIBUTION, so this include is ignored.

  dict->ShowSection("CLOSING_DIV_SECTION");

  // We won't set any of the includes that follow, just to keep things simple

  // First, call SetValueAndShowSection on a non-existence section, should noop
  dict->SetValueAndShowSection("LATENCY_PREFETCH_URL", "/huh?",
                               "UNUSED_SECTION_NAME");
  // Now try the real URL
  dict->SetValueAndShowSection("LATENCY_PREFETCH_URL", string("/latency"),
                               "LATENCY_PREFETCH");

  // JAVASCRIPT_FOOTER_SECTION was meant to be either shown or hidden, but
  // hey, let's try showing it several times, each with a different include.
  // And let's include each one several times.
  TemplateDictionary* jfs1 = dict->AddSectionDictionary(
      "JAVASCRIPT_FOOTER_SECTION");
  // This first dictionary should have an empty HEAD and BODY
  TemplateDictionary* inc1a = jfs1->AddIncludeDictionary("FAST_NEXT_JAVASCRIPT");
  inc1a->SetFilename("template_unittest_test_simple.in");
  // For the second dict, let's set an illegal filename: should be ignored
  TemplateDictionary* inc1b = jfs1->AddIncludeDictionary("FAST_NEXT_JAVASCRIPT");
  inc1b->SetFilename(INVALID1_FN);
  // For the third dict, let's do the same as the first, but with a HEAD
  TemplateDictionary* inc1c = jfs1->AddIncludeDictionary("FAST_NEXT_JAVASCRIPT");
  inc1c->SetFilename("template_unittest_test_simple.in");
  inc1c->SetValue("HEAD", "head");

  // Let's expand the section again with two different includes, and again a
  // third template not meant to be expanded (in this case, don't set filename)
  TemplateDictionary* jfs2 = dict->AddSectionDictionary(
      "JAVASCRIPT_FOOTER_SECTION");
  TemplateDictionary* inc2a = jfs2->AddIncludeDictionary("FAST_NEXT_JAVASCRIPT");
  inc2a->SetFilename("template_unittest_test_simple.in");
  inc2a->SetValue("HEAD", "include-head");
  inc2a->SetEscapedFormattedValue("BODY", ctemplate::html_escape,
                                  "<b>%s</b>: %.4f", "<A HREF=/>", 1.0/3);
  inc2a->SetValue("BI_NEWLINE", "");   // override the global default
  TemplateDictionary* inc2b = jfs2->AddIncludeDictionary("FAST_NEXT_JAVASCRIPT");
  inc2b->SetFilename("template_unittest_test_html.in");
  inc2b->SetValue("HEAD", "should be ignored");
  jfs2->AddIncludeDictionary("FAST_NEXT_JAVASCRIPT");   // ignored: no filename

  // --- These are used by template_unittest_test_html.in

  // This should returns in NO_MOUSEOVER_FUNCTIONS remaining hidden
  dict->SetValueAndShowSection("DUMMY", "", "NO_MOUSEOVER_FUNCTIONS");

  dict->ShowSection("MOUSEOVER_FUNCTIONS");
  TemplateDictionary* foo = dict->AddIncludeDictionary("MOUSEOVER_JAVASCRIPT");
  foo->SetFilename(string("not_a_template"));
  foo->SetValue("BI_NEWLINE", "not gonna matter");

  dict->SetEscapedValue("GOTO_MESSAGE", "print \"Go home\"",
                        ctemplate::javascript_escape);

  dict->SetEscapedValue("UPDATE", "monday & tuesday",
                                      ctemplate::html_escape);
  dict->ShowSection("UPDATE_SECTION");

  dict->SetValue("ALIGNMENT", "\"right\"");   // all results sections see this
  for (int i = 0; i < 3; ++i) {   // we'll do three results
    TemplateDictionary* result = dict->AddSectionDictionary("RESULTS");
    if (i % 2 == 0)
      result->ShowSection("WHITE_BG");  // gives us striped results!
    const char* res = "<&>\"result\" #%d'&'";
    result->SetFormattedValue("RESULT", res, i);
    result->SetEscapedFormattedValue("XML_RESULT",
                                     ctemplate::xml_escape,
                                     res, i);
    result->SetIntValue("GOODNESS", i + 5);
  }

  // For testing auto-escape.
  dict->SetValue("AE_TITLE_GOOD", "Hello World!");
  dict->SetValue("AE_TITLE_BAD", "Hello <script>alert(1)</script> World!");
  dict->SetValue("AE_URL_GOOD", "http://www.google.com/");
  dict->SetValue("AE_URL_BAD", "javascript:alert(1)");
  dict->SetValue("AE_BG_COLOR_GOOD", "red");
  dict->SetValue("AE_BG_COLOR_BAD", "evil! &");
  dict->SetValue("AE_JS_GOOD", "your text here");
  dict->SetValue("AE_JS_BAD", "your text'is clever'thanks");
  dict->SetValue("AE_USERNAME_GOOD", "Mr. Nice");
  dict->SetValue("AE_USERNAME_BAD", "Doctor<script>alert(2)</script>Evil");
  dict->SetValue("AE_START_EDGE", "left");
  dict->SetValue("AE_END_EDGE", ";:center()$$");  // Some invalid chars.
  dict->SetValue("AE_FONT_SIZE_PC", "120%");
  dict->SetValue("AE_FONT_SIZE_PT", "12pt");
  dict->SetValue("AE_MAUVE_RGB", "#FF7BD5");
  dict->SetValue("AE_ITALIC", "italic");

  // This won't see any of the vars *we* set
  TemplateDictionary* footer_dict = dict->AddIncludeDictionary("FOOTER");
  footer_dict->SetFilename("template_unittest_test_footer.in");

  // --- These are used by template_unittest_test_modifiers.in

  // UPDATE and UPDATE_SECTION we inherit from test_html.in
  TemplateDictionary* inc_simple = dict->AddIncludeDictionary("SIMPLE");
  inc_simple->SetFilename("template_unittest_test_simple.in");

  return dict;
}


// Quite opposite of dict1, dict2 is a dictionary that has nothing in it
static TemplateDictionary* MakeDict2() {
  return new TemplateDictionary("dict2");
}


// dict3 tests just the handling of whitespace
static TemplateDictionary* MakeDict3() {
  TemplateDictionary* dict = new TemplateDictionary("dict3");

  dict->SetValue("HEAD", "   ");
  dict->SetValue("BODY", "\r\n");
  return dict;
}

static TemplateDictionary* MakeDictionary(int i) {
  switch (i) {
    case 1: return MakeDict1();
    case 2: return MakeDict2();
    case 3: return MakeDict3();
    default: ASSERT(false);  // No dictionary with this number yet.
  }
  return NULL;
}


static void TestExpand(const vector<Testdata>::const_iterator& begin,
                       const vector<Testdata>::const_iterator& end) {
  for (vector<Testdata>::const_iterator one_test = begin;
       one_test != end; ++one_test) {
    Template* tpl_none = Template::GetTemplate(one_test->input_template_name,
                                               DO_NOT_STRIP);
    Template* tpl_lines = Template::GetTemplate(one_test->input_template_name,
                                                STRIP_BLANK_LINES);
    Template* tpl_ws = Template::GetTemplate(one_test->input_template_name,
                                             STRIP_WHITESPACE);

    // Test TemplateToString while we're at it.
    Template* tplstr_none = Template::StringToTemplate(
        one_test->input_template, DO_NOT_STRIP);
    Template* tplstr_lines = Template::StringToTemplate(
        one_test->input_template, STRIP_BLANK_LINES);
    Template* tplstr_ws = Template::StringToTemplate(
        one_test->input_template, STRIP_WHITESPACE);

    for (vector<string>::const_iterator out = one_test->output.begin();
         out != one_test->output.end(); ++out) {
      int dictnum = out - one_test->output.begin() + 1;  // first dict is 01
      // If output is the empty string, we assume the file does not exist
      if (out->empty())
        continue;

      printf("Testing template %s on dict #%d\n",
             one_test->input_template_name.c_str(), dictnum);
      // If we're expecting output, the template better not have had an error
      ASSERT(tpl_none && tpl_lines && tpl_ws);
      ASSERT(tplstr_none && tplstr_lines && tplstr_ws);

      TemplateDictionary* dict = MakeDictionary(dictnum);

      string stroutput_none, stroutput_lines, stroutput_ws;
      string stroutput_strnone, stroutput_strlines, stroutput_strws;

      tpl_none->Expand(&stroutput_none, dict);
      tpl_lines->Expand(&stroutput_lines, dict);
      tpl_ws->Expand(&stroutput_ws, dict);
      tplstr_none->Expand(&stroutput_strnone, dict);
      tplstr_lines->Expand(&stroutput_strlines, dict);
      tplstr_ws->Expand(&stroutput_strws, dict);

      // "out" is the output for STRIP_WHITESPACE mode.
      ASSERT_STRING_EQ(*out, stroutput_ws);

      // Now compare the variants against each other.
      // NONE and STRIP_LINES may actually be the same on simple inputs
      //ASSERT(output_none != output_lines);
      ASSERT(stroutput_none != stroutput_ws);
      ASSERT(stroutput_lines != stroutput_ws);
      ASSERT_STREQ_EXCEPT(stroutput_none.c_str(), stroutput_lines.c_str(),
                          " \t\v\f\r\n");
      ASSERT_STREQ_EXCEPT(stroutput_none.c_str(), stroutput_ws.c_str(),
                          " \t\v\f\r\n");

      // It shouldn't matter if we read stuff from a file or a string.
      ASSERT(stroutput_none == stroutput_strnone);
      ASSERT(stroutput_lines == stroutput_strlines);
      ASSERT(stroutput_ws == stroutput_strws);


      delete dict;          // it's our responsibility
    }

    // The annotation test is a bit simpler; we only strip one way
    for (vector<string>::const_iterator out = one_test->annotated_output.begin();
         out != one_test->annotated_output.end(); ++out) {
      int dictnum = out - one_test->annotated_output.begin() + 1;
      // If output is the empty string, we assume the file does not exist
      if (out->empty())
        continue;

      printf("Testing template %s on dict #%d (annotated)\n",
             one_test->input_template_name.c_str(), dictnum);

      TemplateDictionary* dict = MakeDictionary(dictnum);
      PerExpandData per_expand_data;
      per_expand_data.SetAnnotateOutput("template_unittest_test");
      string output;
      tpl_lines->ExpandWithData(&output, dict, &per_expand_data);
      ASSERT_STREQ_EXCEPT(out->c_str(), output.c_str(), "\r\n");
      delete dict;   // it's our responsibility
    }
    delete tplstr_none;   // these are our responsibility too
    delete tplstr_lines;
    delete tplstr_ws;
  }
}


int main(int argc, char** argv) {
  // If TEMPLATE_ROOTDIR is set in the environment, it overrides the
  // default of ".".  We use an env-var rather than argv because
  // that's what automake supports most easily.
  const char* template_rootdir = getenv("TEMPLATE_ROOTDIR");
  if (template_rootdir == NULL)
    template_rootdir = DEFAULT_TEMPLATE_ROOTDIR;   // probably "."
  string rootdir = ctemplate::PathJoin(template_rootdir, "src");
  rootdir = ctemplate::PathJoin(rootdir, "tests");
  Template::SetTemplateRootDirectory(rootdir);

  vector<Testdata> testdata = ReadDataFiles(
      Template::template_root_directory().c_str());
  if (testdata.empty()) {
    printf("FATAL ERROR: No test files found for template_regtest\n");
    return 1;
  }

  TestExpand(testdata.begin(), testdata.end());

  printf("DONE\n");
  return 0;
}
