/*
 * Copyright (c) 2007, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ---
 *
 * Author: falmeida@google.com (Filipe Almeida)
 */

#ifndef SECURITY_STREAMHTMLPARSER_HTMLPARSER_H
#define SECURITY_STREAMHTMLPARSER_HTMLPARSER_H

#include <config.h>
#include "htmlparser/statemachine.h"
#include "htmlparser/jsparser.h"

// Annoying stuff for windows in opensource-land -- make sure clients
// (in this case unittests) can import the functions.
#ifndef CTEMPLATE_DLL_DECL
# ifdef _MSC_VER
#   define CTEMPLATE_DLL_DECL  __declspec(dllimport)
# else
#   define CTEMPLATE_DLL_DECL  /* should be the empty string for non-windows */
# endif
#endif

#ifdef __cplusplus
namespace ctemplate_htmlparser {
#endif

/* entity filter */

/* String sizes used in htmlparser and entityfilter structures including the
 * NULL terminator.
 */
#define HTMLPARSER_MAX_STRING STATEMACHINE_RECORD_BUFFER_SIZE
#define HTMLPARSER_MAX_ENTITY_SIZE 10


enum htmlparser_state_external_enum {
    HTMLPARSER_STATE_TEXT,
    HTMLPARSER_STATE_TAG,
    HTMLPARSER_STATE_ATTR,
    HTMLPARSER_STATE_VALUE,
    HTMLPARSER_STATE_COMMENT,
    HTMLPARSER_STATE_JS_FILE,
    HTMLPARSER_STATE_CSS_FILE,
    HTMLPARSER_STATE_ERROR
};

enum htmlparser_mode {
    HTMLPARSER_MODE_HTML,
    HTMLPARSER_MODE_JS,
    HTMLPARSER_MODE_CSS,
    HTMLPARSER_MODE_HTML_IN_TAG
};

enum htmlparser_attr_type {
    HTMLPARSER_ATTR_NONE,
    HTMLPARSER_ATTR_REGULAR,
    HTMLPARSER_ATTR_URI,
    HTMLPARSER_ATTR_JS,
    HTMLPARSER_ATTR_STYLE
};


/* TODO(falmeida): Maybe move some of these declaration to the .c and only keep
 * a forward declaration in here, since these structures are private.
 */

/* entityfilter context structure.
 *
 * The entity filter collection of routines provide a way to decode html
 * entities from an html document in a streaming way.
 *
 * The html_process() function receives a character at a time from the input
 * stream and returns 0 or more characters which should be appended to the
 * resulting decoded document.
 *
 * Currently this collection of functions are only exported for testing purposes
 * and shouldn't be called from outside of htmlparser.c.
 *
 * Since we really only use these functions with the very specific purpose of
 * decoding html entities for javascript attributes, only a small subset of
 * entities are supported: &lt;, &gt;, &quote;, &amp;, &apos, and the numeric
 * character references for both decimal and hexadecimal.
 */
typedef struct entityfilter_ctx_s {

    /* Current position into the buffer. */
    int buffer_pos;

    /* True if currently processing an html entity. */
    int in_entity;

    /* Temporary character buffer that is used while processing html entities.
     */
    char buffer[HTMLPARSER_MAX_ENTITY_SIZE];

    /* String buffer returned to the application after we decoded an html
     * entity.
     */
    char output[HTMLPARSER_MAX_ENTITY_SIZE];
} entityfilter_ctx;

/* Resets the entityfilter to its initial state so it can be reused.
 */
void entityfilter_reset(entityfilter_ctx *ctx);

/* Initializes a new entity filter object.
 */
entityfilter_ctx *entityfilter_new(void);

/* Deallocates an entity filter object.
 */
void entityfilter_delete(entityfilter_ctx *ctx);

/* Copies the context of the entityfilter pointed to by src to the entityfilter
 * dst.
 */
void entityfilter_copy(entityfilter_ctx *dst, entityfilter_ctx *src);

/* Processes a character from the input stream and decodes any html entities
 * in the accumulated buffer.
 *
 * Returns a reference to a string that points to an internal buffer. This
 * buffer will be changed after every call to entityfilter_process(). As
 * such this string should be duplicated before subsequent calls to
 * entityfilter_process().
 */
const char *entityfilter_process(entityfilter_ctx *ctx, char c);


/* html parser */

/* Stores the context of the html parser.
 * If this structure is changed, htmlparser_new(), htmlparser_copy() and
 * htmlparser_reset() should be updated accordingly.
 */
typedef struct htmlparser_ctx_s {

  /* Holds a reference to the statemachine context. */
  statemachine_ctx *statemachine;

  /* Holds a reference to the statemachine definition in use. Right now this is
   * only used so we can deallocate it at the end.
   *
   * It should be readonly and contain the same values across jsparser
   * instances.
   */
  /* TODO(falmeida): Change statemachine_def to const. */
  statemachine_definition *statemachine_def;

  /* Holds a reference to the javascript parser. */
  jsparser_ctx *jsparser;

  /* Holds a reference to the entity filter. Used for decoding html entities
   * inside javascript attributes. */
  entityfilter_ctx *entityfilter;

  /* Offset into the current attribute value where 0 is the first character in
   * the value. */
  int value_index;

  /* True if currently processing javascript. */
  int in_js;

  /* Current tag name. */
  char tag[HTMLPARSER_MAX_STRING];

  /* Current attribute name. */
  char attr[HTMLPARSER_MAX_STRING];

  /* Contents of the current value capped to HTMLPARSER_MAX_STRING. */
  char value[HTMLPARSER_MAX_STRING];

} htmlparser_ctx;

/* Resets the parser to its initial state and to the default mode, which
 * is MODE_HTML.
 *
 * All internal context like tag name, attribute name or the state of the
 * statemachine are reset to its original values as if the object was just
 * created.
 */
extern CTEMPLATE_DLL_DECL
void htmlparser_reset(htmlparser_ctx *ctx);

/* Resets the parser to its initial state and changes the parser mode.
 * All internal context like tag name, attribute name or the state of the
 * statemachine are reset to their original values as if the object was just
 * created.
 *
 * Available modes:
 *  HTMLPARSER_MODE_HTML - Parses html text
 *  HTMLPARSER_MODE_JS - Parses javascript files
 *  HTMLPARSER_MODE_CSS - Parses CSS files. No actual parsing is actually done
 *                        but htmlparser_in_css() always returns true.
 *  HTMLPARSER_MODE_HTML_IN_TAG - Parses an attribute list inside a tag. To
 *                                be used in a template expanded in the
 *                                following context: <a $template>
 *
 */
extern CTEMPLATE_DLL_DECL
void htmlparser_reset_mode(htmlparser_ctx *ctx, int mode);

/* Initializes a new htmlparser instance.
 *
 * Returns a pointer to the new instance or NULL if the initialization fails.
 * Initialization failure is fatal, and if this function fails it may not
 * deallocate all previsouly allocated memory.
 */
extern CTEMPLATE_DLL_DECL
htmlparser_ctx *htmlparser_new(void);

/* Copies the context of the htmlparser pointed to by src to the htmlparser dst.
 *
 * Also copies over the instances of the state machine, the jsparser and the
 * entity filter but not the statemachine definition since this one is read
 * only.
 */
extern CTEMPLATE_DLL_DECL
void htmlparser_copy(htmlparser_ctx *dst, const htmlparser_ctx *src);

/* Receives an htmlparser context and returns the current html state.
 *
 * The return value will be one of the states of htmlparser_state_external_enum.
 */
extern CTEMPLATE_DLL_DECL
int htmlparser_state(htmlparser_ctx *ctx);

/* Parses the input html stream and returns the finishing state.
 *
 * Returns HTMLPARSER_ERROR if unable to parse the input. If htmlparser_parse()
 * is called after an error situation was encountered the behaviour is
 * unspecified. At this point, htmlparser_reset() or htmlparser_reset_mode()
 * can be called to reset the state.
 */
extern CTEMPLATE_DLL_DECL
int htmlparser_parse(htmlparser_ctx *ctx, const char *str, int size);

/* Returns true if the parser is inside an attribute value and the value is
 * surrounded by single or double quotes. */
extern CTEMPLATE_DLL_DECL
int htmlparser_is_attr_quoted(htmlparser_ctx *ctx);

/* Returns true if the parser is currently in javascript. This can be a
 * an attribute that takes javascript, a javascript block or the parser
 * can just be in MODE_JS. */
extern CTEMPLATE_DLL_DECL
int htmlparser_in_js(htmlparser_ctx *ctx);

/* Returns the current tag or NULL if not available or we haven't seen the
 * entire tag yet.
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
 * The tag is correctly filled inside the tag itself and before any new inner
 * tag is closed, at which point the tag will be null.
 *
 * For our current purposes this is not a problem, but we may implement a tag
 * tracking stack in the future for completeness.
 *
 */
extern CTEMPLATE_DLL_DECL
const char *htmlparser_tag(htmlparser_ctx *ctx);

/* Returns the current attribute name if after an attribute name or in an
 * attribute value. Returns NULL otherwise. */
extern CTEMPLATE_DLL_DECL
const char *htmlparser_attr(htmlparser_ctx *ctx);

/* Returns the contents of the current attribute value.
 *
 * Returns NULL if not inside an attribute value.
 */
extern CTEMPLATE_DLL_DECL
const char *htmlparser_value(htmlparser_ctx *ctx);

/* Returns true if the parser is currently inside a CSS construct.
 *
 * Currently this can be either a STYLE tag, a STYLE attribute or the fact that
 * the parser was reset in HTMLPARSER_MODE_CSS using
 * htmlparser_reset_mode().
 */
extern CTEMPLATE_DLL_DECL
int htmlparser_in_css(htmlparser_ctx *ctx);

/* Returns the current state of the javascript state machine.
 *
 * Currently only present for testing purposes.
 */
extern CTEMPLATE_DLL_DECL
int htmlparser_js_state(htmlparser_ctx *ctx);

/* Returns non-zero if currently inside a javascript string literal and zero
 * otherwise.
 */
extern CTEMPLATE_DLL_DECL
int htmlparser_is_js_quoted(htmlparser_ctx *ctx);

/* Returns non-zero if currently inside an attribute value and zero otherwise.
 */
extern CTEMPLATE_DLL_DECL
int htmlparser_value_index(htmlparser_ctx *ctx);

/* Returns true if this is the first character of a url inside an attribute.
 *
 * This function can be used by an html sanitizer or auto escaping system as a
 * hint that it should validate the url for a whitelist of protocol handlers and
 * for well-formedness, or that it should just escape a component of it.
 *
 * For attributes that expect a URL, this will return true if we are at the
 * first character of the URL, false otherwise.
 * For most attributes, this is the same as checking that we are at the first
 * character of the attribute value but it also works correctly for the
 * "content" attribute of the "meta" tag where the URL follows some earlier
 * content.
 * e.g: <meta http-equiv="refresh" * content="5; URL=http://bla.">
 *
 * For any other attributes, the result will always be false.
 */
extern CTEMPLATE_DLL_DECL
int htmlparser_is_url_start(htmlparser_ctx *ctx);

/* Returns the current attribute type.
 *
 * The attribute type can be one of:
 *   HTMLPARSER_ATTR_NONE - not inside an attribute.
 *   HTMLPARSER_ATTR_REGULAR - Inside a normal attribute.
 *   HTMLPARSER_ATTR_URI - Inside an attribute that accepts a uri.
 *   HTMLPARSER_ATTR_JS - Inside a javascript attribute.
 *   HTMLPARSER_ATTR_STYLE - Inside a css style attribute.
 */
extern CTEMPLATE_DLL_DECL
int htmlparser_attr_type(htmlparser_ctx *ctx);

/* Return the current line number. */
extern CTEMPLATE_DLL_DECL
int htmlparser_get_line_number(htmlparser_ctx *ctx);

/* Set the current line number. */
extern CTEMPLATE_DLL_DECL
void htmlparser_set_line_number(htmlparser_ctx *ctx, int line);

/* Return the current column number. */
extern CTEMPLATE_DLL_DECL
int htmlparser_get_column_number(htmlparser_ctx *ctx);

/* Set the current column number. */
extern CTEMPLATE_DLL_DECL
void htmlparser_set_column_number(htmlparser_ctx *ctx, int column);

/* Retrieve a human readable error message in case an error occurred.
 *
 * NULL is returned if the parser didn't encounter an error.
 */
extern CTEMPLATE_DLL_DECL
const char *htmlparser_get_error_msg(htmlparser_ctx *ctx);

/* Invoked by the caller when text is expanded by the caller.
 *
 * Should be invoked when a template directive that expands to content is
 * executed but we don't provide this content to the parser itself. This changes
 * the current state by following the default rule, ensuring we stay in sync
 * with the template.
 *
 * Returns 1 if template directives are accepted for this state and 0 if they
 * are not, which should result in an error condition.
 *
 * Right now the only case being handled are unquoted attribute values and it
 * always returns 1. When insert_text() is called after the equals sign, we
 * assume some text was consumed and we are now in the middle of the attribute
 * value itself. Example:
 *
 * <a href=$HREF_VALUE alt=alternate_text>
 *
 * The template calls insert_text() when it encounters $HREF_VALUE. If it didn't
 * the parser would only have seen the following html:
 *
 * <a href= alt=alternate_text>
 *
 * and would interpret alt=alternate_text as the value of the href attribute.
 */
extern CTEMPLATE_DLL_DECL
int htmlparser_insert_text(htmlparser_ctx *ctx);

/* Deallocates an htmlparser context object.
 */
extern CTEMPLATE_DLL_DECL
void htmlparser_delete(htmlparser_ctx *ctx);

#define htmlparser_parse_chr(a,b) htmlparser_parse(a, &(b), 1);
#ifdef __cplusplus
#define htmlparser_parse_str(a,b) htmlparser_parse(a, b, \
                                                   static_cast<int>(strlen(b)));
#else
#define htmlparser_parse_str(a,b) htmlparser_parse(a, b, (int)strlen(b));
#endif

#ifdef __cplusplus
}  /* namespace security_streamhtmlparser */
#endif

#endif /* SECURITY_STREAMHTMLPARSER_HTMLPARSER_H */
