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
// Heavily inspired from make_tpl_varnames_h.cc
//
// A utility for evaluating the changes in escaping modifiers
// applied to variables between two versions of a template file.
// This may come in handy when converting a template to Auto-Escape:
// If the template previously had escaping modifiers, this tool will show
// the variables for which Auto-Escaped determined a different escaping.
//
// How it works:
//   . You provide two template files, assumed to be identical in content
//     (same variables in the same order) except for escaping modifiers
//     and possibly the AUTOESCAPE pragma. You also provide the Strip mode
//     or a default of STRIP_WHITESPACE is assumed.
//
//   . The tool loads both files and invokes DumpToString on both. It then
//     compares the escaping modifiers for each variable and when they do
//     not match, it prints a line with the variable name as well as
//     the differing modifiers.
//
//   . We accept some command-line flags, the most notable are:
//       --template_dir to set a template root directory other than cwd
//       --strip to set the Strip mode to other than STRIP_WHITESPACE.
//         For correct operation of Auto-Escape, ensure this matches
//         the Strip mode you normally use on these templates.
//
//
// Exit code is zero if there were no differences. It is non-zero
// if we failed to load the templates or we found one or more
// differences.
//
// TODO(jad): Add flag to optionally report differences when a variable
//            does not have modifiers in either template.

// This is for opensource ctemplate on windows.  Even though we
// #include config.h, just like the files used to compile the dll, we
// are actually a *client* of the dll, so we don't get to decl anything.
#include <config.h>
#undef CTEMPLATE_DLL_DECL

#include <stdlib.h>
#include <stdio.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <stdarg.h>
#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif
#include <string.h>
#include <string>
#include <ctemplate/template.h>
#include <ctemplate/template_pathops.h>
using std::string;
using std::vector;
using ctemplate::Template;
using ctemplate::TemplateContext;
using ctemplate::Strip;
using ctemplate::STRIP_WHITESPACE;
using ctemplate::STRIP_BLANK_LINES;
using ctemplate::DO_NOT_STRIP;

enum {LOG_VERBOSE, LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_FATAL};

// A variable name and optional modifiers.
// For example: in {{NAME:j:x-bla}}
// variable_name is "NAME" and modifiers is "j:x-bla".
struct VariableAndMod {
  VariableAndMod(string name, string mods)
      : variable_name(name), modifiers(mods) { }
  string variable_name;
  string modifiers;
};
typedef vector<VariableAndMod> VariableAndMods;

static string FLAG_template_dir(ctemplate::kCWD);   // "./"
static string FLAG_strip = "";      // cmd-line arg -s
static bool FLAG_verbose = false;   // cmd-line arg -v

static void LogPrintf(int severity, const char* pat, ...) {
  if (severity == LOG_VERBOSE && !FLAG_verbose)
    return;
  if (severity == LOG_FATAL)
    fprintf(stderr, "FATAL ERROR: ");
  if (severity == LOG_VERBOSE)
    fprintf(stdout, "[VERBOSE] ");
  va_list ap;
  va_start(ap, pat);
  vfprintf(severity == LOG_INFO || severity == LOG_VERBOSE ? stdout: stderr,
           pat, ap);
  va_end(ap);
  if (severity == LOG_FATAL)
    exit(1);
}

// Prints to outfile -- usually stdout or stderr -- and then exits
static int Usage(const char* argv0, FILE* outfile) {
  fprintf(outfile, "USAGE: %s [-t<dir>] [-v] [-b] [-s<n>] <file1> <file2>\n",
          argv0);

  fprintf(outfile,
          "       -t --template_dir=<dir>  Root directory of templates\n"
          "       -s --strip=<strip>       STRIP_WHITESPACE [default],\n"
          "                                STRIP_BLANK_LINES, DO_NOT_STRIP\n"
          "       -h --help                This help\n"
          "       -v --verbose             For a bit more output\n"
          "       -V --version             Version information\n");
  fprintf(outfile, "\n"
          "This program reports changes to modifiers between two template\n"
          "files assumed to be identical except for modifiers applied\n"
          "to variables. One use case is converting a template to\n"
          "Auto-Escape and using this program to obtain the resulting\n"
          "changes in escaping modifiers.\n"
          "The Strip value should match what you provide in\n"
          "Template::GetTemplate.\n"
          "NOTE: Variables that do not have escaping modifiers in one of\n"
          "two templates are ignored and do not count in the differences.\n");
  exit(0);
}

static int Version(FILE* outfile) {
  fprintf(outfile,
          "diff_tpl_auto_escape (part of google-template 0.9x)\n"
          "\n"
          "Copyright 2008 Google Inc.\n"
          "\n"
          "This is BSD licensed software; see the source for copying conditions\n"
          "and license information.\n"
          "There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A\n"
          "PARTICULAR PURPOSE.\n"
          );
  exit(0);
}

// Populates the vector of VariableAndMods from the DumpToString
// representation of the template file.
//
// Each VariableAndMod represents a variable node found in the template
// along with the optional modifiers attached to it (or empty string).
// The parsing is very simple. It looks for lines of the form:
//    "Variable Node: <VAR_NAME>[:<VAR_MODS>]\n"
// as outputted by DumpToString() and extracts from each such line the
// variable name and modifiers when present.
// Because DumpToString also outputs text nodes, it is possible
// to trip this function. Probably ok since this is just a helper tool.
bool LoadVariables(const char* filename, Strip strip,
                   VariableAndMods& vars_and_mods) {
  const string kVariablePreambleText = "Variable Node: ";
  Template *tpl;
  tpl = Template::GetTemplate(filename, strip);
  if (tpl == NULL) {
    LogPrintf(LOG_FATAL, "Could not load file: %s\n", filename);
    return false;
  }

  string output;
  tpl->DumpToString(filename, &output);

  string::size_type index = 0;
  string::size_type delim, end;
  // TODO(jad): Switch to using regular expressions.
  while((index = output.find(kVariablePreambleText, index)) != string::npos) {
    index += kVariablePreambleText.length();
    end = output.find('\n', index);
    if (end == string::npos) {
      // Should never happen but no need to LOG_FATAL.
      LogPrintf(LOG_ERROR, "%s: Did not find terminating newline...\n",
                filename);
      end = output.length();
    }
    string name_and_mods = output.substr(index, end - index);
    delim = name_and_mods.find(":");
    if (delim == string::npos)          // no modifiers.
      delim = name_and_mods.length();
    VariableAndMod var_mod(name_and_mods.substr(0, delim),
                           name_and_mods.substr(delim));
    vars_and_mods.push_back(var_mod);
  }
  return true;
}

// Returns true if the difference in the modifier strings
// is non-significant and can be safely omitted. This is the
// case when one is ":j:h" and the other is ":j" since
// the :h is a no-op after a :j.
bool SuppressLameDiff(string modifiers_a, string modifiers_b) {
  if ((modifiers_a == ":j:h" && modifiers_b == ":j") ||
      (modifiers_a == ":j" && modifiers_b == ":j:h"))
    return true;
  return false;
}

// Main function to analyze differences in escaping modifiers between
// two template files. These files are assumed to be identical in
// content [strictly speaking: same number of variables in the same order].
// If that is not the case, we fail.
// We return true if there were no differences, false if we failed
// or we found one or more differences.
bool DiffTemplates(const char* filename_a, const char* filename_b,
                   Strip strip) {
  vector<VariableAndMod> vars_and_mods_a, vars_and_mods_b;

  if (!LoadVariables(filename_a, strip, vars_and_mods_a) ||
      !LoadVariables(filename_b, strip, vars_and_mods_b))
    return false;

  if (vars_and_mods_a.size() != vars_and_mods_b.size())
    LogPrintf(LOG_FATAL, "Templates differ: %s [%d vars] vs. %s [%d vars].\n",
              filename_a, vars_and_mods_a.size(),
              filename_b, vars_and_mods_b.size());

  int mismatch_count = 0;      // How many differences there were.
  int no_modifiers_count = 0;  // How many variables without modifiers.
  VariableAndMods::const_iterator iter_a, iter_b;
  for (iter_a = vars_and_mods_a.begin(), iter_b = vars_and_mods_b.begin();
       iter_a != vars_and_mods_a.end() && iter_b != vars_and_mods_b.end();
       ++iter_a, ++iter_b) {
    // The templates have different variables, we fail!
    if (iter_a->variable_name != iter_b->variable_name)
      LogPrintf(LOG_FATAL, "Variable name mismatch: %s vs. %s\n",
                iter_a->variable_name.c_str(),
                iter_b->variable_name.c_str());
    // Variables without modifiers are ignored from the diff. They simply
    // get counted and the count is shown in verbose logging/
    if (iter_a->modifiers == "" || iter_b->modifiers == "") {
      no_modifiers_count++;
    } else {
      if (iter_a->modifiers != iter_b->modifiers &&
          !SuppressLameDiff(iter_a->modifiers, iter_b->modifiers)) {
        mismatch_count++;
        LogPrintf(LOG_INFO, "Difference for variable %s -- %s vs. %s\n",
                  iter_a->variable_name.c_str(),
                  iter_a->modifiers.c_str(), iter_b->modifiers.c_str());
      }
    }
  }

  LogPrintf(LOG_VERBOSE, "Variables Found: Total=%d; Diffs=%d; NoMods=%d\n",
            vars_and_mods_a.size(), mismatch_count, no_modifiers_count);

  return (mismatch_count == 0);
}

int main(int argc, char **argv) {
#if defined(HAVE_GETOPT_LONG)
  static struct option longopts[] = {
    {"help", 0, NULL, 'h'},
    {"strip", 1, NULL, 's'},
    {"template_dir", 1, NULL, 't'},
    {"verbose", 0, NULL, 'v'},
    {"version", 0, NULL, 'V'},
    {0, 0, 0, 0}
  };
  int option_index;
# define GETOPT(argc, argv)  getopt_long(argc, argv, "t:s:hvV", \
                                         longopts, &option_index)
#elif defined(HAVE_GETOPT_H)
# define GETOPT(argc, argv)  getopt(argc, argv, "t:s:hvV")
#else
  // TODO(csilvers): implement something reasonable for windows/etc
# define GETOPT(argc, argv)  -1
  int optind = 1;    // first non-opt argument
  const char* optarg = "";   // not used
#endif

  int r = 0;
  while (r != -1) {   // getopt()/getopt_long() return -1 upon no-more-input
    r = GETOPT(argc, argv);
    switch (r) {
      case 's': FLAG_strip.assign(optarg); break;
      case 't': FLAG_template_dir.assign(optarg); break;
      case 'v': FLAG_verbose = true; break;
      case 'V': Version(stdout); break;
      case -1: break;   // means 'no more input'
      default: Usage(argv[0], stderr);
    }
  }

  Template::SetTemplateRootDirectory(FLAG_template_dir);


  if (argc != (optind + 2))
    LogPrintf(LOG_FATAL,
              "Must specify exactly two template files on the command line.\n");

  // Validate the Strip value. Default is STRIP_WHITESPACE.
  Strip strip = STRIP_WHITESPACE;   // To avoid compiler warnings.
  if (FLAG_strip == "STRIP_WHITESPACE" || FLAG_strip == "")
    strip = STRIP_WHITESPACE;
  else if (FLAG_strip == "STRIP_BLANK_LINES")
    strip = STRIP_BLANK_LINES;
  else if (FLAG_strip == "DO_NOT_STRIP")
    strip = DO_NOT_STRIP;
  else
    LogPrintf(LOG_FATAL, "Unrecognized Strip: %s. Must be one of: "
              "STRIP_WHITESPACE, STRIP_BLANK_LINES or DO_NOT_STRIP\n",
              FLAG_strip.c_str());

  const char* filename_a = argv[optind];
  const char* filename_b = argv[optind + 1];
  LogPrintf(LOG_VERBOSE, "------ Diff of [%s, %s] ------\n",
            filename_a, filename_b);

  if (DiffTemplates(filename_a, filename_b, strip))
    return 0;
  else
    return 1;
}
