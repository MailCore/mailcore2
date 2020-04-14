#!/bin/sh

# Copyright (c) 2008, Google Inc.
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
# Author: jad@google.com (Jad Boutros)
# Heavily inspired from make_tpl_varnames_h_unittest.sh
#
#
# TODO(jad): Add better testing of Strip mode.
# TODO(jad): Add testing for (lame) suppressed diffs.

die() {
    echo "Test failed: $@" 1>&2
    exit 1
}

TEST_SRCDIR=${TEST_SRCDIR-"."}
TEST_TMPDIR=${TMPDIR-"/tmp"}

# Optional first argument is where the executable lives
DIFFTPL=${1-"$TEST_SRCDIR/diff_tpl_auto_escape"}

# Optional second argument is tmpdir to use
TMPDIR=${2-"$TEST_TMPDIR/difftpl"}

rm -rf $TMPDIR
mkdir $TMPDIR || die "$LINENO: Can't make $TMPDIR"

# Let's make some templates
# ok1.tpl is valid HTML and has one correct modifier.
echo '<a href="{{URL}}">{{USER:h}}</a>' > $TMPDIR/ok1.tpl

# ok2.tpl is valid HTML and has one right and one wrong modifier.
echo '<a href="{{URL:U=html}}">{{USER:j}}</a>' > $TMPDIR/ok2.tpl

# ok3.tpl is valid HTML and has both wrong modifiers.
echo '<a href="{{URL:h}}">{{USER:c}}</a>' > $TMPDIR/ok3.tpl

# ok4.tpl is valid HTML and is auto-escaped.
echo '{{%AUTOESCAPE context="HTML"}}' \
     '<a href="{{URL}}">{{USER}}</a>' > $TMPDIR/ok4.tpl
# bad1.tpl will fail auto-escape parsing.
echo '{{%AUTOESCAPE context="HTML"}}<a href={{QC' > $TMPDIR/bad1.tpl

# First, test commandline flags
$DIFFTPL >/dev/null 2>&1 \
   && die "$LINENO: $DIFFTPL with no args didn't give an error"
$DIFFTPL --strip=STRIP_WHITESPACE >/dev/null 2>&1 \
   && die "$LINENO: $DIFFTPL with no template didn't give an error"
$DIFFTPL $TMPDIR/ok1.tpl >/dev/null 2>&1 \
   && die "$LINENO: $DIFFTPL with only one template didn't give an error"
$DIFFTPL -sFOO $TMPDIR/ok1.tpl $TMPDIR/ok1.tpl >/dev/null 2>&1 \
   && die "$LINENO: $DIFFTPL with bad strip didn't given an error"
$DIFFTPL --strip=BLA $TMPDIR/ok1.tpl $TMPDIR/ok1.tpl >/dev/null 2>&1 \
   && die "$LINENO: $DIFFTPL with other bad strip didn't given an error"
$DIFFTPL -h >/dev/null 2>&1 \
   || die "$LINENO: $DIFFTPL -h failed"
$DIFFTPL --help >/dev/null 2>&1 \
   || die "$LINENO: $DIFFTPL --help failed"

# Some weird (broken) shells leave the ending EOF in the here-document,
# hence the grep.
# Diff between ok1.tpl and ok4.tpl. No differences.
expected_test1_verbose=`cat <<EOF | grep -v '^EOF$'
[VERBOSE] ------ Diff of [$TMPDIR/ok1.tpl, $TMPDIR/ok4.tpl] ------
[VERBOSE] Variables Found: Total=2; Diffs=0; NoMods=1
EOF`

expected_test1=`cat <<EOF | grep -v '^EOF$'
EOF`

# Diff between ok1.tpl and ok2.tpl. Expect one difference.
expected_test2_verbose=`cat <<EOF | grep -v '^EOF$'
[VERBOSE] ------ Diff of [$TMPDIR/ok1.tpl, $TMPDIR/ok2.tpl] ------
Difference for variable USER -- :h vs. :j
[VERBOSE] Variables Found: Total=2; Diffs=1; NoMods=1
EOF`

expected_test2=`cat <<EOF | grep -v '^EOF$'
Difference for variable USER -- :h vs. :j
EOF`

# Diff between ok3.tpl and ok4.tpl. Expect two differences.
expected_test3_verbose=`cat <<EOF |  grep -v '^EOF$'
[VERBOSE] ------ Diff of [$TMPDIR/ok3.tpl, $TMPDIR/ok4.tpl] ------
Difference for variable URL -- :h vs. :U=html
Difference for variable USER -- :c vs. :h
[VERBOSE] Variables Found: Total=2; Diffs=2; NoMods=0
EOF`

expected_test3=`cat <<EOF |  grep -v '^EOF$'
Difference for variable URL -- :h vs. :U=html
Difference for variable USER -- :c vs. :h
EOF`

# Diff between ok2.tpl and ok3.tpl. Expect two differences.
expected_test4_verbose=`cat <<EOF |  grep -v '^EOF$'
[VERBOSE] ------ Diff of [$TMPDIR/ok2.tpl, $TMPDIR/ok3.tpl] ------
Difference for variable URL -- :U=html vs. :h
Difference for variable USER -- :j vs. :c
[VERBOSE] Variables Found: Total=2; Diffs=2; NoMods=0
EOF`

expected_test4=`cat <<EOF |  grep -v '^EOF$'
Difference for variable URL -- :U=html vs. :h
Difference for variable USER -- :j vs. :c
EOF`


# syntax-check these templates
echo "TMPDIR is: $TMPDIR"
$DIFFTPL $TMPDIR/ok1.tpl $TMPDIR/ok1.tpl >/dev/null 2>&1 \
   || die "$LINENO: $DIFFTPL gave error parsing identical templates"
$DIFFTPL $TMPDIR/ok1.tpl $TMPDIR/bad1.tpl >/dev/null 2>&1 \
   && die "$LINENO: $DIFFTPL gave no error parsing bad template"
$DIFFTPL $TMPDIR/ok100.tpl $TMPDIR/ok1.tpl >/dev/null 2>&1 \
   && die "$LINENO: $DIFFTPL gave no error parsing non-existent template"

# Now try the same thing, but use template-root so we don't need absdirs
$DIFFTPL --template_root=$TMPDIR ok2.tpl ok2.tpl >/dev/null 2>&1 \
   || die "$LINENO: $DIFFTPL gave error parsing identical templates"

# Diffing the same template produces exit code 0. Check with all Strip values.
$DIFFTPL -sSTRIP_WHITESPACE $TMPDIR/ok1.tpl $TMPDIR/ok1.tpl >/dev/null 2>&1
[ $? = 0 ] || die "$LINENO: $DIFFTPL: wrong error-code on same template: $?"
$DIFFTPL -sSTRIP_BLANK_LINES $TMPDIR/ok1.tpl $TMPDIR/ok1.tpl >/dev/null 2>&1
[ $? = 0 ] || die "$LINENO: $DIFFTPL: wrong error-code on same template: $?"
$DIFFTPL -sDO_NOT_STRIP $TMPDIR/ok1.tpl $TMPDIR/ok1.tpl >/dev/null 2>&1
[ $? = 0 ] || die "$LINENO: $DIFFTPL: wrong error-code on same template: $?"

# Diffing templates with diff, should produce exit code 1.
$DIFFTPL $TMPDIR/ok1.tpl $TMPDIR/ok2.tpl >/dev/null 2>&1
[ $? = 1 ] || die "$LINENO: $DIFFTPL: wrong error-code on diff. templates: $?"

# Diffing templates with failure, should produce exit code 1.
$DIFFTPL $TMPDIR/ok1.tpl $TMPDIR/ok100.tpl >/dev/null 2>&1
[ $? = 1 ] || die "$LINENO: $DIFFTPL: wrong error-code on failed template: $?"

# If you use relative filenames, must first fix expected outputs.
out=`$DIFFTPL -v $TMPDIR/ok1.tpl $TMPDIR/ok4.tpl 2>&1`
[ "$out" != "$expected_test1_verbose" ] &&\
  die "$LINENO: $DIFFTPL: bad output for test1_verbose: $out\n"
out=`$DIFFTPL $TMPDIR/ok1.tpl $TMPDIR/ok4.tpl 2>&1`
[ "$out" != "$expected_test1" ] &&\
  die "$LINENO: $DIFFTPL: bad output for test1: $out\n"

out=`$DIFFTPL -v $TMPDIR/ok1.tpl $TMPDIR/ok2.tpl 2>&1`
[ "$out" != "$expected_test2_verbose" ] &&\
  die "$LINENO: $DIFFTPL: bad output for test2_verbose: $out\n"
out=`$DIFFTPL $TMPDIR/ok1.tpl $TMPDIR/ok2.tpl 2>&1`
[ "$out" != "$expected_test2" ] &&\
  die "$LINENO: $DIFFTPL: bad output for test2: $out\n"

out=`$DIFFTPL -v $TMPDIR/ok3.tpl $TMPDIR/ok4.tpl 2>&1`
[ "$out" != "$expected_test3_verbose" ] &&\
  die "$LINENO: $DIFFTPL: bad output for test3_verbose: $out\n"
out=`$DIFFTPL $TMPDIR/ok3.tpl $TMPDIR/ok4.tpl 2>&1`
[ "$out" != "$expected_test3" ] &&\
  die "$LINENO: $DIFFTPL: bad output for test3: $out\n"

out=`$DIFFTPL -v $TMPDIR/ok2.tpl $TMPDIR/ok3.tpl 2>&1`
[ "$out" != "$expected_test4_verbose" ] &&\
  die "$LINENO: $DIFFTPL: bad output for test4_verbose: $out\n"
out=`$DIFFTPL $TMPDIR/ok2.tpl $TMPDIR/ok3.tpl 2>&1`
[ "$out" != "$expected_test4" ] &&\
  die "$LINENO: $DIFFTPL: bad output for test4: $out\n"

echo "PASSED"
