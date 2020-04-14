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
// A utility for checking syntax and generating headers to
// use with Google Templates.
//
// For example:
//
// > <path_to>/make_tpl_varnames_h some_template_file.tpl
//
// This creates the header file some_template_file.tpl.varnames.h.  If
// there are any syntax errors they are reported to stderr (in which
// case, no header file is created).
//
//
// Exit code is the number of templates we were unable to parse.
//
// Headers can be all written to one output file (via --outputfile)
// or written to one output file per template processed (via --header_dir).
// As such, we have a first stage where we load each template and generate
// its headers and a second stage where we write the headers to disk.
//
// TODO(jad): Prevent -f and -o from being used together.
//            Previously -o would be silently ignored.

// This is for windows.  Even though we #include config.h, just like
// the files used to compile the dll, we are actually a *client* of
// the dll, so we don't get to decl anything.
#include <config.h>
#undef CTEMPLATE_DLL_DECL
#include <ctype.h>    // for toupper(), isalnum()
#include <errno.h>
#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <string>
#include <set>
#include <vector>

#include <ctemplate/template_pathops.h>
#include <ctemplate/template.h>
using std::set;
using std::string;
using std::vector;
using ctemplate::Template;

enum {LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_FATAL};

// Holds information on each template we process.
struct TemplateRecord {
  const string name;       // filename given on cmd-line (may be relative
  bool error;              // true iff an error occurred during template loading
  string header_entries;   // output of tpl->WriteHeaderEntries()

  explicit TemplateRecord(const string& aname)
      : name(aname), error(false) {
  }
};

static void LogPrintf(int severity, int should_log_info, const char* pat, ...) {
  if (severity == LOG_INFO && !should_log_info)
    return;
  if (severity == LOG_FATAL)
    fprintf(stderr, "FATAL ERROR: ");
  va_list ap;
  va_start(ap, pat);
  vfprintf(stderr, pat, ap);
  va_end(ap);
  fprintf(stderr, "\n");
  if (severity == LOG_FATAL)
    exit(1);
}

// prints to outfile -- usually stdout or stderr
static void Usage(const char* argv0, FILE* outfile) {
  fprintf(outfile, "USAGE: %s [-t<dir>] [-o<dir>] [-s<suffix>] [-f<filename>]"
          " [-n] [-d] [-q] <template_filename> ...\n", argv0);
  fprintf(outfile,
          "       -t<dir> --template_dir=<dir>  Root directory of templates\n"
          "       -o<dir> --header_dir=<dir>    Where to place output files\n"
          "       -s<suffix> --outputfile_suffix=<suffix>\n"
          "                                     outname = inname + suffix\n"
          "       -f<filename> --outputfile=<filename>\n"
          "                                     outname = filename (when given, \n"
          "                                     --header_dir is ignored)\n"
          "       -n --noheader                 Just check syntax, no output\n"
          "       -d --dump_templates           Cause templates dump contents\n"
          "       -q --nolog_info               Only log on error\n"
          "          --v=-1                     Obsolete, confusing synonym for -q\n"
          "       -h --help                     This help\n"
          "       -V --version                  Version information\n");
  fprintf(outfile, "\n"
          "This program checks the syntax of one or more google templates.\n"
          "By default (without -n) it also emits a header file to an output\n"
          "directory that defines all valid template keys.  This can be used\n"
          "in programs to minimze the probability of typos in template code.\n");
}

static void Version(FILE* outfile) {
  fprintf(outfile,
          "make_tpl_varnames_h "
          " (part of " PACKAGE_STRING ")"
          "\n\n"
          "Copyright 1998 Google Inc.\n"
          "\n"
          "This is BSD licensed software; see the source for copying conditions\n"
          "and license information.\n"
          "There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A\n"
          "PARTICULAR PURPOSE.\n"
          );
}

// Removes all non alphanumeric characters from a string to form a
// valid C identifier to use as a double-inclusion guard.
static void ConvertToIdentifier(string* s) {
  for (string::size_type i = 0; i < s->size(); i++) {
    if (!isalnum((*s)[i]))
      (*s)[i] = '_';
    else
      (*s)[i] = toupper((*s)[i]);
  }
}

// Returns the given header entries wrapped with a compiler guard
// whose name is generated from the output_file name.
static string WrapWithGuard(const string& output_file,
                            const string& header_entries) {
  string guard(string("TPL_") + output_file);
  ConvertToIdentifier(&guard);
  guard.append("_H_");

  string out;
  out.append(string("#ifndef ") + guard + "\n");
  out.append(string("#define ") + guard + "\n\n");

  // Now append the header-entry info to the intro above
  out.append(header_entries);

  out.append(string("\n#endif  // ") + guard + "\n");
  return out;
}

// Generates a multi-line comment that will go at the top of the output file.
// The comment includes the filename(s) that produced the output, one per line.
static string Boilerplate(const string& progname,
                          const vector<string>& filenames) {
  string out(string("//\n"));
  if (filenames.size() > 1)
    out.append("// This header file auto-generated for the templates\n");
  else
    out.append("// This header file auto-generated for the template\n");

  for (vector<string>::size_type i = 0; i < filenames.size(); ++i)
    out.append("//    " + filenames[i] + "\n");

  out.append("// by " + progname + "\n" +
             "// DO NOT MODIFY THIS FILE DIRECTLY\n" +
             "//\n");
  return out;
}

// Returns true iff line is empty or only contains whitespace
// (space, horizontal tab, vertical tab, form feed, carriage return).
static bool LineIsAllWhitespace(const string& input) {
  static const string kWhitespace(" \f\t\v\r");
  return input.find_first_not_of(kWhitespace) == string::npos;
}

// Splits the input string into lines using the newline (\n)
// as delimiter. The newlines are discarded.
// An empty string input results in one empty line output.
//
// Examples: "Hello\nWorld\n" input results in two lines,
//           "Hello" and "World".
//           Same result for "Hello\nWorld" (not newline terminated).
//
static vector<string> SplitIntoLines(const string &input) {
  vector<string> lines;

  string::size_type begin_index = 0;
  string::size_type input_len = input.length();

  while (1) {
    string::size_type end_index = input.find_first_of('\n', begin_index);
    if (end_index == string::npos) {
        lines.push_back(input.substr(begin_index));
      break;
    }
    lines.push_back(input.substr(begin_index, (end_index - begin_index)));
    begin_index = end_index + 1;
    if (begin_index >= input_len)  // To avoid adding a trailing empty line.
      break;
  }
  return lines;
}

// Receives header entries concatenated together from one or more
// templates and returns a string with the duplicate lines removed.
//
// Duplicate lines that contain only whitespace are not removed,
// all other duplicate lines (identical #include directives and
// identical variable definitions) are removed. If the last
// (or only) input line did not terminate with newline, we add one.
//
// Consider the following two templates:
//   ex1.tpl: <p>{{USER}}</p>
//   ex2.tpl: <a href="{{URL}}">{{USER}}</a>
//
// The header entries for ex1.tpl are:
// #include "template/template_string.h"
// static const ::ctemplate::StaticTemplateString ke_USER =
//   STS_INIT_WITH_HASH(ke_USER, "USER", 3254611514008215315LLU);
//
// The header entries for ex2.tpl are:
// #include "template/template_string.h"
// static const ::ctemplate::StaticTemplateString ke_URL =
//   STS_INIT_WITH_HASH(ke_URL, "URL", 1026025273225241985LLU);
// static const ::ctemplate::StaticTemplateString ke_USER =
//   STS_INIT_WITH_HASH(ke_USER, "USER", 3254611514008215315LLU);
//
// Simply concatenating both header entries will result in
// duplicate #include directives and duplicate definitions of
// the ke_USER variable. This function instead outputs:
//
// #include "template/template_string.h"
// static const ::ctemplate::StaticTemplateString ke_USER =
//   STS_INIT_WITH_HASH(ke_USER, "USER", 3254611514008215315LLU);
// static const ::ctemplate::StaticTemplateString ke_URL =
//   STS_INIT_WITH_HASH(ke_URL, "URL", 1026025273225241985LLU);
//
static string TextWithDuplicateLinesRemoved(const string& header_entries) {
  string output;
  set<string> lines_seen;
  vector<string> lines = SplitIntoLines(header_entries);
  const int lines_len = lines.size();
  for (int i = 0; i < lines_len; ++i) {
    const string& line = lines[i];
    if (LineIsAllWhitespace(line) ||    // Blank lines always go in
        !lines_seen.count(line)) {  // So do new lines
      output.append(line);
      output.append("\n");
      lines_seen.insert(line);
    }
  }
  return output;
}

// Writes the given text to the filename header_file.
// Returns true if it succeeded, false otherwise.
static bool WriteToDisk(bool log_info, const string& output_file,
                        const string& text) {
  FILE* outfile = fopen(output_file.c_str(), "wb");
  if (!outfile) {
    LogPrintf(LOG_ERROR, log_info, "Can't open %s", output_file.c_str());
    return false;
  }
  LogPrintf(LOG_INFO, log_info, "Creating %s", output_file.c_str());
  if (fwrite(text.c_str(), 1, text.length(), outfile) != text.length()) {
    LogPrintf(LOG_ERROR, log_info, "Can't write %s: %s",
              output_file.c_str(), strerror(errno));
  }
  fclose(outfile);
  return true;
}

int main(int argc, char **argv) {
  string FLAG_template_dir(ctemplate::kCWD);   // "./"
  string FLAG_header_dir(ctemplate::kCWD);
  ctemplate::NormalizeDirectory(&FLAG_header_dir);   // adds trailing slash
  string FLAG_outputfile_suffix(".varnames.h");
  string FLAG_outputfile("");
  bool FLAG_header = true;
  bool FLAG_dump_templates = false;
  bool FLAG_log_info = true;

#if defined(HAVE_GETOPT_LONG)
  static struct option longopts[] = {
    {"help", 0, NULL, 'h'},
    {"version", 0, NULL, 'V'},
    {"template_dir", 1, NULL, 't'},
    {"header_dir", 1, NULL, 'o'},
    {"outputfile_suffix", 1, NULL, 's'},
    {"outputfile", 1, NULL, 'f'},
    {"noheader", 0, NULL, 'n'},
    {"dump_templates", 0, NULL, 'd'},
    {"nolog_info", 0, NULL, 'q'},
    {"v", 1, NULL, 'q'},
    {0, 0, 0, 0}
  };
  int option_index;
# define GETOPT(argc, argv)  getopt_long(argc, argv, "t:o:s:f:ndqhV", \
                                         longopts, &option_index)
#elif defined(HAVE_GETOPT_H)
# define GETOPT(argc, argv)  getopt(argc, argv, "t:o:s:f:ndqhV")
#else    // TODO(csilvers): implement something reasonable for windows
# define GETOPT(argc, argv)  -1
  int optind = 1;    // first non-opt argument
  const char* optarg = "";   // not used
#endif

  int r = 0;
  while (r != -1) {   // getopt()/getopt_long() return -1 upon no-more-input
    r = GETOPT(argc, argv);
    switch (r) {
      case 't': FLAG_template_dir.assign(optarg); break;
      case 'o': FLAG_header_dir.assign(optarg); break;
      case 's': FLAG_outputfile_suffix.assign(optarg); break;
      case 'f': FLAG_outputfile.assign(optarg); break;
      case 'n': FLAG_header = false; break;
      case 'd': FLAG_dump_templates = true; break;
      case 'q': FLAG_log_info = false; break;
      case 'V': Version(stdout); return 0; break;
      case 'h': Usage(argv[0], stderr); return 0; break;
      case -1: break;   // means 'no more input'
      default: Usage(argv[0], stderr); return 1; break;
    }
  }

  if (optind >= argc) {
    LogPrintf(LOG_FATAL, FLAG_log_info,
              "Must specify at least one template file on the command line.");
  }

  Template::SetTemplateRootDirectory(FLAG_template_dir);


  // Initialize the TemplateRecord array. It holds one element per
  // template given on the command-line.
  vector<TemplateRecord*> template_records;
  for (int i = optind; i < argc; ++i) {
    TemplateRecord *template_rec = new TemplateRecord(argv[i]);
    template_records.push_back(template_rec);
  }

  // Iterate through each template and (unless -n is given), write
  // its header entries into the headers array.
  int num_errors = 0;
  for (vector<TemplateRecord*>::iterator it = template_records.begin();
       it != template_records.end(); ++it) {
    const char* tplname = (*it)->name.c_str();
    LogPrintf(LOG_INFO, FLAG_log_info, "\n------ Checking %s ------", tplname);

    // The last two arguments in the following call do not matter
    // since they control how the template gets expanded and we never
    // expand the template after loading it here
    Template * tpl = Template::GetTemplate(tplname, ctemplate::DO_NOT_STRIP);

    // The call to GetTemplate (above) loads the template from disk
    // and attempts to parse it. If it cannot find the file or if it
    // detects any template syntax errors, the parsing routines
    // report the error and GetTemplate returns NULL. Syntax errors
    // include such things as mismatched double-curly-bracket pairs,
    // e.g. '{{VAR}', Invalid characters in template variables or
    // section names, e.g.  '{{BAD_VAR?}}' [the question mark is
    // illegal], improperly nested section/end section markers,
    // e.g. a section close marker with no section start marker or a
    // section start of a different name.
    // If that happens, since the parsing errors have already been reported
    // we just continue on to the next one.
    if (!tpl) {
      LogPrintf(LOG_ERROR, FLAG_log_info, "Could not load file: %s", tplname);
      num_errors++;
      (*it)->error = true;
      continue;
    } else {
      LogPrintf(LOG_INFO, FLAG_log_info, "No syntax errors detected in %s",
                tplname);
      if (FLAG_dump_templates)
        tpl->Dump(tpl->template_file());
    }

    // The rest of the loop creates the header file
    if (!FLAG_header)
      continue;            // They don't want header files

    tpl->WriteHeaderEntries(&((*it)->header_entries));
  }

  // We have headers to emit:
  // . If --outputfile was given, we combine all the header entries and
  //   write them to the given output file. If any template had errors,
  //   we fail and do not generate an output file.
  // . Otherwise, we write one output file per template we processed.
  // . In both cases, we add proper boilerplate first.
  if (FLAG_header) {
    string progname = argv[0];

    if (!FLAG_outputfile.empty()) {  // All header entries written to one file.
      // If any template had an error, we do not produce an output file.
      if (num_errors == 0) {
        vector<string> template_filenames;
        string all_header_entries;
        for (vector<TemplateRecord*>::const_iterator
             it = template_records.begin(); it != template_records.end(); ++it) {
          all_header_entries.append((*it)->header_entries);
          template_filenames.push_back((*it)->name);
        }
        string output = Boilerplate(progname, template_filenames);
        const string cleantext =
            TextWithDuplicateLinesRemoved(all_header_entries);
        output.append(WrapWithGuard(FLAG_outputfile, cleantext));
        if (!WriteToDisk(FLAG_log_info, FLAG_outputfile, output))
          num_errors++;
      }
    } else {
      // Each template will have its own output file. Skip any that had errors.
      for (vector<TemplateRecord*>::const_iterator
           it = template_records.begin(); it != template_records.end(); ++it) {
        if ((*it)->error)
          continue;
        string basename = ctemplate::Basename((*it)->name);
        string output_file =
            ctemplate::PathJoin(FLAG_header_dir,
                                basename + FLAG_outputfile_suffix);
        vector<string> template_filenames;   // Contains one template filename.
        template_filenames.push_back((*it)->name);
        string output = Boilerplate(progname, template_filenames);
        output.append(WrapWithGuard(output_file, (*it)->header_entries));
        if (!WriteToDisk(FLAG_log_info, output_file, output))
          num_errors++;
      }
    }
  }

  // Free dynamic memory
  for (vector<TemplateRecord*>::iterator it = template_records.begin();
       it != template_records.end(); ++it) {
    delete *it;
  }

  // Cap at 127 to avoid causing problems with return code
  return num_errors > 127 ? 127 : num_errors;
}
