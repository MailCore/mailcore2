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
//
// ---
// Author: csilvers@google.com (Craig Silverstein)
//
// These are used by template.cc and when registering new modifiers.
// (Or more exactly, registering new modifier/value pairs.)
// They are not intended for any other users.
//
// If you do find yourself needing to use them directly, please email
// template-users.
//
// Known outside-template users of this class:
//    template/bidi/bidi_modifiers_test.cc
//
// These routines are implemented in template_modifiers.cc.

#ifndef TEMPLATE_TEMPLATE_MODIFIERS_INTERNAL_H_
#define TEMPLATE_TEMPLATE_MODIFIERS_INTERNAL_H_

#include <config.h>
#include <sys/types.h>   // for size_t
#include <string.h>      // for strchr
#include <string>
#include <vector>
#include <ctemplate/template_modifiers.h>   // for null_modifier

// Annoying stuff for windows -- make sure clients (in this case
// unittests) can import the class definitions and variables.
#ifndef CTEMPLATE_DLL_DECL
# ifdef _MSC_VER
#   define CTEMPLATE_DLL_DECL  __declspec(dllimport)
# else
#   define CTEMPLATE_DLL_DECL  /* should be the empty string for non-windows */
# endif
#endif

using std::string;
using std::vector;

namespace ctemplate_htmlparser {
class HtmlParser;
}

namespace ctemplate {

class TemplateModifier;

// A Modifier belongs to an XssClass which determines whether
// it is an XSS safe addition to a modifier chain or not. This
// is used by the Auto-Escape mode when determining how to handle
// extra modifiers provided in template. For example, :j is a safe
// addition to :h because they are both in the same class (XSS_WEB_STANDARD).
//
// XssClass is not exposed in any API and cannot be set in custom
// modifiers, it is for internal use only (for Auto-Escape). We currently
// have only three classes.
//
// XSS_UNUSED: not used.
// XSS_WEB_STANDARD: All the curent built-in escaping modifiers.
// XSS_UNIQUE: Set for all custom modifiers added via AddModifier()
//             and may need to be escaped.
// XSS_SAFE: Set for all custom modifiers added via AddXssSafeModifier()
//           that are considered to produce safe output and hence
//           do not need further escaping. Also includes the :none modifier.
enum XssClass {
  XSS_UNUSED,
  XSS_WEB_STANDARD,
  XSS_UNIQUE,
  XSS_SAFE,
};

// TODO(csilvers): collapse this into the TemplateModifier class?
struct ModifierInfo {
  // longname should end in an '=' iff the modifier takes a value
  //   (same as in getopt(3)).
  // To specialize -- add a modifier that applies only when we see the name
  //   with a particular value -- specify longname like so: "longname=value".
  //   (See example in the comment-doc below, for AddModifier.)
  // sn can be '\0' if there is no associated shortname.
  // m should be NULL *only if* default-registering a user-defined longname
  //   that the user neglected to register themselves.  In this case, we
  //   use the null modifier as the actual modifier.
  // xss_class indicates an equivalence class this modifier is
  // in, such that any other modifier in the class could be applied
  // after this modifier without affecting its XSS-safety.  If in
  // doubt, say XSS_UNIQUE, which is the most conservative choice.
  ModifierInfo(string ln, char sn, XssClass xc, const TemplateModifier* m)
      : long_name(ln), short_name(sn),
        modval_required(strchr(ln.c_str(), '=') != NULL),
        is_registered(m != NULL), xss_class(xc),
        modifier(m ? m : &null_modifier) { }
  string long_name;
  char short_name;
  bool modval_required;           // true iff ln has an '=' in it
  bool is_registered;             // true for built-in and AddModifier mods
  XssClass xss_class;
  const TemplateModifier* modifier;
};

// An escaping directive is completely defined by the escaping function to use
// (ModifierInfo.modifier) as well as the optional value it may require. This
// structure is a small wrapper on ModifierInfo to convey that needed value.
// Note: The given value pointer must be valid for the life of the struct.
// Also, value is not null-terminated.
struct ModifierAndValue {
  ModifierAndValue(const ModifierInfo* mod_info, const char* val,
                   size_t val_len)
      : modifier_info(mod_info), value(val), value_len(val_len) { }
  const ModifierInfo* modifier_info;
  const char* value;
  size_t value_len;
};

// Returns whether or not candidate can be safely (w.r.t XSS)
// used in lieu of our ModifierInfo. This is true iff:
//   1. Both have the same modifier function OR
//   2. Candidate's modifier function is in our ModifierInfo's
//      list (vector) of safe alternative modifier functions.
// Note that this function is not commutative therefore
// IsSafeXSSAlternative(a, b) may not be equal to IsSafeXSSAlternative(b, a).
extern CTEMPLATE_DLL_DECL
bool IsSafeXSSAlternative(const ModifierInfo& our,
                          const ModifierInfo& candidate);

// modname is the name of the modifier (shortname or longname).
// value is the modifier-value (empty string if there is no modval).
// Returns a pointer into g_modifiers, or NULL if not found.
extern CTEMPLATE_DLL_DECL
const ModifierInfo* FindModifier(const char* modname, size_t modname_len,
                                 const char* modval, size_t modval_len);


// Convenience function to dump the (zero or more) modifiers (and values)
// in the format:
// :<modifier1>[=<val1>]<seperator>[:<modifier2>][=<val2>]...
// If the modifier does not have a short_name, we print its long_name instead.
// The separator may be an empty string.
extern CTEMPLATE_DLL_DECL
string PrettyPrintModifiers(
    const vector<const ModifierAndValue*>& modvals,
    const string& separator);

extern CTEMPLATE_DLL_DECL
string PrettyPrintOneModifier(const ModifierAndValue& modval);

// Returns the appropriate escaping directives to escape content in an
// HTML or Javascript context. HTML and Javascript contexts exercise
// the same parser APIs and hence are combined here.
// If an error occurs, we return NULL and append and error to error_msg.
// The htmlparser and error_msg arguments must be non-NULL.
// Currently, on success, we always return a vector of length 1, meaning
// we never need to chain escaping directives. However, this is subject
// to change.
extern CTEMPLATE_DLL_DECL
vector<const ModifierAndValue*> GetModifierForHtmlJs(
    ctemplate_htmlparser::HtmlParser* htmlparser, string* error_msg);

// Returns the appropriate escaping directives to escape content
// in a CSS context.
// Currently always returns cleanse_css and hence does not require the
// parser nor can it fail. This will change once the parser is able to
// distinguish between different CSS contexts, in particular CSS properties
// that take URLs, which require a different escaping function (non-existent).
extern CTEMPLATE_DLL_DECL
vector<const ModifierAndValue*> GetModifierForCss(
    ctemplate_htmlparser::HtmlParser* htmlparser, string* error_msg);

// Returns the appropriate escaping directives to escape content
// in an XML context.
// Currently always returns xml_escape and hence does not require the
// parser nor can it fail. This may change once the parser can parse XML.
extern CTEMPLATE_DLL_DECL
vector<const ModifierAndValue*> GetModifierForXml(
    ctemplate_htmlparser::HtmlParser* htmlparser, string* error_msg);

// Returns the appropriate escaping directives to escape content
// in a JSON context.
// Currently always returns javascript_escape and hence does not require the
// parser nor can it fail. This may change once the parser can parse
// and distinguish different contexts within JSON.
extern CTEMPLATE_DLL_DECL
vector<const ModifierAndValue*> GetModifierForJson(
    ctemplate_htmlparser::HtmlParser* htmlparser, string* error_msg);

// Return the default escaping directives to escape content for the given
// context. These methods are useful when the caller does not have
// access to a parser or when the parsed failed to parse.

// GetDefaultModifierForHtml
// GetDefaultModifierForJs
// GetDefaultModifierForCss
// GetDefaultModifierForXxml
// GetDefaultModifierForJson
//   These functions are different from the GetModifierForXXX functions
//   in that they do not take a parser and cannot fail. They simply
//   return the most common escaping directive for the context they refer to.
//
//   Some of these contexts (currently HTML and Javascript) have more than
//   one escaping directive associated with them and so we usually rely on
//   the current state of the parser to determine which directive to chose.
//   However, in some cases, the parser may fail to parse a given input
//   and so we may want to select the most likely escaping directive that
//   applies to the given context. Hence we use these functions instead of
//   the corresponding GetModifierForXXX ones.
extern CTEMPLATE_DLL_DECL
std::vector<const ModifierAndValue*> GetDefaultModifierForHtml();
extern CTEMPLATE_DLL_DECL
std::vector<const ModifierAndValue*> GetDefaultModifierForJs();
extern CTEMPLATE_DLL_DECL
std::vector<const ModifierAndValue*> GetDefaultModifierForCss();
extern CTEMPLATE_DLL_DECL
std::vector<const ModifierAndValue*> GetDefaultModifierForXml();
extern CTEMPLATE_DLL_DECL
std::vector<const ModifierAndValue*> GetDefaultModifierForJson();

}

#endif  // TEMPLATE_TEMPLATE_MODIFIERS_INTERNAL_H_
