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

/* TODO(falmeida): Breaks on NULL characters in the stream. fix.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "htmlparser/statemachine.h"
#include "htmlparser/htmlparser.h"
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
#endif

/* Generated state machine definition. */
#include "htmlparser/htmlparser_fsm.h"

#define is_js_attribute(attr) ((attr)[0] == 'o' && (attr)[1] == 'n')
#define is_style_attribute(attr) (strcmp((attr), "style") == 0)

/* html entity filter */
static struct entityfilter_table_s {
    const char *entity;
    const char *value;
} entityfilter_table[] = {
    { "lt",     "<" },
    { "gt",     ">" },
    { "quot",   "\"" },
    { "amp",    "&" },
    { "apos",   "\'" },
    { NULL,     NULL }
};

/* Utility functions */

/* Similar to strncpy() but avoids the NULL padding. */
static inline void nopad_strncpy(char *dst, const char *src, size_t dst_size,
                                 size_t src_size)
{
  size_t size;

  /* size = min(dst_size, src_size) */
  size = dst_size > src_size ? src_size : dst_size;
  strncpy(dst, src, size);
  if (size > 0)
    dst[size - 1] = '\0';
}

/* Converts the internal state into the external superstate.
 */
static int state_external(int st)
{
    if (st == STATEMACHINE_ERROR)
      return HTMLPARSER_STATE_ERROR;
    else
      return htmlparser_states_external[st];
}

/* Returns true if the character is considered an html whitespace character.
 *
 * From: http://www.w3.org/TR/html401/struct/text.html#h-9.1
 */
static inline int html_isspace(char chr)
{
  if (chr == ' ' || chr == '\t' || chr == '\n' || chr == '\r') {
    return 1;
  } else {
    return 0;
  }
}

/* Returns true if the attribute is expected to contain a url
 * This list was taken from: http://www.w3.org/TR/html4/index/attributes.html
 */
static int is_uri_attribute(char *attr)
{
  if (attr == NULL)
    return 0;

  switch (attr[0]) {
    case 'a':
      if (strcmp(attr, "action") == 0)
        return 1;
      /* TODO(falmeida): This is a uri list. Should we treat it diferently? */
      if (strcmp(attr, "archive") == 0)  /* This is a uri list */
        return 1;
      break;

    case 'b':
      if (strcmp(attr, "background") == 0)
        return 1;
      break;

    case 'c':
      if (strcmp(attr, "cite") == 0)
        return 1;
      if (strcmp(attr, "classid") == 0)
        return 1;
      if (strcmp(attr, "codebase") == 0)
        return 1;
      break;

    case 'd':
      if (strcmp(attr, "data") == 0)
        return 1;
      if (strcmp(attr, "dynsrc") == 0) /* from msdn */
        return 1;
      break;

    case 'h':
      if (strcmp(attr, "href") == 0)
        return 1;
      break;

    case 'l':
      if (strcmp(attr, "longdesc") == 0)
        return 1;
      break;

    case 's':
      if (strcmp(attr, "src") == 0)
        return 1;
      break;

    case 'u':
      if (strcmp(attr, "usemap") == 0)
        return 1;
      break;
  }

  return 0;

}

/* Convert a string to lower case characters inplace.
 */
static void tolower_str(char *s)
{
    while (*s != '\0') {
      *s = CAST(char, tolower(CAST(unsigned char,*s)));
      s++;
    }
}

static const char *ignore_spaces_or_digits(const char *value) {
  while (html_isspace(*value) || ((*value >= '0' && *value <= '9')))
    value++;

  return value;
}

static const char *ignore_spaces(const char *value) {
  while (html_isspace(*value))
    value++;

  return value;
}

/* Return type of the function meta_redirect_type.
 */
enum meta_redirect_type_enum {
  META_REDIRECT_TYPE_NONE,
  META_REDIRECT_TYPE_URL_START,
  META_REDIRECT_TYPE_URL
};

/* Analyzes a string for the presence of a meta refresh type url.
 *
 * This function receives the value of the content attribute of a meta tag and
 * parses it in order to identify if a url is going to be present. This is the
 * format of such tag:
 *
 * <meta http-equiv="refresh" content="5; URL=http://www.google.com">
 *
 * Using a regular expression library would be the most obvious way to implement
 * this functionality, but introducing such a dependency is undesirable. We
 * opted instead to parse programmaticly since the expression is simple enough.
 *
 * For reference, this is the spec on the meta http refresh tag:
 * http://dev.w3.org/html5/spec/Overview.html#attr-meta-http-equiv-refresh
 *
 * If the value has no content after the expression, we know we are at the start
 * of the URL. Otherwise we are past the start of the URL.
 *
 *
 * Returns:
 *
 * This functions returns one of the following values:
 *   META_REDIRECT_TYPE_NONE - A url was not identified in the input string.
 *   META_REDIRECT_TYPE_URL_START - The input string ends exactly at the start
 *   of the url.
 *   META_REDIRECT_TYPE_URL - The input string ends somewhere in the middle or
 *   the end of the url.
 *
 * A few examples:
 *   "5"
 *   Returns META_REDIRECT_TYPE_NONE since we don't expect a url to follow.
 *
 *   "5; URL = "
 *   The function returns META_REDIRECT_TYPE_URL_START since we expect a url to
 *   follow.
 *
 *   "5; URL = http://www.google.com/?"
 *   Returns META_REDIRECT_TYPE_URL since the input value terminates in the
 *   middle or end of a url.
 *
 *
 * Caveats: We are only recording up to 256 characters of attribute values, so
 * our analysis is limited to that. This shouldn't be an issue in practice
 * though as it would be unexpected for the part of the string that we are
 * matching to be so long.
 */
enum meta_redirect_type_enum meta_redirect_type(const char *value) {

  if (value == NULL)
    return META_REDIRECT_TYPE_NONE;

  /* Match while [ \t\r\n0-9]* */
  value = ignore_spaces_or_digits(value);

  /* Verify that we got a semi-colon character */
  if (*value != ';')
    return META_REDIRECT_TYPE_NONE;
  value++;

  /* Match while [ \t\r\n]* */
  value = ignore_spaces(value);

  /* Validate that we have 'URL' */
  if (strncasecmp(value, "url", strlen("url")) != 0)
    return META_REDIRECT_TYPE_NONE;

  value += strlen("url");

  /* Match while [ \t\r\n]* */
  value = ignore_spaces(value);

  if (*value != '=')
    return META_REDIRECT_TYPE_NONE;
  value++;

  /* Match while [ \t\r\n]* */
  value = ignore_spaces(value);

  /* The HTML5 spec allows for the url to be quoted, so we skip a single or
   * double quote if we find one.
   */
  if (*value == '"' || *value == '\'')
    value++;

  if (*value == '\0')
    return META_REDIRECT_TYPE_URL_START;
  else
    return META_REDIRECT_TYPE_URL;
}


/* Resets the entityfilter to it's initial state so it can be reused.
 */
void entityfilter_reset(entityfilter_ctx *ctx)
{
    ctx->buffer[0] = 0;
    ctx->buffer_pos = 0;
    ctx->in_entity = 0;
}

/* Initializes a new entity filter object.
 */
entityfilter_ctx *entityfilter_new()
{
    entityfilter_ctx *ctx;
    ctx = CAST(entityfilter_ctx *,
               malloc(sizeof(entityfilter_ctx)));

    if (ctx == NULL)
      return NULL;
    ctx->buffer[0] = 0;
    ctx->buffer_pos = 0;
    ctx->in_entity = 0;

    return ctx;
}

/* Copies the context of the entityfilter pointed to by src to the entityfilter
 * dst.
 */
void entityfilter_copy(entityfilter_ctx *dst, entityfilter_ctx *src)
{
  assert(src != NULL);
  assert(dst != NULL);
  assert(src != dst);
  memcpy(dst, src, sizeof(entityfilter_ctx));
}


/* Deallocates an entity filter object.
 */
void entityfilter_delete(entityfilter_ctx *ctx)
{
    free(ctx);
}

/* Converts a string containing an hexadecimal number to a string containing
 * one character with the corresponding ascii value.
 *
 * The provided output char array must be at least 2 chars long.
 */
static const char *parse_hex(const char *s, char *output)
{
    int n;
    n = strtol(s, NULL, 16);
    output[0] = n;
    output[1] = 0;
    /* TODO(falmeida): Make this function return void */
    return output;
}

/* Converts a string containing a decimal number to a string containing one
 * character with the corresponding ascii value.
 *
 * The provided output char array must be at least 2 chars long.
 */
static const char *parse_dec(const char *s, char *output)
{
    int n;
    n = strtol(s, NULL, 10);
    output[0] = n;
    output[1] = 0;
    return output;
}

/* Converts a string with an html entity to it's encoded form, which is written
 * to the output string.
 */
static const char *entity_convert(const char *s, char *output, char terminator)
{
  /* TODO(falmeida): Handle wide char encodings */
    struct entityfilter_table_s *t = entityfilter_table;

    if (s[0] == '#') {
      if (s[1] == 'x' || s[1] == 'X') { /* hex */
          return parse_hex(s + 2, output);
      } else { /* decimal */
          return parse_dec(s + 1, output);
      }
    }

    while (t->entity != NULL) {
        if (strcasecmp(t->entity, s) == 0)
            return t->value;
        t++;
    }

    snprintf(output, HTMLPARSER_MAX_ENTITY_SIZE, "&%s%c", s, terminator);
    output[HTMLPARSER_MAX_ENTITY_SIZE - 1] = '\0';

    return output;
}


/* Processes a character from the input stream and decodes any html entities
 * in the processed input stream.
 */
const char *entityfilter_process(entityfilter_ctx *ctx, char c)
{
    if (ctx->in_entity) {
        if (c == ';' || html_isspace(c)) {
            ctx->in_entity = 0;
            ctx->buffer[ctx->buffer_pos] = '\0';
            ctx->buffer_pos = 0;
            return entity_convert(ctx->buffer, ctx->output, c);
        } else {
            ctx->buffer[ctx->buffer_pos++] = c;
            if (ctx->buffer_pos >= HTMLPARSER_MAX_ENTITY_SIZE - 2) {
                /* No more buffer to use, finalize and return.
                 * We need two characters left, one for the '&' character and
                 * another for the NULL termination. */
                ctx->buffer[ctx->buffer_pos] = '\0';
                ctx->in_entity=0;
                ctx->buffer_pos = 0;
                snprintf(ctx->output, HTMLPARSER_MAX_ENTITY_SIZE, "&%s",
                         ctx->buffer);
                ctx->output[HTMLPARSER_MAX_ENTITY_SIZE - 1] = '\0';
                return ctx->output;
            }
        }
    } else {
        if (c == '&') {
            ctx->in_entity = 1;
            ctx->buffer_pos = 0;
        } else {
            ctx->output[0] = c;
            ctx->output[1] = 0;
            return ctx->output;
        }
    }
    return "";
}

/* Called when the parser enters a new tag. Starts recording it's name into
 * html->tag.
 */
static void enter_tag_name(statemachine_ctx *ctx, int start, char chr, int end)
{
    htmlparser_ctx *html = CAST(htmlparser_ctx *, ctx->user);
    assert(html != NULL);

    html->tag[0] = '\0';
    statemachine_start_record(ctx);
}

/* Called when the parser exits the tag name in order to finalize the recording.
 *
 * It converts the tag name to lowercase, and if the tag was closed, just
 * clears html->tag.
 */
static void exit_tag_name(statemachine_ctx *ctx, int start, char chr, int end)
{
    htmlparser_ctx *html = CAST(htmlparser_ctx *, ctx->user);
    assert(html != NULL);

    nopad_strncpy(html->tag, statemachine_stop_record(ctx),
                  HTMLPARSER_MAX_STRING, statemachine_record_length(ctx));

    tolower_str(html->tag);

    if (html->tag[0] == '/')
      html->tag[0] = '\0';
}

/* Called when the parser enters a new tag. Starts recording it's name into
 * html->attr
 */
static void enter_attr(statemachine_ctx *ctx, int start, char chr, int end)
{
    htmlparser_ctx *html = CAST(htmlparser_ctx *, ctx->user);
    assert(html != NULL);

    html->attr[0] = '\0';
    statemachine_start_record(ctx);
}

/* Called when the parser exits the attribute name in order to finalize the
 * recording.
 *
 * It converts the tag name to lowercase.
 */
static void exit_attr(statemachine_ctx *ctx, int start, char chr, int end)
{
    htmlparser_ctx *html = CAST(htmlparser_ctx *, ctx->user);
    assert(html != NULL);

    nopad_strncpy(html->attr, statemachine_stop_record(ctx),
                  HTMLPARSER_MAX_STRING, statemachine_record_length(ctx));

    tolower_str(html->attr);
}

/* Called when we enter an attribute value.
 *
 * Keeps track of a position index inside the value and initializes the
 * javascript state machine for attributes that accept javascript.
 */
static void enter_value(statemachine_ctx *ctx, int start, char chr, int end)
{
  htmlparser_ctx *html = CAST(htmlparser_ctx *, ctx->user);
  assert(html != NULL);

  html->value_index = 0;

  if (is_js_attribute(html->attr)) {
    entityfilter_reset(html->entityfilter);
    jsparser_reset(html->jsparser);
    html->in_js = 1;
  } else {
    html->in_js = 0;
  }
}

/* Called when we enter the contents of an attribute value.
 *
 * Initializes the recording of the contents of the value.
 */
static void enter_value_content(statemachine_ctx *ctx, int start, char chr,
                                int end)
{
  htmlparser_ctx *html = CAST(htmlparser_ctx *, ctx->user);
  assert(html != NULL);

  html->value[0] = '\0';
  statemachine_start_record(ctx);
}

/* Called when we exit the contents of an attribute value.
 *
 * Finalizes the recording of the contents of the value.
 */
static void exit_value_content(statemachine_ctx *ctx, int start, char chr,
                                int end)
{
  htmlparser_ctx *html = CAST(htmlparser_ctx *, ctx->user);
  assert(html != NULL);

  nopad_strncpy(html->value, statemachine_stop_record(ctx),
                HTMLPARSER_MAX_STRING, statemachine_record_length(ctx));

  html->in_js = 0;
}

/* Called for every character inside an attribute value.
 *
 * Used to process javascript and keep track of the position index inside the
 * attribute value.
 */
static void in_state_value(statemachine_ctx *ctx, int start, char chr, int end)
{
  htmlparser_ctx *html = CAST(htmlparser_ctx *, ctx->user);
  assert(html != NULL);

  html->value_index++;

  if (html->in_js == 1) {
    const char *output;
    output = entityfilter_process(html->entityfilter, chr);
    jsparser_parse_str(html->jsparser, output);
  }
}

/* Called everytime the parser leaves a tag definition.
 *
 * When we encounter a script tag, we initialize the js parser and switch the
 * state to cdata. We also switch to the cdata state when we encounter any
 * other CDATA/RCDATA tag (style, title or textarea) except that we do not
 * initialize the js parser.
 *
 * To simplify the code, we treat RCDATA and CDATA sections the same since the
 * differences between them don't affect the context we are in.
 */
static void tag_close(statemachine_ctx *ctx, int start, char chr, int end)
{
    htmlparser_ctx *html = CAST(htmlparser_ctx *, ctx->user);
    assert(html != NULL);

    if (strcmp(html->tag, "script") == 0) {
      ctx->next_state = HTMLPARSER_STATE_INT_CDATA_TEXT;
      jsparser_reset(html->jsparser);
      html->in_js = 1;
    } else if (strcmp(html->tag, "style") == 0 ||
               strcmp(html->tag, "title") == 0 ||
               strcmp(html->tag, "textarea") == 0) {
      ctx->next_state = HTMLPARSER_STATE_INT_CDATA_TEXT;
      html->in_js = 0;
    }
}

/* Called inside cdata blocks in order to parse the javascript.
 *
 * Calls the javascript parser if currently in a script tag.
 */
static void in_state_cdata(statemachine_ctx *ctx, int start, char chr, int end)
{
  htmlparser_ctx *html = CAST(htmlparser_ctx *, ctx->user);
  assert(html != NULL);

  if (html->in_js)
    jsparser_parse_chr(html->jsparser, chr);
}

/* Called if we encounter a '<' character in a cdata section.
 *
 * When encountering a '<' character inside cdata, we need to find the closing
 * tag name in order to know if the tag is going to be closed or not, so we
 * start recording the name of what could be the closing tag.
 */
static void enter_state_cdata_may_close(statemachine_ctx *ctx, int start,
                                        char chr, int end)
{
  statemachine_start_record(ctx);
}

/* Called when we finish reading what could be a closing cdata tag.
 *
 * Checks if the closing tag name matches the current entity, and if so closes
 * the element.
 */
static void exit_state_cdata_may_close(statemachine_ctx *ctx, int start,
                                       char chr, int end)
{
  htmlparser_ctx *html = CAST(htmlparser_ctx *, ctx->user);
  const char *cdata_close_tag;
  assert(html != NULL);

  cdata_close_tag = statemachine_stop_record(ctx);
  assert(cdata_close_tag[0] == '/');

  if (strcasecmp(&cdata_close_tag[1], html->tag) == 0 &&
      (chr == '>' || html_isspace(chr))) { /* Make sure we have a delimiter */
    html->tag[0] = '\0';  /* Empty tag mimicking exit_tag_name(). */
    html->in_js = 0;  /* In case this was a script tag. */
  } else {
    /* Does not close the CDATA section. Go back to CDATA. */
    ctx->next_state = HTMLPARSER_STATE_INT_CDATA_TEXT;
  }
}

/* Resets the parser to it's initial state and changes the parser mode.
 */
void htmlparser_reset_mode(htmlparser_ctx *ctx, int mode)
{
  assert(ctx != NULL);
  statemachine_reset(ctx->statemachine);
  ctx->in_js = 0;
  ctx->tag[0] = '\0';
  ctx->attr[0] = '\0';
  ctx->value[0] = '\0';

  jsparser_reset(ctx->jsparser);

  switch (mode) {
    case HTMLPARSER_MODE_HTML:
      ctx->statemachine->current_state = HTMLPARSER_STATE_INT_TEXT;
      break;
    case HTMLPARSER_MODE_JS:
      ctx->statemachine->current_state = HTMLPARSER_STATE_INT_JS_FILE;
      ctx->in_js = 1;
      break;
    case HTMLPARSER_MODE_CSS:
      ctx->statemachine->current_state = HTMLPARSER_STATE_INT_CSS_FILE;
      break;
    case HTMLPARSER_MODE_HTML_IN_TAG:
      ctx->statemachine->current_state = HTMLPARSER_STATE_INT_TAG_SPACE;
      break;
    default:
      assert("Invalid mode in htmlparser_reset_mode()." && 0);
  }
}

/* Resets the parser to it's initial state and to the default mode, which
 * is MODE_HTML.
 */
void htmlparser_reset(htmlparser_ctx *ctx)
{
    assert(ctx != NULL);
    htmlparser_reset_mode(ctx, HTMLPARSER_MODE_HTML);
}

/* Creates a new state machine definition and initializes the events for the
 * state transitions.
 *
 * Although each instance of the parser has it's own private instance of a
 * statemachine definition, they are still identical across html parser objects
 * and are never modified after creation. As such, changes to this definition
 * should not occur outside this function and should not depend on properties
 * of this particular parser instance as in the future we may opt to use a
 * single public definition across parser objects.
 */
static statemachine_definition *create_statemachine_definition()
{
  statemachine_definition *def;
  def = statemachine_definition_new(HTMLPARSER_NUM_STATES);
  if (def == NULL)
    return NULL;

  statemachine_definition_populate(def, htmlparser_state_transitions,
                                   htmlparser_states_internal_names);

  statemachine_enter_state(def, HTMLPARSER_STATE_INT_TAG_NAME,
                           enter_tag_name);
  statemachine_exit_state(def, HTMLPARSER_STATE_INT_TAG_NAME, exit_tag_name);

  statemachine_enter_state(def, HTMLPARSER_STATE_INT_ATTR, enter_attr);
  statemachine_exit_state(def, HTMLPARSER_STATE_INT_ATTR, exit_attr);

  statemachine_enter_state(def, HTMLPARSER_STATE_INT_TAG_CLOSE, tag_close);

  /* CDATA states. We must list all cdata and javascript states here. */
  /* TODO(falmeida): Declare this list in htmlparser_fsm.config so it doesn't
   * go out of sync.
   */
  statemachine_in_state(def, HTMLPARSER_STATE_INT_CDATA_TEXT, in_state_cdata);
  statemachine_in_state(def, HTMLPARSER_STATE_INT_CDATA_COMMENT_START,
                        in_state_cdata);
  statemachine_in_state(def, HTMLPARSER_STATE_INT_CDATA_COMMENT_START_DASH,
                        in_state_cdata);
  statemachine_in_state(def, HTMLPARSER_STATE_INT_CDATA_COMMENT_BODY,
                        in_state_cdata);
  statemachine_in_state(def, HTMLPARSER_STATE_INT_CDATA_COMMENT_DASH,
                        in_state_cdata);
  statemachine_in_state(def, HTMLPARSER_STATE_INT_CDATA_COMMENT_DASH_DASH,
                        in_state_cdata);
  statemachine_in_state(def, HTMLPARSER_STATE_INT_CDATA_LT, in_state_cdata);
  statemachine_in_state(def, HTMLPARSER_STATE_INT_CDATA_MAY_CLOSE,
                        in_state_cdata);

  /* For simplification, we treat the javascript mode as if it were cdata. */
  statemachine_in_state(def, HTMLPARSER_STATE_INT_JS_FILE, in_state_cdata);

  statemachine_enter_state(def, HTMLPARSER_STATE_INT_CDATA_MAY_CLOSE,
                           enter_state_cdata_may_close);
  statemachine_exit_state(def, HTMLPARSER_STATE_INT_CDATA_MAY_CLOSE,
                          exit_state_cdata_may_close);
  /* value states */
  statemachine_enter_state(def, HTMLPARSER_STATE_INT_VALUE, enter_value);

  /* Called when we enter the content of the value */
  statemachine_enter_state(def, HTMLPARSER_STATE_INT_VALUE_TEXT,
                           enter_value_content);
  statemachine_enter_state(def, HTMLPARSER_STATE_INT_VALUE_Q,
                           enter_value_content);
  statemachine_enter_state(def, HTMLPARSER_STATE_INT_VALUE_DQ,
                           enter_value_content);

  /* Called when we exit the content of the value */
  statemachine_exit_state(def, HTMLPARSER_STATE_INT_VALUE_TEXT,
                          exit_value_content);
  statemachine_exit_state(def, HTMLPARSER_STATE_INT_VALUE_Q,
                          exit_value_content);
  statemachine_exit_state(def, HTMLPARSER_STATE_INT_VALUE_DQ,
                          exit_value_content);

  statemachine_in_state(def, HTMLPARSER_STATE_INT_VALUE_TEXT, in_state_value);
  statemachine_in_state(def, HTMLPARSER_STATE_INT_VALUE_Q, in_state_value);
  statemachine_in_state(def, HTMLPARSER_STATE_INT_VALUE_DQ, in_state_value);

  return def;
}


/* Initializes a new htmlparser instance.
 *
 * Returns a pointer to the new instance or NULL if the initialization fails.
 * Initialization failure is fatal, and if this function fails it may not
 * deallocate all previsouly allocated memory.
 */
htmlparser_ctx *htmlparser_new()
{
  htmlparser_ctx *html;

  html = CAST(htmlparser_ctx *, calloc(1, sizeof(htmlparser_ctx)));
  if (html == NULL)
    return NULL;

  html->statemachine_def = create_statemachine_definition();
  if (html->statemachine_def == NULL)
    return NULL;

  html->statemachine = statemachine_new(html->statemachine_def, html);
  if (html->statemachine == NULL)
    return NULL;

  html->jsparser = jsparser_new();
  if (html->jsparser == NULL)
    return NULL;

  html->entityfilter = entityfilter_new();
  if (html->entityfilter == NULL)
    return NULL;

  htmlparser_reset(html);

  return html;
}

/* Copies the context of the htmlparser pointed to by src to the htmlparser dst.
 */
void htmlparser_copy(htmlparser_ctx *dst, const htmlparser_ctx *src)
{
  dst->value_index = src->value_index;
  dst->in_js = src->in_js;
  strcpy(dst->tag, src->tag);
  strcpy(dst->attr, src->attr);
  strcpy(dst->value, src->value);

  statemachine_copy(dst->statemachine,
                    src->statemachine,
                    dst->statemachine_def,
                    dst);

  jsparser_copy(dst->jsparser, src->jsparser);

  entityfilter_copy(dst->entityfilter, src->entityfilter);

}

/* Receives an htmlparser context and Returns the current html state.
 */
int htmlparser_state(htmlparser_ctx *ctx)
{
  return state_external(ctx->statemachine->current_state);
}

/* Parses the input html stream and returns the finishing state.
 */
int htmlparser_parse(htmlparser_ctx *ctx, const char *str, int size)
{
    int internal_state;
    internal_state = statemachine_parse(ctx->statemachine, str, size);
    return state_external(internal_state);
}


/* Returns true if the parser is inside an attribute value and the value is
 * surrounded by single or double quotes. */
int htmlparser_is_attr_quoted(htmlparser_ctx *ctx) {
  int st = statemachine_get_state(ctx->statemachine);
  if (st == HTMLPARSER_STATE_INT_VALUE_Q_START ||
      st == HTMLPARSER_STATE_INT_VALUE_Q ||
      st == HTMLPARSER_STATE_INT_VALUE_DQ_START ||
      st == HTMLPARSER_STATE_INT_VALUE_DQ)
      return 1;
  else
      return 0;
}

/* Returns true if the parser is currently in javascript.
 */
int htmlparser_in_js(htmlparser_ctx *ctx) {
  int st = statemachine_get_state(ctx->statemachine);

/* CDATA states plus JS_FILE. We must list all cdata and javascript states
 * here. */
/* TODO(falmeida): Declare this list in htmlparser_fsm.config so it doesn't go
 * out of sync. */
  if (ctx->in_js &&
      (st == HTMLPARSER_STATE_INT_CDATA_TEXT ||
       st == HTMLPARSER_STATE_INT_CDATA_COMMENT_START ||
       st == HTMLPARSER_STATE_INT_CDATA_COMMENT_START_DASH ||
       st == HTMLPARSER_STATE_INT_CDATA_COMMENT_BODY ||
       st == HTMLPARSER_STATE_INT_CDATA_COMMENT_DASH ||
       st == HTMLPARSER_STATE_INT_CDATA_COMMENT_DASH_DASH ||
       st == HTMLPARSER_STATE_INT_CDATA_LT ||
       st == HTMLPARSER_STATE_INT_CDATA_MAY_CLOSE ||
       st == HTMLPARSER_STATE_INT_JS_FILE))
    return 1;

  if (state_external(st) == HTMLPARSER_STATE_VALUE && ctx->in_js)
      return 1;
  else
      return 0;
}

/* Returns the current tag or NULL if not available or we haven't seen the
 * entire tag yet.
 */
const char *htmlparser_tag(htmlparser_ctx *ctx)
{
  if (ctx->tag[0] != '\0')
    return ctx->tag;
  else
    return NULL;
}

/* Returns true if inside an attribute or a value */
int htmlparser_in_attr(htmlparser_ctx *ctx)
{
    int ext_state = state_external(statemachine_get_state(ctx->statemachine));
    return ext_state == HTMLPARSER_STATE_ATTR ||
           ext_state == HTMLPARSER_STATE_VALUE;
}

/* Returns the current attribute name if after an attribute name or in an
 * attribute value. Returns NULL otherwise. */
const char *htmlparser_attr(htmlparser_ctx *ctx)
{
  if (htmlparser_in_attr(ctx))
    return ctx->attr;
  else
    return NULL;
}

/* Returns true if the parser is currently inside a CSS construct.
 */
int htmlparser_in_css(htmlparser_ctx *ctx) {
  int state = statemachine_get_state(ctx->statemachine);
  const char *tag = htmlparser_tag(ctx);
  int external_state = state_external(state);

  if (state == HTMLPARSER_STATE_INT_CSS_FILE ||
      (external_state == HTMLPARSER_STATE_VALUE &&
       htmlparser_attr_type(ctx) == HTMLPARSER_ATTR_STYLE) ||
      (tag && strcmp(tag, "style") == 0)) {
    return 1;
  } else {
    return 0;
  }
}

/* Returns the contents of the current attribute value.
 */
const char *htmlparser_value(htmlparser_ctx *ctx)
{
  int ext_state = state_external(statemachine_get_state(ctx->statemachine));
  if (ext_state == HTMLPARSER_STATE_VALUE) {
    strncpy(ctx->value, statemachine_record_buffer(ctx->statemachine),
            HTMLPARSER_MAX_STRING);
    ctx->value[HTMLPARSER_MAX_STRING - 1] = '\0';
    return ctx->value;
  } else {
    return NULL;
  }
}


/* Returns the current state of the javascript state machine
 *
 * Currently only present for testing purposes.
 */
int htmlparser_js_state(htmlparser_ctx *ctx)
{
   return jsparser_state(ctx->jsparser);
}

/* True is currently inside a javascript string literal
 */
int htmlparser_is_js_quoted(htmlparser_ctx *ctx)
{
    if (htmlparser_in_js(ctx)) {
      int st = jsparser_state(ctx->jsparser);
      if (st == JSPARSER_STATE_Q ||
          st == JSPARSER_STATE_DQ)
        return 1;
    }
    return 0;
}

/* True if currently inside an attribute value
 */
int htmlparser_in_value(htmlparser_ctx *ctx)
{
    int ext_state = state_external(statemachine_get_state(ctx->statemachine));
    return ext_state == HTMLPARSER_STATE_VALUE;
}

/* Returns the position inside the current attribute value
 */
int htmlparser_value_index(htmlparser_ctx *ctx)
{
    if (htmlparser_in_value(ctx))
        return ctx->value_index;

    return -1;
}

/* Returns true if this is the first character of a url inside an attribute.
 */
int htmlparser_is_url_start(htmlparser_ctx *ctx)
{
  if (htmlparser_attr_type(ctx) == HTMLPARSER_ATTR_URI) {
    const char* tag = htmlparser_tag(ctx);
    /*const char* attr =*/ htmlparser_attr(ctx);

    if ((tag && strcmp(tag, "meta") == 0 &&
         meta_redirect_type(htmlparser_value(ctx)) ==
         META_REDIRECT_TYPE_URL_START) ||
        htmlparser_value_index(ctx) == 0)
      return 1;

  }
  return 0;
}

/* Returns the current attribute type.
 */
int htmlparser_attr_type(htmlparser_ctx *ctx)
{
    if (!htmlparser_in_attr(ctx))
        return HTMLPARSER_ATTR_NONE;

    if (is_js_attribute(ctx->attr))
        return HTMLPARSER_ATTR_JS;

    if (is_uri_attribute(ctx->attr))
        return HTMLPARSER_ATTR_URI;

    if (is_style_attribute(ctx->attr))
        return HTMLPARSER_ATTR_STYLE;

    const char* tag = htmlparser_tag(ctx);
    const char* attr = htmlparser_attr(ctx);

    /* Special logic to handle meta redirect type tags. */
    if (tag && strcmp(tag, "meta") == 0 &&
        attr && strcmp(attr, "content") == 0) {

      const char* value = htmlparser_value(ctx);
      meta_redirect_type_enum redirect_type = meta_redirect_type(value);

      if (redirect_type == META_REDIRECT_TYPE_URL ||
          redirect_type == META_REDIRECT_TYPE_URL_START)
        return HTMLPARSER_ATTR_URI;
    }

    return HTMLPARSER_ATTR_REGULAR;
}

/* Return the current line number. */
int htmlparser_get_line_number(htmlparser_ctx *ctx) {
  return statemachine_get_line_number(ctx->statemachine);
}

/* Set the current line number. */
void htmlparser_set_line_number(htmlparser_ctx *ctx, int line) {
  statemachine_set_line_number(ctx->statemachine, line);
}

/* Return the current column number. */
int htmlparser_get_column_number(htmlparser_ctx *ctx) {
  return statemachine_get_column_number(ctx->statemachine);
}

/* Set the current column number. */
void htmlparser_set_column_number(htmlparser_ctx *ctx, int column) {
  statemachine_set_column_number(ctx->statemachine, column);
}

/* Retrieve a human readable error message in case an error occurred.
 *
 * NULL is returned if the parser didn't encounter an error.
 */
const char *htmlparser_get_error_msg(htmlparser_ctx *ctx) {
  return statemachine_get_error_msg(ctx->statemachine);
}

/* Invoked by the caller when text is expanded by the caller.
 */
int htmlparser_insert_text(htmlparser_ctx *ctx)
{
  /* TODO(falmeida): Generalize and use a table for these values. */

  if (statemachine_get_state(ctx->statemachine) == HTMLPARSER_STATE_INT_VALUE) {
    statemachine_set_state(ctx->statemachine, HTMLPARSER_STATE_INT_VALUE_TEXT);
  }
  return 1;
}

/* Deallocates an htmlparser context object.
 */
void htmlparser_delete(htmlparser_ctx *ctx)
{
    assert(ctx != NULL);
    statemachine_definition_delete(ctx->statemachine_def);
    statemachine_delete(ctx->statemachine);
    jsparser_delete(ctx->jsparser);
    entityfilter_delete(ctx->entityfilter);
    free(ctx);
}

#ifdef __cplusplus
}  /* namespace security_streamhtmlparser */
#endif
