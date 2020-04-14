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
// Author: csilvers@google.com (Craig Silverstein)
//
// When we expand a template, we expand into an abstract "emitter".
// This is typically a string, but could be a file-wrapper, or any
// other data structure that supports this very simple "append" API.

#ifndef TEMPLATE_TEMPLATE_EMITTER_H_
#define TEMPLATE_TEMPLATE_EMITTER_H_

#include <sys/types.h>     // for size_t
#include <string>



namespace ctemplate {

class  ExpandEmitter {
 public:
  ExpandEmitter() {}
  virtual ~ExpandEmitter() {}
  virtual void Emit(char c) = 0;
  virtual void Emit(const std::string& s) = 0;
  virtual void Emit(const char* s) = 0;
  virtual void Emit(const char* s, size_t slen) = 0;
};


class  StringEmitter : public ExpandEmitter {
  std::string* const outbuf_;
 public:
  StringEmitter(std::string* outbuf) : outbuf_(outbuf) {}
  virtual void Emit(char c) { *outbuf_ += c; }
  virtual void Emit(const std::string& s) { *outbuf_ += s; }
  virtual void Emit(const char* s) { *outbuf_ += s; }
  virtual void Emit(const char* s, size_t slen) { outbuf_->append(s, slen); }
};

}

#endif  // TEMPLATE_TEMPLATE_EMITTER_H_
