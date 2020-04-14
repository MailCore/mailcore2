#!/bin/bash
#
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
#
# Author: falmeida@google.com (Filipe Almeida)

die() {
  echo "Test failed: $@" 1>&2
  exit 1
}
TEST_SRCDIR=${1:-$TEST_SRCDIR}
TOOLS_DIR="$TEST_SRCDIR/src/htmlparser"
TESTDATA_DIR="$TEST_SRCDIR/src/tests/htmlparser_testdata"

# Find input files
INPUT_FILE="$TESTDATA_DIR/sample_fsm.config"
OUTPUT_FILE="$TESTDATA_DIR/sample_fsm.c"
GENERATE_FSM="$TOOLS_DIR/generate_fsm.py"

EXPECTED="`cat $OUTPUT_FILE`"
if [ -z "$EXPECTED" ]; then die "Error reading $OUTPUT_FILE"; fi

# Let's make sure the script works with python2.2 and above
for PYTHON in "" "python2.2" "python2.3" "python2.4" "python2.5" "python2.6"; do
  # Skip the versions of python that are not installed.
  if [ -n "$PYTHON" ]; then
    $PYTHON -h >/dev/null 2>/dev/null || continue
  else    # use the python that's in the shebang line
    SHEBANG_PYTHON=`head -n1 "$GENERATE_FSM" | tr -d '#!'`
    # SHEBANG_PYTHON could be something like "env python" so don't quotify it
    $SHEBANG_PYTHON -h >/dev/null 2>/dev/null || continue
  fi
  echo "-- Running $PYTHON $GENERATE_FSM $INPUT_FILE"
  # The tr is to get rid of windows-style line endings (\r)
  GENERATED="`$PYTHON $GENERATE_FSM $INPUT_FILE | tr -d '\015'`"
  if [ -z "$GENERATED" ]; then die "Error running $GENERATE_FSM"; fi

  if [ "$EXPECTED" != "$GENERATED" ]; then
    echo "Test failed ($PYTHON $GENERATE_FSM $INPUT_FILE)" 1>&2
    echo "-- EXPECTED --" 1>&2
    echo "$EXPECTED" 1>&2
    echo "-- GENERATED --"  1>&2
    echo "$GENERATED" 1>&2
    echo "--"
    exit 1
  fi
done

echo "PASS"
