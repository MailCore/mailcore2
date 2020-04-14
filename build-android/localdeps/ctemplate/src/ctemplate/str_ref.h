// Copyright (c) 2012, Olaf van der Spek <olafvdspek@gmail.com>
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
// Author: Olaf van der Spek <olafvdspek@gmail.com>

#ifndef TEMPLATE_STR_REF_H_
#define TEMPLATE_STR_REF_H_

#include <cstddef>



namespace ctemplate {

template <class T>
class str_ref_basic
{
public:
  str_ref_basic()
  {
    clear();
  }

  template <class U>
  str_ref_basic(const U& c)
  {
    if (c.end() != c.begin())
      assign(&*c.begin(), c.end() - c.begin() + &*c.begin());
    else
      clear();
  }

  str_ref_basic(const void* b, const void* e)
  {
    assign(b, e);
  }

  str_ref_basic(const void* b, size_t sz)
  {
    assign(b, sz);
  }

  str_ref_basic(const char* b)
  {
    if (b)
      assign(b, strlen(b));
    else
      clear();
  }

  void clear()
  {
    begin_ = end_ = NULL;
  }

  void assign(const void* b, const void* e)
  {
    begin_ = reinterpret_cast<T>(b);
    end_ = reinterpret_cast<T>(e);
  }

  void assign(const void* b, size_t sz)
  {
    begin_ = reinterpret_cast<T>(b);
    end_ = begin_ + sz;
  }

  T begin() const
  {
    return begin_;
  }

  T end() const
  {
    return end_;
  }

  T data() const
  {
    return begin();
  }

  size_t size() const
  {
    return end() - begin();
  }

  bool empty() const
  {
    return begin() == end();
  }
private:
  T begin_;
  T end_;
};

typedef str_ref_basic<const unsigned char*> data_ref;
typedef str_ref_basic<const char*> str_ref;

}

#endif // TEMPLATE_STR_REF_H_
