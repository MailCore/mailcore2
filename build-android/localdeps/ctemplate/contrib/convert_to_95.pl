#!/usr/bin/perl -pli.bak

# Copyright (c) 2006, Google Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# ---
# Author: Craig Silverstein

# This is a perl script for editing your source files that were
# written with ctemplate <0.95, to use the new #include paths,
# namespace names, and function locations in ctemplate 0.95.
#
# This script will modify your files in place.  It is safe to
# call it multiple times.  To call it, do something like:
#    find <root of my project tree> -print0 | xargs -0 ./convert_to_95.pl
#
# Issues:
#    TemplateFromString logic is tricky.  Best to check up by hand.
#    Can't always tell when 1st arg to TemplateFromString is empty string.
#      If it might be for your app, change the logic to use StringToTemplate.
#    Fails if TemplateFromString or StringToTemplate span multiple lines.

# Change all the #include lines
s@google/template.h@ctemplate/template.h@g;
s@google/per_expand_data.h@ctemplate/per_expand_data.h@g;
s@google/template.h@ctemplate/template.h@g;
s@google/template_annotator.h@ctemplate/template_annotator.h@g;
s@google/template_dictionary.h@ctemplate/template_dictionary.h@g;
s@google/template_dictionary_interface.h@ctemplate/template_dictionary_interface.h@g;
s@google/template_emitter.h@ctemplate/template_emitter.h@g;
s@google/template_enums.h@ctemplate/template_enums.h@g;
s@google/template_from_string.h@ctemplate/template_from_string.h@g;
s@google/template_modifiers.h@ctemplate/template_modifiers.h@g;
s@google/template_namelist.h@ctemplate/template_namelist.h@g;
s@google/template_pathops.h@ctemplate/template_pathops.h@g;
s@google/template_string.h@ctemplate/template_string.h@g;

# Change all the namespace uses
s@namespace\s+google@namespace ctemplate@g;   # brought into google namespace
s@\bgoogle::@ctemplate::@g;
s@namespace\s+template_modifiers\s*{@@ && ($in_tm = 1);
$in_tm && s@}@@ && ($in_tm = 0);
s@\btemplate_modifiers::@@g;

# Change TemplateFromString::GetTemplate to
#   Template::StringToTemplateCache(cache_key, template_text);
#   return Template::GetTemplate(cache_key, strip);
# unless cache_key is the empty string, in which case it should be
#   return Template::StringToTemplate(template_text, strip, TC_MANUAL);
# We have to be tricky, to do this in one command.
# Better would be to just do Template::StringToTemplate(), but then we
# have to free the memory when we're done, which is too hard to do here.
s@TemplateFromString::GetTemplate\s*\(""\s*,([^,]+),([^,]+)\)@Template::StringToTemplate($1, $2, ctemplate::TC_MANUAL)@;
s@TemplateFromString::GetTemplate\s*\(([^,]+),([^,]+),([^,]+)\)@Template::GetTemplate((ctemplate::Template::StringToTemplateCache($1, $2), $1), $3)@;

if (m@TemplateFromString::GetTemplate\s*\([^)]*$@) {
  warn("$ARGV:$.: TemplateFromString spans mutiple lines.  You will need " .
       "to clean it up by hand.  See comments in $0.");
}

# Get rid of the 4th arg to Template::TemplateToString.  If it's
# TC_MANUAL, that's fine.  If it's not, we need to tell the user to
# fix this up manually by adding an {{%PRAGMA AUTOESCAPE}} to the
# beginning of their string.
if (s@(\bStringToTemplate\s*\([^)]*),\s*TC_([A-Z_]*)@$1@g) {
  if ($2 ne "MANUAL") {    # TC_MANUAL is the new default
    warn("WARNING: $ARGV:$.: You will need to manually fix up the " .
         "StringToTemplate change to add an AUTOESCAPE {{\%pragma}} to " .
         "the beginning of the input string, to set the context to TC_$2\n");
  }
}
if (m@\bStringToTemplate\s*\([^)]*$@) {
  warn("$ARGV:$.: StringToTemplate spans mutiple lines.  You will need " .
       "to clean it up by hand: get rid of the TC_foo arg.  If it's not " .
       "TC_MANUAL, you will need to add an AUTOESCAPE pragma.  " .
       "See comments in $0.");
}

# Change all the TemplateDictionary escapes.  Now they are globals in
# the ctemplate namespace.
s@TemplateDictionary::html_escape@html_escape@g;
s@TemplateDictionary::pre_escape@pre_escape@g;
s@TemplateDictionary::xml_escape@xml_escape@g;
s@TemplateDictionary::javascript_escape@javascript_escape@g;
s@TemplateDictionary::url_query_escape@url_query_escape@g;
s@TemplateDictionary::json_escape@json_escape@g;

# This cleans up anything messy we left behind.
s@\bctemplate::ctemplate::@ctemplate::@g;   # get rid of redundant specifier

close ARGV if eof;        # to reset $. for each file
