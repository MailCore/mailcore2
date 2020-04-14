#!/bin/sh

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

# ---
# Author: csilvers@google.com (Craig Silverstein)
#

die() {
    echo "Test failed: $@" 1>&2
    exit 1
}

MAKETPL=${1-"$TEST_SRCDIR/make_tpl_varnames_h"}

# Optional second argument is tmpdir to use
TMPDIR=${2-"$TEST_TMPDIR/maketpl"}

rm -rf $TMPDIR
mkdir $TMPDIR || die "$LINENO: Can't make $TMPDIR"

# Let's make some templates: three that are ok, and three that are not
echo '<a href={{QCHAR}}{{HREF}}{{QCHAR}} {{PARAMS}}>' > $TMPDIR/ok1.tpl
echo '<img {{#ATTRIBUTES}}{{ATTRIBUTE}}{{/ATTRIBUTES}}>' > $TMPDIR/ok2.tpl
echo '<html><head><title>{{TITLE}}</title></head></html>' > $TMPDIR/ok3.tpl
echo '<a href={{QCHAR}}{{HREF}}{{QC' > $TMPDIR/bad1.tpl
echo '<img {{#ATTRIBUTES}}{{ATTRIBUTE}}>' > $TMPDIR/bad2.tpl
echo '<html><head><title>{{TITLE?}}</title></head></html>' > $TMPDIR/bad3.tpl

# We'll make some templates with modifiers as well.
echo '<a href={{HREF:h}} {{PARAMS}}>' > $TMPDIR/ok4.tpl
echo '<a href={{HREF:html_escape_with_arg=url}} {{PARAMS}}>' > $TMPDIR/ok5.tpl
echo '<a href={{HREF:x-custom-modifier}} {{PARAMS}}>' > $TMPDIR/ok6.tpl
echo '<a href={{HREF:x-custom-modifier=arg}} {{PARAMS}}>' > $TMPDIR/ok7.tpl
echo '<a href={{HREF:x-custom-modifier=}} {{PARAMS}}>' > $TMPDIR/ok8.tpl


# First, test commandline flags
$MAKETPL >/dev/null 2>&1 \
   && die "$LINENO: $MAKETPL with no args didn't give an error"
$MAKETPL -o$TMPDIR/foo >/dev/null 2>&1 \
   && die "$LINENO: $MAKETPL with no template didn't give an error"
$MAKETPL -h >/dev/null 2>&1 \
   || die "$LINENO: $MAKETPL -h failed"
$MAKETPL --help >/dev/null 2>&1 \
   || die "$LINENO: $MAKETPL --help failed"
$MAKETPL --nonsense >/dev/null 2>&1 \
   && die "$LINENO: $MAKETPL --nonsense didn't give an error"
$MAKETPL -f$TMPDIR/bar.h >/dev/null 2>&1 \
   && die "$LINENO: $MAKETPL -f with no templates didn't give an error"

# Some weird (broken) shells leave the ending EOF in the here-document,
# hence the grep.
expected_ok1=`cat <<EOF | grep -v '^EOF$'
#ifndef %%%OUTPUT_NAME%%%
#define %%%OUTPUT_NAME%%%

#include <ctemplate/template_string.h>
static const ::ctemplate::StaticTemplateString ko_QCHAR = STS_INIT_WITH_HASH(ko_QCHAR, "QCHAR", 13739615363438531061ULL);
static const ::ctemplate::StaticTemplateString ko_HREF = STS_INIT_WITH_HASH(ko_HREF, "HREF", 4441707909033668369ULL);
static const ::ctemplate::StaticTemplateString ko_PARAMS = STS_INIT_WITH_HASH(ko_PARAMS, "PARAMS", 10755877064288701757ULL);

#endif  // %%%OUTPUT_NAME%%%
EOF`

expected_ok2=`cat <<EOF | grep -v '^EOF$'
#ifndef %%%OUTPUT_NAME%%%
#define %%%OUTPUT_NAME%%%

#include <ctemplate/template_string.h>
static const ::ctemplate::StaticTemplateString ko_ATTRIBUTES = STS_INIT_WITH_HASH(ko_ATTRIBUTES, "ATTRIBUTES", 11813232524653503831ULL);
static const ::ctemplate::StaticTemplateString ko_ATTRIBUTE = STS_INIT_WITH_HASH(ko_ATTRIBUTE, "ATTRIBUTE", 14959290143384361001ULL);

#endif  // %%%OUTPUT_NAME%%%
EOF`

expected_ok3=`cat <<EOF | grep -v '^EOF$'
#ifndef %%%OUTPUT_NAME%%%
#define %%%OUTPUT_NAME%%%

#include <ctemplate/template_string.h>
static const ::ctemplate::StaticTemplateString ko_TITLE = STS_INIT_WITH_HASH(ko_TITLE, "TITLE", 8931122033088041025ULL);

#endif  // %%%OUTPUT_NAME%%%
EOF`

expected_ok4=`cat <<EOF | grep -v '^EOF$'
#ifndef %%%OUTPUT_NAME%%%
#define %%%OUTPUT_NAME%%%

#include <ctemplate/template_string.h>
static const ::ctemplate::StaticTemplateString ko_HREF = STS_INIT_WITH_HASH(ko_HREF, "HREF", 4441707909033668369ULL);
static const ::ctemplate::StaticTemplateString ko_PARAMS = STS_INIT_WITH_HASH(ko_PARAMS, "PARAMS", 10755877064288701757ULL);

#endif  // %%%OUTPUT_NAME%%%
EOF`

expected_ok5=`echo "$expected_ok4" | sed s/ok4/ok5/g`
expected_ok6=`echo "$expected_ok4" | sed s/ok4/ok6/g`
expected_ok7=`echo "$expected_ok4" | sed s/ok4/ok7/g`
expected_ok8=`echo "$expected_ok4" | sed s/ok4/ok8/g`

# When -f (--output-file) is used on ok1.tpl and ok2.tpl
# Note that there are no variables in common in these two templates.
# All should be returned.
expected_ok1and2=`cat <<EOF | grep -v '^EOF$'
#ifndef %%%OUTPUT_NAME%%%
#define %%%OUTPUT_NAME%%%

#include <ctemplate/template_string.h>
static const ::ctemplate::StaticTemplateString ko_QCHAR = STS_INIT_WITH_HASH(ko_QCHAR, "QCHAR", 13739615363438531061ULL);
static const ::ctemplate::StaticTemplateString ko_HREF = STS_INIT_WITH_HASH(ko_HREF, "HREF", 4441707909033668369ULL);
static const ::ctemplate::StaticTemplateString ko_PARAMS = STS_INIT_WITH_HASH(ko_PARAMS, "PARAMS", 10755877064288701757ULL);
static const ::ctemplate::StaticTemplateString ko_ATTRIBUTES = STS_INIT_WITH_HASH(ko_ATTRIBUTES, "ATTRIBUTES", 11813232524653503831ULL);
static const ::ctemplate::StaticTemplateString ko_ATTRIBUTE = STS_INIT_WITH_HASH(ko_ATTRIBUTE, "ATTRIBUTE", 14959290143384361001ULL);

#endif  // %%%OUTPUT_NAME%%%
EOF`

# When -f (--output-file) is used on ok1.tpl and ok4.tpl
# Note that both variables in ok4.tpl will be duplicates and hence not returned.
expected_ok1and4=`echo "$expected_ok1" | sed s/ok1/ok1and4/g`

# Suppress unimportant aspects of the make_tpl_varnames_h output.
Cleanse() {
  # Replace the file name guard with %%%OUTPUT_NAME%%% so we can use
  # the same expected_ok* variables for different file names.
  # Note that we only append 'H_' to the end of the string, instead
  # of '_H_'.  This is because the first call to 'tr' is already
  # adding a '_' at the end of the converted $1 (due to the newline
  # emitted by echo).
  n="`basename $1 | sed -e 's/[^0-9a-zA-Z]/_/g' | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`"
  grep -v '^//' "$1" | sed -e "s:TPL_.*${n}_H_:%%%OUTPUT_NAME%%%:" > "$1.cleansed"
}

# syntax-check these templates
$MAKETPL -n $TMPDIR/ok1.tpl $TMPDIR/ok2.tpl $TMPDIR/ok3.tpl >/dev/null 2>&1 \
   || die "$LINENO: $MAKETPL gave error parsing good templates"
$MAKETPL -n $TMPDIR/ok1.tpl $TMPDIR/ok2.tpl $TMPDIR/bad3.tpl >/dev/null 2>&1 \
   && die "$LINENO: $MAKETPL gave no error parsing bad template"
$MAKETPL -n $TMPDIR/ok1.tpl $TMPDIR/ok2.tpl $TMPDIR/ok100.tpl >/dev/null 2>&1 \
   && die "$LINENO: $MAKETPL gave no error parsing non-existent template"

# Now try the same thing, but use template-root so we don't need absdirs
$MAKETPL -n --template_dir=$TMPDIR ok1.tpl ok2.tpl ok3.tpl >/dev/null 2>&1 \
   || die "$LINENO: $MAKETPL gave error parsing good templates"

# Parse the templates.  Bad one in the middle should be ignored.
$MAKETPL --header_dir=$TMPDIR $TMPDIR/ok1.tpl $TMPDIR/bad2.tpl $TMPDIR/ok3.tpl >/dev/null 2>&1
[ $? = 1 ] || die "$LINENO: $MAKETPL gave wrong error-code parsing 1 bad template: $?"
Cleanse "$TMPDIR/ok1.tpl.varnames.h"
echo "$expected_ok1" | diff - "$TMPDIR/ok1.tpl.varnames.h.cleansed" \
   || die "$LINENO: $MAKETPL didn't make ok1 output correctly"
[ -f "$TMPDIR/bad2.tpl.varnames.h" ] \
   && die "$LINENO: $MAKETPL >did< make bad2 output"
Cleanse "$TMPDIR/ok3.tpl.varnames.h"
echo "$expected_ok3" | diff - "$TMPDIR/ok3.tpl.varnames.h.cleansed" \
   || die "$LINENO: $MAKETPL didn't make ok3 output correctly"

# Now try the same but with a different suffix.  Try an alternate -t/-o form too.
# Also test not being able to output the file for some reason.
$MAKETPL -t$TMPDIR -o$TMPDIR -s.out ok1.tpl bad2.tpl ok3.tpl >/dev/null 2>&1
[ $? = 1 ] || die "$LINENO: $MAKETPL gave wrong error-code parsing 1 bad template: $?"
Cleanse "$TMPDIR/ok1.tpl.out"
echo "$expected_ok1" | diff - "$TMPDIR/ok1.tpl.out.cleansed" \
   || die "$LINENO: $MAKETPL didn't make ok1 output correctly"
[ -f "$TMPDIR/bad2.tpl.out" ] && die "$LINENO: $MAKETPL >did< make bad2 output"
Cleanse "$TMPDIR/ok3.tpl.out"
echo "$expected_ok3" | diff - "$TMPDIR/ok3.tpl.out.cleansed" \
   || die "$LINENO: $MAKETPL didn't make ok3 output correctly"

# Verify that -f generates the requested output file:
# -f with one file
$MAKETPL -t$TMPDIR -f$TMPDIR/ok1.h ok1.tpl >/dev/null 2>&1
Cleanse "$TMPDIR/ok1.h"
echo "$expected_ok1" | diff - "$TMPDIR/ok1.h.cleansed" \
   || die "$LINENO: $MAKETPL didn't make ok1.h output correctly"
# -f with two files - no common template variables
$MAKETPL -t$TMPDIR -f$TMPDIR/ok1and2.h ok1.tpl ok2.tpl >/dev/null 2>&1
Cleanse "$TMPDIR/ok1and2.h"
echo "$expected_ok1and2" | diff - "$TMPDIR/ok1and2.h.cleansed" \
   || die "$LINENO: $MAKETPL didn't make ok1and2.h output correctly"
# -f with two files - two common template variables
$MAKETPL -t$TMPDIR -f$TMPDIR/ok1and4.h ok1.tpl ok4.tpl >/dev/null 2>&1
Cleanse "$TMPDIR/ok1and4.h"
echo "$expected_ok1and4" | diff - "$TMPDIR/ok1and4.h.cleansed" \
   || die "$LINENO: $MAKETPL didn't make ok1and4.h output correctly"
# -f with a bad file should not produce an output
$MAKETPL -t$TMPDIR -f$TMPDIR/bar.h ok1.tpl bad1.tpl >/dev/null 2>&1 \
  && die "$LINENO: $MAKETPL -f gave no error parsing bad template"

# Verify we don't give any output iff everything works, with -q flag.
# Also test using a different output dir.  Also, test *every* ok template.
mkdir $TMPDIR/output
# Normally I'd do {1,2,3,4,...}, but solaris sh doesn't understand that syntax
out=`$MAKETPL -q -t$TMPDIR -o$TMPDIR/output -s"#" \
     ok1.tpl ok2.tpl ok3.tpl ok4.tpl ok5.tpl ok6.tpl ok7.tpl ok8.tpl \
     2>&1`
[ -z "$out" ] || die "$LINENO: $MAKETPL -q wasn't so quiet: '$out'"
for i in 1 2 3 4 5 6 7 8; do
  Cleanse "$TMPDIR/output/ok$i.tpl#"
  eval "echo \"\$expected_ok$i\"" | diff - "$TMPDIR/output/ok$i.tpl#.cleansed" \
     || die "$LINENO: $MAKETPL didn't make ok$i output correctly"
done

out=`$MAKETPL -q --outputfile_suffix=2 $TMPDIR/bad{1,2,3}.tpl 2>&1`
[ -z "$out" ] && die "$LINENO: $MAKETPL -q was too quiet"
for i in 1 2 3; do
  [ -f "$TMPDIR/output/bad$i.tpl2" ] && die "$LINENO: $MAKETPL made bad$i output"
done

echo "PASSED"
