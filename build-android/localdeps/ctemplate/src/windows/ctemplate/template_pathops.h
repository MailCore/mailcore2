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

#ifndef TEMPLATE_TEMPLATE_PATHOPS_H_
#define TEMPLATE_TEMPLATE_PATHOPS_H_

#include <string>

// NOTE: if you are statically linking the template library into your binary
// (rather than using the template .dll), set '/D CTEMPLATE_DLL_DECL='
// as a compiler flag in your project file to turn off the dllimports.
#ifndef CTEMPLATE_DLL_DECL
# define CTEMPLATE_DLL_DECL  __declspec(dllimport)
#endif

namespace ctemplate {

extern CTEMPLATE_DLL_DECL
const char kCWD[];       // a string that's equivalent to "./"
extern CTEMPLATE_DLL_DECL
const char kRootdir[];   // a string that's equivalent to "/"

extern CTEMPLATE_DLL_DECL
std::string PathJoin(const std::string& a, const std::string& b);

extern CTEMPLATE_DLL_DECL
bool IsAbspath(const std::string& path);

extern CTEMPLATE_DLL_DECL
bool IsDirectory(const std::string& path);         // checks if path ends with /

extern CTEMPLATE_DLL_DECL
void NormalizeDirectory(std::string* dir);         // adds trailing / if needed

extern CTEMPLATE_DLL_DECL
std::string Basename(const std::string& path);          // everything after last /

// Returns true iff text contains the word as a full word, i.e. delimited by one
// of [.,_-#*?:] on both the sides.
// This is used while loading a template, to check that the file's name matches
// the auto-escape mode specified by it.
// NOTE: This assumes that the word doesn't contain any of the delimiter
// characters.
extern CTEMPLATE_DLL_DECL
bool ContainsFullWord(const std::string& text, const std::string& word);

}

#endif  // TEMPLATE_TEMPLATE_PATHOPS_H_
