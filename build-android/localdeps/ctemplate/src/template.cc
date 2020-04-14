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
#include "base/mutex.h"     // This must go first so we get _XOPEN_SOURCE
#include <ctemplate/template.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>          // for fwrite, fflush
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif         // for stat() and open() and getcwd()
#include <algorithm>        // for binary_search()
#include <functional>       // for binary_function()
#include HASH_MAP_H
#include <iterator>
#include <list>
#include <string>
#include <utility>          // for pair
#include <vector>

#include "base/thread_annotations.h"
#include "htmlparser/htmlparser_cpp.h"
#include <ctemplate/per_expand_data.h>
#include <ctemplate/template_annotator.h>
#include <ctemplate/template_cache.h>
#include <ctemplate/template_dictionary.h>
#include <ctemplate/template_dictionary_interface.h>   // also gets kIndent
#include <ctemplate/template_modifiers.h>
#include "template_modifiers_internal.h"
#include <ctemplate/template_pathops.h>
#include <ctemplate/template_string.h>
#include "base/fileutil.h"
#include <ctype.h>
#include <iostream>
#include <sstream>          // for ostringstream

#ifndef PATH_MAX
#ifdef MAXPATHLEN
#define PATH_MAX        MAXPATHLEN
#else
#define PATH_MAX        4096         // seems conservative for max filename len!
#endif
#endif

#define arraysize(x)  ( sizeof(x) / sizeof(*(x)) )

#define AS_STR1(x)  #x
#define AS_STR(x)   AS_STR1(x)

// A very simple logging system
#undef LOG   // a non-working version is provided in base/util.h; redefine it
static int kVerbosity = 0;   // you can change this by hand to get vlogs
#define LOG(level)   std::cerr << #level ": "
#define VLOG(level)  if (kVerbosity >= level)  LOG(level)

// TODO(csilvers): use our own tables for these?
static bool ascii_isalnum(char c) {
  return ((c & 0x80) == 0) && isalnum(c);  // 7-bit ascii, and an alnum
}

static bool ascii_isspace(char c) {
  return ((c & 0x80) == 0) && isspace(c);  // 7-bit ascii, and a space
}

#define strsuffix(str, suffix)                                          \
   ( strlen(str) > (sizeof("" suffix "") - 1) &&                            \
     strcmp(str + strlen(str) - (sizeof(suffix) - 1), suffix) == 0 )

using std::endl;
using std::string;
using std::list;
using std::vector;
using std::pair;
using std::binary_search;
using HASH_NAMESPACE::unordered_map;

namespace ctemplate {

using ctemplate_htmlparser::HtmlParser;

TemplateId GlobalIdForSTS_INIT(const TemplateString& s) {
  return s.GetGlobalId();   // normally this method is private
}

int Template::num_deletes_ = 0;

namespace {
// Mutex for protecting Expand calls against ReloadIfChanged, which
// might change a template while it's being expanded.  This mutex used
// to be a per-template mutex, rather than a global mutex, which seems
// like it would be strictly better, but we ran into subtle problems
// with deadlocks when a template would sub-include itself (thus
// requiring a recursive read-lock during Expand), and the template
// was Expanded and ReloadIfChanged at the same time.  Rather than
// deal with that complication, we just go with a global mutex.  Since
// ReloadIfChanged is deprecated, in most applications all the mutex
// uses will be as read-locks, so this shouldn't cause much contention.
static Mutex g_template_mutex(base::LINKER_INITIALIZED);

// Mutex for protecting vars_seen in WriteOneHeaderEntry, below.
// g_template_mutex and g_header_mutex are never held at the same time.
// TODO(csilvers): assert this in the codebase.
static Mutex g_header_mutex(base::LINKER_INITIALIZED);

// It's not great to have a global variable with a constructor, but
// it's safe in this case: the constructor is trivial and does not
// depend on any other global constructors running first, and the
// variable is used in only one place below, always after main() has
// started.
// It is ok for this modifier to be in XssClass XSS_WEB_STANDARD because
// it only adds indentation characters - typically whitespace - iff these
// are already present in the text. If such characters were XSS-harmful
// in a given context, they would have already been escaped or replaced
// by earlier escaping such as H=attribute.
static const ModifierInfo g_prefix_line_info("", '\0', XSS_WEB_STANDARD,
                                             &prefix_line);

const char * const kDefaultTemplateDirectory = kCWD;   // "./"
// Note this name is syntactically impossible for a user to accidentally use.
const char * const kMainSectionName = "__{{MAIN}}__";

// A TemplateString object that precomputes its hash. This can be
// useful in places like template filling code, where we'd like to
// hash the string once then reuse it many times.  This should not be
// used for filling any part of a template dictionary, since we don't
// map the id to its corresponding string or manage memory for the
// string - it is for lookups *only*.
class HashedTemplateString : public TemplateString {
 public:
  HashedTemplateString(const char* s, size_t slen) : TemplateString(s, slen) {
    CacheGlobalId();
  }
};

#define LOG_TEMPLATE_NAME(severity, template)                           \
  LOG(severity) << "Template " << template->template_file() << ": "

#define LOG_AUTO_ESCAPE_ERROR(error_msg, my_template) do {      \
    LOG_TEMPLATE_NAME(ERROR, my_template);                      \
    LOG(ERROR) << "Auto-Escape: " << error_msg << endl;         \
  } while (0)

// We are in auto-escape mode.
#define AUTO_ESCAPE_MODE(context) ((context) != TC_MANUAL)

// Auto-Escape contexts which utilize the HTML Parser.
#define AUTO_ESCAPE_PARSING_CONTEXT(context)                            \
  ((context) == TC_HTML || (context) == TC_JS || (context) == TC_CSS)

// ----------------------------------------------------------------------
// PragmaId
// PragmaDefinition
// PragmaMarker
//   Functionality to support the PRAGMA marker in the template, i.e
//   the {{%IDENTIFIER [name1="value1" [name2="value2"]...]}} syntax:
//     . IDENTIFIER as well as all attribute names are case-insensitive
//       whereas attribute values are case-sensitive.
//     . No extraneous whitespace is allowed (e.g. between name and '=').
//     . Double quotes inside an attribute value need to be backslash
//       escaped, i.e. " -> \". We unescape them during parsing.
//
//   The only identifier currently supported is AUTOESCAPE which is
//   used to auto-escape a given template. Its syntax is:
//   {{%AUTOESCAPE context="context" [state="state"]}} where:
//     . context is one of: "HTML", "JAVASCRIPT", "CSS", "XML", "JSON".
//     . state may be omitted or equivalently, it may be set to "default".
//       It also accepts the value "IN_TAG" in the HTML context to
//       indicate the template contains HTML attribute name/value
//       pairs that are enclosed in a tag specified in a parent template.
//       e.g: Consider the parent template:
//              <a href="/bla" {{>INC}}>text</a>
//            and the included template:
//              class="{{CLASS}}" target="{{TARGET}}"
//       Then, for the included template to be auto-escaped properly, it
//       must have the pragma: {{%AUTOESCAPE context="HTML" state="IN_TAG"}}.
//       This is a very uncommon template structure.
//
//   To add a new pragma identifier, you'll have to at least:
//     1. Add a new id for it in PragmaId enum.
//     2. Add the corresponding definition in static g_pragmas array
//     3. If you accept more than 2 attributes, increase the size
//        of attribute_names in the PragmaDefinition struct.
//     4. Add handling of that pragma in SectionTemplateNode::GetNextToken()
//        and possibly SectionTemplateNode::AddPragmaNode()
// ----------------------------------------------------------------------

// PragmaId
//   Identify all the pragma identifiers we support. Currently only
//   one (for AutoEscape). PI_ERROR is only for internal error reporting,
//   and is not a valid pragma identifier.
enum PragmaId { PI_UNUSED, PI_ERROR, PI_AUTOESCAPE, NUM_PRAGMA_IDS };

// Each pragma definition has a unique identifier as well as a list of
// attribute names it accepts. This allows initial error checking while
// parsing a pragma definition. Such error checking will need supplementing
// with more pragma-specific logic in SectionTemplateNode::GetNextToken().
static struct PragmaDefinition {
  PragmaId pragma_id;
  const char* identifier;
  const char* attribute_names[2];  // Increase as needed.
} g_pragmas[NUM_PRAGMA_IDS] = {
  /* PI_UNUSED     */ { PI_UNUSED, NULL, {} },
  /* PI_ERROR      */ { PI_ERROR, NULL, {} },
  /* PI_AUTOESCAPE */ { PI_AUTOESCAPE, "AUTOESCAPE", {"context", "state"} }
};

// PragmaMarker
//   Functionality to parse the {{%...}} syntax and extract the
//   provided attribute values. We store the PragmaId as well
//   as a vector of all the attribute names and values provided.
class PragmaMarker {
 public:
  // Constructs a PragmaMarker object from the PRAGMA marker
  // {{%ID [[name1=\"value1"] ...]}}. On error (unable to parse
  // the marker), returns an error description in error_msg. On
  // success, error_msg is cleared.
  PragmaMarker(const char* token_start, const char* token_end,
               string* error_msg);

  // Returns the attribute value for the corresponding attribute name
  // or NULL if none is found (as is the case with optional attributes).
  // Ensure you only call it on attribute names registered in g_pragmas
  // for that PragmaId.
  const string* GetAttributeValue(const char* attribute_name) const;

 private:
  // Checks that the identifier given matches one of the pragma
  // identifiers we know of, in which case returns the corresponding
  // PragmaId. In case of error, returns PI_ERROR.
  static PragmaId GetPragmaId(const char* id, size_t id_len);

  // Parses an attribute value enclosed in double quotes and updates
  // value_end to point at ending double quotes. Returns the attribute
  // value. If an error occurred, error_msg is set with information.
  // It is cleared on success.
  // Unescapes backslash-escaped double quotes ('\"' -> '"') if present.
  static string ParseAttributeValue(const char* value_start,
                                    const char** value_end,
                                    string* error_msg);

  // Returns true if the attribute name is an accepted one for that
  // given PragmaId. Otherwise returns false.
  static bool IsValidAttribute(PragmaId pragma_id, const char* name,
                               size_t namelen);

  PragmaId pragma_id_;
  // A vector of attribute (name, value) pairs.
  vector<pair<string, string> > names_and_values_;
};

PragmaId PragmaMarker::GetPragmaId(const char* id, size_t id_len) {
  for (int i = 0; i < NUM_PRAGMA_IDS; ++i) {
    if (g_pragmas[i].identifier == NULL)  // PI_UNUSED, PI_ERROR
      continue;
    if ((strlen(g_pragmas[i].identifier) == id_len) &&
        (strncasecmp(id, g_pragmas[i].identifier, id_len) == 0))
      return g_pragmas[i].pragma_id;
  }
  return PI_ERROR;
}

bool PragmaMarker::IsValidAttribute(PragmaId pragma_id, const char* name,
                                    size_t namelen) {
  const int kMaxAttributes = sizeof(g_pragmas[0].attribute_names) /
      sizeof(*g_pragmas[0].attribute_names);
  for (int i = 0; i < kMaxAttributes; ++i) {
    const char* attr_name = g_pragmas[pragma_id].attribute_names[i];
    if (attr_name == NULL)
      break;
    if ((strlen(attr_name) == namelen) &&
        (strncasecmp(attr_name, name, namelen) == 0))
      // We found the given name in our accepted attribute list.
      return true;
  }
  return false;  // We did not find the name.
}

const string* PragmaMarker::GetAttributeValue(
    const char* attribute_name) const {
  // Developer error if assert triggers.
  assert(IsValidAttribute(pragma_id_, attribute_name, strlen(attribute_name)));
  for (vector<pair<string, string> >::const_iterator it =
           names_and_values_.begin(); it != names_and_values_.end(); ++it) {
    if (strcasecmp(attribute_name, it->first.c_str()) == 0)
      return &it->second;
  }
  return NULL;
}

string PragmaMarker::ParseAttributeValue(const char* value_start,
                                         const char** value_end,
                                         string* error_msg) {
  assert(error_msg);
  if (*value_start != '"') {
    error_msg->append("Attribute value is not enclosed in double quotes.");
    return "";
  }
  const char* current = ++value_start;   // Advance past the leading '"'
  const char* val_end;
  do {
    if (current >= *value_end ||
        ((val_end =
          (const char*)memchr(current, '"', *value_end - current)) == NULL)) {
      error_msg->append("Attribute value not terminated.");
      return "";
    }
    current = val_end + 1;              // Advance past the current '"'
  } while (val_end[-1] == '\\');

  string attribute_value(value_start, val_end - value_start);
  // Now replace \" with "
  size_t found;
  while ((found = attribute_value.find("\\\"")) != string::npos)
    attribute_value.erase(found, 1);
  *value_end = val_end;
  error_msg->clear();
  return attribute_value;
}

PragmaMarker::PragmaMarker(const char* token_start, const char* token_end,
                           string* error_msg) {
  assert(error_msg);
  string error;
  const char* identifier_end =
      (const char*)memchr(token_start, ' ', token_end - token_start);
  if (identifier_end == NULL)
    identifier_end = token_end;
  pragma_id_ = PragmaMarker::GetPragmaId(token_start,
                                         identifier_end - token_start);
  if (pragma_id_ == PI_ERROR) {
    error = "Unrecognized pragma identifier.";
  } else {
    const char* val_end;
    // Loop through attribute name/value pairs.
    for (const char* nameval = identifier_end; nameval < token_end;
         nameval = val_end + 1) {
      // Either after identifier or afer a name/value pair. Must be whitespace.
      if (*nameval++ != ' ') {
        error = "Extraneous text.";
        break;
      }
      const char* val = (const char*)memchr(nameval, '=', token_end - nameval);
      if (val == NULL || val == nameval) {
        error = "Missing attribute name or value";
        break;
      }
      const string attribute_name(nameval, val - nameval);
      if (!PragmaMarker::IsValidAttribute(pragma_id_, attribute_name.data(),
                                          attribute_name.length())) {
        error = "Unrecognized attribute name: " + attribute_name;
        break;
      }
      ++val;  // Advance past '='
      val_end = token_end;
      const string attribute_value = ParseAttributeValue(val, &val_end, &error);
      if (!error.empty())  // Failed to parse attribute value.
        break;
      names_and_values_.push_back(pair<const string, const string>(
          attribute_name, attribute_value));
    }
  }
  if (error.empty())   // Success
    error_msg->clear();
  else                 // Error
    error_msg->append("In PRAGMA directive '" +
                      string(token_start, token_end - token_start) +
                      "' Error: " + error);
}

// ----------------------------------------------------------------------
// memmatch()
//    Return a pointer to the first occurrences of the given
//    length-denominated string, inside a bigger length-denominated
//    string, or NULL if not found.  The mem version of strstr.
// ----------------------------------------------------------------------

static const char *memmatch(const char *haystack, size_t haystack_len,
                            const char *needle, size_t needle_len) {
  if (needle_len == 0)
    return haystack;    // even if haystack_len is 0
  else if (needle_len > haystack_len)
    return NULL;

  const char* match;
  const char* hayend = haystack + haystack_len - needle_len + 1;
  while ((match = (const char*)memchr(haystack, needle[0],
                                      hayend - haystack))) {
    if (memcmp(match, needle, needle_len) == 0)
      return match;
    else
      haystack = match + 1;
  }
  return NULL;
}

// ----------------------------------------------------------------------
// FilenameValidForContext()
// GetTemplateContextFromPragma()
// GetModifierForContext()
// FindLongestMatch()
// PrettyPrintTokenModifiers()
//    Static methods for the auto-escape mode specifically.

// Perfoms matching of filename against the TemplateContext
// and warns in the log on mismatch using "unwritten" filename
// conventions below for templates in our codebase:
//   1. If filename contains "css", "stylesheet" or "style"
//      check that it has type TC_CSS.
//   2. If filename contains "js" or "javascript" check that
//      it has type TC_JS.
// Returns false if there was a mismatch although currently
// we ignore it and just rely on the LOG(WARNING) in the logs.
static bool FilenameValidForContext(const string& filename,
                                    TemplateContext context) {
  string stripped_filename = Basename(filename);

  if (ctemplate::ContainsFullWord(stripped_filename, "css") ||
      ctemplate::ContainsFullWord(stripped_filename, "stylesheet") ||
      ctemplate::ContainsFullWord(stripped_filename, "style")) {
    if (context != TC_CSS) {
      LOG(WARNING) << "Template filename " << filename
                   << " indicates CSS but given TemplateContext"
                   << " was not TC_CSS." << endl;
      return false;
    }
  } else if (ctemplate::ContainsFullWord(stripped_filename, "js") ||
             ctemplate::ContainsFullWord(stripped_filename, "javascript")) {
    if (context != TC_JS) {
      LOG(WARNING) << "Template filename " << filename
                   << " indicates javascript but given TemplateContext"
                   << " was not TC_JS." << endl;
      return false;
    }
  }
  return true;
}

// Returns a string containing a human-readable description of
// the modifiers in the vector. The format is:
// :modifier1[=val1][:modifier2][=val2]...
static string PrettyPrintTokenModifiers(
    const vector<ModifierAndValue>& modvals) {
  string out;
  for (vector<ModifierAndValue>::const_iterator it =
           modvals.begin(); it != modvals.end();  ++it) {
    string one_mod = PrettyPrintOneModifier(*it);
    out.append(one_mod);
  }
  return out;
}

// Returns the TemplateContext corresponding to the "context" attribute
// of the AUTOESCAPE pragma. Returns TC_MANUAL to indicate an error,
// meaning an invalid context was given in the pragma.
static TemplateContext GetTemplateContextFromPragma(
    const PragmaMarker& pragma) {
  const string* context = pragma.GetAttributeValue("context");
  if (context == NULL)
    return TC_MANUAL;
  if (*context == "HTML" || *context == "html")
    return TC_HTML;
  else if (*context == "JAVASCRIPT" || *context == "javascript")
    return TC_JS;
  else if (*context == "CSS" || *context == "css")
    return TC_CSS;
  else if (*context == "JSON" || *context == "json")
    return TC_JSON;
  else if (*context == "XML" || *context == "xml")
    return TC_XML;
  return TC_MANUAL;
}

// Based on the state of the parser, determines the appropriate escaping
// directive and returns a pointer to the corresponding
// global ModifierAndValue vector. Called when a variable template node
// is traversed.
// Returns NULL if there is no suitable modifier for that context in
// which the case the caller is expected to fail the template initialization.
static const vector<const ModifierAndValue*> GetModifierForContext(
    TemplateContext my_context, HtmlParser *htmlparser,
    const Template* my_template) {
  assert(AUTO_ESCAPE_MODE(my_context));
  vector<const ModifierAndValue*> modvals;
  string error_msg;

  switch (my_context) {
    case TC_XML:
      modvals = GetModifierForXml(htmlparser, &error_msg);
      break;
    case TC_JSON:
      modvals = GetModifierForJson(htmlparser, &error_msg);
      break;
    case TC_CSS:
      assert(htmlparser);  // Parser is active in CSS
      modvals = GetModifierForCss(htmlparser, &error_msg);
      break;
    default:
      // Must be in TC_HTML or TC_JS. Parser is active in these modes.
      assert(AUTO_ESCAPE_PARSING_CONTEXT(my_context));
      assert(htmlparser);
      modvals = GetModifierForHtmlJs(htmlparser, &error_msg);
  }
  // In any mode, there should be at least one modifier.
  if (modvals.empty())
    LOG_AUTO_ESCAPE_ERROR(error_msg, my_template);
  return modvals;
}

// Returns the largest int N indicating how many XSS safe alternative
// modifiers are in the in-template modifiers already.
// . If N is equal to the number of modifiers determined by the Auto Escaper,
//   we have a full match and the in-template modifiers were safe. We leave
//   them untouched.
// . Otherwise, N is less (or zero) and we have a partial match (or none).
//   The in-template modifiers are not XSS safe and need the missing ones,
//   i.e. those in the auto escape modifiers which are not in the first N.
//
// We allow in-template modifiers to have extra modifiers than we deem
// necessary, for e.g. :j:h when :j would have sufficed. But to make sure
// these modifiers do not introduce XSS concerns we require that they
// be in the same XssClass as the modifier we had.
// For example :h:x-bla is not safe in HTML context because x-bla is
// in a different XssClass as our :h whereas :h:j would be safe.
static size_t FindLongestMatch(
    const vector<ModifierAndValue>& modvals_man,
    const vector<const ModifierAndValue*>& modvals_auto) {
  if (modvals_auto.empty())
    return 0;

  // See if modvals_auto is "consistent" with the modifiers that are
  // already present (modvals_man).  This is true if all the
  // modifiers in auto also occur in man, and any gaps between them
  // (if any) are filled by "neutral" modifiers that do not affect
  // xss-safety. We go through the vectors backwards.
  // If all of modvals_auto is not consistent, maybe a prefix of it
  // is; that's better than nothing, since we only need to auto-apply
  // the suffix that's not already in modvals_man.
  typedef vector<const ModifierAndValue*>::const_reverse_iterator
      ModAutoIterator;
  typedef vector<ModifierAndValue>::const_reverse_iterator ModManIterator;
  for (ModAutoIterator end_of_prefix = modvals_auto.rbegin();
       end_of_prefix != modvals_auto.rend();
       ++end_of_prefix) {
    ModAutoIterator curr_auto = end_of_prefix;
    ModManIterator curr_man = modvals_man.rbegin();
    while (curr_auto != modvals_auto.rend() &&
           curr_man != modvals_man.rend()) {
      if (IsSafeXSSAlternative(*(*curr_auto)->modifier_info,
                               *curr_man->modifier_info)) {
        ++curr_auto;
        ++curr_man;
      } else if ((curr_man->modifier_info->xss_class ==
                  (*curr_auto)->modifier_info->xss_class) &&
                 (curr_man->modifier_info->xss_class != XSS_UNIQUE)) {
        ++curr_man;  // Ignore this modifier: it's harmless.
      } else {
        break;      // An incompatible modifier; we've failed
      }
    }
    if (curr_auto == modvals_auto.rend())  // got through them all, full match!
      return curr_auto - end_of_prefix;
  }
  return 0;
}

// ----------------------------------------------------------------------
// WriteOneHeaderEntry()
//    This dumps information about a template that is useful to
//    make_tpl_varnames_h -- information about the variable and
//    section names used in a template, so we can define constants
//    to refer to them instead of having to type them in by hand.
//    Output is *appended* to outstring.
// ----------------------------------------------------------------------

static void WriteOneHeaderEntry(
    string *outstring, const string& variable, const string& full_pathname)
    LOCKS_EXCLUDED(g_header_mutex) {
  MutexLock ml(&g_header_mutex);

  // we use hash_map instead of hash_set just to keep the stl size down
  static unordered_map<string, bool, StringHash> vars_seen
      GUARDED_BY(g_header_mutex);
  static string current_file GUARDED_BY(g_header_mutex);
  static string prefix GUARDED_BY(g_header_mutex);

  if (full_pathname != current_file) {
    // changed files so re-initialize the static variables
    vars_seen.clear();
    current_file = full_pathname;

    // remove the path before the filename
    string filename(Basename(full_pathname));

    prefix = "k";
    bool take_next = true;

    for (string::size_type i = 0; i < filename.length(); i++) {
      if (filename[i] == '.') {
        // stop when we find the dot
        break;
      }
      if (take_next) {
        if (filename.substr(i, 4) == "post") {
          // stop before we process post...
          break;
        }
        prefix = prefix + filename[i];
        take_next = false;
      }
      if (filename[i] == '_') {
        take_next = true;
      }
    }
    prefix = prefix + "_";
  }

  // print out the variable, but only if we haven't seen it before.
  if (!vars_seen.count(variable)) {
    if (variable == kMainSectionName || variable.find("BI_") == 0) {
      // We don't want to write entries for __MAIN__ or the built-ins
    } else {
      const TemplateId id = GlobalIdForSTS_INIT(TemplateString(variable));
      std::ostringstream outstream;
      outstream << "static const "
                << "::ctemplate::StaticTemplateString "
                << prefix << variable << " = STS_INIT_WITH_HASH("
                << prefix << variable << ", \"" << variable << "\", "
                << id << "ULL);\n";
      outstring->append(outstream.str());
    }
    vars_seen[variable] = true;
  }
}

// ----------------------------------------------------------------------
// TemplateToken
//    A TemplateToken is a string marked with a token type enum.  The string
//    has different meanings for different token types.  For text, the
//    string is the text itself.   For variable and template types, the
//    string is the name of the variable holding the value or the
//    template name, resp.  For section types, the string is the name
//    of the section, used to retrieve the hidden/visible state and
//    the associated list of dictionaries, if any. For pragma type,
//    the string is the full text of the marker and is only used for
//    debug information.
// ----------------------------------------------------------------------

enum TemplateTokenType { TOKENTYPE_UNUSED,        TOKENTYPE_TEXT,
                         TOKENTYPE_VARIABLE,      TOKENTYPE_SECTION_START,
                         TOKENTYPE_SECTION_END,   TOKENTYPE_TEMPLATE,
                         TOKENTYPE_COMMENT,       TOKENTYPE_SET_DELIMITERS,
                         TOKENTYPE_PRAGMA,        TOKENTYPE_NULL,
                         TOKENTYPE_HIDDEN_DEFAULT_SECTION,
                       };

}  // unnamed namespace

// A sorted array of Template variable names that Auto-Escape should
// not escape. Variables that you may want to add here typically
// satisfy all the following conditions:
// 1. Are "trusted" variables, meaning variables you know to not
//    contain potentially harmful content.
// 2. Contain some markup that gets broken when escaping is
//    applied to them.
// 3. Are used often such that requiring developers to add
//    ":none" to each use is error-prone and inconvenient.
//
// Note: Keep this array sorted as you add new elements!
//
const char * const Template::kSafeWhitelistedVariables[] = {
  ""   // a placekeeper element: replace with your real values!
};
const size_t Template::kNumSafeWhitelistedVariables =
    arraysize(Template::kSafeWhitelistedVariables);

// A TemplateToken is a typed string. The semantics of the string depends on the
// token type, as follows:
//   TOKENTYPE_TEXT          - the text
//   TOKENTYPE_VARIABLE      - the name of the variable
//   TOKENTYPE_SECTION_START - the name of the section being started
//   TOKENTYPE_SECTION_END   - the name of the section being ended
//   TOKENTYPE_TEMPLATE      - the name of the variable whose value will be
//                             the template filename
//   TOKENTYPE_COMMENT       - the empty string, not used
//   TOKENTYPE_SET_DELIMITERS- the empty string, not used
//   TOKENTYPE_PRAGMA        - identifier and optional set of name/value pairs
//                           - exactly as given in the template
//   TOKENTYPE_NULL          - the empty string
//   TOKENTYPE_HIDDEN_DEFAULT_SECTION
//                           - like TOKENTYPE_SECTION_START, but defaults to
//                             hidden
// All non-comment tokens may also have modifiers, which follow the name
// of the token: the syntax is {{<PREFIX><NAME>:<mod>:<mod>:<mod>...}}
// The modifiers are also stored as a string, starting with the first :
struct TemplateToken {
  TemplateTokenType type;
  const char* text;
  size_t textlen;
  vector<ModifierAndValue> modvals;
  TemplateToken(TemplateTokenType t, const char* txt, size_t len,
                const vector<ModifierAndValue>* m)
      : type(t), text(txt), textlen(len) {
    if (m) modvals = *m;
  }

  string ToString() const {   // used for debugging (annotations)
    string retval(text, textlen);
    for (vector<ModifierAndValue>::const_iterator it = modvals.begin();
         it != modvals.end();  ++it) {
      const string& modname = it->modifier_info->long_name;
      retval += string(":") + modname;
      if (!it->modifier_info->is_registered)
        retval += "<not registered>";
    }
    return retval;
  }

  // Updates the correct modifiers for the token (variable or template node)
  // based on our computed modifiers from the HTML parser context as well
  // as the in-template modifiers that may have been provided.
  // If the in-template modifiers are considered safe, we use them
  // without modification. This could happen in one of three cases:
  //   1. The token has the ":none" modifier as one of the modifiers.
  //   2. The token has a custom modifier considered XSS-Safe as one of
  //      the modifiers. The modifier was added via AddXssSafeModifier()
  //      and has the XSS_SAFE XssClass.
  //   3. The escaping modifiers are XSS-equivalent to the ones we computed.
  //
  // If the in-template modifiers are not found to be safe, we add
  // the escaping modifiers we determine missing. This is done based on a
  // longest match search between the two modifiers vectors, refer to comment
  // in FindLongestMatch. We also issue a warning in the log, unless the
  // in-template modifiers were all not escaping related (e.g. custom)
  // since that case is similar to that of not providing any modifiers.
  void UpdateModifier(const vector<const ModifierAndValue*>& auto_modvals) {
    // Common case: no modifiers given in template. Assign our own. No warning.
    if (modvals.empty()) {
      for (vector<const ModifierAndValue*>::const_iterator it
               = auto_modvals.begin(); it != auto_modvals.end(); ++it) {
        modvals.push_back(**it);
      }
      return;
    }

    // Look for any XSS-Safe modifiers (added via AddXssSafeModifier or :none).
    // If one is found anywhere in the vector, consider the variable safe.
    for (vector<ModifierAndValue>::const_iterator it = modvals.begin();
         it != modvals.end(); ++it) {
      if (it->modifier_info->xss_class == XSS_SAFE)
        return;
    }

    size_t longest_match = FindLongestMatch(modvals, auto_modvals);
    if (longest_match == auto_modvals.size()) {
      return;             // We have a complete match, nothing to do.
    } else {              // Copy missing ones and issue warning.
      assert(longest_match >= 0 && longest_match < auto_modvals.size());
      // We only log if one or more of the in-template modifiers was
      // escaping-related which we infer from the XssClass. Currently,
      // all escaping modifiers are in XSS_WEB_STANDARD except for 'none'
      // but that one is handled above.
      bool do_log = false;
      for (vector<ModifierAndValue>::const_iterator it = modvals.begin();
           it != modvals.end(); ++it) {
        if (it->modifier_info->xss_class == XSS_WEB_STANDARD) {
          do_log = true;
          break;
        }
      }
      string before = PrettyPrintTokenModifiers(modvals);  // for logging
      for (vector<const ModifierAndValue*>::const_iterator it
               = auto_modvals.begin() + longest_match;
           it != auto_modvals.end(); ++it) {
        modvals.push_back(**it);
      }
      if (do_log)
        LOG(ERROR)
            << "Token: " << string(text, textlen)
            << " has missing in-template modifiers. You gave " << before
            << " and we computed " << PrettyPrintModifiers(auto_modvals, "")
            << ". We changed to " << PrettyPrintTokenModifiers(modvals) << endl;
    }
  }
};

static bool AnyMightModify(const vector<ModifierAndValue>& modifiers,
                           const PerExpandData* data) {
  for (vector<ModifierAndValue>::const_iterator it = modifiers.begin();
       it != modifiers.end();  ++it) {
    string value_string(it->value, it->value_len);
    if (it->modifier_info->modifier->MightModify(data, value_string)) {
      return true;
    }
  }
  return false;
}

// This applies the modifiers to the string in/inlen, and writes the end
// result directly to the end of outbuf.  Precondition: |modifiers| > 0.
//
// TODO(user): In the case of multiple modifiers, we are applying
// all of them if any of them MightModify the output.  We can do
// better.  We should store the MightModify values that we use to
// compute AnyMightModify and respect them here.
static void EmitModifiedString(const vector<ModifierAndValue>& modifiers,
                               const char* in, size_t inlen,
                               const PerExpandData* data,
                               ExpandEmitter* outbuf) {
  string result;
  string value_string;
  if (modifiers.size() > 1) {
    // If there's more than one modifiers, we need to store the
    // intermediate results in a temp-buffer.  We use a string.
    // We'll assume that each modifier adds about 12% to the input
    // size.
    result.reserve((inlen + inlen/8) + 16);
    StringEmitter scratchbuf(&result);
    value_string = string(modifiers.front().value, modifiers.front().value_len);
    modifiers.front().modifier_info->modifier->Modify(in, inlen, data,
                                                      &scratchbuf,
                                                      value_string);
    // Only used when modifiers.size() > 2
    for (vector<ModifierAndValue>::const_iterator it = modifiers.begin() + 1;
         it != modifiers.end()-1;  ++it) {
      string output_of_this_modifier;
      output_of_this_modifier.reserve(result.size() + result.size()/8 + 16);
      StringEmitter scratchbuf2(&output_of_this_modifier);
      value_string = string(it->value, it->value_len);
      it->modifier_info->modifier->Modify(result.c_str(), result.size(), data,
                                          &scratchbuf2, value_string);
      result.swap(output_of_this_modifier);
    }
    in = result.data();
    inlen = result.size();
  }
  // For the last modifier, we can write directly into outbuf
  assert(!modifiers.empty());
  value_string = string(modifiers.back().value, modifiers.back().value_len);
  modifiers.back().modifier_info->modifier->Modify(in, inlen, data, outbuf,
                                                   value_string);
}

static void AppendTokenWithIndent(int level, string *out, const string& before,
                                  const TemplateToken& token,
                                  const string& after) {
  out->append(string(level * kIndent, ' '));
  string token_string(token.text, token.textlen);
  out->append(before + token_string + after);
}

// ----------------------------------------------------------------------
// TemplateNode
//    When we read a template, we decompose it into its components:
//    variables, sections, include-templates, and runs of raw text.
//    Each of these we see becomes one TemplateNode.  TemplateNode
//    is the abstract base class; each component has its own type.
// ----------------------------------------------------------------------

class TemplateNode {
 public:
  TemplateNode() {}
  virtual ~TemplateNode() {}

  // Expands the template node using the supplied dictionary. The
  // result is placed into output_buffer.  If
  // per_expand_data->annotate() is true, the output is annotated.
  // Returns true iff all the template files load and parse correctly.
  virtual bool Expand(ExpandEmitter *output_buffer,
                      const TemplateDictionaryInterface *dictionary,
                      PerExpandData *per_expand_data,
                      const TemplateCache *cache) const = 0;

  // Writes entries to a header file to provide syntax checking at
  // compile time.
  virtual void WriteHeaderEntries(string *outstring,
                                  const string& filename) const = 0;

  // Appends a representation of the node and its subnodes to a string
  // as a debugging aid.
  virtual void DumpToString(int level, string *out) const = 0;

 protected:
  typedef list<TemplateNode *> NodeList;

 private:
  TemplateNode(const TemplateNode&);   // disallow copying
  void operator=(const TemplateNode&);
};

// ----------------------------------------------------------------------
// TextTemplateNode
//    The simplest template-node: it holds runs of raw template text,
//    that should be emitted verbatim.  The text points into
//    template_text_.
// ----------------------------------------------------------------------

class TextTemplateNode : public TemplateNode {
 public:
  explicit TextTemplateNode(const TemplateToken& token)
      : token_(token) {
    VLOG(2) << "Constructing TextTemplateNode: "
            << string(token_.text, token_.textlen) << endl;
  }
  virtual ~TextTemplateNode() {
    VLOG(2) << "Deleting TextTemplateNode: "
            << string(token_.text, token_.textlen) << endl;
  }

  // Expands the text node by simply outputting the text string. This
  // virtual method does not use TemplateDictionaryInterface or PerExpandData.
  // Returns true iff all the template files load and parse correctly.
  virtual bool Expand(ExpandEmitter *output_buffer,
                      const TemplateDictionaryInterface *,
                      PerExpandData *,
                      const TemplateCache *) const {
    output_buffer->Emit(token_.text, token_.textlen);
    return true;
  }

  // A noop for text nodes
  virtual void WriteHeaderEntries(string *outstring,
                                  const string& filename) const {
    return;
  }

  // Appends a representation of the text node to a string.
  virtual void DumpToString(int level, string *out) const {
    assert(out);
    AppendTokenWithIndent(level, out, "Text Node: -->|", token_, "|<--\n");
  }

 private:
  TemplateToken token_;  // The text held by this node.
};

// ----------------------------------------------------------------------
// VariableTemplateNode
//    Holds a variable to be replaced when the template is expanded.
//    The variable is stored in a token object, which has a char*
//    that points into template_text_.  There may also be modifiers,
//    which are applied at Expand time.
// ----------------------------------------------------------------------

class VariableTemplateNode : public TemplateNode {
 public:
  explicit VariableTemplateNode(const TemplateToken& token)
      : token_(token),
        variable_(token_.text, token_.textlen) {
    VLOG(2) << "Constructing VariableTemplateNode: "
            << string(token_.text, token_.textlen) << endl;
  }
  virtual ~VariableTemplateNode() {
    VLOG(2) << "Deleting VariableTemplateNode: "
            << string(token_.text, token_.textlen) << endl;
  }

  // Expands the variable node by outputting the value (if there is one)
  // of the node variable which is retrieved from the dictionary
  // Returns true iff all the template files load and parse correctly.
  virtual bool Expand(ExpandEmitter *output_buffer,
                      const TemplateDictionaryInterface *dictionary,
                      PerExpandData *per_expand_data,
                      const TemplateCache *cache) const;

  virtual void WriteHeaderEntries(string *outstring,
                                  const string& filename) const {
    WriteOneHeaderEntry(outstring, string(token_.text, token_.textlen),
                        filename);
  }

  // Appends a representation of the variable node to a string. We
  // also append the modifiers for that variable in the form:
  // :modifier1[=val1][:modifier2][=val2]...\n
  virtual void DumpToString(int level, string *out) const {
    assert(out);
    AppendTokenWithIndent(level, out, "Variable Node: ", token_,
                          PrettyPrintTokenModifiers(token_.modvals) + "\n");
  }

 private:
  const TemplateToken token_;
  const HashedTemplateString variable_;
};

bool VariableTemplateNode::Expand(ExpandEmitter *output_buffer,
                                  const TemplateDictionaryInterface *dictionary,
                                  PerExpandData* per_expand_data,
                                  const TemplateCache *cache) const {
  if (per_expand_data->annotate()) {
    per_expand_data->annotator()->EmitOpenVariable(output_buffer,
                                                   token_.ToString());
  }

  const TemplateString value = dictionary->GetValue(variable_);

  if (AnyMightModify(token_.modvals, per_expand_data)) {
    EmitModifiedString(token_.modvals, value.data(), value.size(),
                       per_expand_data, output_buffer);
  } else {
    // No need to modify value, so just emit it.
    output_buffer->Emit(value.data(), value.size());
  }

  if (per_expand_data->annotate()) {
    per_expand_data->annotator()->EmitCloseVariable(output_buffer);
  }

  return true;
}

// ----------------------------------------------------------------------
// PragmaTemplateNode
//   It simply stores the text given inside the pragma marker
//   {{%...}} for possible use in DumpToString().
// ----------------------------------------------------------------------

class PragmaTemplateNode : public TemplateNode {
 public:
  explicit PragmaTemplateNode(const TemplateToken& token)
      : token_(token) {
    VLOG(2) << "Constructing PragmaTemplateNode: "
            << string(token_.text, token_.textlen) << endl;
  }
  virtual ~PragmaTemplateNode() {
    VLOG(2) << "Deleting PragmaTemplateNode: "
            << string(token_.text, token_.textlen) << endl;
  }

  // A no-op for pragma nodes.
  virtual bool Expand(ExpandEmitter *output_buffer,
                      const TemplateDictionaryInterface *,
                      PerExpandData *,
                      const TemplateCache *) const {
    return true;
  };

  // A no-op for pragma nodes.
  virtual void WriteHeaderEntries(string *outstring,
                                  const string& filename) const { }

  // Appends a representation of the pragma node to a string. We output
  // the full text given in {{%...}} verbatim.
  virtual void DumpToString(int level, string *out) const {
    assert(out);
    AppendTokenWithIndent(level, out, "Pragma Node: -->|", token_, "|<--\n");
  }

 private:
  TemplateToken token_;  // The text of the pragma held by this node.
};

// ----------------------------------------------------------------------
// TemplateTemplateNode
//    Holds a variable to be replaced by an expanded (included)
//    template whose filename is the value of the variable in the
//    dictionary.
//    Also holds the TemplateContext which it passes on to
//      GetTemplateCommon when this included template is initialized.
//    The indentation_ string is used by the PrefixLine modifier so be
//    careful not to perform any operation on it that might invalidate
//    its character array (indentation_.data()).
//
//    In the Auto Escape mode, the PrefixLine modifier is added *after*
//    auto-escape has updated the modifiers that may be present for that
//    template include, but that is ok because PrefixLine does not invalidate
//    their XSS-safety.
// ----------------------------------------------------------------------

class TemplateTemplateNode : public TemplateNode {
 public:
  explicit TemplateTemplateNode(const TemplateToken& token, Strip strip,
                                const string& indentation)
      : token_(token),
        variable_(token_.text, token_.textlen),
        strip_(strip), indentation_(indentation) {
    VLOG(2) << "Constructing TemplateTemplateNode: "
            << string(token_.text, token_.textlen) << endl;

    // If this template is indented (eg, " {{>SUBTPL}}"), make sure
    // every line of the expanded template is indented, not just the
    // first one.  We do this by adding a modifier that applies to
    // the entire template node, that inserts spaces after newlines.
    if (!indentation_.empty()) {
      token_.modvals.push_back(ModifierAndValue(&g_prefix_line_info,
                                                indentation_.data(),
                                                indentation_.length()));
    }
  }
  virtual ~TemplateTemplateNode() {
    VLOG(2) << "Deleting TemplateTemplateNode: "
            << string(token_.text, token_.textlen) << endl;
  }

  // Expands the template node by retrieving the name of a template
  // file from the supplied dictionary, expanding it (using this
  // dictionary if none other is provided in the TemplateDictionary),
  // and then outputting this newly expanded template in place of the
  // original variable.
  // Returns true iff all the template files load and parse correctly.
  virtual bool Expand(ExpandEmitter *output_buffer,
                      const TemplateDictionaryInterface *dictionary,
                      PerExpandData *per_expand_data,
                      const TemplateCache *cache) const;

  virtual void WriteHeaderEntries(string *outstring,
                                  const string& filename) const {
    WriteOneHeaderEntry(outstring, string(token_.text, token_.textlen),
                        filename);
  }

  virtual void DumpToString(int level, string *out) const {
    assert(out);
    AppendTokenWithIndent(level, out, "Template Node: ", token_, "\n");
  }

 private:
  TemplateToken token_;   // text is the name of a template file.
  const HashedTemplateString variable_;
  Strip strip_;       // Flag to pass from parent template to included template.
  const string indentation_;   // Used by ModifierAndValue for g_prefix_line.

  // A helper used for expanding one child dictionary.
  bool ExpandOnce(ExpandEmitter *output_buffer,
                  const TemplateDictionaryInterface &dictionary,
                  const char* const filename,
                  PerExpandData *per_expand_data,
                  const TemplateCache *cache) const;
};

// If no value is found in the dictionary for the template variable
// in this node, then no output is generated in place of this variable.
bool TemplateTemplateNode::Expand(ExpandEmitter *output_buffer,
                                  const TemplateDictionaryInterface *dictionary,
                                  PerExpandData *per_expand_data,
                                  const TemplateCache *cache) const {
  if (dictionary->IsHiddenTemplate(variable_)) {
    // if this "template include" section is "hidden", do nothing
    return true;
  }

  TemplateDictionaryInterface::Iterator* di =
      dictionary->CreateTemplateIterator(variable_);

  if (!di->HasNext()) {  // empty dict means 'expand once using containing dict'
    delete di;
    // TODO(csilvers): have this return a TemplateString instead?
    const char* const filename =
        dictionary->GetIncludeTemplateName(variable_, 0);
    // If the filename wasn't set then treat it as if it were "hidden", i.e, do
    // nothing
    if (filename && *filename) {
      return ExpandOnce(output_buffer, *dictionary, filename, per_expand_data,
                        cache);
    } else {
      return true;
    }
  }

  bool error_free = true;
  for (int dict_num = 0; di->HasNext(); ++dict_num) {
    const TemplateDictionaryInterface& child = di->Next();
    // We do this in the loop, because maybe one day we'll support
    // each expansion having its own template dictionary.  That's also
    // why we pass in the dictionary-index as an argument.
    const char* const filename = dictionary->GetIncludeTemplateName(
        variable_, dict_num);
    // If the filename wasn't set then treat it as if it were "hidden", i.e, do
    // nothing
    if (filename && *filename) {
      error_free &= ExpandOnce(output_buffer, child, filename, per_expand_data,
                               cache);
    }
  }
  delete di;

  return error_free;
}

static void EmitMissingInclude(const char* const filename,
                               ExpandEmitter *output_buffer,
                               PerExpandData *per_expand_data) {
  // if there was a problem retrieving the template, bail!
  if (per_expand_data->annotate()) {
    TemplateAnnotator* annotator = per_expand_data->annotator();
    annotator->EmitFileIsMissing(output_buffer, filename);
  }
  LOG(ERROR) << "Failed to load included template: \"" << filename << "\"\n";
}

bool TemplateTemplateNode::ExpandOnce(
    ExpandEmitter *output_buffer,
    const TemplateDictionaryInterface &dictionary,
    const char* const filename,
    PerExpandData *per_expand_data,
    const TemplateCache *cache) const {
  bool error_free = true;
  // NOTE: Although we do this const_cast here, if the cache is frozen
  // the expansion doesn't mutate the cache, and is effectively 'const'.
  TemplateCache* cache_ptr = const_cast<TemplateCache*>(cache);

  // Expand the included template once for each "template specific"
  // dictionary.  Normally this will only iterate once, but it's
  // possible to supply a list of more than one sub-dictionary and
  // then the template explansion will be iterative, just as though
  // the included template were an iterated section.
  if (per_expand_data->annotate()) {
    per_expand_data->annotator()->EmitOpenInclude(output_buffer,
                                                  token_.ToString());
  }
  // sub-dictionary NULL means 'just use the current dictionary instead'.
  // We force children to annotate the output if we have to.
  // If the include-template has modifiers, we need to expand to a string,
  // modify the string, and append to output_buffer.  Otherwise (common
  // case), we can just expand into the output-buffer directly.
  if (AnyMightModify(token_.modvals, per_expand_data)) {
    string sub_template;
    StringEmitter subtemplate_buffer(&sub_template);
    if (!cache_ptr->ExpandLocked(filename, strip_,
                                 &subtemplate_buffer,
                                 &dictionary,
                                 per_expand_data)) {
      EmitMissingInclude(filename, output_buffer, per_expand_data);
      error_free = false;
    } else {
      EmitModifiedString(token_.modvals,
                         sub_template.data(), sub_template.size(),
                         per_expand_data, output_buffer);
    }
  } else {
    // No need to modify sub-template
    if (!cache_ptr->ExpandLocked(filename, strip_,
                                 output_buffer,
                                 &dictionary,
                                 per_expand_data)) {
      EmitMissingInclude(filename, output_buffer, per_expand_data);
      error_free = false;
    }
  }
  if (per_expand_data->annotate()) {
    per_expand_data->annotator()->EmitCloseInclude(output_buffer);
  }
  return error_free;
}

// ----------------------------------------------------------------------
// SectionTemplateNode
//    Holds the name of a section and a list of subnodes contained
//    in that section.
// ----------------------------------------------------------------------

class SectionTemplateNode : public TemplateNode {
 public:
  SectionTemplateNode(const TemplateToken& token, bool hidden_by_default);
  virtual ~SectionTemplateNode();

  // The highest level parsing method. Reads a single token from the
  // input -- taken from my_template->parse_state_ -- and adds the
  // corresponding type of node to the template's parse
  // tree.  It may add a node of any type, whether text, variable,
  // section, or template to the list of nodes contained in this
  // section.  Returns true iff we really added a node and didn't just
  // end a section or hit a syntax error in the template file.
  // You should hold the g_template_mutex write-lock when calling this
  // (unless you're calling it from a constructor).
  bool AddSubnode(Template *my_template);

  // Expands a section node as follows:
  //   - Checks to see if the section is hidden and if so, does nothing but
  //     return
  //   - Tries to retrieve a list of dictionaries from the supplied dictionary
  //     stored under this section's name
  //   - If it finds a non-empty list of dictionaries, it iterates over the
  //     list and calls itself recursively to expand the section once for
  //     each dictionary
  //   - If there is no dictionary list (or an empty dictionary list somehow)
  //     is found, then the section is expanded once using the supplied
  //     dictionary. (This is the mechanism used to expand each single
  //     iteration of the section as well as to show a non-hidden section,
  //     allowing the section template syntax to be used for both conditional
  //     and iterative text).
  // Returns true iff all the template files load and parse correctly.
  virtual bool Expand(ExpandEmitter *output_buffer,
                      const TemplateDictionaryInterface *dictionary,
                      PerExpandData* per_expand_data,
                      const TemplateCache *cache) const;

  // Writes a header entry for the section name and calls the same
  // method on all the nodes in the section
  virtual void WriteHeaderEntries(string *outstring,
                                  const string& filename) const;

  virtual void DumpToString(int level, string *out) const;

 private:
  const TemplateToken token_;   // text is the name of the section
  const HashedTemplateString variable_;
  NodeList node_list_;  // The list of subnodes in the section
  // A sub-section named "OURNAME_separator" is special.  If we see it
  // when parsing our section, store a pointer to it for ease of use.
  SectionTemplateNode* separator_section_;

  // When the last node read was literal text that ends with "\n? +"
  // (that is, leading whitespace on a line), this stores the leading
  // whitespace.  This is used to properly indent included
  // sub-templates.
  string indentation_;

  // If true, hide sections that have not explicitly had their hidden/visible
  // state set. If false, use the underlying template dictionary's default
  // behavior for hiding.
  // This bool is currently always set to true.
  bool hidden_by_default_;

  // A protected method used in parsing the template file
  // Finds the next token in the file and return it. Anything not inside
  // a template marker is just text. Each template marker type, delimited
  // by "{{" and "}}" (or parser_state_->marker_delimiters.start_marker
  // and .end_marker, more precisely) is a different type of token. The
  // first character inside the opening curly braces indicates the type
  // of the marker, as follows:
  //    # - Start a section
  //    / - End a section
  //    > - A template file variable (the "include" directive)
  //    ! - A template comment
  //    % - A pragma such as AUTOESCAPE
  //    = - Change marker delimiters (from the default of '{{' and '}}')
  //    <alnum or _> - A scalar variable
  // One more thing. Before a name token is returned, if it happens to be
  // any type other than a scalar variable, and if the next character after
  // the closing curly braces is a newline, then the newline is eliminated
  // from the output. This reduces the number of extraneous blank
  // lines in the output. If the template author desires a newline to be
  // retained after a final marker on a line, they must add a space character
  // between the marker and the linefeed character.
  TemplateToken GetNextToken(Template* my_template);

  // Helper routine used by Expand
  virtual bool ExpandOnce(
      ExpandEmitter *output_buffer,
      const TemplateDictionaryInterface *dictionary,
      PerExpandData* per_expand_data,
      bool is_last_child_dict,
      const TemplateCache *cache) const;

  // The specific methods called used by AddSubnode to add the
  // different types of nodes to this section node.
  // Currently only reasons to fail (return false) are if the
  // HTML parser failed to parse in auto-escape mode or the
  // PRAGMA marker was invalid in the template.
  bool AddTextNode(const TemplateToken* token, Template* my_template);
  bool AddVariableNode(TemplateToken* token, Template* my_template);
  bool AddPragmaNode(TemplateToken* token, Template* my_template);
  bool AddTemplateNode(TemplateToken* token, Template* my_template,
                       const string& indentation);
  bool AddSectionNode(const TemplateToken* token, Template* my_template,
                      bool hidden_by_default);
  bool AddSectionNode(const TemplateToken* token, Template* my_template);
};

// --- constructor and destructor, Expand, Dump, and WriteHeaderEntries

SectionTemplateNode::SectionTemplateNode(const TemplateToken& token,
                                         bool hidden_by_default)

    : token_(token),
      variable_(token_.text, token_.textlen),
      separator_section_(NULL), indentation_("\n"),
      hidden_by_default_(hidden_by_default) {
  VLOG(2) << "Constructing SectionTemplateNode: "
          << string(token_.text, token_.textlen) << endl;
}

SectionTemplateNode::~SectionTemplateNode() {
  VLOG(2) << "Deleting SectionTemplateNode: "
          << string(token_.text, token_.textlen) << " and its subnodes"
          << endl;

  // Need to delete the member of the list because the list is a list
  // of pointers to these instances.
  NodeList::iterator iter = node_list_.begin();
  for (; iter != node_list_.end(); ++iter) {
    delete (*iter);
  }
  VLOG(2) << "Finished deleting subnodes of SectionTemplateNode: "
          << string(token_.text, token_.textlen) << endl;
}

bool SectionTemplateNode::ExpandOnce(
    ExpandEmitter *output_buffer,
    const TemplateDictionaryInterface *dictionary,
    PerExpandData *per_expand_data,
    bool is_last_child_dict,
    const TemplateCache* cache) const {
  bool error_free = true;

  if (per_expand_data->annotate()) {
    per_expand_data->annotator()->EmitOpenSection(output_buffer,
                                                  token_.ToString());
  }

  // Expand using the section-specific dictionary.
  // We force children to annotate the output if we have to.
  NodeList::const_iterator iter = node_list_.begin();
  for (; iter != node_list_.end(); ++iter) {
    error_free &=
        (*iter)->Expand(output_buffer, dictionary, per_expand_data, cache);
    // If this sub-node is a "separator section" -- a subsection
    // with the name "OURNAME_separator" -- expand it every time
    // through but the last.
    if (*iter == separator_section_ && !is_last_child_dict) {
      // We call ExpandOnce to make sure we always expand,
      // even if *iter would normally be hidden.
      error_free &= separator_section_->ExpandOnce(output_buffer, dictionary,
                                                   per_expand_data, true,
                                                   cache);
    }
  }

  if (per_expand_data->annotate()) {
    per_expand_data->annotator()->EmitCloseSection(output_buffer);
  }

  return error_free;
}

bool SectionTemplateNode::Expand(
    ExpandEmitter *output_buffer,
    const TemplateDictionaryInterface *dictionary,
    PerExpandData *per_expand_data,
    const TemplateCache *cache) const {
  // The section named __{{MAIN}}__ is special: you always expand it
  // exactly once using the containing (main) dictionary.
  if (token_.text == kMainSectionName) {
    return ExpandOnce(output_buffer, dictionary, per_expand_data, true, cache);
  } else if (hidden_by_default_ ?
             !dictionary->IsUnhiddenSection(variable_) :
             dictionary->IsHiddenSection(variable_)) {
    // Some dictionaries might have sections that can be explicitly hidden
    // and unhidden, so by default both IsHidden() and IsUnhidden() are false,
    // in which case hidden_by_default_ controls the behavior.
    return true;      // if this section is "hidden", do nothing
  }

  TemplateDictionaryInterface::Iterator* di =
      dictionary->CreateSectionIterator(variable_);

  // If there are no child dictionaries, that means we should expand with the
  // current dictionary instead. This corresponds to the situation where
  // template variables within a section are set on the template-wide dictionary
  // instead of adding a dictionary to the section and setting them there.
  if (!di->HasNext()) {
    delete di;
    return ExpandOnce(output_buffer, dictionary, per_expand_data,
                      true, cache);
  }

  // Otherwise, there's at least one child dictionary, and when expanding this
  // section, we should use the child dictionaries instead of the current one.
  bool error_free = true;
  while (di->HasNext()) {
    const TemplateDictionaryInterface& child = di->Next();
    error_free &= ExpandOnce(output_buffer, &child, per_expand_data,
                             !di->HasNext(), cache);
  }
  delete di;
  return error_free;
}

void SectionTemplateNode::WriteHeaderEntries(string *outstring,
                                             const string& filename) const {
  WriteOneHeaderEntry(outstring, string(token_.text, token_.textlen),
                      filename);

  NodeList::const_iterator iter = node_list_.begin();
  for (; iter != node_list_.end(); ++iter) {
    (*iter)->WriteHeaderEntries(outstring, filename);
  }
}

void SectionTemplateNode::DumpToString(int level, string *out) const {
  assert(out);
  AppendTokenWithIndent(level, out, "Section Start: ", token_, "\n");
  NodeList::const_iterator iter = node_list_.begin();
  for (; iter != node_list_.end(); ++iter) {
    (*iter)->DumpToString(level + 1, out);
  }
  AppendTokenWithIndent(level, out, "Section End: ", token_, "\n");
}

// --- AddSubnode and its sub-routines

// Under auto-escape (and parsing-enabled modes) advance the parser state.
// TextTemplateNode is the only TemplateNode type that can change
// the state of the parser.
// Returns false only if the HTML parser failed to parse in
// auto-escape mode.
bool SectionTemplateNode::AddTextNode(const TemplateToken* token,
                                      Template* my_template) {
  assert(token);
  bool success = true;
  HtmlParser *htmlparser = my_template->htmlparser_;

  if (token->textlen > 0) {  // ignore null text sections
    node_list_.push_back(new TextTemplateNode(*token));
    if (AUTO_ESCAPE_PARSING_CONTEXT(my_template->initial_context_)) {
      assert(htmlparser);
      if (htmlparser->state() == HtmlParser::STATE_ERROR ||
          htmlparser->Parse(token->text, static_cast<int>(token->textlen)) ==
          HtmlParser::STATE_ERROR) {
        string error_msg =  "Failed parsing: " +
            string(token->text, token->textlen) +
            "\nIn: " + string(token_.text, token_.textlen);
        LOG_AUTO_ESCAPE_ERROR(error_msg, my_template);
        success = false;
      }
    }
  }
  return success;
}

// In Auto Escape mode, we update the variable modifiers based on what
// modifiers are specified in the template and what Auto-Escape computes
// for that context. Returns false only if the HTML parser failed to parse
// in auto-escape mode.
//
// We also have special logic for BI_SPACE and BI_NEWLINE.
// Even though they look like variables, they're really not: the user
// is expected to use them in situations where they'd normally put
// a space character or a newline character, but can't for technical
// reasons (namely, that the template parser would strip these
// characters because of the STRIP mode it's in).  So unlike other
// variables, we want to treat these variables as literal text.  This
// means that we never add modifiers to them, but we do let the
// htmlparser know about them in order to update its state. Existing
// modifiers will be honored.
//
// Finally, we check if the variable is whitelisted, in which case
// Auto-Escape does not apply escaping to it. See comment for global
// array kSafeWhitelistedVariables[].
bool SectionTemplateNode::AddVariableNode(TemplateToken* token,
                                          Template* my_template) {
  assert(token);
  bool success = true;
  HtmlParser *htmlparser = my_template->htmlparser_;
  TemplateContext initial_context = my_template->initial_context_;

  if (AUTO_ESCAPE_MODE(initial_context)) {
    // Determines modifiers for the variable in auto escape mode.
    string variable_name(token->text, token->textlen);
    // We declare in the documentation that if the user changes the
    // value of these variables, they must only change it to a value
    // that's "equivalent" from the point of view of an html parser.
    // So it's ok to hard-code in that these are " " and "\n",
    // respectively, even though in theory the user could change them
    // (to say, BI_NEWLINE == "\r\n").
    if (variable_name == "BI_SPACE" || variable_name == "BI_NEWLINE") {
      if (AUTO_ESCAPE_PARSING_CONTEXT(initial_context)) {
        assert(htmlparser);
        if (htmlparser->state() == HtmlParser::STATE_ERROR ||
            htmlparser->Parse(variable_name == "BI_SPACE" ? " " : "\n") ==
            HtmlParser::STATE_ERROR)
          success = false;
      }
    } else if (binary_search(Template::kSafeWhitelistedVariables,
                             Template::kSafeWhitelistedVariables +
                             arraysize(Template::kSafeWhitelistedVariables),
                             variable_name.c_str(),
                             // Luckily, StringHash(a, b) is defined as "a < b"
                             StringHash())) {
      // Do not escape the variable, it is whitelisted.
    } else {
      vector<const ModifierAndValue*> modvals =
          GetModifierForContext(initial_context, htmlparser, my_template);
      // There should always be at least one modifier in any Auto-Escape mode.
      if (modvals.empty())
        success = false;
      else
        token->UpdateModifier(modvals);
    }
  }
  node_list_.push_back(new VariableTemplateNode(*token));
  return success;
}

// AddPragmaNode
//   Create a pragma node from the given token and add it
//   to the node list.
//   The AUTOESCAPE pragma is only allowed at the top of a template
//   file (above any non-comment node) to minimize the chance of the
//   HTML parser being out of sync with the template text. So we check
//   that the section is the MAIN section and we are the first node.
//   Note: Since currently we only support one pragma, we apply the check
//   always but when other pragmas are added we'll need to propagate the
//   Pragma identifier from GetNextToken().
bool SectionTemplateNode::AddPragmaNode(TemplateToken* token,
                                        Template* my_template) {
  if (token_.text != kMainSectionName || !node_list_.empty())
    return false;

  node_list_.push_back(new PragmaTemplateNode(*token));
  return true;
}

// AddSectionNode
bool SectionTemplateNode::AddSectionNode(const TemplateToken* token,
                                         Template* my_template,
                                         bool hidden_by_default) {
  assert(token);
  SectionTemplateNode *new_node = new SectionTemplateNode(*token,
                                                          hidden_by_default);

  // Not only create a new section node, but fill it with all *its*
  // subnodes by repeatedly calling AddSubNode until it returns false
  // (indicating either the end of the section or a syntax error)
  while (new_node->AddSubnode(my_template)) {
    // Found a new subnode to add
  }
  node_list_.push_back(new_node);
  // Check the name of new_node.  If it's "OURNAME_separator", store it
  // as a special "separator" section.
  if (token->textlen == token_.textlen + sizeof("_separator")-1 &&
      memcmp(token->text, token_.text, token_.textlen) == 0 &&
      memcmp(token->text + token_.textlen, "_separator", sizeof("_separator")-1)
      == 0)
    separator_section_ = new_node;
  return true;
}

// Note: indentation will be used in constructor of TemplateTemplateNode.
// Note on Auto-Escape: Each template is Auto-Escaped independently of
// the template it may be included from or templates it may include.
// The context does not carry on and hence does not need to be provided
// to the new TemplateNode.
bool SectionTemplateNode::AddTemplateNode(TemplateToken* token,
                                          Template* my_template,
                                          const string& indentation) {
  assert(token);
  bool success = true;
  node_list_.push_back(
      new TemplateTemplateNode(*token, my_template->strip_, indentation));
  return success;
}

// If "text" ends with a newline followed by whitspace, returns a
// string holding that whitespace.  Otherwise, returns the empty
// string.  If implicit_newline is true, also consider the text to be
// an indentation if it consists entirely of whitespace; this is set
// when we know that right before this text there was a newline, or
// this text is the beginning of a document.
static string GetIndentation(const char* text, size_t textlen,
                             bool implicit_newline) {
  const char* nextline;    // points to one char past the last newline
  for (nextline = text + textlen; nextline > text; --nextline)
    if (nextline[-1] == '\n') break;
  if (nextline == text && !implicit_newline)
    return "";                // no newline found, so no indentation

  bool prefix_is_whitespace = true;
  for (const char* p = nextline; p < text + textlen; ++p) {
    if (*p != ' ' && *p != '\t') {
      prefix_is_whitespace = false;
      break;
    }
  }
  if (prefix_is_whitespace && text + textlen > nextline)
    return string(nextline, text + textlen - nextline);
  else
    return "";
}

bool SectionTemplateNode::AddSubnode(Template *my_template) {
  bool auto_escape_success = true;
  // Don't proceed if we already found an error
  if (my_template->state() == TS_ERROR) {
    return false;
  }

  // Stop when the buffer is empty.
  if (my_template->parse_state_.bufstart >= my_template->parse_state_.bufend) {
    // running out of file contents ends the section too
    if (token_.text != kMainSectionName) {
      // if we are not in the main section, we have a syntax error in the file
      LOG_TEMPLATE_NAME(ERROR, my_template);
      LOG(ERROR) << "File ended before all sections were closed" << endl;
      my_template->set_state(TS_ERROR);
    }
    return false;
  }

  TemplateToken token = GetNextToken(my_template);

  switch (token.type) {
    case TOKENTYPE_TEXT:
      auto_escape_success = this->AddTextNode(&token, my_template);
      // Store the indentation (trailing whitespace after a newline), if any.
      this->indentation_ = GetIndentation(token.text, token.textlen,
                                          indentation_ == "\n");
      break;
    case TOKENTYPE_VARIABLE:
      auto_escape_success = this->AddVariableNode(&token, my_template);
      this->indentation_.clear();  // clear whenever last read wasn't whitespace
      break;
    case TOKENTYPE_SECTION_START:
      auto_escape_success = this->AddSectionNode(&token, my_template, false);
      this->indentation_.clear();  // clear whenever last read wasn't whitespace
      break;
    case TOKENTYPE_HIDDEN_DEFAULT_SECTION:
      auto_escape_success = this->AddSectionNode(&token, my_template, true);
      this->indentation_.clear();  // clear whenever last read wasn't whitespace
      break;
    case TOKENTYPE_SECTION_END:
      // Don't add a node. Just make sure we are ending the right section
      // and return false to indicate the section is complete
      if (token.textlen != token_.textlen ||
          memcmp(token.text, token_.text, token.textlen)) {
        LOG_TEMPLATE_NAME(ERROR, my_template);
        LOG(ERROR) << "Found end of different section than the one I am in"
                   << "\nFound: " << string(token.text, token.textlen)
                   << "\nIn: " << string(token_.text, token_.textlen) << endl;
        my_template->set_state(TS_ERROR);
      }
      this->indentation_.clear();  // clear whenever last read wasn't whitespace
      return false;
      break;
    case TOKENTYPE_TEMPLATE:
      auto_escape_success = this->AddTemplateNode(&token, my_template,
                                                  this->indentation_);
      this->indentation_.clear();  // clear whenever last read wasn't whitespace
      break;
    case TOKENTYPE_COMMENT:
      // Do nothing. Comments just drop out of the file altogether.
      break;
    case TOKENTYPE_SET_DELIMITERS:
      if (!Template::ParseDelimiters(
              token.text, token.textlen,
              &my_template->parse_state_.current_delimiters)) {
        LOG_TEMPLATE_NAME(ERROR, my_template);
        LOG(ERROR) << "Invalid delimiter-setting command."
                   << "\nFound: " << string(token.text, token.textlen)
                   << "\nIn: " << string(token_.text, token_.textlen) << endl;
        my_template->set_state(TS_ERROR);
      }
      break;
    case TOKENTYPE_PRAGMA:
      // We can do nothing and simply drop the pragma of the file as is done
      // for comments. But, there is value in keeping it for debug purposes
      // (via DumpToString) so add it as a pragma node.
      if (!this->AddPragmaNode(&token, my_template)) {
        LOG_TEMPLATE_NAME(ERROR, my_template);
        LOG(ERROR) << "Pragma marker must be at the top of the template: '"
                   << string(token.text, token.textlen) << "'" << endl;
        my_template->set_state(TS_ERROR);
      }
      break;
    case TOKENTYPE_NULL:
      // GetNextToken either hit the end of the file or a syntax error
      // in the file. Do nothing more here. Just return false to stop
      // processing.
      return false;
      break;
    default:
      // This shouldn't happen. If it does, it's a programmer error.
      LOG_TEMPLATE_NAME(ERROR, my_template);
      LOG(ERROR) << "Invalid token type returned from GetNextToken" << endl;
  }

  if (!auto_escape_success) {
    // The error is logged where it happens. Here indicate
    // the initialization failed.
    my_template->set_state(TS_ERROR);
    return false;
  }

  // for all the cases where we did not return false
  return true;
}

// --- GetNextToken and its subroutines

// A valid marker name is made up of alphanumerics and underscores...
// nothing else.
static bool IsValidName(const char* name, int namelen) {
  for (const char *cur_char = name; cur_char - name <  namelen; ++cur_char) {
    if (!ascii_isalnum(*cur_char) && *cur_char != '_')
      return false;
  }
  return true;
}

// If we're pointing to the end of a line, and in a high enough strip mode,
// pass over the newline.  If the line ends in a \, we skip over the \ and
// keep the newline.  Returns a pointer to the new 'start' location, which
// is either 'start' or after a newline.
static const char* MaybeEatNewline(const char* start, const char* end,
                                   Strip strip) {
  // first, see if we have the escaped linefeed sequence
  if (end - start >= 2 && start[0] == '\\' && start[1] == '\n') {
    ++start;    // skip over the \, which keeps the \n
  } else if (end - start >= 1 && start[0] == '\n' &&
             strip >= STRIP_WHITESPACE) {
    ++start;    // skip over the \n in high strip_ modes
  }
  return start;
}

// When the parse fails, we take several actions.  msg is a stream
#define FAIL(msg)   do {                                                \
    LOG_TEMPLATE_NAME(ERROR, my_template);                              \
    LOG(ERROR) << msg << endl;                                          \
    my_template->set_state(TS_ERROR);                                   \
    /* make extra-sure we never try to parse anything more */           \
    my_template->parse_state_.bufstart = my_template->parse_state_.bufend; \
    return TemplateToken(TOKENTYPE_NULL, "", 0, NULL);                  \
  } while (0)

// Parses the text of the template file in the input_buffer as
// follows: If the buffer is empty, return the null token.  If getting
// text, search for the next "{{" sequence (more precisely, for
// parse_state_->marker_delimiters.start_marker).  If one is found,
// return all the text collected up to that sequence in a TextToken
// and change the token-parsing phase variable to GETTING_NAME, so the
// next call will know to look for a named marker, instead of more
// text.  If getting a name, read the next character to learn what
// kind of marker it is.  Then collect the characters of the name up
// to the "}}" sequence.  If the "name" is a template comment, then we
// do not return the text of the comment in the token.  If it is any
// other valid type of name, we return the token with the appropriate
// type and the name.  If any syntax errors are discovered (like
// inappropriate characters in a name, not finding the closing curly
// braces, etc.) an error message is logged, the error state of the
// template is set, and a NULL token is returned.  Updates
// parse_state_.  You should hold the g_template_mutex write-lock
// when calling this (unless you're calling it from a constructor).
TemplateToken SectionTemplateNode::GetNextToken(Template *my_template) {
  Template::ParseState* ps = &my_template->parse_state_;   // short abbrev.
  const char* token_start = ps->bufstart;

  if (ps->bufstart >= ps->bufend) {    // at end of buffer
    return TemplateToken(TOKENTYPE_NULL, "", 0, NULL);
  }

  switch (ps->phase) {
    case Template::ParseState::GETTING_TEXT: {
      const char* token_end = memmatch(ps->bufstart, ps->bufend - ps->bufstart,
                                       ps->current_delimiters.start_marker,
                                       ps->current_delimiters.start_marker_len);
      if (!token_end) {
        // Didn't find the start-marker ('{{'), so just grab all the
        // rest of the buffer.
        token_end = ps->bufend;
        ps->bufstart = ps->bufend;   // next token will start at EOF
      } else {
        // If we see code like this: "{{{VAR}}, we want to match the
        // second "{{", not the first.
        while ((token_end + 1 + ps->current_delimiters.start_marker_len
                <= ps->bufend) &&
               memcmp(token_end + 1, ps->current_delimiters.start_marker,
                      ps->current_delimiters.start_marker_len) == 0)
          token_end++;
        ps->phase = Template::ParseState::GETTING_NAME;
        ps->bufstart = token_end + ps->current_delimiters.start_marker_len;
      }
      return TemplateToken(TOKENTYPE_TEXT, token_start,
                           token_end - token_start, NULL);
    }

    case Template::ParseState::GETTING_NAME: {
      TemplateTokenType ttype;
      const char* token_end = NULL;
      // Find out what type of name we are getting
      switch (token_start[0]) {
        case '#':
          ttype = TOKENTYPE_SECTION_START;
          ++token_start;
          break;
        case '/':
          ttype = TOKENTYPE_SECTION_END;
          ++token_start;
          break;
        case '!':
          ttype = TOKENTYPE_COMMENT;
          ++token_start;
          break;
        case '=':
          ttype = TOKENTYPE_SET_DELIMITERS;
          // Keep token_start the same; the token includes the leading '='.
          // But we have to figure token-end specially: it should be "=}}".
          if (ps->bufend > (token_start + 1))
            token_end = (char*)memchr(token_start + 1, '=',
                                      ps->bufend - (token_start + 1));
          if (!token_end ||
              token_end + ps->current_delimiters.end_marker_len > ps->bufend ||
              memcmp(token_end + 1, ps->current_delimiters.end_marker,
                     ps->current_delimiters.end_marker_len) != 0)
            token_end = NULL;   // didn't find it, fall through to code below
          else
            token_end++;        // advance past the "=" to the "}}".
          break;
        case '>':
          ttype = TOKENTYPE_TEMPLATE;
          ++token_start;
          break;
        case '%':
          ttype = TOKENTYPE_PRAGMA;
          ++token_start;
          break;
        default:
          // the assumption that the next char is alnum or _ will be
          // tested below in the call to IsValidName().
          ttype = TOKENTYPE_VARIABLE;
      }

      // Now get the name (or the comment, as the case may be)
      if (!token_end)   // that is, it wasn't set in special-case code above
        token_end = memmatch(token_start, ps->bufend - token_start,
                             ps->current_delimiters.end_marker,
                             ps->current_delimiters.end_marker_len);
      if (!token_end) {   // Didn't find the '}}', so name never ended.  Error!
        FAIL("No ending '" << string(ps->current_delimiters.end_marker,
                                     ps->current_delimiters.end_marker_len)
             << "' when parsing name starting with "
             << "'" << string(token_start, ps->bufend-token_start) << "'");
      }

      if (ttype == TOKENTYPE_PRAGMA) {
        string error_msg;
        const PragmaMarker pragma(token_start, token_end, &error_msg);
        if (!error_msg.empty())
          FAIL(error_msg);
        TemplateContext context = GetTemplateContextFromPragma(pragma);
        if (context == TC_MANUAL)  // TC_MANUAL is used to indicate error.
          FAIL("Invalid context in Pragma directive.");
        const string* parser_state = pragma.GetAttributeValue("state");
        bool in_tag = false;
        if (parser_state != NULL) {
          if (context == TC_HTML && (*parser_state == "IN_TAG" ||
                                     *parser_state == "in_tag"))
            in_tag = true;
          else if (*parser_state != "default")
            FAIL("Unsupported state '" + *parser_state +
                 "'in Pragma directive.");
        }
        // Only an AUTOESCAPE pragma can change the initial_context
        // away from TC_MANUAL and we do not support multiple such pragmas.
        assert(my_template->initial_context_ == TC_MANUAL);
        my_template->initial_context_ = context;
        my_template->MaybeInitHtmlParser(in_tag);
        // ParseState change will happen below.
      }

      // Comments are a special case, since they don't have a name or action.
      // The set-delimiters command is the same way.
      if (ttype == TOKENTYPE_COMMENT || ttype == TOKENTYPE_SET_DELIMITERS ||
          ttype == TOKENTYPE_PRAGMA) {
        ps->phase = Template::ParseState::GETTING_TEXT;
        ps->bufstart = token_end + ps->current_delimiters.end_marker_len;
        // If requested, remove any unescaped linefeed following a comment
        ps->bufstart = MaybeEatNewline(ps->bufstart, ps->bufend,
                                       my_template->strip_);
        // For comments, don't bother returning the text
        if (ttype == TOKENTYPE_COMMENT)
          token_start = token_end;
        return TemplateToken(ttype, token_start, token_end - token_start, NULL);
      }

      // Now we have the name, possibly with following modifiers.
      // Find the modifier-start.
      const char* mod_start = (const char*)memchr(token_start, ':',
                                                  token_end - token_start);
      if (mod_start == NULL)
        mod_start = token_end;

      // Make sure the name is legal.
      if (!IsValidName(token_start, mod_start - token_start)) {
        FAIL("Illegal name in template '"
             << string(token_start, mod_start-token_start) << "'");
      }

      // Figure out what all the modifiers are.  Mods are colon-separated.
      vector<ModifierAndValue> modifiers;
      const char* mod_end;
      for (const char* mod = mod_start; mod < token_end; mod = mod_end) {
        assert(*mod == ':');
        ++mod;   // skip past the starting colon
        mod_end = (const char*)memchr(mod, ':', token_end - mod);
        if (mod_end == NULL)
          mod_end = token_end;
        // Modifiers can be of the form :modname=value.  Extract out value
        const char* value = (const char*)memchr(mod, '=', mod_end - mod);
        if (value == NULL)
          value = mod_end;
        string value_string(value, mod_end - value);
        // Convert the string to a functor, and error out if we can't.
        const ModifierInfo* modstruct = FindModifier(mod, value - mod,
                                                     value, mod_end - value);
        // There are various ways a modifier syntax can be illegal.
        if (modstruct == NULL) {
          FAIL("Unknown modifier for variable "
               << string(token_start, mod_start - token_start) << ": "
               << "'" << string(mod, value - mod) << "'");
        } else if (!modstruct->modval_required && value < mod_end) {
          FAIL("Modifier for variable "
               << string(token_start, mod_start - token_start) << ":"
               << string(mod, value - mod) << " "
               << "has illegal mod-value '" << value_string << "'");
        } else if (modstruct->modval_required && value == mod_end) {
          FAIL("Modifier for variable "
               << string(token_start, mod_start - token_start) << ":"
               << string(mod, value - mod) << " "
               << "is missing a required mod-value");
        }

        // We rely on the fact that the memory pointed to by 'value'
        // remains valid throughout the life of this token since
        // ModifierAndValue does not itself manage its memory.
        modifiers.push_back(
            ModifierAndValue(modstruct, value, mod_end - value));
      }

      // For now, we only allow variable and include nodes to have
      // modifiers.  I think it's better not to have this for
      // sections, but instead to modify all the text and vars in the
      // section appropriately, but I could be convinced otherwise.
      if (!modifiers.empty() &&
          ttype != TOKENTYPE_VARIABLE && ttype != TOKENTYPE_TEMPLATE) {
        FAIL(string(token_start, token_end - token_start)
             << "malformed: only variables and template-includes "
             << "are allowed to have modifiers");
      }

      // Whew!  We passed the gauntlet.  Get ready for the next token
      ps->phase = Template::ParseState::GETTING_TEXT;
      ps->bufstart = token_end + ps->current_delimiters.end_marker_len;
      // If requested, remove any linefeed following a comment,
      // or section start or end, or template marker, unless
      // it is escaped by '\'
      if (ttype != TOKENTYPE_VARIABLE) {
        ps->bufstart = MaybeEatNewline(ps->bufstart, ps->bufend,
                                       my_template->strip_);
      }

      // create and return the TEXT token that we found
      return TemplateToken(ttype, token_start, mod_start - token_start,
                           &modifiers);
    }

    default: {
      FAIL("Programming error: Unexpected parse phase while "
           << "parsing template: " << ps->phase);
    }
  }
}

// ----------------------------------------------------------------------
// CreateTemplateCache()
// default_template_cache()
// mutable_default_template_cache()
//    These create the default TemplateCache object, that Template
//    often just delegates (deprecated) operations to.
// ----------------------------------------------------------------------

static TemplateCache* g_default_template_cache = NULL;
GoogleOnceType g_default_cache_init_once = GOOGLE_ONCE_INIT;

static void CreateTemplateCache() {
  g_default_template_cache = new TemplateCache();
}

const TemplateCache* default_template_cache() {
  GoogleOnceInit(&g_default_cache_init_once, &CreateTemplateCache);
  return g_default_template_cache;
}

TemplateCache* mutable_default_template_cache() {
  GoogleOnceInit(&g_default_cache_init_once, &CreateTemplateCache);
  return g_default_template_cache;
}

// ----------------------------------------------------------------------
// Template::StringToTemplate()
//    StringToTemplate reads a string representing a template (eg
//    "Hello {{WORLD}}"), and parses it to a Template*.  It returns
//    the parsed template, or NULL if there was a parsing error.
//    StringToTemplateCache does the same, but then inserts the
//    resulting Template* into the template cache, for future retrieval
//    via GetTemplate.  You pass in the key to use with GetTemplate.
//    It returns a bool indicating success or failure of template
//    creation/insertion.  (Insertion will fail if a string or file
//    with that key already exists in the cache.)
//       RemoveStringFromTemplateCache() lets you remove a string that
//    you had previously interned via StringToTemplateCache().
// ----------------------------------------------------------------------

Template* Template::StringToTemplate(const TemplateString& content,
                                     Strip strip) {
  // An empty original_filename_ keeps ReloadIfChangedLocked from performing
  // file operations.

  Template *tpl = new Template("", strip, NULL);

  // But we have to do the "loading" and parsing ourselves:

  // BuildTree deletes the buffer when done, so we need a copy for it.
  char* buffer = new char[content.size()];
  size_t content_len = content.size();
  memcpy(buffer, content.data(), content_len);
  tpl->StripBuffer(&buffer, &content_len);
  if ( tpl->BuildTree(buffer, buffer + content_len) ) {
    assert(tpl->state() == TS_READY);
  } else {
    assert(tpl->state() != TS_READY);
    delete tpl;
    return NULL;
  }
  return tpl;
}

// ----------------------------------------------------------------------
// Template::Template()
// Template::~Template()
// Template::MaybeInitHtmlParser()
//   Calls ReloadIfChanged to load the template the first time.
//   The constructor is private; GetTemplate() is the factory
//   method used to actually construct a new template if needed.
//   GetTemplateCommon() first looks in the two caches -- the
//   cache of parsed template trees, and the cache of raw
//   template-file contents -- before trying to load the
//   template-file from disk.
// ----------------------------------------------------------------------

Template::Template(const TemplateString& filename, Strip strip,
                   TemplateCache* owner)
    // TODO(csilvers): replace ToString() with an is_immutable() check
    : original_filename_(filename.data(), filename.size()), resolved_filename_(),
      filename_mtime_(0), strip_(strip), state_(TS_EMPTY),
      template_cache_(owner), template_text_(NULL), template_text_len_(0),
      tree_(NULL), parse_state_(),
      initial_context_(TC_MANUAL), htmlparser_(NULL) {
  VLOG(2) << "Constructing Template for " << template_file()
          << "; with context " << initial_context_
          << "; and strip " << strip_ << endl;

  // Preserve whitespace in Javascript files because carriage returns
  // can convey meaning for comment termination and closures
  if (strsuffix(original_filename_.c_str(), ".js") &&
      strip_ == STRIP_WHITESPACE) {
    strip_ = STRIP_BLANK_LINES;
  }
  ReloadIfChangedLocked();
}

Template::~Template() {
  VLOG(2) << endl << "Deleting Template for " << template_file()
          << "; with context " << initial_context_
          << "; and strip " << strip_ << endl;
  // Since this is only used by tests, we don't bother with locking
  num_deletes_++;
  delete tree_;
  // Delete this last, since tree has pointers into template_text_
  delete[] template_text_;
  delete htmlparser_;
}

// In TemplateContexts where the HTML parser is needed, we initialize it in
// the appropriate mode. Also we do a sanity check (cannot fail) on the
// template filename. This function is invoked when an AUTOESCAPE pragma is
// found during template parsing and should at most be called once per template.
//
// In_tag is only meaningful for TC_HTML: It is true for templates that
// start inside an HTML tag and hence are expected to contain HTML attribute
// name/value pairs only. It is false for standard HTML templates.
void Template::MaybeInitHtmlParser(bool in_tag) {
  assert(!htmlparser_);
  if (AUTO_ESCAPE_PARSING_CONTEXT(initial_context_)) {
    htmlparser_ = new HtmlParser();
    switch (initial_context_) {
      case TC_JS:
        htmlparser_->ResetMode(HtmlParser::MODE_JS);
        break;
      case TC_CSS:
        htmlparser_->ResetMode(HtmlParser::MODE_CSS);
        break;
      default:
        if (in_tag)
          htmlparser_->ResetMode(HtmlParser::MODE_HTML_IN_TAG);
        break;
    }
    FilenameValidForContext(original_filename_, initial_context_);
  }
}

// ----------------------------------------------------------------------
// Template::BuildTree()
// Template::WriteHeaderEntry()
// Template::Dump()
//    These kick off their various parsers -- BuildTree for the
//    main task of parsing a Template when it's read from memory,
//    WriteHeaderEntry for parsing for make_tpl_varnames_h, and
//    Dump() for when Dump() is called by the caller.
// ----------------------------------------------------------------------

// NOTE: BuildTree takes over ownership of input_buffer, and will delete it.
//       It should have been created via new[].
// You should hold a write-lock on g_template_mutex before calling this
// (unless you're calling it from a constructor).
// In auto-escape mode, the HTML context is tracked as the tree is being
// built, in a single pass. When this function completes, all variables
// will have the proper modifiers set.
bool Template::BuildTree(const char* input_buffer,
                         const char* input_buffer_end) {
  set_state(TS_EMPTY);
  parse_state_.bufstart = input_buffer;
  parse_state_.bufend = input_buffer_end;
  parse_state_.phase = ParseState::GETTING_TEXT;
  parse_state_.current_delimiters = Template::MarkerDelimiters();
  // Assign an arbitrary name to the top-level node
  SectionTemplateNode *top_node = new SectionTemplateNode(
      TemplateToken(TOKENTYPE_SECTION_START,
                    kMainSectionName, strlen(kMainSectionName), NULL),
      false);
  while (top_node->AddSubnode(this)) {
    // Add the rest of the template in.
  }

  // get rid of the old tree, whenever we try to build a new one.
  delete tree_;
  delete[] template_text_;
  tree_ = top_node;
  template_text_ = input_buffer;
  template_text_len_ = input_buffer_end - input_buffer;

  // TS_ERROR can also be set by the auto-escape mode, at the point
  // where the parser failed to parse.
  if (state() != TS_ERROR) {
    set_state(TS_READY);
    return true;
  } else {
    delete tree_;
    tree_ = NULL;
    delete[] template_text_;
    template_text_ = NULL;
    template_text_len_ = 0;
    return false;
  }
}

void Template::WriteHeaderEntries(string *outstring) const {
  if (state() == TS_READY) {   // only write header entries for 'good' tpls
    outstring->append("#include <ctemplate/template_string.h>\n");
    tree_->WriteHeaderEntries(outstring, template_file());
  }
}

// Dumps the parsed structure of the template for debugging assistance.
// It goes to stdout instead of LOG to avoid possible truncation due to size.
void Template::Dump(const char *filename) const {
  string out;
  DumpToString(filename, &out);
  fwrite(out.data(), 1, out.length(), stdout);
  fflush(stdout);
}

void Template::DumpToString(const char *filename, string *out) const {
  if (!out)
    return;
  out->append("------------Start Template Dump [" + string(filename) +
              "]--------------\n");
  if (tree_) {
    tree_->DumpToString(1, out);
  } else {
    out->append("No parse tree has been produced for this template\n");
  }
  out->append("------------End Template Dump----------------\n");
}

// -------------------------------------------------------------------------
// Template::state()
// Template::set_state()
// Template::template_file()
// Template::original_filename()
// Template::strip()
// Template::mtime()
//    Various introspection methods.  state() is the parse-state
//    (success, error).  template_file() is the resolved filename of a
//    given template object's input. original_filename() is the unresolved,
//    original filename, strip() is the Strip type. mtime() is
//    the lastmod time. For string-based templates, not backed by a file,
//    mtime() returns 0.
// -------------------------------------------------------------------------

void Template::set_state(TemplateState new_state) {
  state_ = new_state;
}

TemplateState Template::state() const {
  return state_;
}

const char *Template::template_file() const {
  return resolved_filename_.c_str();
}

const char *Template::original_filename() const {
  return original_filename_.c_str();
}

Strip Template::strip() const {
  return strip_;
}

time_t Template::mtime() const {
  return filename_mtime_;
}

// ----------------------------------------------------------------------
// Template::GetTemplate()
// Template::StringToTemplateCache()
// Template::SetTemplateRootDirectory()
// Template::AddAlternateTemplateRootDirectory()
// Template::template_root_directory()
// Template::FindTemplateFilename()
// Template::RemoveStringFromTemplateCache()
// Template::ClearCache()
// Template::ReloadAllIfChanged()
//    These are deprecated static methods that have been moved to
//    template_cache.h.  We just forward to them, using the global
//    default template cache.
// ----------------------------------------------------------------------

Template *Template::GetTemplate(const TemplateString& filename, Strip strip) {
  // Until I've resolved the TODO that lets me return a const Template*
  // here, I have to do an ugly cast. :-(
  return const_cast<Template*>(
      mutable_default_template_cache()->GetTemplate(filename, strip));
}

// This method is deprecated (and slow).  Instead, use the above
// StringToTemplateCache method that takes a Strip argument.
bool Template::StringToTemplateCache(const TemplateString& key,
                                     const TemplateString& content) {
  // We say the insert succeeded only if it succeded for all strip values.
  bool retval = true;
  for (int i = 0; i < static_cast<int>(NUM_STRIPS); ++i) {
    if (!ctemplate::StringToTemplateCache(key, content, static_cast<Strip>(i)))
      retval = false;
  }
  return retval;
}

// ----------------------------------------------------------------------
// Template::ParseDelimiters()
//    Given an input that looks like =XXX YYY=, set the
//    MarkerDelimiters to point to XXX and YYY.  This is used to parse
//    {{=XXX YYY=}} markers, which reset the marker delimiters.
//    Returns true if successfully parsed (starts and ends with =,
//    exactly one space, no internal ='s), false else.
// ----------------------------------------------------------------------

bool Template::ParseDelimiters(const char* text, size_t textlen,
                               MarkerDelimiters* delim) {
  const char* space = (const char*)memchr(text, ' ', textlen);
  if (textlen < 3 ||
      text[0] != '=' || text[textlen - 1] != '=' ||        // no = at ends
      memchr(text + 1, '=', textlen - 2) ||                // = in the middle
      !space ||                                            // no interior space
      memchr(space + 1, ' ', text + textlen - (space+1)))  // too many spaces
    return false;

  delim->start_marker = text + 1;
  delim->start_marker_len = space - delim->start_marker;
  delim->end_marker = space + 1;
  delim->end_marker_len = text + textlen - 1 - delim->end_marker;
  return true;
}

// ----------------------------------------------------------------------
// StripTemplateWhiteSpace()
// Template::IsBlankOrOnlyHasOneRemovableMarker()
// Template::InsertLine()
// Template::StripBuffer()
//    This mini-parser modifies an input buffer, replacing it with a
//    new buffer that is the same as the old, but with whitespace
//    removed as is consistent with the given strip-mode:
//    STRIP_WHITESPACE, STRIP_BLANK_LINES, DO_NOT_STRIP (the last
//    of these is a no-op).  This parser may work by allocating
//    a new buffer and deleting the input buffer when it's done).
//    The trickiest bit if in STRIP_BLANK_LINES mode, if we see
//    a line that consits entirely of one "removable" marker on it,
//    and nothing else other than whitespace.  ("Removable" markers
//    are comments, start sections, end sections, pragmas and
//    template-include.)  In such a case, we elide the newline at
//    the end of that line.
// ----------------------------------------------------------------------

// We define our own version rather than using the one in strutil, mostly
// so we can take a size_t instead of an int.  The code is simple enough.
static void StripTemplateWhiteSpace(const char** str, size_t* len) {
  // Strip off trailing whitespace.
  while ((*len) > 0 && ascii_isspace((*str)[(*len)-1])) {
    (*len)--;
  }

  // Strip off leading whitespace.
  while ((*len) > 0 && ascii_isspace((*str)[0])) {
    (*len)--;
    (*str)++;
  }
}

// Adjusts line and length iff condition is met, and RETURNS true.
// MarkerDelimiters are {{ and }}, or equivalent.
bool Template::IsBlankOrOnlyHasOneRemovableMarker(
    const char** line, size_t* len, const Template::MarkerDelimiters& delim) {
  const char *clean_line = *line;
  size_t new_len = *len;
  StripTemplateWhiteSpace(&clean_line, &new_len);

  // If there was only white space on the line, new_len will now be zero.
  // In that case the line should be removed, so return true.
  if (new_len == 0) {
    *line = clean_line;
    *len = new_len;
    return true;
  }

  // The smallest removable marker is at least start_marker_len +
  // end_marker_len + 1 characters long.  If there aren't enough
  // characters, then keep the line by returning false.
  if (new_len < delim.start_marker_len + delim.end_marker_len + 1) {
    return false;
  }

  // Only {{#...}}, {{/....}, {{>...}, {{!...}, {{%...}} and {{=...=}}
  // are "removable"
  if (memcmp(clean_line, delim.start_marker, delim.start_marker_len) != 0 ||
      !strchr("#/>!%=", clean_line[delim.start_marker_len])) {
    return false;
  }

  const char *found_end_marker = memmatch(clean_line + delim.start_marker_len,
                                          new_len - delim.start_marker_len,
                                          delim.end_marker,
                                          delim.end_marker_len);

  // Make sure the end marker comes at the end of the line.
  if (!found_end_marker ||
      found_end_marker + delim.end_marker_len != clean_line + new_len) {
    return false;
  }

  // else return the line stripped of its white space chars so when the
  // marker is removed in expansion, no white space is left from the line
  // that has now been removed
  *line = clean_line;
  *len = new_len;
  return true;
}

size_t Template::InsertLine(const char *line, size_t len, Strip strip,
                            const MarkerDelimiters& delim, char* buffer) {
  bool add_newline = (len > 0 && line[len-1] == '\n');
  if (add_newline)
    len--;                 // so we ignore the newline from now on

  if (strip >= STRIP_WHITESPACE) {
    StripTemplateWhiteSpace(&line, &len);
    add_newline = false;

    // IsBlankOrOnlyHasOneRemovableMarker may modify the two input
    // parameters if the line contains only spaces or only one input
    // marker.  This modification must be done before the line is
    // written to the input buffer. Hence the need for the boolean flag
    // add_newline to be referenced after the Write statement.
  } else if (strip >= STRIP_BLANK_LINES
             && IsBlankOrOnlyHasOneRemovableMarker(&line, &len, delim)) {
    add_newline = false;
  }

  memcpy(buffer, line, len);

  if (add_newline) {
    buffer[len++] = '\n';
  }
  return len;
}

void Template::StripBuffer(char **buffer, size_t* len) {
  if (strip_ == DO_NOT_STRIP)
    return;

  char* bufend = *buffer + *len;
  char* retval = new char[*len];
  char* write_pos = retval;

  MarkerDelimiters delim;

  const char* next_pos = NULL;
  for (const char* prev_pos = *buffer; prev_pos < bufend; prev_pos = next_pos) {
    next_pos = (char*)memchr(prev_pos, '\n', bufend - prev_pos);
    if (next_pos)
      next_pos++;          // include the newline
    else
      next_pos = bufend;   // for the last line, when it has no newline

    write_pos += InsertLine(prev_pos, next_pos - prev_pos, strip_, delim,
                            write_pos);
    assert(write_pos >= retval &&
           static_cast<size_t>(write_pos-retval) <= *len);

    // Before looking at the next line, see if the current line
    // changed the marker-delimiter.  We care for
    // IsBlankOrOnlyHasOneRemovableMarker, so we don't need to be
    // perfect -- we don't have to handle the delimiter changing in
    // the middle of a line -- just make sure that the next time
    // there's only one marker on a line, we notice because we know
    // the right delim.
    const char* end_marker = NULL;
    for (const char* marker = prev_pos; marker; marker = end_marker) {
      marker = memmatch(marker, next_pos - marker,
                        delim.start_marker, delim.start_marker_len);
      if (!marker)  break;
      end_marker = memmatch(marker + delim.start_marker_len,
                            next_pos - (marker + delim.start_marker_len),
                            delim.end_marker, delim.end_marker_len);
      if (!end_marker)  break;
      end_marker += delim.end_marker_len;  // needed for the for loop
      // This tries to parse the marker as a set-delimiters marker.
      // If it succeeds, it updates delim. If not, it ignores it.
      assert(((end_marker - delim.end_marker_len)
              - (marker + delim.start_marker_len)) >= 0);
      Template::ParseDelimiters(marker + delim.start_marker_len,
                                ((end_marker - delim.end_marker_len)
                                 - (marker + delim.start_marker_len)),
                                &delim);
    }
  }
  assert(write_pos >= retval);

  // Replace the input retval with our new retval.
  delete[] *buffer;
  *buffer = retval;
  *len = static_cast<size_t>(write_pos - retval);
}

// ----------------------------------------------------------------------
// Template::ReloadIfChanged()
// Template::ReloadIfChangedLocked()
//    If one template, try immediately to reload it from disk.  If all
//    templates, just set all their reload statuses to true, so next time
//    GetTemplate() is called on the template, it will be reloaded from disk if
//    the disk version is newer than the one currently in memory.
//    ReloadIfChanged() returns true if the file changed and disk *and* we
//    successfully reloaded and parsed it.  It never returns true if
//    original_filename_ is "".
// ----------------------------------------------------------------------

// Besides being called when locked, it's also ok to call this from
// the constructor, when you know nobody else will be messing with
// this object.
bool Template::ReloadIfChangedLocked()
    EXCLUSIVE_LOCKS_REQUIRED(g_template_mutex) {
  // TODO(panicker): Remove this duplicate code when constructing the template,
  // after deprecating this method.
  // TemplateCache::GetTemplate() already checks if the template filename is
  // valid and resolvable. It also checks if the file needs to be reloaded
  // based on mtime.

  // NOTE(panicker): we should not be using original_filename_ to determine
  // if a template is string-based, instead use the boolean 'string_based'
  // in the template cache.
  if (original_filename_.empty()) {
    // string-based templates don't reload
    return false;
  }

  FileStat statbuf;
  if (resolved_filename_.empty()) {
    if (!template_cache_->ResolveTemplateFilename(original_filename_,
                                                  &resolved_filename_,
                                                  &statbuf)) {
      LOG(WARNING) << "Unable to locate file " << original_filename_ << endl;
      set_state(TS_ERROR);
      return false;
    }
  } else {
    if (!File::Stat(resolved_filename_, &statbuf)) {
      LOG(WARNING) << "Unable to stat file " << resolved_filename_ << endl;
      // We keep the old tree if there is one, otherwise we're in error
      set_state(TS_ERROR);
      return false;
    }
  }

  if (statbuf.IsDirectory()) {
    LOG(WARNING) << resolved_filename_
                 << "is a directory and thus not readable" << endl;
    // We keep the old tree if there is one, otherwise we're in error
    set_state(TS_ERROR);
    return false;
  }
  if (statbuf.mtime == filename_mtime_ && filename_mtime_ > 0
      && tree_) {   // force a reload if we don't already have a tree_
    VLOG(1) << "Not reloading file " << resolved_filename_
            << ": no new mod-time" << endl;
    set_state(TS_READY);
    return false;   // file's timestamp hasn't changed, so no need to reload
  }

  File* fp = File::Open(resolved_filename_.c_str(), "r");
  if (fp == NULL) {
    LOG(ERROR) << "Can't find file " << resolved_filename_
               << "; skipping" << endl;
    // We keep the old tree if there is one, otherwise we're in error
    set_state(TS_ERROR);
    return false;
  }
  size_t buflen = statbuf.length;
  char* file_buffer = new char[buflen];
  if (fp->Read(file_buffer, buflen) != buflen) {
    LOG(ERROR) << "Error reading file " << resolved_filename_
               << ": " << strerror(errno) << endl;
    fp->Close();
    delete[] file_buffer;
    // We could just keep the old tree, but probably safer to say 'error'
    set_state(TS_ERROR);
    return false;
  }
  fp->Close();

  // Now that we know we've read the file ok, mark the new mtime
  filename_mtime_ = statbuf.mtime;

  // Parse the input one line at a time to get the "stripped" input.
  StripBuffer(&file_buffer, &buflen);

  // Re-initialize Auto-Escape data. Delete the parser and reset the template
  // context back to TC_MANUAL. If the new content has the AUTOESCAPE pragma,
  // the parser will then be re-created.
  initial_context_ = TC_MANUAL;
  delete htmlparser_;
  htmlparser_ = NULL;

  // Now parse the template we just read.  BuildTree takes over ownership
  // of input_buffer in every case, and will eventually delete it.
  if ( BuildTree(file_buffer, file_buffer + buflen) ) {
    assert(state() == TS_READY);
    return true;
  } else {
    assert(state() != TS_READY);
    return false;
  }
}

// ----------------------------------------------------------------------
// Template::ExpandLocked()
// Template::ExpandWithDataAndCache()
//    This is the main function clients call: it expands a template
//    by expanding its parse tree (which starts with a top-level
//    section node).  For each variable/section/include-template it
//    sees, it replaces the name stored in the parse-tree with the
//    appropriate value from the passed-in dictionary.
// ----------------------------------------------------------------------

bool Template::ExpandLocked(ExpandEmitter *expand_emitter,
                            const TemplateDictionaryInterface *dict,
                            PerExpandData *per_expand_data,
                            const TemplateCache *cache) const
    SHARED_LOCKS_REQUIRED(g_template_mutex) {
  // Accumulator for the results of Expand for each sub-tree.
  bool error_free = true;

  // TODO(csilvers): could make this static if it's expensive to construct.
  PerExpandData empty_per_expand_data;
  if (per_expand_data == NULL)
    per_expand_data = &empty_per_expand_data;

  if (state() != TS_READY) {
    // We'd like to reload if reload status is true, but ExpandWD() is const
    return false;
  }

  if (per_expand_data->annotate()) {
    // Remove the machine dependent prefix from the template file name.
    const char* file = template_file();
    const char* short_file = strstr(file, per_expand_data->annotate_path());
    if (short_file != NULL) {
      file = short_file;
    }
    per_expand_data->annotator()->EmitOpenFile(expand_emitter,
                                               string(file));
  }

  // If the client registered an expand-modifier, which is a modifier
  // meant to modify all templates after they are expanded, apply it
  // now.
  const TemplateModifier* modifier =
      per_expand_data->template_expansion_modifier();
  if (modifier && modifier->MightModify(per_expand_data, template_file())) {
    // We found a expand TemplateModifier.  Apply it.
    //
    // Since the expand-modifier doesn't ever have an arg (it doesn't
    // have a name and can't be applied in the text of a template), we
    // pass the template name in as the string arg in this case.
    string value;
    StringEmitter tmp_emitter(&value);
    error_free &= tree_->Expand(&tmp_emitter, dict, per_expand_data, cache);
    modifier->Modify(value.data(), value.size(), per_expand_data,
                     expand_emitter, template_file());
  } else {
    // No need to modify this template.
    error_free &= tree_->Expand(expand_emitter, dict, per_expand_data, cache);
  }

  if (per_expand_data->annotate()) {
    per_expand_data->annotator()->EmitCloseFile(expand_emitter);
  }

  return error_free;
}

bool Template::ExpandWithDataAndCache(
    ExpandEmitter *expand_emitter,
    const TemplateDictionaryInterface *dict,
    PerExpandData *per_expand_data,
    const TemplateCache *cache) const LOCKS_EXCLUDED(g_template_mutex) {
  // We hold g_template_mutex the entire time we expand, because
  // ReloadIfChanged(), which also holds template_mutex, is allowed to
  // delete tree_, and we want to make sure it doesn't do that (in another
  // thread) while we're expanding.  We also protect state_, etc.
  // Note we only need a read-lock here, so many expands can go on at once.
  // TODO(csilvers): We can remove this once we delete ReloadIfChanged.
  //                 When we do that, ExpandLocked() can go away as well.
  ReaderMutexLock ml(&g_template_mutex);
  return ExpandLocked(expand_emitter, dict, per_expand_data, cache);
}

}
