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
// c++ bindings for htmlparser.

#ifndef SECURITY_STREAMHTMLPARSER_HTMLPARSER_CPP_H__
#define SECURITY_STREAMHTMLPARSER_HTMLPARSER_CPP_H__

#include <config.h>
#include <string>
#include "htmlparser/htmlparser.h"
#include "htmlparser/jsparser.h"
#include "base/util.h"

namespace ctemplate_htmlparser {

class JavascriptParser {
  public:
    enum State {
      STATE_TEXT = JSPARSER_STATE_TEXT,
      STATE_Q = JSPARSER_STATE_Q,
      STATE_DQ = JSPARSER_STATE_DQ,
      STATE_REGEXP = JSPARSER_STATE_REGEXP,
      STATE_COMMENT = JSPARSER_STATE_COMMENT,
    };
};

class HtmlParser {
  public:

    /* html states */
    enum State {
      STATE_TEXT = HTMLPARSER_STATE_TEXT,
      STATE_TAG = HTMLPARSER_STATE_TAG,
      STATE_ATTR = HTMLPARSER_STATE_ATTR,
      STATE_VALUE = HTMLPARSER_STATE_VALUE,
      STATE_COMMENT = HTMLPARSER_STATE_COMMENT,
      STATE_JS_FILE = HTMLPARSER_STATE_JS_FILE,
      STATE_CSS_FILE = HTMLPARSER_STATE_CSS_FILE,
      STATE_ERROR = HTMLPARSER_STATE_ERROR
    };

    /* attribute types */
    enum AttributeType {
      ATTR_NONE = HTMLPARSER_ATTR_NONE,
      ATTR_REGULAR = HTMLPARSER_ATTR_REGULAR,
      ATTR_URI = HTMLPARSER_ATTR_URI,
      ATTR_JS = HTMLPARSER_ATTR_JS,
      ATTR_STYLE = HTMLPARSER_ATTR_STYLE
    };

    /* Parser modes */
    enum Mode {
      MODE_HTML = HTMLPARSER_MODE_HTML,
      MODE_JS = HTMLPARSER_MODE_JS,
      MODE_CSS = HTMLPARSER_MODE_CSS,
      MODE_HTML_IN_TAG = HTMLPARSER_MODE_HTML_IN_TAG
    };

    HtmlParser() {
      parser_ = htmlparser_new();
      CHECK(parser_ != NULL);
    };

    /* Parses the input html stream and returns the finishing state.
     *
     * Returns HtmlParser::STATE_ERROR if unable to parse the input. If
     * htmlparser_parse() is called after an error situation was encountered
     * the behaviour is unspecified. At this point, Reset() or ResetMode()
     * can be called to reset the state so it can be used to parse a new file.
     */
    int Parse(const char *str, int len) {
      return htmlparser_parse(parser_, str, len);
    };

    int Parse(const std::string &str) {
      return Parse(str.c_str(), static_cast<int>(str.length()));
    };

    /* Returns the current state the parser is in */
    int state() const {
      return htmlparser_state(parser_);
    };

    /* Returns the current tag or NULL if not available.
     *
     * There is no stack implemented because we currently don't have a need for
     * it, which means tag names are tracked only one level deep.
     *
     * This is better understood by looking at the following example:
     *
     * <b [tag=b]>
     *   [tag=b]
     *   <i>
     *    [tag=i]
     *   </i>
     *  [tag=NULL]
     * </b>
     *
     * The tag is correctly filled inside the tag itself and before any new
     * inner tag is closed, at which point the tag will be set to NULL.
     *
     * For our current purposes this is not a problem, but we may implement a
     * tag tracking stack in the future for completeness.
     */
    const char *tag() const {
      return htmlparser_tag(parser_);
    }

    /* Returns the current attribute name if inside an attribute name or an
     * attribute value. Returns NULL otherwise. */
    const char *attribute() const {
      return htmlparser_attr(parser_);
    }

    /* Returns the contents of the current attribute value. */
    const char *value() const {
      return htmlparser_value(parser_);
    }

    /* Returns true if inside javascript. This can be a javascript block, a
     * javascript attribute value or the parser may just be in javascript mode
     * (HtmlParser::MODE_JS) */
    bool InJavascript() const {
      return static_cast<bool>(htmlparser_in_js(parser_));
    }

    /* Returns true if the parser is currently inside a CSS construct.
     *
     * Currently this can be either a STYLE tag, a STYLE attribute or the fact
     * that the parser was reset using MODE_CSS using ResetMode().
     */
    bool InCss() const {
      return static_cast<bool>(htmlparser_in_css(parser_));
    }

    /* Returns true if the current attribute is quoted */
    bool IsAttributeQuoted() const {
      return static_cast<bool>(htmlparser_is_attr_quoted(parser_));
    }

    /* Returns true if the parser is inside a js string literal.
     */
    bool IsJavascriptQuoted() const {
      return static_cast<bool>(htmlparser_is_js_quoted(parser_));
    }

    /* Returns the index within the current value or -1 if the parser is not
     * inside an attribute value */
    int ValueIndex() const {
      return htmlparser_value_index(parser_);
    }

    /* Returns true if this is the first character of a url inside an attribute.
     *
     * This function can be used by an html sanitizer or auto escaping system as
     * a hint that it should validate the url for a whitelist of protocol
     * handlers and for well-formedness, or that it should just escape a
     * component of it.
     *
     * For attributes that expect a url this will return true if we are at the
     * first character of the attribute, but for the special case of a meta
     * redirect tag some analysis is made in order to verify if we are at the
     * start of a url or not.
     *
     * For any other attributes, the result will always be false.
     *
     */
    bool IsUrlStart() const {
      return htmlparser_is_url_start(parser_);
    }

    /* Returns the current attribute type.
     *
     * The attribute type can be one of:
     *   ATTR_NONE - not inside an attribute
     *   ATTR_REGULAR - Inside a normal attribute
     *   ATTR_URI - Inside an attribute that accepts a uri
     *   ATTR_JS - Inside a javascript attribute
     *   ATTR_STYLE - Inside a css style attribute
     * */
    int AttributeType() const {
      return htmlparser_attr_type(parser_);
    }

    /* Return the current line number. */
    int line_number() const {
      return htmlparser_get_line_number(parser_);
    }

    /* Set the current line number. */
    void set_line_number(int line) {
      return htmlparser_set_line_number(parser_, line);
    }

    /* Return the current column number. */
    int column_number() const {
      return htmlparser_get_column_number(parser_);
    }

    /* Set the current line number. */
    void set_column_number(int column) {
      return htmlparser_set_column_number(parser_, column);
    }

    /* Retrieve a human readable error message in case an error occurred.
     *
     * NULL is returned if the parser didn't encounter an error.
     */
    const char *GetErrorMessage() {
      return htmlparser_get_error_msg(parser_);
    }

    /* Returns the current state the javascript parser is in.
     *
     * Should only be used for testing.
     */
    int javascript_state() const {
      return htmlparser_js_state(parser_);
    };

    /* Resets the parser to it's initial state and changes the parser mode.
     *
     * Internal state (tag name, attribute name, state of statemachine) is
     * reset as * though the object was just created.
     *
     * Available modes:
     *  MODE_HTML - Parses html text
     *  MODE_JS - Parses javascript files
     *  MODE_CSS - Parses CSS files. No actual parsing is actually done
     *             but InCss() always returns true.
     *  MODE_HTML_IN_TAG - Parses an attribute list inside a tag. To
     *                     be used in a template expanded in the
     *                     following context: <a $template>
     */
    void ResetMode(enum Mode mode) {
      return htmlparser_reset_mode(parser_, mode);
    }

    /* Resets the parser to it's initial state and to the default mode, which is
     * MODE_HTML.
     *
     * All internal context like tag name, attribute name or the state of the
     * statemachine are reset to it's original values as if the object was just
     * created.
     */
    void Reset() {
      return htmlparser_reset(parser_);
    }

    /* Invoked when text is inserted by the caller.
     *
     * Should be called before a template directive that expands to content is
     * found. This changes the current state by following the default rule,
     * ensuring we stay in sync with template.
     *
     * Returns true if template directives are accepted for this state and
     * false if they are not, which should result in an error condition.
     *
     * Right now the only case being handled are unquoted attribute values and
     * it always returns true. In the future we can handle more cases and
     * restrict the states were we allow template directives by returning false
     * for those.
     */
    bool InsertText() {
      return static_cast<bool>(htmlparser_insert_text(parser_));
    }

    /* Copies the context of the HtmlParser object referenced in source to the
     * current object.
     */
    void CopyFrom(const HtmlParser *source) {
      CHECK(this != source);
      CHECK(source != NULL);
      htmlparser_copy(parser_, source->parser_);
    }

    ~HtmlParser() {
      htmlparser_delete(parser_);
    };


  private:
    htmlparser_ctx *parser_;
    DISALLOW_COPY_AND_ASSIGN(HtmlParser);
};

}  // namespace security_streamhtmlparser

#endif  // SECURITY_STREAMHTMLPARSER_HTMLPARSER_CPP_H__
