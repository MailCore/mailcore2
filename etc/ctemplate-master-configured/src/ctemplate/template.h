// Copyright (c) 2006, Google Inc.
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
// This file implements the Template class.  For information about
// how to use this class, and to write the templates it takes as input,
// see the doc/ directory.

#ifndef CTEMPLATE_TEMPLATE_H_
#define CTEMPLATE_TEMPLATE_H_

#include <time.h>             // for time_t
#include <string>
#include <ctemplate/template_cache.h>
#include <ctemplate/template_enums.h>
#include <ctemplate/template_string.h>

// We include this just so folks don't have to include both template.h
// and template_dictionary.h, or template_namelist.h etc, to use the
// template system; we don't actually use anything in these files
// ourselves.
#if 1
#include <ctemplate/template_dictionary.h>
#include <ctemplate/template_namelist.h>
#include <ctemplate/per_expand_data.h>
#else
namespace ctemplate {
class TemplateDictionaryInterface;
class PerExpandData;
}
#endif

namespace ctemplate_htmlparser {
class HtmlParser;
}



namespace ctemplate {

// These free functions form the "simple" template API, and support
// the most common operations (expanding a template from a file, and
// from a string).  They all just delegate to a default instance of
// the TemplateCache object.
//
// For more sophisticated use of the template system, you may need
// to create your own TemplateCache object, and work directly with
// it.  See template_cache.h for details.

extern  const TemplateCache* default_template_cache();
extern  TemplateCache* mutable_default_template_cache();


// ---- EXPANDING A TEMPLATE -------
//    ExpandTemplate
//    ExpandWithData

// Loads the template named filename from disk if necessary -- it
// gets it from the cache instead, if the template had been loaded
// before or if it had been put explicitly in the cache via a call
// to StringToTemplateCache() -- and expands it using the given
// dictionary.
// The first version is the most general, followed by common-case code.
inline bool ExpandTemplate(const TemplateString& filename, Strip strip,
                           const TemplateDictionaryInterface *dictionary,
                           ExpandEmitter* output) {
  return mutable_default_template_cache()->ExpandWithData(
      filename, strip, dictionary, NULL, output);
}
inline bool ExpandTemplate(const TemplateString& filename, Strip strip,
                           const TemplateDictionaryInterface* dictionary,
                           std::string* output_buffer) {
  return mutable_default_template_cache()->ExpandWithData(
      filename, strip, dictionary, NULL, output_buffer);
}

// If you want any per-expand data to be used at expand time, call
// this routine instead of Expand.  You pass in an extra
// PerExpandData structure (see per_expand_data.h) which sets this
// data: whether or not you want the template to be annotated, and
// any data you want to pass in to template modifers.  If
// per_expand_data is NULL, this is exactly the same as Expand().
// The first version is the most general, followed by common-case code.
inline bool ExpandWithData(const TemplateString& filename, Strip strip,
                           const TemplateDictionaryInterface *dictionary,
                           PerExpandData* per_expand_data,
                           ExpandEmitter* output) {
  return mutable_default_template_cache()->ExpandWithData(
      filename, strip, dictionary, per_expand_data, output);
}
inline bool ExpandWithData(const TemplateString& filename, Strip strip,
                           const TemplateDictionaryInterface* dictionary,
                           PerExpandData* per_expand_data,
                           std::string* output_buffer) {
  return mutable_default_template_cache()->ExpandWithData(
      filename, strip, dictionary, per_expand_data, output_buffer);
}

// ---- INSERTING INTO THE CACHE -------
//   LoadTemplate
//   StringToTemplateCache

// Reads a file from disk and inserts it into the template, if it's
// not already there.  Returns true on success or false if the
// template could not be found, or could not be parsed.  It's never
// necessary to call this -- Expand() will load templates lazily if
// needed -- but you may want to if you want to make sure templates
// exist before trying to expand them, or because you want to
// control disk access patterns, or for some other reason.
inline bool LoadTemplate(const TemplateString& filename, Strip strip) {
  return mutable_default_template_cache()->LoadTemplate(filename, strip);
}

// Inserts the given string into the default template cache, as if
// it were a file read from disk.  You can call Expand() with its
// first arg (filename) the same as the key you use here.  You can
// also use this key as the 'filename' for sub-included templates,
// in TemplateDictionary::SetFilename().
inline bool StringToTemplateCache(const TemplateString& key,
                                  const TemplateString& content,
                                  Strip strip) {
  return mutable_default_template_cache()->StringToTemplateCache(
      key, content, strip);
}
inline bool StringToTemplateCache(const TemplateString& key,
                                  const char* content, size_t content_len,
                                  Strip strip) {
  return mutable_default_template_cache()->StringToTemplateCache(
      key, content, content_len, strip);
}


// ---------------------------------------------------------------------
// The follow are deprecated.
// TODO(csilvers): move to parsed_template.h

// TemplateState of a template is:
// - TS_EMPTY before parsing is complete,
// - TS_ERROR if a syntax error was found during parsing, and
// - TS_READY if parsing has completed successfully
// (TS_UNUSED is not used)
enum TemplateState { TS_UNUSED, TS_EMPTY, TS_ERROR, TS_READY };

// Used for Auto-Escape. It represents the different contexts a template may
// be initialized in via the AUTOESCAPE pragma in the template file
// (or string). It is only public for testing. The contexts are:
// - TC_HTML: The template contains HTML code. Need not be a complete HTML
//            page just content the browser interprets in the context of
//            HTML parsing. This should be the most common context to use.
//            This mode activates our HTML parser.
// - TC_JS:   The template contains raw javascript. If your template
//            starts with <script> tag, it is of type TC_HTML not TC_JS.
//            TC_JS is typically associated with a content-type of
//            text/javascript. This mode activates our HTML parser.
// - TC_CSS:  The template contains CSS (cascaded style-sheet). If your
//            template starts with a <style> tag, it is of type TC_HTML
//            not TC_CSS. A TC_CSS template is typically associated with a
//            text/css content-type header. Currently treated same as
//            TC_HTML but don't rely on that. We may later develop
//            CSS-specific sanitizers and parsers.
// - TC_JSON: The template contains raw JSON. Applies javascript_escape
//            to variables. Note: javascript_escape is safer than
//            json_escape which we may want to remove.
// - TC_XML:  The template contains raw XML. Applies xml_escape to variables.
//            CAUTION: This mode is not suitable for cases where the
//            application data encapsulated in XML requires special
//            escaping, such as the case of XHTML.
//            TC_XML is typically associated with text/xml content-type.
// - TC_MANUAL: Equivalent to not specifying auto-escaping at all.
//
// TODO(csilvers): Make this a private part of the Template class.
enum TemplateContext { TC_UNUSED, TC_HTML, TC_JS, TC_CSS, TC_JSON,
                       TC_XML, TC_MANUAL };


// This class is deprecated.  Old code uses this class heavily (via
// GetTemplate() to obtain a Template*, and then methods on that
// Template*) but new code should use the free functions above.
class  Template {
 public:
  // ---- METHODS FOR TOOLS ----
  //   These are not intended for normal use, but are public so a
  //   tool can use them.

  // Used by make_tpl_varnames_h.cc.
  void WriteHeaderEntries(std::string *outstring) const;

  // ---- DEPRECATED METHODS ----
  //   These methods used to be the primary way of using the Template
  //   object, but have been deprecated in favor of the (static)
  //   methods above.  If you are using these deprecated methods,
  //   consider moving to the above methods instead, or to moving to
  //   using your own TemplateCache (which supports richer operations
  //   on parsed templates).

  // Loads a template from disk or cache or string, and returns the Template*.
  // INSTEAD, use the static Expand that takes a filename.
  static Template *GetTemplate(const TemplateString& filename, Strip strip);
  virtual ~Template();  // when the time comes to delete these Template*'s.

  // Parses a string immediately and returns the resulting Template*.
  // You can call the (deprecated) non-static Expand() method on this
  // template in order to expand it with a dictionary.  You are
  // responsible for deleting the Template* when you are done with it.
  // INSTEAD, use StringToTemplateCache (with a key) plus the static Expand().
  // TOOO(csilvers): return a const Template* instead.
  static Template* StringToTemplate(const TemplateString& content,
                                    Strip strip);
  static Template* StringToTemplate(const char* content, size_t content_len,
                                    Strip strip) {
    return StringToTemplate(TemplateString(content, content_len), strip);
  }

  // Non-static Expand*() works on a Template* returned from GetTemplate().
  // INSTEAD, use static expand with a filename (or key-name for strings).
  bool ExpandWithData(ExpandEmitter* output,
                      const TemplateDictionaryInterface* dictionary,
                      PerExpandData* per_expand_data) const {
    return ExpandWithDataAndCache(output, dictionary, per_expand_data,
                                  default_template_cache());
  }
  bool ExpandWithData(std::string* output_buffer,
                      const TemplateDictionaryInterface* dictionary,
                      PerExpandData* per_expand_data) const {
    if (output_buffer == NULL)  return false;
    StringEmitter e(output_buffer);
    return ExpandWithData(&e, dictionary, per_expand_data);
  }
  bool Expand(ExpandEmitter* output,
              const TemplateDictionaryInterface* dictionary) const {
    return ExpandWithData(output, dictionary, NULL);
  }
  bool Expand(std::string* output_buffer,
              const TemplateDictionaryInterface* dictionary) const {
    return ExpandWithData(output_buffer, dictionary, NULL);
  }

  // Dump to stdout or a string.  filename is just used to annotate output.
  void Dump(const char *filename) const;
  void DumpToString(const char *filename, std::string *out) const;

  // Retrieves the state, template-file, or strip mode of this Template.
  TemplateState state() const;
  const char *template_file() const;
  const char *original_filename() const;
  Strip strip() const;

  // Work at the level of groups of templates, so just call through to
  // the default TemplateCache; see template_cache.h for what these do.
  // INSTEAD, create your own TemplateCache and call these methods on that.
  static bool SetTemplateRootDirectory(const std::string& dir) {
    return mutable_default_template_cache()->SetTemplateRootDirectory(dir);
  }
  static bool AddAlternateTemplateRootDirectory(const std::string& dir) {
    return mutable_default_template_cache()->AddAlternateTemplateRootDirectory(
        dir);
  }
  static std::string template_root_directory() {
    return default_template_cache()->template_root_directory();
  }
  static std::string FindTemplateFilename(
      const std::string& unresolved) {
    return default_template_cache()->FindTemplateFilename(unresolved);
  }
  static void RemoveStringFromTemplateCache(const std::string& key) {
    mutable_default_template_cache()->Delete(key);
  }
  static void ClearCache() {
    mutable_default_template_cache()->ClearCache();
  }
  static void ReloadAllIfChanged() {
    mutable_default_template_cache()->ReloadAllIfChanged(
        TemplateCache::LAZY_RELOAD);
  }

  // ---- EXTRA-DEPRECATED METHODS ----
  //   These methods were deprecated even before the move to
  //   TemplateCache.  We'd really like you to move from these to one
  //   of the "approved" methods, or even one of the deprecated
  //   methods.  Comments here don't even describe what these
  //   functions do, just how to transition off of using them.

  // INSTEAD, use the StringToTemplateCache function that takes the strip mode.
  static bool StringToTemplateCache(const TemplateString& key,
                                    const TemplateString& content);
  static bool StringToTemplateCache(const TemplateString& key,
                                    const char* content, size_t content_len) {
    return StringToTemplateCache(key, TemplateString(content, content_len));
  }
  // This is to prevent against typos: you want the global (free-function)
  // StringToTemplateCache here, not the one in Template.
  static bool StringToTemplateCache(const TemplateString& key,
                                    const char* content, Strip);

 protected:
  friend class SectionTemplateNode;  // for access to set_state(), ParseState
  friend class TemplateTemplateNode; // for recursive call to Expand()

  // Template constructor
  //   Reads the template file and parses it into a parse tree of TemplateNodes
  //   by calling the method ReloadIfChanged
  //   The top node is a section node with the arbitrary name "__{{MAIN}}__"
  //   'Strip' indicates how to handle whitespace when expanding the
  //   template.  DO_NOT_STRIP keeps the template exactly as-is.
  //   STRIP_BLANK_LINES elides all blank lines in the template.
  //   STRIP_WHITESPACE elides all blank lines, and also all whitespace
  //   at either the beginning or end of a line.  It also removes
  //   any linefeed (possibly following whitespace) that follows a closing
  //   '}}' of any kind of template marker EXCEPT a template variable.
  //   This means a linefeed may be removed anywhere by simply placing
  //   a comment marker as the last element on the line.
  //   These two options allow the template to include whitespace for
  //   readability without adding to the expanded output.
  Template(const TemplateString& filename, Strip strip, TemplateCache* owner);

  // MaybeInitHtmlParser
  //   In TemplateContexts where the HTML parser is needed, we
  //   initialize it in the appropriate mode. Also we do a sanity
  //   check (cannot fail) on the template filename. This function is
  //   called at most once for a Template.  In_tag is only meaningful
  //   for TC_HTML: It is true for templates that start inside an HTML
  //   tag and hence are expected to contain HTML attribute name/value
  //   pairs only. It is false for standard HTML templates.
  void MaybeInitHtmlParser(bool in_tag);

  // BuildTree
  //   Parses the contents of the file (retrieved via ReloadIfChanged)
  //   and stores the resulting parse structure in tree_.  Returns true
  //   iff the tree-builder encountered no errors.  Note: takes
  //   ownership of input_buffer, and will delete it.  It should have
  //   been created via new[].
  bool BuildTree(const char *input_buffer, const char* input_buffer_end);

  // Internal version of ReloadIfChanged, used when the function already
  // has a write-lock on g_template_mutex.
  bool ReloadIfChangedLocked();

  // set_state
  //   Sets the state of the template.  Used during BuildTree().
  void set_state(TemplateState new_state);

  // StripBuffer
  //   Modifies buffer in-place based on the strip_ mode, to remove
  //   extra whitespace.  May delete[] the input buffer and replace
  //   it with a new buffer.  Used by ReloadIfChanged().
  void StripBuffer(char **buffer, size_t* len);

  // The file we originally got from the Template() constructor
  const std::string original_filename_;
  // The pathname as fully resolved on the filesystem
  std::string resolved_filename_;
  time_t filename_mtime_;   // lastmod time for filename last time we loaded it

  // What to do with whitespace at template-expand time
  Strip strip_;

  // Keeps track of where we are in reloading, or if there was an error loading
  TemplateState state_;

  // The cache we got this template from.  This is not well-defined: a
  // Template can be in more than one cache.
  // TODO(csilvers): remove this once we deprecate the one user, which
  //                 is ReloadIfChanged.
  TemplateCache* template_cache_;

  // The current template-contents, as read from the file
  const char* template_text_;
  int template_text_len_;

  // The current parsed template structure.  Has pointers into template_text_.
  class SectionTemplateNode *tree_;       // defined in template.cc

  // Template markers have the form {{VARIABLE}}, etc.  These constants
  // define the {{ and }} that delimit template markers.
  struct  MarkerDelimiters {
    const char* start_marker;
    size_t start_marker_len;
    const char* end_marker;
    size_t end_marker_len;

    MarkerDelimiters() {
      start_marker = "{{";    // The default start-marker
      start_marker_len = strlen(start_marker);
      end_marker = "}}";
      end_marker_len = strlen(end_marker);
    }
  };

  // The current parsing state.  Used in BuildTree() and subroutines
  struct  ParseState {
    const char* bufstart;
    const char* bufend;
    enum { PS_UNUSED, GETTING_TEXT, GETTING_NAME } phase;
    MarkerDelimiters current_delimiters;
    ParseState()
        : bufstart(NULL), bufend(NULL), phase(PS_UNUSED), current_delimiters()
    {}
  };
  ParseState parse_state_;

  // All templates are initialized to TC_MANUAL (no Auto-Escape). Then,
  // during template parsing (BuildTree()), if an AUTOESCAPE pragma is
  // encountered, the context changes appropriately.
  TemplateContext initial_context_;
  // Non-null if the template was initialized in an Auto-Escape mode that
  // requires a parser (currently TC_HTML, TC_CSS and TC_JS).
  ctemplate_htmlparser::HtmlParser *htmlparser_;

  // A sorted list of trusted variable names, declared here because a unittest
  // needs to verify that it is appropriately sorted (an unsorted array would
  // lead to the binary search of this array failing).
  static const char * const kSafeWhitelistedVariables[];
  static const size_t kNumSafeWhitelistedVariables;

 private:
  friend class TemplateCache;
  friend class TemplateCachePeer;  // to access num_deletes_

  // Internal implementation of Expand
  bool ExpandWithDataAndCache(ExpandEmitter* output,
                              const TemplateDictionaryInterface *dictionary,
                              PerExpandData* per_expand_data,
                              const TemplateCache* cache) const;

  // This is called for recursive expands, when we already hold template_lock.
  bool ExpandLocked(ExpandEmitter* output,
                    const TemplateDictionaryInterface *dictionary,
                    PerExpandData* per_expand_data,
                    const TemplateCache* cache) const;

  // Returns the lastmod time in mtime_
  // For string-based templates, not backed by a file, this returns 0
  time_t mtime() const;

  // These are helper routines to StripFile.  I would make them static
  // inside template.cc, but they use the MarerDelimiters struct.
  static bool ParseDelimiters(const char* text, size_t textlen,
                              MarkerDelimiters* delim);
  static bool IsBlankOrOnlyHasOneRemovableMarker(const char** line, size_t* len,
                                                 const MarkerDelimiters& delim);
  static size_t InsertLine(const char *line, size_t len, Strip strip,
                           const MarkerDelimiters& delim, char* buffer);

  // This is only used by template_cache_test, via TemplateCachePeer.
  static int num_deletes() { return num_deletes_; }

  static int num_deletes_;  // how many times the destructor has been called

  // Can't invoke copy constructor or assignment operator
  Template(const Template&);
  void operator=(const Template &);
};

}

#endif // CTEMPLATE_TEMPLATE_H_
