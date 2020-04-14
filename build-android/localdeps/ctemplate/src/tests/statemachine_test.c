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
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "htmlparser/statemachine.h"

enum {
  SIMPLE_STATE_A,
  SIMPLE_STATE_B,
  SIMPLE_STATE_C,
  SIMPLE_STATE_D,
  SIMPLE_STATE_ERROR_TEST
};

/* Include the test state machine definition. */
#include "tests/statemachine_test_fsm.h"

/* Taken from google templates */

#define ASSERT(cond)  do {                                      \
  if (!(cond)) {                                                \
    printf("%s: %d: ASSERT FAILED: %s\n", __FILE__, __LINE__,   \
           #cond);                                              \
    assert(cond);                                               \
    exit(1);                                                    \
  }                                                             \
} while (0)

#define ASSERT_STREQ(a, b)  do {                                          \
  if (strcmp((a), (b))) {                                                 \
    printf("%s: %d: ASSERT FAILED: '%s' != '%s'\n", __FILE__, __LINE__,   \
           (a), (b));                                                     \
    assert(!strcmp((a), (b)));                                            \
    exit(1);                                                              \
  }                                                                       \
} while (0)

#define ASSERT_STRSTR(text, substr)  do {                       \
  if (!strstr((text), (substr))) {                              \
    printf("%s: %d: ASSERT FAILED: '%s' not in '%s'\n",         \
           __FILE__, __LINE__, (substr), (text));               \
    assert(strstr((text), (substr)));                           \
    exit(1);                                                    \
  }                                                             \
} while (0)


#define NUM_STATES 10

/* To simply the tests */
#define statemachine_parse_str(a,b) statemachine_parse(a, b, strlen(b));

/* Simple state machine test. */
int test_simple()
{
  statemachine_definition *def;
  statemachine_ctx *sm;
  def = statemachine_definition_new(NUM_STATES);
  sm = statemachine_new(def, NULL);

  statemachine_definition_populate(def, simple_state_transitions,
                                   simple_states_internal_names);
  ASSERT(sm->current_state == SIMPLE_STATE_A);

  statemachine_parse(sm, "001", 3);
  ASSERT(sm->current_state == SIMPLE_STATE_B);

  statemachine_parse(sm, "001", 3);
  ASSERT(sm->current_state == SIMPLE_STATE_C);

  statemachine_parse(sm, "2", 1);
  ASSERT(sm->current_state == SIMPLE_STATE_B);

  statemachine_parse(sm, "11", 2);
  ASSERT(sm->current_state == SIMPLE_STATE_D);

  statemachine_delete(sm);
  return 0;
}

/* Tests error handling logic when we try to follow non existent transitions. */
int test_error()
{
  statemachine_definition *def;
  statemachine_ctx *sm;
  int res;

  def = statemachine_definition_new(NUM_STATES);
  sm = statemachine_new(def, NULL);

  statemachine_definition_populate(def, simple_state_transitions,
                                   NULL);
  ASSERT(sm->current_state == SIMPLE_STATE_A);

  ASSERT(statemachine_get_error_msg(sm) == NULL);

  res = statemachine_parse_str(sm, "00E");
  ASSERT(sm->current_state == SIMPLE_STATE_ERROR_TEST);
  ASSERT(sm->current_state == res);

  res = statemachine_parse(sm, "3", 1);
  ASSERT(res == STATEMACHINE_ERROR);
  ASSERT_STREQ(statemachine_get_error_msg(sm),
               "Unexpected character '3'");

  statemachine_reset(sm);
  ASSERT(statemachine_get_error_msg(sm) == NULL);

  statemachine_delete(sm);

  def = statemachine_definition_new(NUM_STATES);
  sm = statemachine_new(def, NULL);

  statemachine_definition_populate(def, simple_state_transitions,
                                   simple_states_internal_names);
  ASSERT(sm->current_state == SIMPLE_STATE_A);

  res = statemachine_parse_str(sm, "00E");
  ASSERT(sm->current_state == SIMPLE_STATE_ERROR_TEST);
  ASSERT(sm->current_state == res);

  res = statemachine_parse(sm, "3", 1);
  ASSERT(res == STATEMACHINE_ERROR);
  ASSERT_STREQ(statemachine_get_error_msg(sm),
               "Unexpected character '3' in state 'error_test'");

  statemachine_delete(sm);

  return 0;
}

/* Tests htmlparser_start_record() and htmlparser_end_record() logic. */

int test_record()
{
  statemachine_definition *def;
  statemachine_ctx *sm;
  const char *actual;
  char expected[STATEMACHINE_RECORD_BUFFER_SIZE];
  int res;
  int counter;
  def = statemachine_definition_new(NUM_STATES);
  sm = statemachine_new(def, NULL);

  statemachine_definition_populate(def, simple_state_transitions,
                                   simple_states_internal_names);

  ASSERT(sm->current_state == SIMPLE_STATE_A);

  res = statemachine_parse_str(sm, "001");
  ASSERT(sm->current_state == SIMPLE_STATE_B);
  ASSERT(sm->current_state == res);

  statemachine_start_record(sm);
  statemachine_parse_str(sm, "121212");
  ASSERT_STREQ("121212", statemachine_stop_record(sm));

  statemachine_parse_str(sm, "not recorded");

  statemachine_start_record(sm);
  statemachine_parse_str(sm, "121212000");
  ASSERT_STREQ("121212000", statemachine_stop_record(sm));

  statemachine_start_record(sm);
  statemachine_parse_str(sm, "1234567890");
  ASSERT_STREQ("1234567890", statemachine_record_buffer(sm));

  statemachine_parse_str(sm, "test");
  ASSERT_STREQ("1234567890test", statemachine_stop_record(sm));

  statemachine_start_record(sm);

  /* Record 1000 chars + strlen("beginning-") */
  statemachine_parse_str(sm, "beginning-");
  for (counter = 0; counter < 100; counter++) {
    statemachine_parse_str(sm, "1234567890");
  }

  /* Make sure we preserved the start of the buffer. */
  ASSERT_STRSTR(statemachine_record_buffer(sm), "beginning-");

  /* And make sure the size is what we expect. */
  ASSERT(STATEMACHINE_RECORD_BUFFER_SIZE - 1 ==
         strlen(statemachine_stop_record(sm)));

  statemachine_start_record(sm);
  for (counter = 0; counter < 100; counter++) {
    statemachine_parse_str(sm, "0123456789ABCDEF");
  }

  expected[0] = '\0';
  /* Fill the buffer with a pattern 255 chars long (16 * 15 + 15). */
  for (counter = 0; counter < 15; counter++) {
    strcat(expected, "0123456789ABCDEF");
  }
  strcat(expected, "0123456789ABCDE");
  actual = statemachine_stop_record(sm);
  ASSERT_STREQ(expected, actual);

  statemachine_delete(sm);
  return 0;
}

/* Test with characters outside of the ascii range */
int test_no_ascii()
{
  statemachine_definition *def;
  statemachine_ctx *sm;
  def = statemachine_definition_new(NUM_STATES);
  sm = statemachine_new(def, NULL);

  statemachine_definition_populate(def, simple_state_transitions,
                                   simple_states_internal_names);

  ASSERT(sm->current_state == SIMPLE_STATE_A);

  statemachine_parse(sm, "\xf0\xf0\xf1", 3);
  ASSERT(sm->current_state == SIMPLE_STATE_B);

  statemachine_parse(sm, "\xf0\xf0\xf1", 3);
  ASSERT(sm->current_state == SIMPLE_STATE_C);

  statemachine_parse(sm, "\xf2", 1);
  ASSERT(sm->current_state == SIMPLE_STATE_B);

  statemachine_parse(sm, "\xf1\xf1", 2);
  ASSERT(sm->current_state == SIMPLE_STATE_D);

  statemachine_delete(sm);
  return 0;

}

int test_copy()
{
  statemachine_definition *def;
  statemachine_ctx *sm1;
  statemachine_ctx *sm2;
  statemachine_ctx *sm3;
  def = statemachine_definition_new(NUM_STATES);
  sm1 = statemachine_new(def, NULL);

  statemachine_definition_populate(def, simple_state_transitions,
                                   simple_states_internal_names);

  ASSERT(sm1->current_state == SIMPLE_STATE_A);

  sm2 = statemachine_duplicate(sm1, def, NULL);
  ASSERT(sm2->current_state == SIMPLE_STATE_A);

  statemachine_parse(sm1, "001", 3);
  ASSERT(sm1->current_state == SIMPLE_STATE_B);
  ASSERT(sm2->current_state == SIMPLE_STATE_A);


  statemachine_parse(sm1, "001", 3);
  statemachine_parse(sm2, "001", 3);
  ASSERT(sm1->current_state == SIMPLE_STATE_C);
  ASSERT(sm2->current_state == SIMPLE_STATE_B);

  sm3 = statemachine_duplicate(sm2, def, NULL);
  ASSERT(sm3->current_state == SIMPLE_STATE_B);

  statemachine_parse(sm1, "001", 3);
  statemachine_parse(sm2, "001", 3);
  statemachine_parse(sm3, "002", 3);
  ASSERT(sm1->current_state == SIMPLE_STATE_D);
  ASSERT(sm2->current_state == SIMPLE_STATE_C);
  ASSERT(sm3->current_state == SIMPLE_STATE_A);

  statemachine_delete(sm1);
  statemachine_delete(sm2);
  statemachine_delete(sm3);

  return 0;
}

/* Tests statemachine_encode_char().
 */
int test_encode_char()
{
  char encoded_char[10];
  int i;

  struct {
    char chr;
    const char *result;
  } encode_map[] = {
    { 'x', "x" },
    { '0', "0" },
    { '\n', "\\n" },
    { '\r', "\\r" },
    { '\t', "\\t" },
    { '\\', "\\\\" },
    { '\0', "\\x00" },
    { '\xF0', "\\xf0" },
    { '\0', NULL}  // Terminates when output == NULL
  };

  for (i = 0; encode_map[i].result; i++) {
    statemachine_encode_char(encode_map[i].chr, encoded_char,
                             sizeof(encoded_char) / sizeof(*encoded_char));
    ASSERT_STREQ(encoded_char, encode_map[i].result);
  }

  statemachine_encode_char('\xFF', encoded_char, 1);
  ASSERT_STREQ(encoded_char, "");

  statemachine_encode_char('\xFF', encoded_char, 2);
  ASSERT_STREQ(encoded_char, "\\");

  statemachine_encode_char('\xFF', encoded_char, 3);
  ASSERT_STREQ(encoded_char, "\\x");

  statemachine_encode_char('\xFF', encoded_char, 4);
  ASSERT_STREQ(encoded_char, "\\xf");

  statemachine_encode_char('\xFF', encoded_char, 5);
  ASSERT_STREQ(encoded_char, "\\xff");

  return 0;
}

int main(int argc, char **argv)
{
  test_simple();
  test_error();
  test_record();
  test_no_ascii();
  test_copy();
  test_encode_char();
  printf("DONE.\n");
  return 0;
}
