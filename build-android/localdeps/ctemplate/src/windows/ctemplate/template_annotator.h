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
//
// The template expansion system provides a set of hooks that allow for
// the insertion of diagnostic content into the output stream for the use
// by content developers and testers.  For instance, the default text
// annotation mode inserts strings bracketed by {{ }} to describe every
// occurrence of a dynamic substitution feature.  That mode turns the
// rendering into a logical text description of the construction of
// template-based output.  It is useful for regression testing of output
// in conjunction with text-based diffing tools.
//
// An annotation mode is implemented with the TemplateAnnotator interface.
// When template annotation is turned on, then during template expansion
// whenever a dynamic substitution feature is encountered, a call is made
// to one of the TemplateAnnotator functions.  In response to a call
// an implementation can render any additional content into the passed
// emitter, which is the same emitter that the rendering output is going
// to.
//
// Template annotation is turned on and the template annotator subclass
// set by methods in ctemplate::PerExpandData.

#ifndef TEMPLATE_TEMPLATE_ANNOTATOR_H_
#define TEMPLATE_TEMPLATE_ANNOTATOR_H_

#include <string>

// NOTE: if you are statically linking the template library into your binary
// (rather than using the template .dll), set '/D CTEMPLATE_DLL_DECL='
// as a compiler flag in your project file to turn off the dllimports.
#ifndef CTEMPLATE_DLL_DECL
# define CTEMPLATE_DLL_DECL  __declspec(dllimport)
#endif

namespace ctemplate {

class ExpandEmitter;

// This is the abstract interface for an annotation mode.  A new annotation
// mode is introduced by subclassing and implementing each function
// to add annotation content.  There is one function for each internal
// template expansion event type.  The emitter argument passed to the
// function is the same stream that the expanding content is being output to;
// so the action of an implementation will be to add additional inline
// content.  The emitter argument is never to be remembered beyond each
// function call.
class CTEMPLATE_DLL_DECL TemplateAnnotator {
 public:
  TemplateAnnotator() { }
  virtual ~TemplateAnnotator() { }

  // Called before processing a subtemplate include marker.
  // Passed value is the include marker name.
  virtual void EmitOpenInclude(ExpandEmitter* emitter, const std::string& value) = 0;
  // Called after processing a subtemplate include marker.
  virtual void EmitCloseInclude(ExpandEmitter* emitter) = 0;

  // Called before opening a template or subtemplate file for processing.
  // Passed value is the filename.
  virtual void EmitOpenFile(ExpandEmitter* emitter, const std::string& value) = 0;
  // Called after processing a template or subtemplate file.
  virtual void EmitCloseFile(ExpandEmitter* emitter) = 0;

  // Called before processing a section.
  // Passed value is the section name.
  virtual void EmitOpenSection(ExpandEmitter* emitter, const std::string& value) = 0;
  // Called after processing a section.
  virtual void EmitCloseSection(ExpandEmitter* emitter) = 0;

  // Called before processing a variable marker.
  // Passed value is the variable name.
  virtual void EmitOpenVariable(ExpandEmitter* emitter,
                                const std::string& value) = 0;
  // Called after processing a variable marker.
  virtual void EmitCloseVariable(ExpandEmitter* emitter) = 0;

  virtual void EmitFileIsMissing(ExpandEmitter* emitter,
                                 const std::string& value) = 0;

 private:
  // Can't invoke copy constructor or assignment operator
  TemplateAnnotator(const TemplateAnnotator&);
  void operator=(const TemplateAnnotator&);
};

// This is a concrete template annotator class that inserts annotations
// that have a standard text form bracketed by {{ }}.  It is used as
// the default annotation implementation when annotation is turned on
// by PerExpandData and no annotator type is specified.
class CTEMPLATE_DLL_DECL TextTemplateAnnotator : public TemplateAnnotator {
 public:
  TextTemplateAnnotator() { }
  virtual void EmitOpenInclude(ExpandEmitter* emitter, const std::string& value);
  virtual void EmitCloseInclude(ExpandEmitter* emitter);
  virtual void EmitOpenFile(ExpandEmitter* emitter, const std::string& value);
  virtual void EmitCloseFile(ExpandEmitter* emitter);
  virtual void EmitOpenSection(ExpandEmitter* emitter, const std::string& value);
  virtual void EmitCloseSection(ExpandEmitter* emitter);
  virtual void EmitOpenVariable(ExpandEmitter* emitter, const std::string& value);
  virtual void EmitCloseVariable(ExpandEmitter* emitter);
  virtual void EmitFileIsMissing(ExpandEmitter* emitter,
                                 const std::string& value);

 private:
  // Can't invoke copy constructor or assignment operator
  TextTemplateAnnotator(const TextTemplateAnnotator&);
  void operator=(const TextTemplateAnnotator&);
};

}

#endif  // TEMPLATE_TEMPLATE_ANNOTATOR_H_
