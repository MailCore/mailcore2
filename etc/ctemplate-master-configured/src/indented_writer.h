// Copyright (c) 2009, Google Inc.
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
// Author: williasr@google.com (Scott Williams)

#ifndef TEMPLATE_INDENTED_WRITER_H_
#define TEMPLATE_INDENTED_WRITER_H_

#include <config.h>
#include <string>

namespace ctemplate {

using std::string;

// An indented writer is a wrapper around a string buffer. It takes care of
// tracking and applying leading whitespace to the buffer at the beginning of
// new lines.
class IndentedWriter {
 public:
  IndentedWriter(string* out, int starting_indentation)
    : out_(out), current_indentation_(starting_indentation),
      original_indentation_(starting_indentation), line_state_(AT_BEGINNING) { }

  ~IndentedWriter() {
    assert(original_indentation_ == current_indentation_);
  }

  // Append some output to the buffer. If the string ends with a newline, then
  // the output buffer will be indented before the next Write() call. If the
  // output contains embedded newlines, these won't have proper indentation, so
  // call Write() at least once per physical line of output.
  void Write(string s1,
             string s2 = string(),
             string s3 = string(),
             string s4 = string(),
             string s5 = string(),
             string s6 = string(),
             string s7 = string()) {
    DoWrite(s1);
    if (!s2.empty()) DoWrite(s2);
    if (!s3.empty()) DoWrite(s3);
    if (!s4.empty()) DoWrite(s4);
    if (!s5.empty()) DoWrite(s5);
    if (!s6.empty()) DoWrite(s6);
    if (!s7.empty()) DoWrite(s7);
  }

  // Increment the indentation level. This only has a meaning after outputting a
  // complete line (otherwise, are you saying you want to modify the indentation
  // of the current line or the next line?)
  void Indent() {
    assert(line_state_ == AT_BEGINNING);
    current_indentation_ += kIndent;
  }

  // Decrement the indentation level. This only has a meaning after outputting a
  // complete line (otherwise, are you saying you want to modify the indentation
  // of the current line or the next line?)
  void Dedent() {
    assert(line_state_ == AT_BEGINNING);
    current_indentation_ -= kIndent;
    assert(current_indentation_ >= original_indentation_);
  }

  // Get access to the underlying indentation level and string buffer. Most
  // useful for interfacing with non-IndentedWriter printing code.
  int GetIndent() const { return current_indentation_; }
  string* GetBuffer() { return out_; }

 private:
  void DoWrite(const string& line) {
    if (line_state_ == AT_BEGINNING) {
      IndentLine();
    }
    out_->append(line);
    if (EndsWithNewline(line)) {
      line_state_ = AT_BEGINNING;
    } else {
      line_state_ = MID_LINE;
    }
  }

  static bool EndsWithNewline(const string& line) {
    return !line.empty() && (*(line.end() - 1) == '\n');
  }

  void IndentLine() {
    assert(line_state_ == AT_BEGINNING);
    out_->append(string(current_indentation_, ' ') +
                 (current_indentation_ ? " " : ""));
  }

  string* out_;
  int current_indentation_;
  int original_indentation_;
  enum LineState {
    AT_BEGINNING,
    MID_LINE
  } line_state_;

  const static int kIndent = 2;            // num spaces to indent each level
};

}

#endif  // TEMPLATE_INDENTED_WRITER_H_
