/* Copyright (c) 2007, Google Inc.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "htmlparser/statemachine.h"
#include "htmlparser/jsparser.h"

/* So we can support both C and C++ compilers, we use the CAST() macro instead
 * of using C style casts or static_cast<>() directly.
 */
#ifdef __cplusplus
  #define CAST(type, expression) (static_cast<type>(expression))
#else
  #define CAST(type, expression) ((type)(expression))
#endif

#ifdef __cplusplus
namespace ctemplate_htmlparser {
#endif /* __cplusplus */

/* Generated state machine definition. */
#include "htmlparser/jsparser_fsm.h"

/* List of keywords that can precede a regular expression literal. Taken from:
 * http://www.mozilla.org/js/language/js20-2000-07/rationale/syntax.html
 *
 * 'void' was added to this list.
 * The list is used as input to a binary search, so it must be kept in a sorted
 * form.
 * There are a large number of keywords in here that don't exist in
 * ecmascript 3, either because they are reserved or because they are part of
 * ecmascript 4. However they weren't removed in order to keep the list in sync
 * with the previous document.
 */
static const char *regexp_token_prefix[] = {
  "abstract",
  "break",
  "case",
  "catch",
  "class",
  "const",
  "continue",
  "debugger",
  "default",
  "delete",
  "do",
  "else",
  "enum",
  "eval",
  "export",
  "extends",
  "field",
  "final",
  "finally",
  "for",
  "function",
  "goto",
  "if",
  "implements",
  "import",
  "in",
  "instanceof",
  "native",
  "new",
  "package",
  "private",
  "protected",
  "public",
  "return",
  "static",
  "switch",
  "synchronized",
  "throw",
  "throws",
  "transient",
  "try",
  "typeof",
  "var",
  "void",
  "volatile",
  "while",
  "with"
};

/* Utility functions */

/* Converts the internal state into the external superstate.
 */
static inline int state_external(int state)
{
  assert(state < JSPARSER_NUM_STATES);
  assert(state >= 0);
  return jsparser_states_external[state];
}

/* Returns true if the character is an ecmascript whitespace or line terminator
 * character. Includes the characters in sections 7.2 and 7.3 of ECMAScript 3
 * with the exception of unicode space and line terminators:
 * http://www.ecma-international.org/publications/files/ECMA-ST/Ecma-262.pdf
 */
static inline int js_is_whitespace(char c)
{
  return c == '\t' ||   /* Tab 0x09 */
         c == '\v' ||   /* Vertical Tab 0x0B */
         c == '\f' ||   /* Form Feed 0x0C */
         c == ' ' ||    /* Space 0x20 */
         c == '\xa0' || /* No-Break Space 0xA0 */
         c == '\n' ||   /* line Feed 0x0A */
         c == '\r';     /* Carriage Return 0x0D */
}

/* Returns true if the character is part of a javascript identifier. The rules
 * are pretty relaxed here since we don't accept unicode and don't care if the
 * first character doesn't contain numbers or not.
 *
 * For more detail on the limitations of having this relaxed set of characters
 * please see the comments in_state_js_text().
 */
static inline int js_is_identifier(char c) {
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
         (c >= '0' && c <= '9') ||
         c == '_' ||
         c == '$';
}

/* Appends a character to the ring buffer. Sequences of whitespace and newlines
 * are folded into one.
 *
 * js->buffer_start points to the first character in the buffer and
 * js->buffer_end points to the position of the next character to be written,
 * or one plus the last character written. If the buffer is full there will be
 * an empty slot position so we can diferentiate an empty buffer from a full
 * buffer.
 *
 * If the buffer is empty, then:
 *   js->buffer_start == js->buffer_end.
 * If the buffer is full, then:
 *   (js->buffer_end + 1) % JSPARSER_RING_BUFFER_SIZE == js->buffer_start.
 *
 */
void jsparser_buffer_append_chr(jsparser_ctx *js, char chr)
{
  /* Fold whitespace so we have enough space in the buffer. */
  if (js_is_whitespace(chr) &&
      js_is_whitespace(jsparser_buffer_get(js, -1))) {
    return;
  }

  js->buffer[js->buffer_end] = chr;
  js->buffer_end = (js->buffer_end + 1) % JSPARSER_RING_BUFFER_SIZE;
  if (js->buffer_end == js->buffer_start) {
    js->buffer_start = (js->buffer_end + 1) %
        JSPARSER_RING_BUFFER_SIZE;
  }
}

/* Appends a string to the ring buffer. Sequences of whitespace and newlines
 * are folded into one.
 */
void jsparser_buffer_append_str(jsparser_ctx *js, const char *str)
{
  assert(js != NULL);
  assert(str != NULL);

  while(*str != '\0') {
    jsparser_buffer_append_chr(js, *str++);
  }
}

/* Returns the position relative to the start of the buffer or -1 if past the
 * size of the buffer..
 */
static inline int jsparser_buffer_absolute_pos(jsparser_ctx *js, int pos)
{
  int absolute_pos;
  int buffer_len;
  assert(pos < 0);

  if(pos <= -JSPARSER_RING_BUFFER_SIZE) {
    return -1;
  }

  buffer_len = js->buffer_end - js->buffer_start;
  if (buffer_len < 0) {
    buffer_len += JSPARSER_RING_BUFFER_SIZE;
  }

  if (pos < -buffer_len) {
    return -1;
  }

  absolute_pos = (pos + js->buffer_end) % JSPARSER_RING_BUFFER_SIZE;
  if (absolute_pos < 0) {
    absolute_pos += JSPARSER_RING_BUFFER_SIZE;
  }

  return absolute_pos;
}

/* Returns the last appended character and removes it from the buffer. If the
 * buffer is empty, then it returns ASCII 0 ('\0').
 */
char jsparser_buffer_pop(jsparser_ctx *js)
{
  if (js->buffer_start == js->buffer_end) {
    return '\0';
  }

  js->buffer_end--;
  if (js->buffer_end < 0) {
    js->buffer_end += JSPARSER_RING_BUFFER_SIZE;
  }

  return js->buffer[js->buffer_end];
}

/* Returns the value of the character at a certain index in the buffer or an
 * ASCII 0 ('\0') character if the index is outside the buffer boundaries.
 *
 * Index positions are negative, were -1 is the last character appended to the
 * buffer. Using positive integers for the index will result in undefined
 * behaviour.
 */
char jsparser_buffer_get(jsparser_ctx *js, int pos)
{
  int absolute_pos;
  assert(pos < 0);

  absolute_pos = jsparser_buffer_absolute_pos(js, pos);
  if (absolute_pos < 0) {
    return '\0';
  }

  return js->buffer[absolute_pos];
}

/* Sets the value of the character at a certain index in the buffer. Returns
 * true if the write was successful or false if there was an attempt to write
 * outside of the buffer boundaries.
 *
 * Index positions are negative, were -1 is the last character appended to the
 * buffer. Using positive integers for the index will result in undefined
 * behaviour.
 */
int jsparser_buffer_set(jsparser_ctx *js, int pos, char value)
{
  int absolute_pos;
  assert(pos < 0);

  absolute_pos = jsparser_buffer_absolute_pos(js, pos);
  if (absolute_pos < 0) {
    return 0;
  }

  js->buffer[absolute_pos] = value;
  return 1;
}

/* Copies a slice of the buffer to the string pointed to by output. start and
 * end are the indexes of the sliced region. If start extends beyond the
 * beginning of the buffer, the slice will only contain character from the
 * beginning of the buffer.
 */
void jsparser_buffer_slice(jsparser_ctx *js, char *output, int start, int end)
{
  int pos;

  assert(start <= end);
  assert(start < 0);
  assert(end < 0);

  for (pos = start; pos <= end; ++pos) {
    char c;
    c = jsparser_buffer_get(js, pos);
    if (c != '\0') {
      *output++ = jsparser_buffer_get(js, pos);
    }
  }
  *output++ = '\0';
}

/* Copy the last javascript identifier or keyword found in the buffer to the
 * string pointed by identifier.
 *
 * For simplicity, we consider an identifier to be a sequence of alphanumeric
 * characters (as opposed to a digit followed by an alphanumeric character.
 *
 * Returns 0 if no identifier was matched, in which case the identifier
 * argument is replaced with an empty string, or non 0 if the identifier was
 * found.
 */
int jsparser_buffer_last_identifier(jsparser_ctx *js, char *identifier)
{
  int end;
  int pos;

  assert(identifier != NULL);

  end = -1;
  /* Ignore the optional whitespace delimiter */
  if (js_is_whitespace(jsparser_buffer_get(js, -1))) {
    --end;
  }

  /* Find the beginning of the identifier. This loop ends either when we find a
   * character that doesn't belong to an identifier, or when we find a '\0'
   * character, which means we reached the end of the buffer. */
  for(pos = end; js_is_identifier(jsparser_buffer_get(js, pos)); --pos) {
  }
  if (pos + 1 >= end) {
    identifier[0] = '\0';
    return 0;
  }

  jsparser_buffer_slice(js, identifier, pos + 1, end);
  return 1;
}

/* Callback used in bsearch() for comparing a string against an array of
 * strings.
 */
static int bsearch_strcmp(const void *a, const void *b)
{
  return strcmp(CAST(const char*, a), *CAST(const char * const *, b));
}

/* Returns true if the token argument can be a token prefix to a javascript
 * regular expression.
 *
 * The token argument is compared against a list of identifiers that can
 * precede a regular expression in the javascript grammar, and returns true if
 * the argument is found on that list.
 */
static inline int is_regexp_token_prefix(char *token)
{
  assert(token != NULL);

  return bsearch(token,
                 regexp_token_prefix,
                 sizeof(regexp_token_prefix) / sizeof(char *),
                 sizeof(char *), bsearch_strcmp) != NULL;
}

/* Called for every character in state text.
 *
 * We copy every character we find when we are in state text to the ring
 * buffer. This has the side effect of also pushing slash characters that are
 * part of comments into the buffer, although for parsing purposes these should
 * be treated as whitespace. This issue is addressed in
 * enter_state_js_comment_ml_after().
 */
static void in_state_js_text(statemachine_ctx *ctx, int start, char chr,
                             int end)
{
  jsparser_ctx *js = CAST(jsparser_ctx *, ctx->user);
  assert(js != NULL);

  jsparser_buffer_append_chr(js, chr);
}

/* This function is called every time we find a slash ('/') character in the
 * javascript text (except for slashes that close comments or regexp literals).
 *
 * Implements the logic to figure out if this slash character is a division
 * operator or if it opens a regular expression literal. This is heavily
 * inspired by the syntactic resynchronization for javascript 2.0:
 * http://www.mozilla.org/js/language/js20-2000-07/rationale/syntax.html
 *
 * When we receive a '/', we look at the previous non space character to figure
 * out if it's the ending of a punctuator that can precede a regexp literal, in
 * which case we assume the current '/' is part of a regular expression literal
 * (or the opening of a javascript comment, but that part is dealt with in the
 * state machine). The exceptions to this are unary operators, so we look back
 * a second character to rule out '++' and '--'. Although it is not
 * straightforward to figure out if the binary operator is a postfix of the
 * previous expression or a prefix of the regular expression, we rule out the
 * later as it is an uncommon practice.
 *
 * If we ruled out the previous token to be a valid regexp preceding
 * punctuator, we extract the last identifier in the buffer and match against a
 * list of keywords that are known to precede expressions in the grammar. If we
 * get a match on any of these keywords, then we are opening a regular
 * expression, if not, then we have a division operator.
 *
 * Known cases that are accepted by the grammar but we handle differently,
 * although I don't believe there is a legitimate usage for those:
 *
 * Division of a regular expression:
 * var result = /test/ / 5;
 *
 * Prefix unary increment of a regular expression:
 * var result = ++/test/;
 *
 * Division of an object literal:
 * { a: 1 } /x/.exec('x');
 *
 * We only support ascii right now, so unicode characters in identifiers will
 * be treated as delimiters, effectively breaking the identifier name where
 * they appear, and this may cause issues in very specific situations. Namely,
 * if we have a unicode character in an identifier directly preceding a suffix
 * that matches one of the keywords in regexp_token_prefix[], if this
 * identifier precedes a / (slash) character:
 *
 * var x = test<unicode char>return / 5;
 *
 * We will interpret that slash as the start of a regular expression, when in
 * reality it is a division operator.
 */
static void enter_state_js_slash(statemachine_ctx *ctx, int start, char chr,
                                 int end)
{
  jsparser_ctx *js;
  char buffer[JSPARSER_RING_BUFFER_SIZE];
  int pos;

  assert(ctx != NULL);
  assert(ctx->user != NULL);

  js = CAST(jsparser_ctx *, ctx->user);
  assert(js != NULL);

  pos = -1;
  /* Consume the last whitespace. */
  if (js_is_whitespace(jsparser_buffer_get(js, pos))) {
    --pos;
  }

  switch (jsparser_buffer_get(js, pos)) {
    /* Ignore unary increment */
    case '+':
      if (jsparser_buffer_get(js, pos - 1) != '+') {
        ctx->next_state = JSPARSER_STATE_INT_JS_REGEXP_SLASH;
      }
      break;

    /* Ignore unary decrement */
    case '-':
      if (jsparser_buffer_get(js, pos - 1) != '-') {
        ctx->next_state = JSPARSER_STATE_INT_JS_REGEXP_SLASH;
      }
      break;

    /* List of punctuator endings except ), ], }, + and - */
    case '=':
    case '<':
    case '>':
    case '&':
    case '|':
    case '!':
    case '%':
    case '*':
    case '/':
    case ',':
    case ';':
    case '?':
    case ':':
    case '^':
    case '~':
    case '{':
    case '(':
    case '[':
    case '}':
    case '\0':
      ctx->next_state = JSPARSER_STATE_INT_JS_REGEXP_SLASH;
      break;

    default:
      if (jsparser_buffer_last_identifier(js, buffer) &&
          is_regexp_token_prefix(buffer)) {
        ctx->next_state = JSPARSER_STATE_INT_JS_REGEXP_SLASH;
      }
  }

  jsparser_buffer_append_chr(js, chr);
}

/* Called at the end of a javascript comment.
 *
 * When we open a comment, the initial '/' was inserted into the ring buffer,
 * but it is not a token and should be considered whitespace for parsing
 * purposes.
 *
 * When we first saw the '/' character, we didn't yet know if it was the
 * beginning of a comment, a division operator, or a regexp.
 *
 * In this function we just replace the inital '/' with a whitespace character,
 * unless we had a preceding whitespace character, in which case we just remove
 * the '/'. This is needed to ensure all spaces in the buffer are correctly
 * folded.
 */
static void enter_state_js_comment_after(statemachine_ctx *ctx, int start,
                                         char chr, int end)
{
  jsparser_ctx *js;

  assert(ctx != NULL);
  assert(ctx->user != NULL);

  js = CAST(jsparser_ctx *, ctx->user);

  if (js_is_whitespace(jsparser_buffer_get(js, -2))) {
    (void)jsparser_buffer_pop(js);
  } else {
    jsparser_buffer_set(js, -1, ' ');
  }
}

static statemachine_definition *create_statemachine_definition()
{
  statemachine_definition *def;
  def = statemachine_definition_new(JSPARSER_NUM_STATES);
  if (def == NULL)
    return NULL;

  /* TODO(falmeida): Check return value. */
  statemachine_definition_populate(def, jsparser_state_transitions,
                                   jsparser_states_internal_names);

  statemachine_in_state(def, JSPARSER_STATE_INT_JS_TEXT,
                        in_state_js_text);
  statemachine_enter_state(def, JSPARSER_STATE_INT_JS_SLASH,
                           enter_state_js_slash);
  statemachine_enter_state(def, JSPARSER_STATE_INT_JS_COMMENT_AFTER,
                           enter_state_js_comment_after);

  return def;
}


/* Initializes a new jsparser instance.
 *
 * Returns a pointer to the new instance or NULL if the initialization
 * fails.
 *
 * Initialization failure is fatal, and if this function fails it may not
 * deallocate all previsouly allocated memory.
 */

jsparser_ctx *jsparser_new()
{
    jsparser_ctx *js;

    js = CAST(jsparser_ctx *, calloc(1, sizeof(jsparser_ctx)));
    if (js == NULL)
      return NULL;

    js->statemachine_def = create_statemachine_definition();
    if (js->statemachine_def == NULL)
      return NULL;

    js->statemachine = statemachine_new(js->statemachine_def, js);
    if (js->statemachine == NULL)
      return NULL;

    jsparser_reset(js);

    return js;
}

/* Returns a pointer to a context which is a duplicate of the jsparser src.
 */
jsparser_ctx *jsparser_duplicate(jsparser_ctx *src)
{
  jsparser_ctx *dst;
  assert(src != NULL);

  dst = jsparser_new();
  if (dst == NULL)
    return NULL;

  jsparser_copy(dst, src);

  return dst;
}

/* Copies the context of the jsparser pointed to by src to the jsparser dst.
 *
 * The state machine definition is preserved since it is read only.
 */
void jsparser_copy(jsparser_ctx *dst, jsparser_ctx *src)
{

  dst->buffer_start = src->buffer_start;
  dst->buffer_end = src->buffer_end;
  memcpy(dst->buffer, src->buffer, sizeof(src->buffer));

  statemachine_copy(dst->statemachine,
                    src->statemachine,
                    dst->statemachine_def,
                    dst);

}

void jsparser_reset(jsparser_ctx *ctx)
{
  assert(ctx != NULL);
  ctx->statemachine->current_state = 0;
  ctx->buffer_start = 0;
  ctx->buffer_end = 0;
}

int jsparser_state(jsparser_ctx *ctx)
{
  return state_external(ctx->statemachine->current_state);
}

int jsparser_parse(jsparser_ctx *ctx, const char *str, int size)
{
    int internal_state;
    internal_state = statemachine_parse(ctx->statemachine, str, size);
    return state_external(internal_state);
}

void jsparser_delete(jsparser_ctx *ctx)
{
    assert(ctx != NULL);
    statemachine_delete(ctx->statemachine);
    statemachine_definition_delete(ctx->statemachine_def);
    free(ctx);
}

#ifdef __cplusplus
}  /* namespace security_streamhtmlparser */
#endif /* __cplusplus */
