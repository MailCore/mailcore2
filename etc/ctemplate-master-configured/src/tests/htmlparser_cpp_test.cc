// Copyright (c) 2007, Google Inc.
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
// Author: falmeida@google.com (Filipe Almeida)
//
// Verify at different points during HTML processing that the parser is in the
// correct state.
//
// The annotated file consists of regular html blocks and html processing
// instructions with a target name of "state" and a list of comma separated key
// value pairs describing the expected state or invoking a parser method.
// Example:
//
// <html><body><?state state=text, tag=body ?>
//
// For a more detailed explanation of the acceptable values please consult
// htmlparser_cpp.h. Following is a list of the possible keys:
//
// state: Current parser state as returned by HtmlParser::state().
//        Possible values: text, tag, attr, value, comment or error.
// tag: Current tag name as returned by HtmlParser::tag()
// attr: Current attribute name as returned by HtmlParser::attr()
// attr_type: Current attribute type as returned by HtmlParser::attr_type()
//            Possible values: none, regular, uri, js or style.
// attr_quoted: True if the attribute is quoted, false if it's not.
// in_js: True if currently processing javascript (either an attribute value
//        that expects javascript, a script block or the parser being in
//        MODE_JS)
// js_quoted: True if inside a javascript string literal.
// js_state: Current javascript state as returned by
//           HtmlParser::javascript_state().
//           Possible values: text, q, dq, regexp or comment.
// in_css: True if currently inside a CSS section or attribute.
// line_number: Integer value containing the current line count.
// column_number: Integer value containing the current column count.
// value_index: Integer value containing the current character index in the
//              current value starting from 0.
// is_url_start: True if if this is the first character of a url attribute.
// reset: If true, resets the parser state to it's initial values.
// reset_mode: Similar to reset but receives an argument that changes the
//             parser mode into either mode html or mode js.
// insert_text: Executes HtmlParser::InsertText() if the argument is true.

#include "config_for_unittests.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include "htmlparser/htmlparser_cpp.h"
#include "ctemplate/template_pathops.h"
#include "base/util.h"

#define FAIL()  EXPECT_TRUE(false)
TEST_INIT  // Among other things, defines RUN_ALL_TESTS

using std::map;
using std::pair;
using std::string;
using std::vector;
using ctemplate::PathJoin;

namespace ctemplate_htmlparser {

// Maximum file size limit.
static const int kMaxFileSize = 1000000;

static void ReadToString(const char* filename, string* s) {
  const int bufsize = 8092;
  char buffer[bufsize];
  size_t n;
  FILE* fp = fopen(filename, "rb");
  if (!fp)  PFATAL(filename);
  while ((n=fread(buffer, 1, bufsize, fp)) > 0) {
    if (ferror(fp))  PFATAL(filename);
    s->append(string(buffer, n));
  }
  fclose(fp);
}

class HtmlparserCppTest : public testing::Test {
 protected:

  typedef map<string, HtmlParser *> ContextMap;

  // Structure that stores the mapping between an id and a name.
  struct IdNameMap {
    int id;
    const char *name;
  };

  // Mapping between the enum and the string representation of the state.
  static const struct IdNameMap kStateMap[];

  // Mapping between the enum and the string representation of the javascript
  // state.
  static const struct IdNameMap kJavascriptStateMap[];

  // Mapping between the enum and the string representation of the attribute
  // type.
  static const struct IdNameMap kAttributeTypeMap[];

  // Mapping between the enum and the string representation of the reset mode.
  static const struct IdNameMap kResetModeMap[];

  // String that marks the start of an annotation.
  static const char kDirectiveBegin[];

  // String that marks the end of an annotation.
  static const char kDirectiveEnd[];

  // Count the number of lines in a string.
  static int UpdateLines(const string &str, int line);

  // Count the number of columns in a string.
  static int UpdateColumns(const string &str, int column);

  // Converts a string to a boolean.
  static bool StringToBool(const string &value);

  // Returns the name of the corresponding enum_id by consulting an array of
  // type IdNameMap.
  const char *IdToName(const struct IdNameMap *list, int enum_id);

  // Returns the enum_id of the correspondent name by consulting an array of
  // type IdNameMap.
  int NameToId(const struct IdNameMap *list, const string &name);

  // Reads the filename of an annotated html file and validates the
  // annotations against the html parser state.
  void ValidateFile(string filename);

  // Validate an annotation string against the current parser state.
  void ProcessAnnotation(const string &dir);

  // Validate the parser state against the provided state.
  void ValidateState(const string &tag);

  // Validate the parser tag name against the provided tag name.
  void ValidateTag(const string &tag);

  // Validate the parser attribute name against the provided attribute name.
  void ValidateAttribute(const string &attr);

  // Validate the parser attribute value contents against the provided string.
  void ValidateValue(const string &contents);

  // Validate the parser attribute type against the provided attribute type.
  void ValidateAttributeType(const string &attr);

  // Validate the parser attribute quoted state against the provided
  // boolean.
  void ValidateAttributeQuoted(const string &quoted);

  // Validates the parser in javascript state against the provided boolean.
  void ValidateInJavascript(const string &quoted);

  // Validate the current parser javascript quoted state against the provided
  // boolean.
  void ValidateJavascriptQuoted(const string &quoted);

  // Validate the javascript parser state against the provided state.
  void ValidateJavascriptState(const string &expected_state);

  // Validates the parser css state against the provided boolean.
  void ValidateInCss(const string &quoted);

  // Validate the line count against the expected count.
  void ValidateLine(const string &expected_line);

  // Validate the line count against the expected count.
  void ValidateColumn(const string &expected_column);

  // Validate the current parser value index against the provided index.
  void ValidateValueIndex(const string &value_index);

  // Validate the parser is_url_start value against the provided one.
  void ValidateIsUrlStart(const string &expected_is_url_start);

  void SetUp() {
    parser_.Reset();
  }

  void TearDown() {
    // Delete all parser instances from the context map
    for (ContextMap::iterator iter = contextMap.begin();
        iter != contextMap.end(); ++iter) {
      delete iter->second;
    }
    contextMap.clear();
  }

  // Map containing the registers where the parser context is saved.
  ContextMap contextMap;

  // Parser instance
  HtmlParser parser_;

  friend class Test_HtmlparserTest_TestFiles;
};

const char HtmlparserCppTest::kDirectiveBegin[] = "<?state";
const char HtmlparserCppTest::kDirectiveEnd[] = "?>";

const struct HtmlparserCppTest::IdNameMap
             HtmlparserCppTest::kStateMap[] = {
  { HtmlParser::STATE_TEXT,     "text" },
  { HtmlParser::STATE_TAG,      "tag" },
  { HtmlParser::STATE_ATTR,     "attr" },
  { HtmlParser::STATE_VALUE,    "value" },
  { HtmlParser::STATE_COMMENT,  "comment" },
  { HtmlParser::STATE_JS_FILE,  "js_file" },
  { HtmlParser::STATE_CSS_FILE, "css_file" },
  { HtmlParser::STATE_ERROR,    "error" },
  { 0, NULL }
};

const struct HtmlparserCppTest::IdNameMap
             HtmlparserCppTest::kAttributeTypeMap[] = {
  { HtmlParser::ATTR_NONE,    "none" },
  { HtmlParser::ATTR_REGULAR, "regular" },
  { HtmlParser::ATTR_URI,     "uri" },
  { HtmlParser::ATTR_JS,      "js" },
  { HtmlParser::ATTR_STYLE,   "style" },
  { 0, NULL }
};

const struct HtmlparserCppTest::IdNameMap
             HtmlparserCppTest::kJavascriptStateMap[] = {
  { JavascriptParser::STATE_TEXT,    "text" },
  { JavascriptParser::STATE_Q,       "q" },
  { JavascriptParser::STATE_DQ,      "dq" },
  { JavascriptParser::STATE_REGEXP,  "regexp" },
  { JavascriptParser::STATE_COMMENT, "comment" },
  { 0, NULL }
};

const struct HtmlparserCppTest::IdNameMap
             HtmlparserCppTest::kResetModeMap[] = {
  { HtmlParser::MODE_HTML,                "html" },
  { HtmlParser::MODE_JS,                  "js" },
  { HtmlParser::MODE_CSS,                 "css" },
  { HtmlParser::MODE_HTML_IN_TAG,         "html_in_tag" },
  { 0, NULL }
};


// Count the number of lines in a string.
int HtmlparserCppTest::UpdateLines(const string &str, int line) {
  int linecount = line;
  for (string::size_type i = 0; i < str.length(); ++i) {
    if (str[i] == '\n')
      ++linecount;
  }
  return linecount;
}

// Count the number of columns in a string.
int HtmlparserCppTest::UpdateColumns(const string &str, int column) {
  // Number of bytes since the last newline.
  size_t last_newline = str.rfind('\n');

  // If no newline was found, we just sum up all the characters in the
  // annotation.
  if (last_newline == string::npos) {
    return static_cast<int>(column + str.size() +
                            strlen(kDirectiveBegin) + strlen(kDirectiveEnd));
  // If a newline was found, the new column count becomes the number of
  // characters after the last newline.
  } else {
    return static_cast<int>(str.size() + strlen(kDirectiveEnd) - last_newline);
  }
}


// Converts a string to a boolean.
bool HtmlparserCppTest::StringToBool(const string &value) {
  if (strcasecmp(value.c_str(), "true") == 0) {
    return true;
  } else if (strcasecmp(value.c_str(), "false") == 0) {
    return false;
  } else {
    LOG(FATAL) << "Unknown boolean value";
  }
}

// Returns the name of the corresponding enum_id by consulting an array of
// type IdNameMap.
const char *HtmlparserCppTest::IdToName(const struct IdNameMap *list,
                                        int enum_id) {
  CHECK(list != NULL);
  while (list->name) {
    if (enum_id == list->id) {
      return list->name;
    }
    list++;
  }
  LOG(FATAL) << "Unknown id";
}

// Returns the enum_id of the correspondent name by consulting an array of
// type IdNameMap.
int HtmlparserCppTest::NameToId(const struct IdNameMap *list,
                                const string &name) {
  CHECK(list != NULL);
  while (list->name) {
    if (name.compare(list->name) == 0) {
      return list->id;
    }
    list++;
  }
  LOG(FATAL) << "Unknown name";
}

// Validate the parser state against the provided state.
void HtmlparserCppTest::ValidateState(const string &expected_state) {
  const char* parsed_state = IdToName(kStateMap, parser_.state());
  EXPECT_TRUE(parsed_state != NULL);
  EXPECT_TRUE(!expected_state.empty());
  EXPECT_EQ(expected_state, string(parsed_state))
      << "Unexpected state at line " << parser_.line_number();
}

// Validate the parser tag name against the provided tag name.
void HtmlparserCppTest::ValidateTag(const string &expected_tag) {
  EXPECT_TRUE(parser_.tag() != NULL);
  EXPECT_TRUE(expected_tag == parser_.tag())
      << "Unexpected attr tag name at line " << parser_.line_number();
}

// Validate the parser attribute name against the provided attribute name.
void HtmlparserCppTest::ValidateAttribute(const string &expected_attr) {
  EXPECT_TRUE(parser_.attribute() != NULL);
  EXPECT_EQ(expected_attr, parser_.attribute())
      << "Unexpected attr name value at line " << parser_.line_number();
}

// Validate the parser attribute value contents against the provided string.
void HtmlparserCppTest::ValidateValue(const string &expected_value) {
  EXPECT_TRUE(parser_.value() != NULL);
  const string parsed_state(parser_.value());
  EXPECT_EQ(expected_value, parsed_state)
      << "Unexpected value at line " << parser_.line_number();
}

// Validate the parser attribute type against the provided attribute type.
void HtmlparserCppTest::ValidateAttributeType(
    const string &expected_attr_type) {
  const char *parsed_attr_type = IdToName(kAttributeTypeMap,
                                          parser_.AttributeType());
  EXPECT_TRUE(parsed_attr_type != NULL);
  EXPECT_TRUE(!expected_attr_type.empty());
  EXPECT_EQ(expected_attr_type, string(parsed_attr_type))
      << "Unexpected attr_type value at line " << parser_.line_number();
}

// Validate the parser attribute quoted state against the provided
// boolean.
void HtmlparserCppTest::ValidateAttributeQuoted(
    const string &expected_attr_quoted) {
  bool attr_quoted_bool = StringToBool(expected_attr_quoted);
  EXPECT_EQ(attr_quoted_bool, parser_.IsAttributeQuoted())
      << "Unexpected attr_quoted value at line " << parser_.line_number();
}

// Validates the parser in javascript state against the provided boolean.
void HtmlparserCppTest::ValidateInJavascript(const string &expected_in_js) {
  bool in_js_bool = StringToBool(expected_in_js);
  EXPECT_EQ(in_js_bool, parser_.InJavascript())
      << "Unexpected in_js value at line " << parser_.line_number();
}

// Validate the current parser javascript quoted state against the provided
// boolean.
void HtmlparserCppTest::ValidateJavascriptQuoted(
    const string &expected_js_quoted) {
  bool js_quoted_bool = StringToBool(expected_js_quoted);
  EXPECT_EQ(js_quoted_bool, parser_.IsJavascriptQuoted())
      << "Unexpected js_quoted value at line " << parser_.line_number();
}

// Validate the javascript parser state against the provided state.
void HtmlparserCppTest::ValidateJavascriptState(const string &expected_state) {
  const char* parsed_state = IdToName(kJavascriptStateMap,
                                      parser_.javascript_state());
  EXPECT_TRUE(parsed_state != NULL);
  EXPECT_TRUE(!expected_state.empty());
  EXPECT_EQ(expected_state, string(parsed_state))
      << "Unexpected javascript state at line " << parser_.line_number();
}

// Validates the parser css state against the provided boolean.
void HtmlparserCppTest::ValidateInCss(const string &expected_in_css) {
  bool in_css_bool = StringToBool(expected_in_css);
  EXPECT_EQ(in_css_bool, parser_.InCss())
      << "Unexpected in_css value at line " << parser_.line_number();
}

// Validate the line count against the expected count.
void HtmlparserCppTest::ValidateLine(const string &expected_line) {
  int line;
  CHECK(safe_strto32(expected_line, &line));
  EXPECT_EQ(line, parser_.line_number())
      << "Unexpected line count at line " << parser_.line_number();
}

// Validate the line count against the expected count.
void HtmlparserCppTest::ValidateColumn(const string &expected_column) {
  int column;
  CHECK(safe_strto32(expected_column, &column));
  EXPECT_EQ(column, parser_.column_number())
      << "Unexpected column count at line " << parser_.line_number();
}

// Validate the current parser value index against the provided index.
void HtmlparserCppTest::ValidateValueIndex(const string &expected_value_index) {
  int index;
  CHECK(safe_strto32(expected_value_index, &index));
  EXPECT_EQ(index, parser_.ValueIndex())
      << "Unexpected value_index value at line " << parser_.line_number();
}

// Validate the parser is_url_start value against the provided one.
void HtmlparserCppTest::ValidateIsUrlStart(
    const string &expected_is_url_start) {
  bool is_url_start_bool = StringToBool(expected_is_url_start);
  EXPECT_EQ(is_url_start_bool, parser_.IsUrlStart())
      << "Unexpected is_url_start value at line " << parser_.line_number();
}

// Validate an annotation string against the current parser state.
//
// Split the annotation into a list of key value pairs and call the appropriate
// handler for each pair.
void HtmlparserCppTest::ProcessAnnotation(const string &annotation) {
  vector< pair< string, string > > pairs;
  SplitStringIntoKeyValuePairs(annotation, "=", ",", &pairs);

  vector< pair< string, string > >::iterator iter;

  iter = pairs.begin();
  for (iter = pairs.begin(); iter != pairs.end(); ++iter) {
    StripWhiteSpace(&iter->first);
    StripWhiteSpace(&iter->second);

    if (iter->first.compare("state") == 0) {
      ValidateState(iter->second);
    } else if (iter->first.compare("tag") == 0) {
      ValidateTag(iter->second);
    } else if (iter->first.compare("attr") == 0) {
      ValidateAttribute(iter->second);
    } else if (iter->first.compare("value") == 0) {
      ValidateValue(iter->second);
    } else if (iter->first.compare("attr_type") == 0) {
      ValidateAttributeType(iter->second);
    } else if (iter->first.compare("attr_quoted") == 0) {
      ValidateAttributeQuoted(iter->second);
    } else if (iter->first.compare("in_js") == 0) {
      ValidateInJavascript(iter->second);
    } else if (iter->first.compare("js_quoted") == 0) {
      ValidateJavascriptQuoted(iter->second);
    } else if (iter->first.compare("js_state") == 0) {
      ValidateJavascriptState(iter->second);
    } else if (iter->first.compare("in_css") == 0) {
      ValidateInCss(iter->second);
    } else if (iter->first.compare("line_number") == 0) {
      ValidateLine(iter->second);
    } else if (iter->first.compare("column_number") == 0) {
      ValidateColumn(iter->second);
    } else if (iter->first.compare("value_index") == 0) {
      ValidateValueIndex(iter->second);
    } else if (iter->first.compare("is_url_start") == 0) {
      ValidateIsUrlStart(iter->second);
    } else if (iter->first.compare("save_context") == 0) {
      if (!contextMap.count(iter->second)) {
        contextMap[iter->second] = new HtmlParser();
      }
      contextMap[iter->second]->CopyFrom(&parser_);
    } else if (iter->first.compare("load_context") == 0) {
      CHECK(contextMap.count(iter->second));
      parser_.CopyFrom(contextMap[iter->second]);
    } else if (iter->first.compare("reset") == 0) {
      if (StringToBool(iter->second)) {
        parser_.Reset();
      }
    } else if (iter->first.compare("reset_mode") == 0) {
      HtmlParser::Mode mode =
           static_cast<HtmlParser::Mode>(NameToId(kResetModeMap, iter->second));
      parser_.ResetMode(mode);
    } else if (iter->first.compare("insert_text") == 0) {
      if (StringToBool(iter->second)) {
        parser_.InsertText();
      }
    } else {
      FAIL() << "Unknown test directive: " << iter->first;
    }
  }
}

// Validates an html annotated file against the parser state.
//
// It iterates over the html file splitting it into html blocks and annotation
// blocks. It sends the html block to the parser and uses the annotation block
// to validate the parser state.
void HtmlparserCppTest::ValidateFile(string filename) {
  // If TEMPLATE_ROOTDIR is set in the environment, it overrides the
  // default of ".".  We use an env-var rather than argv because
  // that's what automake supports most easily.
  const char* template_rootdir = getenv("TEMPLATE_ROOTDIR");
  if (template_rootdir == NULL)
    template_rootdir = DEFAULT_TEMPLATE_ROOTDIR;   // probably "."
  string dir = PathJoin(template_rootdir, "src");
  dir = PathJoin(dir, "tests");
  dir = PathJoin(dir, "htmlparser_testdata");
  const string fullpath = PathJoin(dir, filename);
  fprintf(stderr, "Validating %s", fullpath.c_str());
  string buffer;
  ReadToString(fullpath.c_str(), &buffer);

  // Start of the current html block.
  size_t start_html = 0;

  // Start of the next annotation.
  size_t start_annotation = buffer.find(kDirectiveBegin, 0);

  // Ending of the current annotation.
  size_t end_annotation = buffer.find(kDirectiveEnd, start_annotation);

  while (start_annotation != string::npos) {
    string html_block(buffer, start_html, start_annotation - start_html);
    parser_.Parse(html_block);

    start_annotation += strlen(kDirectiveBegin);

    string annotation_block(buffer, start_annotation,
                            end_annotation - start_annotation);
    ProcessAnnotation(annotation_block);

    // Update line and column count.
    parser_.set_line_number(UpdateLines(annotation_block,
                                        parser_.line_number()));
    parser_.set_column_number(UpdateColumns(annotation_block,
                                            parser_.column_number()));

    start_html = end_annotation + strlen(kDirectiveEnd);
    start_annotation = buffer.find(kDirectiveBegin, start_html);
    end_annotation = buffer.find(kDirectiveEnd, start_annotation);

    // Check for unclosed annotation.
    CHECK(!(start_annotation != string::npos &&
            end_annotation == string::npos));
  }
}

static vector<string> g_filenames;
#define TEST_FILE(testname, filename)                           \
  struct Register_##testname {                                  \
    Register_##testname() { g_filenames.push_back(filename); }  \
  };                                                            \
  static Register_##testname g_register_##testname

TEST(HtmlparserTest, TestFiles) {
  HtmlparserCppTest tester;
  for (vector<string>::const_iterator it = g_filenames.begin();
       it != g_filenames.end(); ++it) {
    tester.SetUp();
    tester.ValidateFile(*it);
    tester.TearDown();
  }
}

TEST_FILE(SimpleHtml, "simple.html");
TEST_FILE(Comments, "comments.html");
TEST_FILE(JavascriptBlock, "javascript_block.html");
TEST_FILE(JavascriptAttribute, "javascript_attribute.html");
TEST_FILE(JavascriptRegExp, "javascript_regexp.html");
TEST_FILE(Tags, "tags.html");
TEST_FILE(Context, "context.html");
TEST_FILE(Reset, "reset.html");
TEST_FILE(CData, "cdata.html");
TEST_FILE(LineCount, "position.html");

TEST(Htmlparser, Error) {
  HtmlParser html;

  EXPECT_EQ(html.GetErrorMessage(), (const char *)NULL);
  EXPECT_EQ(html.Parse("<a href='http://www.google.com' ''>\n"),
            HtmlParser::STATE_ERROR);

  EXPECT_STREQ(html.GetErrorMessage(),
               "Unexpected character '\\'' in state 'tag_space'");
  html.Reset();
  EXPECT_EQ(html.GetErrorMessage(), (const char *)NULL);
}

}  // namespace security_streamhtmlparser

int main(int argc, char **argv) {

  return RUN_ALL_TESTS();
}
