#!/bin/sh
# This is a shell archive (produced by GNU sharutils 4.2.1).
# To extract the files from this archive, save it to some FILE, remove
# everything before the `!/bin/sh' line above, then type `sh FILE'.
#
# Made on 2006-08-08 13:38 PDT by <patlac@borabora.crchul.ulaval.ca>.
# Commandline: shar -T .vim
#
# Existing files will *not* be overwritten unless `-c' is specified.
#
# This shar contains:
# length mode       name
# ------ ---------- ------------------------------------------
#   1477 -rw-r--r-- .vim/syntax/tpl.vim
#     56 -rw-r--r-- .vim/ftdetect/tpl.vim
#
echo >/dev/null <<_NOTES_EOF
From: Patrick Lacasse <patlac@borabora.crchul.ulaval.ca>
Subject: vim color for google-ctemplate howto
To: google-ctemplate@googlegroups.com
Date: Fri, 4 Aug 2006 11:38:39 -0400

Hi group,

  I'm now using google-ctemplate. My text editor is vim. Here is a little gift 
for other people like me.

Howto have google-ctemplate colored by vim :

In your home directory, run 'sh $0'.

Now restart vim.

This will autodetects file with tpl extension and colors them. You can change 
the color by changing the HiLink lines ( try changing String by 
Identifier ) .

I'm not sure exactly about what are the legal marker names. Feel free to 
change the regexes.

I only tryed this with vim 6.4 , I just cut and past the things about version 
checking.

For more information about syntax higlithning in vim, try 
:help syntax

Amusez-vous bien,

Patrick Lacasse
patlac@borabora.crchul.ulaval.ca
_NOTES_EOF

save_IFS="${IFS}"
IFS="${IFS}:"
gettext_dir=FAILED
locale_dir=FAILED
first_param="$1"
for dir in $PATH
do
  if test "$gettext_dir" = FAILED && test -f $dir/gettext \
     && ($dir/gettext --version >/dev/null 2>&1)
  then
    set `$dir/gettext --version 2>&1`
    if test "$3" = GNU
    then
      gettext_dir=$dir
    fi
  fi
  if test "$locale_dir" = FAILED && test -f $dir/shar \
     && ($dir/shar --print-text-domain-dir >/dev/null 2>&1)
  then
    locale_dir=`$dir/shar --print-text-domain-dir`
  fi
done
IFS="$save_IFS"
if test "$locale_dir" = FAILED || test "$gettext_dir" = FAILED
then
  echo=echo
else
  TEXTDOMAINDIR=$locale_dir
  export TEXTDOMAINDIR
  TEXTDOMAIN=sharutils
  export TEXTDOMAIN
  echo="$gettext_dir/gettext -s"
fi
if touch -am -t 200112312359.59 $$.touch >/dev/null 2>&1 && test ! -f 200112312359.59 -a -f $$.touch; then
  shar_touch='touch -am -t $1$2$3$4$5$6.$7 "$8"'
elif touch -am 123123592001.59 $$.touch >/dev/null 2>&1 && test ! -f 123123592001.59 -a ! -f 123123592001.5 -a -f $$.touch; then
  shar_touch='touch -am $3$4$5$6$1$2.$7 "$8"'
elif touch -am 1231235901 $$.touch >/dev/null 2>&1 && test ! -f 1231235901 -a -f $$.touch; then
  shar_touch='touch -am $3$4$5$6$2 "$8"'
else
  shar_touch=:
  echo
  $echo 'WARNING: not restoring timestamps.  Consider getting and'
  $echo "installing GNU \`touch', distributed in GNU File Utilities..."
  echo
fi
rm -f 200112312359.59 123123592001.59 123123592001.5 1231235901 $$.touch
#
if mkdir _sh09814; then
  $echo 'x -' 'creating lock directory'
else
  $echo 'failed to create lock directory'
  exit 1
fi
# ============= .vim/syntax/tpl.vim ==============
if test ! -d '.vim'; then
  $echo 'x -' 'creating directory' '.vim'
  mkdir '.vim'
fi
if test ! -d '.vim/syntax'; then
  $echo 'x -' 'creating directory' '.vim/syntax'
  mkdir '.vim/syntax'
fi
if test -f '.vim/syntax/tpl.vim' && test "$first_param" != -c; then
  $echo 'x -' SKIPPING '.vim/syntax/tpl.vim' '(file already exists)'
else
  $echo 'x -' extracting '.vim/syntax/tpl.vim' '(text)'
  sed 's/^X//' << 'SHAR_EOF' > '.vim/syntax/tpl.vim' &&
" Vim syntax file
" Language:     google-ctemplate
" Maintainer:   Patrick Lacasse <patlac@borabora.crchul.ulaval.ca>
" Last Change:  2006 Août 03
"
" For information about google-ctemplate see 
"   http://goog-ctemplate.sourceforge.net/
"
" This vim syntax file works on vim 5.6, 5.7, 5.8 and 6.x.
" It implements Bram Moolenaar's April 25, 2001 recommendations to make
" the syntax file maximally portable across different versions of vim.
X
" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
X  syntax clear
elseif exists("b:current_syntax")
X  finish
endif
X
syntax match tplMarkerError "{{}\?\([^}]\+}\?\)*}}"
syntax match tplSectionMarker "{{[#/][A-Za-z_]\+}}"
syntax match tplInclude "{{>[A-Za-z_]\+}}"
syntax match tplComment "{{![A-Za-z_]\+}}"
syntax match tplVariableMarker "{{[_A-Za-z]\+}}"
X
" Define the default highlighting.
" For version 5.7 and earlier: only when not done already
" For version 5.8 and later: only when an item doesn't have highlighting yet
if version >= 508 || !exists("did_tpl_syn_inits")
X  if version < 508
X    let did_tpl_syn_inits = 1
X    command -nargs=+ HiLink hi link <args>
X  else
X    command -nargs=+ HiLink hi def link <args>
X  endif
X
X  HiLink tplSectionMarker   Repeat
X  HiLink tplInclude         Include
X  HiLink tplComment         Comment
X  HiLink tplVariableMarker  String
X  HiLink tplMarkerError     Error
X
X  delcommand HiLink
endif
X
let b:current_syntax = "tpl"
SHAR_EOF
  (set 20 06 08 08 13 34 11 '.vim/syntax/tpl.vim'; eval "$shar_touch") &&
  chmod 0644 '.vim/syntax/tpl.vim' ||
  $echo 'restore of' '.vim/syntax/tpl.vim' 'failed'
  if ( md5sum --help 2>&1 | grep 'sage: md5sum \[' ) >/dev/null 2>&1 \
  && ( md5sum --version 2>&1 | grep -v 'textutils 1.12' ) >/dev/null; then
    md5sum -c << SHAR_EOF >/dev/null 2>&1 \
    || $echo '.vim/syntax/tpl.vim:' 'MD5 check failed'
536faef79eff0597e642c5db04c1f79d  .vim/syntax/tpl.vim
SHAR_EOF
  else
    shar_count="`LC_ALL= LC_CTYPE= LANG= wc -c < '.vim/syntax/tpl.vim'`"
    test 1477 -eq "$shar_count" ||
    $echo '.vim/syntax/tpl.vim:' 'original size' '1477,' 'current size' "$shar_count!"
  fi
fi
# ============= .vim/ftdetect/tpl.vim ==============
if test ! -d '.vim/ftdetect'; then
  $echo 'x -' 'creating directory' '.vim/ftdetect'
  mkdir '.vim/ftdetect'
fi
if test -f '.vim/ftdetect/tpl.vim' && test "$first_param" != -c; then
  $echo 'x -' SKIPPING '.vim/ftdetect/tpl.vim' '(file already exists)'
else
  $echo 'x -' extracting '.vim/ftdetect/tpl.vim' '(text)'
  sed 's/^X//' << 'SHAR_EOF' > '.vim/ftdetect/tpl.vim' &&
au BufRead,BufNewFile *.tpl            set filetype=tpl
SHAR_EOF
  (set 20 06 08 08 13 34 20 '.vim/ftdetect/tpl.vim'; eval "$shar_touch") &&
  chmod 0644 '.vim/ftdetect/tpl.vim' ||
  $echo 'restore of' '.vim/ftdetect/tpl.vim' 'failed'
  if ( md5sum --help 2>&1 | grep 'sage: md5sum \[' ) >/dev/null 2>&1 \
  && ( md5sum --version 2>&1 | grep -v 'textutils 1.12' ) >/dev/null; then
    md5sum -c << SHAR_EOF >/dev/null 2>&1 \
    || $echo '.vim/ftdetect/tpl.vim:' 'MD5 check failed'
774fd4a092b77400ef6e74a7256ff8ef  .vim/ftdetect/tpl.vim
SHAR_EOF
  else
    shar_count="`LC_ALL= LC_CTYPE= LANG= wc -c < '.vim/ftdetect/tpl.vim'`"
    test 56 -eq "$shar_count" ||
    $echo '.vim/ftdetect/tpl.vim:' 'original size' '56,' 'current size' "$shar_count!"
  fi
fi
rm -fr _sh09814
exit 0
