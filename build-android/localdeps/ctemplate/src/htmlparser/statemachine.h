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

#ifndef SECURITY_STREAMHTMLPARSER_STATEMACHINE_H
#define SECURITY_STREAMHTMLPARSER_STATEMACHINE_H

#include <config.h>
#ifdef __cplusplus
namespace ctemplate_htmlparser {
#endif

/* TODO(falmeida): I'm not sure about these limits, but since right now we only
 * have 24 states it should be fine */

enum {
    STATEMACHINE_ERROR = 127
};

#define STATEMACHINE_RECORD_BUFFER_SIZE 256

#define STATEMACHINE_MAX_STR_ERROR 80

struct statemachine_ctx_s;

typedef void(*state_event_function)(struct statemachine_ctx_s *, int, char,
                                    int);

typedef struct statemachine_definition_s {
    int num_states;
    const int* const* transition_table;

    /* Array containing the name of the states as a C string.
     * This field is optional and if not in use it should be set to NULL.
     */
    const char* const* state_names;
    state_event_function *in_state_events;
    state_event_function *enter_state_events;
    state_event_function *exit_state_events;
} statemachine_definition;

typedef struct statemachine_ctx_s {
    int current_state;
    int next_state;
    statemachine_definition *definition;
    char current_char;

    /* Current line number. */
    int line_number;

    /* Current column number. */
    int column_number;
    char record_buffer[STATEMACHINE_RECORD_BUFFER_SIZE];
    size_t record_pos;

    /* True if we are recording the stream to record_buffer. */
    int recording;

    /* In case there was an error (we are in state STATEMACHINE_ERROR), it will
     * contain a human readable description of the error.
     */
    char error_msg[STATEMACHINE_MAX_STR_ERROR];

    /* Storage space for the layer above. */
    void *user;
} statemachine_ctx;

/* Populates the statemachine definition.
 *
 * Receives a transition table and an optional array of state names. It uses
 * this data to populate the state machine definition.
 *
 * The transition table structure is a list of lists of ints (int **). The
 * outer list indexes the source state and the inner list contains the
 * destination state for each of the possible input characters:
 *
 * const int* const* transitions[source][input] == destination.
 *
 * The optional argument state_names points to a list of strings containing
 * human readable state names. These strings are used when reporting error
 * messages.
 */
void statemachine_definition_populate(statemachine_definition *def,
                                     const int* const* transition_table,
                                     const char* const* state_names);

void statemachine_in_state(statemachine_definition *def, int st,
                           state_event_function func);
void statemachine_enter_state(statemachine_definition *def, int st,
                                     state_event_function func);
void statemachine_exit_state(statemachine_definition *def, int st,
                                    state_event_function func);

statemachine_definition *statemachine_definition_new(int states);
void statemachine_definition_delete(statemachine_definition *def);

int statemachine_get_state(statemachine_ctx *ctx);
void statemachine_set_state(statemachine_ctx *ctx, int state);

void statemachine_start_record(statemachine_ctx *ctx);
const char *statemachine_stop_record(statemachine_ctx *ctx);
const char *statemachine_record_buffer(statemachine_ctx *ctx);

/* Returns the the number of characters currently stored in the record buffer.
 */
static inline size_t statemachine_record_length(statemachine_ctx *ctx) {
  return ctx->record_pos + 1;
}

/* Return the current line number. */
static inline int statemachine_get_line_number(statemachine_ctx *ctx) {
  return ctx->line_number;
}

/* Set the current line number. */
static inline void statemachine_set_line_number(statemachine_ctx *ctx,
                                                int line) {
  ctx->line_number = line;
}

/* Return the current column number. */
static inline int statemachine_get_column_number(statemachine_ctx *ctx) {
  return ctx->column_number;
}

/* Set the current column number. */
static inline void statemachine_set_column_number(statemachine_ctx *ctx,
                                                  int column) {
  ctx->column_number = column;
}


/* Retrieve a human readable error message in case an error occurred.
 *
 * NULL is returned if the parser didn't encounter an error.
 */
static inline const char *statemachine_get_error_msg(statemachine_ctx *ctx) {
  if (ctx->next_state == STATEMACHINE_ERROR) {
    return ctx->error_msg;
  } else {
    return NULL;
  }
}

/* Reset the statemachine.
 *
 * The state is set to the initialization values. This includes setting the
 * state to the default state (0), stopping recording and setting the line
 * number to 1.
 */
void statemachine_reset(statemachine_ctx *ctx);

/* Initializes a new statemachine. Receives a statemachine definition object
 * that should have been initialized with statemachine_definition_new() and a
 * user reference to be used by the caller.
 *
 * Returns NULL if initialization fails.
 *
 * Initialization failure is fatal, and if this function fails it may not
 * deallocate all previsouly allocated memory.
 */
statemachine_ctx *statemachine_new(statemachine_definition *def,
                                   void *user);

/* Returns a pointer to a context which is a duplicate of the statemachine src.
 * The statemachine definition and the user pointer have to be provided since
 * these references are not owned by the statemachine itself.
 */
statemachine_ctx *statemachine_duplicate(statemachine_ctx *ctx,
                                         statemachine_definition *def,
                                         void *user);

/* Copies the context of the statemachine pointed to by src to the statemachine
 * provided by dst.
 * The statemachine definition and the user pointer have to be provided since
 * these references are not owned by the statemachine itself.
 */
void statemachine_copy(statemachine_ctx *dst,
                       statemachine_ctx *src,
                       statemachine_definition *def,
                       void *user);

int statemachine_parse(statemachine_ctx *ctx, const char *str, int size);

void statemachine_delete(statemachine_ctx *ctx);


/*****
 * The following functions are only exported for testing purposes and should
 * be treated as private. */


/* Encode the character as an escaped C string.
 *
 * Encode the character chr into the string output. Writes at most len
 * characters to the output string but makes sure output is NULL terminated.
 */
void statemachine_encode_char(char chr, char *output, size_t len);


#ifdef __cplusplus
}  /* namespace security_streamhtmlparser */
#endif

#endif /* SECURITY_STREAMHTMLPARSER_STATEMACHINE_H */
