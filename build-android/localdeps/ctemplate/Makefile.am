## Process this file with automake to produce Makefile.in

# Make sure that when we re-make ./configure, we get the macros we need
ACLOCAL_AMFLAGS = -I m4

# This is so we can #include <ctemplate/foo>
AM_CPPFLAGS = -I$(top_srcdir)/src

# These are good warnings to turn on by default
if GCC
AM_CXXFLAGS = -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare
endif

dist_doc_DATA = AUTHORS ChangeLog NEWS README

# mingw does weird directory munging on /tmp, which causes some of our
# tests to fail, so use a different tmpdir there.
if MINGW
TMPDIR = ./
dist_doc_DATA += README_windows.txt
else
TMPDIR = /tmp
endif

ctemplateincludedir = $(includedir)/ctemplate
## The .h files you want to install (that is, .h files that people
## who install this package can include in their own applications.)
## We have to include both the .h and .h.in forms.  The latter we
## put in noinst_HEADERS.
## NOTE: If you add a file here, also add it to the end of configure.ac
nodist_ctemplateinclude_HEADERS = \
	src/ctemplate/template.h \
	src/ctemplate/template_cache.h \
	src/ctemplate/template_string.h \
	src/ctemplate/template_enums.h \
	src/ctemplate/template_pathops.h \
	src/ctemplate/template_modifiers.h \
	src/ctemplate/template_dictionary.h \
	src/ctemplate/template_dictionary_interface.h \
	src/ctemplate/template_annotator.h \
	src/ctemplate/template_emitter.h \
	src/ctemplate/template_namelist.h \
	src/ctemplate/per_expand_data.h \
	src/ctemplate/str_ref.h
noinst_HEADERS = \
	src/ctemplate/template.h.in \
	src/ctemplate/template_cache.h.in \
	src/ctemplate/template_string.h.in \
	src/ctemplate/template_enums.h.in \
	src/ctemplate/template_pathops.h.in \
	src/ctemplate/template_modifiers.h.in \
	src/ctemplate/template_dictionary.h.in \
	src/ctemplate/template_dictionary_interface.h.in \
	src/ctemplate/template_annotator.h.in \
	src/ctemplate/template_emitter.h.in \
	src/ctemplate/template_namelist.h.in \
	src/ctemplate/per_expand_data.h.in \
	src/ctemplate/str_ref.h.in

## This is for HTML and other documentation you want to install.
## Add your documentation files (in doc/) in addition to these
## top-level boilerplate files.  Also add a TODO file if you have one.
dist_html_DATA = doc/designstyle.css doc/index.html \
	doc/howto.html doc/guide.html doc/reference.html \
	doc/tips.html doc/example.html doc/auto_escape.html \
	doc/xss_resources.html

## The libraries (.so's) you want to install
lib_LTLIBRARIES =
## Libraries that we want to use only internally (for our unittest binaries)
noinst_LTLIBRARIES =
## The binaries you want to install
bin_PROGRAMS =
bin_SCRIPTS =
## The location of the windows project file for each binary we make
WINDOWS_PROJECTS = ctemplate.sln


## unittests you want to run when people type 'make check'.
## TESTS is for binary unittests, check_SCRIPTS for script-based unittests.
## TESTS_ENVIRONMENT sets environment variables for when you run unittest,
## but it only seems to take effect for *binary* unittests (argh!)
TESTS =
check_SCRIPTS =
TESTS_ENVIRONMENT = TEMPLATE_ROOTDIR=$(top_srcdir)
# Every time you add a unittest to check_SCRIPTS, add it here too
noinst_SCRIPTS =
# Add to this for tests that use data
TESTDATA =

## vvvv RULES TO MAKE THE LIBRARIES, BINARIES, AND UNITTESTS

# These files are auto-generated via generate_fsm.py.  Since we don't want
# to require python to build ctemplate, we include these in the dist
src/htmlparser/htmlparser_fsm.h: src/htmlparser/generate_fsm.py src/htmlparser/fsm_config.py src/htmlparser/htmlparser_fsm.config
	$(top_srcdir)/src/htmlparser/generate_fsm.py $(top_srcdir)/src/htmlparser/htmlparser_fsm.config > $@

src/htmlparser/jsparser_fsm.h: src/htmlparser/generate_fsm.py src/htmlparser/fsm_config.py src/htmlparser/jsparser_fsm.config
	$(top_srcdir)/src/htmlparser/generate_fsm.py $(top_srcdir)/src/htmlparser/jsparser_fsm.config > $@

src/tests/statemachine_test_fsm.h: src/htmlparser/generate_fsm.py src/htmlparser/fsm_config.py src/tests/statemachine_test_fsm.config
	$(top_srcdir)/src/htmlparser/generate_fsm.py $(top_srcdir)/src/tests/statemachine_test_fsm.config > $@

# This is a required hack for auto-generated .h files: cf the automake info pages
# NOTE: since we put these .h files in BUILT_SOURCES, we don't need to put
# them in as deps for the binaries that use them.  In fact, it's important
# that we don't; instead we put them in EXTRA_DIST.  This mean, in practice,
# they'll go at the end of the distribution tarfile, which means they'll
# have a later timestamp than the .config files that generate them, which
# means users won't try to rebuild them.  Ah, the joys of automake.
BUILT_SOURCES = src/htmlparser/htmlparser_fsm.h \
                src/htmlparser/jsparser_fsm.h \
                src/tests/statemachine_test_fsm.h


# These are the symbols (classes, mostly) we want to export from our library.
# Note this regexp applies to the *mangled* name, which is why we have to
# be careful where we want to assert [^A-Za-z].  (Particularly bad on Darwin.)
# template_modifiers and BaseArena and UnsafeArena perhaps shouldn't be
# exported, but they're needed -- at least -- for some unittests.
CTEMPLATE_SYMBOLS = '(ctemplate|template_modifiers|BaseArena|UnsafeArena|[^A-Za-z]Template[^A-Za-z]|TemplateDictionary|TemplateNamelist|TemplateFromString|TemplateString|TemplateState|[^A-Za-z]Strip[^A-Za-z])'

lib_LTLIBRARIES += libctemplate.la
WINDOWS_PROJECTS += vsprojects/libctemplate/libctemplate.vcxproj
libctemplate_la_SOURCES = $(nodist_ctemplateinclude_HEADERS) \
	src/base/arena-inl.h \
	src/base/arena.cc \
	src/base/arena.h \
	src/base/fileutil.h \
	src/base/macros.h \
	src/base/manual_constructor.h \
	src/base/mutex.h \
	src/base/small_map.h \
	src/base/thread_annotations.h \
	src/base/util.h \
	src/indented_writer.h \
	src/per_expand_data.cc \
	src/template.cc \
	src/template_annotator.cc \
	src/template_cache.cc \
	src/template_dictionary.cc \
	src/template_modifiers.cc \
	src/template_modifiers_internal.h \
	src/template_namelist.cc \
	src/template_pathops.cc \
	src/template_string.cc \
	src/htmlparser/htmlparser.cc \
	src/htmlparser/htmlparser.h \
	src/htmlparser/htmlparser_cpp.h \
	src/htmlparser/jsparser.cc \
	src/htmlparser/jsparser.h \
	src/htmlparser/statemachine.cc \
	src/htmlparser/statemachine.h
libctemplate_la_CXXFLAGS = $(PTHREAD_CFLAGS) -DNDEBUG $(AM_CXXFLAGS)
# -version-info gets passed to libtool
libctemplate_la_LDFLAGS = $(PTHREAD_CFLAGS) \
                          -export-symbols-regex $(CTEMPLATE_SYMBOLS) \
                          -no-undefined \
                          -version-info @SO_VERSION@
libctemplate_la_LIBADD = $(PTHREAD_LIBS)

# For MinGW, we need to bring in the windows port files
if MINGW
libctemplate_la_SOURCES += src/windows/port.h src/windows/port.cc
endif MINGW

# automake will make different .o files for this library, which is good,
# because we use an extra compiler flag.
lib_LTLIBRARIES += libctemplate_nothreads.la
libctemplate_nothreads_la_SOURCES = $(libctemplate_la_SOURCES)
libctemplate_nothreads_la_DEPENDENCIES = $(libctemplate_la_DEPENDENCIES)
libctemplate_nothreads_la_CXXFLAGS = -DNDEBUG -DNO_THREADS $(AM_CXXFLAGS)
libctemplate_nothreads_la_LDFLAGS = -export-symbols-regex $(CTEMPLATE_SYMBOLS) \
                                    -no-undefined \
                                    -version-info @SO_VERSION@

# For our tests, we want versions of these libraries that include asserts.
noinst_LTLIBRARIES += libctemplate_debug.la
libctemplate_debug_la_SOURCES = $(libctemplate_la_SOURCES)
libctemplate_debug_la_DEPENDENCIES = $(libctemplate_la_DEPENDENCIES)
libctemplate_debug_la_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)
libctemplate_debug_la_LDFLAGS = $(libctemplate_la_LDFLAGS)
libctemplate_debug_la_LIBADD = $(libctemplate_la_LIBADD)
noinst_LTLIBRARIES += libctemplate_nothreads_debug.la
libctemplate_nothreads_debug_la_SOURCES = $(libctemplate_nothreads_la_SOURCES)
libctemplate_nothreads_debug_la_DEPENDENCIES = $(libctemplate_nothreads_la_DEPENDENCIES)
libctemplate_nothreads_debug_la_CXXFLAGS = -DNO_THREADS $(AM_CXXFLAGS)
libctemplate_nothreads_debug_la_LDFLAGS = $(libctemplate_nothreads_la_LDFLAGS)

# We could also make a library that has the TemplateDictionaryPeer
# class.  This class is useful for testing (it provides introspection
# on the TemplateDictionary hierarchy that's easier to use than the
# default debugging method, TemplateDictionary::Dump()) but by the
# same token also violates abstraction in ways that might be fragile.
# We don't expose the library for now, until the demonstrated need
# outweighs the costs.  If you'd like to use this library, please send
# mail to google-ctemplate@googlegroups.com!
##lib_LTLIBRARIES += libctemplate_testing.la
noinst_LTLIBRARIES += libctemplate_testing.la libctemplate_testing_nothreads.la
libctemplate_testing_la_SOURCES = $(nodist_ctemplateinclude_HEADERS) \
                                  src/tests/template_test_util.h \
                                  src/tests/template_test_util.cc
libctemplate_testing_la_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)
CTEMPLATE_TESTING_SYMBOLS = 'TemporaryRegisterTemplate|TemplateDictionaryPeer'
libctemplate_testing_la_LDFLAGS = $(PTHREAD_CFLAGS) \
                                  -export-symbols-regex $(CTEMPLATE_TESTING_SYMBOLS) \
                                  -no-undefined \
                                  -version-info @SO_VERSION@
libctemplate_testing_la_LIBADD = $(PTHREAD_LIBS)
libctemplate_testing_nothreads_la_SOURCES = $(libctemplate_testing_la_SOURCES)
libctemplate_testing_nothreads_la_CXXFLAGS = $(AM_CXXFLAGS) -DNO_THREADS
libctemplate_testing_nothreads_la_LDFLAGS = -export-symbols-regex $(CTEMPLATE_TESTING_SYMBOLS) \
                                            -no-undefined \
                                            -version-info @SO_VERSION@

# This library depends on libctemplate, but it can use either libctemplate
# or libctemplate_nothreads.  Rather than have two versions of this
# library, I just leave out the deps entirely: you must be sure to specify
# one of the two whenever you link this library into your code.
#libctemplate_testing_la_LIBADD = libctemplate.la # or libctemplate_nothreads.la


# Helper apps
bin_PROGRAMS += make_tpl_varnames_h
WINDOWS_PROJECTS += vsprojects/make_tpl_varname_h/make_tpl_varname_h.vcxproj
make_tpl_varnames_h_SOURCES = $(nodist_ctemplateinclude_HEADERS) \
	src/make_tpl_varnames_h.cc
make_tpl_varnames_h_LDADD = libctemplate_nothreads.la

bin_PROGRAMS += diff_tpl_auto_escape
WINDOWS_PROJECTS += vsprojects/diff_tpl_auto_escape/diff_tpl_auto_escape.vcxproj
diff_tpl_auto_escape_SOURCES = $(nodist_ctemplateinclude_HEADERS) \
	src/diff_tpl_auto_escape.cc
diff_tpl_auto_escape_LDADD = libctemplate_nothreads.la

bin_SCRIPTS += src/template-converter

# For each of the tests, we test with and without threads

TESTS += compile_test compile_nothreads_test
WINDOWS_PROJECTS += vsprojects/compile_test/compile_test.vcxproj
compile_test_SOURCES = $(nodist_ctemplateinclude_HEADERS) \
                       src/tests/compile_test.cc
compile_test_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)
compile_test_LDFLAGS = $(PTHREAD_CFLAGS)
compile_test_LDADD = libctemplate_debug.la $(PTHREAD_LIBS)
compile_nothreads_test_SOURCES = $(compile_test_SOURCES)
compile_nothreads_test_CXXFLAGS = -DNO_THREADS $(AM_CXXFLAGS)
compile_nothreads_test_LDADD = libctemplate_nothreads_debug.la

# With compile_test, we are making sure that the code compiles without
# needing config.h/etc.  The best test is to make sure we can compile
# with just the installed header files.  The best way to do this is at
# installcheck time.  When this rule is run (via 'make distcheck'),
# 'includedir' is the installed include-dir.
installcheck-local:
	$(CXX) -I $(includedir) -c $(top_srcdir)/src/tests/compile_test.cc \
	       -o installcheck_compile_test.o

TESTS += template_test_util_test template_test_util_nothreads_test
WINDOWS_PROJECTS += vsprojects/template_test_util_test/template_test_util_test.vcxproj
template_test_util_test_SOURCES = src/tests/config_for_unittests.h \
                                   src/template_dictionary.cc \
                                   src/tests/template_test_util_test.cc
template_test_util_test_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)
template_test_util_test_LDFLAGS = $(PTHREAD_CFLAGS)
template_test_util_test_LDADD = libctemplate_testing.la libctemplate_debug.la \
                                 $(PTHREAD_LIBS)
template_test_util_nothreads_test_SOURCES = $(template_test_util_test_SOURCES)
template_test_util_nothreads_test_CXXFLAGS = -DNO_THREADS $(AM_CXXFLAGS)
template_test_util_nothreads_test_LDADD = libctemplate_testing_nothreads.la \
                                          libctemplate_nothreads_debug.la

TESTS += template_dictionary_unittest template_dictionary_nothreads_unittest
WINDOWS_PROJECTS += vsprojects/template_dictionary_unittest/template_dictionary_unittest.vcxproj
template_dictionary_unittest_SOURCES = src/tests/config_for_unittests.h \
                                       src/base/arena.h \
                                       src/tests/template_dictionary_unittest.cc
template_dictionary_unittest_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)
template_dictionary_unittest_LDFLAGS = $(PTHREAD_CFLAGS)
template_dictionary_unittest_LDADD = libctemplate_testing.la libctemplate_debug.la \
                                     $(PTHREAD_LIBS)
template_dictionary_nothreads_unittest_SOURCES = $(template_dictionary_unittest_SOURCES)
template_dictionary_nothreads_unittest_CXXFLAGS = -DNO_THREADS $(AM_CXXFLAGS)
template_dictionary_nothreads_unittest_LDADD = libctemplate_testing_nothreads.la \
                                               libctemplate_nothreads_debug.la

TESTS += template_modifiers_unittest template_modifiers_nothreads_unittest
WINDOWS_PROJECTS += vsprojects/template_modifiers_unittest/template_modifiers_unittest.vcxproj
template_modifiers_unittest_SOURCES = src/tests/config_for_unittests.h \
                                      src/tests/template_modifiers_unittest.cc
template_modifiers_unittest_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)
template_modifiers_unittest_LDFLAGS = $(PTHREAD_CFLAGS)
template_modifiers_unittest_LDADD = libctemplate_testing.la libctemplate_debug.la \
                                    $(PTHREAD_LIBS)
template_modifiers_nothreads_unittest_SOURCES = $(template_modifiers_unittest_SOURCES)
template_modifiers_nothreads_unittest_CXXFLAGS = -DNO_THREADS $(AM_CXXFLAGS)
template_modifiers_nothreads_unittest_LDADD = libctemplate_testing_nothreads.la \
                                              libctemplate_nothreads_debug.la

TESTS += template_setglobals_unittest template_setglobals_nothreads_unittest
WINDOWS_PROJECTS += vsprojects/template_setglobals_unittest/template_setglobals_unittest.vcxproj
template_setglobals_unittest_SOURCES = src/tests/config_for_unittests.h \
                                       src/tests/template_setglobals_unittest.cc
template_setglobals_unittest_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)
template_setglobals_unittest_LDFLAGS = $(PTHREAD_CFLAGS)
template_setglobals_unittest_LDADD = libctemplate_testing.la libctemplate_debug.la \
                                     $(PTHREAD_LIBS)
template_setglobals_nothreads_unittest_SOURCES = $(template_setglobals_unittest_SOURCES)
template_setglobals_nothreads_unittest_CXXFLAGS = -DNO_THREADS $(AM_CXXFLAGS)
template_setglobals_nothreads_unittest_LDADD = libctemplate_testing_nothreads.la \
                                               libctemplate_nothreads_debug.la

TESTS += template_cache_test template_cache_nothreads_test
WINDOWS_PROJECTS += vsprojects/template_cache_test/template_cache_test.vcxproj
template_cache_test_SOURCES = src/tests/config_for_unittests.h \
                              src/template_modifiers_internal.h \
                              src/tests/template_cache_test.cc
template_cache_test_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)
template_cache_test_LDFLAGS = $(PTHREAD_CFLAGS)
template_cache_test_LDADD = libctemplate_testing.la libctemplate_debug.la \
                            $(PTHREAD_LIBS)
template_cache_nothreads_test_SOURCES = $(template_cache_test_SOURCES)
template_cache_nothreads_test_CXXFLAGS = -DNO_THREADS $(AM_CXXFLAGS)
template_cache_nothreads_test_LDADD = libctemplate_testing_nothreads.la \
                                      libctemplate_nothreads_debug.la

TESTS += template_unittest template_nothreads_unittest
WINDOWS_PROJECTS += vsprojects/template_unittest/template_unittest.vcxproj
WINDOWS_PROJECTS += vsprojects/template_unittest_static/template_unittest_static.vcxproj
template_unittest_SOURCES = src/tests/config_for_unittests.h \
                            src/template_modifiers_internal.h \
                            src/tests/template_unittest.cc
template_unittest_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)
template_unittest_LDFLAGS = $(PTHREAD_CFLAGS)
template_unittest_LDADD = libctemplate_testing.la libctemplate_debug.la \
                          $(PTHREAD_LIBS)
template_nothreads_unittest_SOURCES = $(template_unittest_SOURCES)
template_nothreads_unittest_CXXFLAGS = -DNO_THREADS $(AM_CXXFLAGS)
template_nothreads_unittest_LDADD = libctemplate_testing_nothreads.la \
                                    libctemplate_nothreads_debug.la

TESTS += template_regtest template_nothreads_regtest
WINDOWS_PROJECTS += vsprojects/template_regtest/template_regtest.vcxproj
template_regtest_SOURCES = src/tests/template_regtest.cc
template_regtest_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)
template_regtest_LDFLAGS = $(PTHREAD_CFLAGS)
template_regtest_LDADD = libctemplate_debug.la $(PTHREAD_LIBS)
template_nothreads_regtest_SOURCES = $(template_regtest_SOURCES)
template_nothreads_regtest_CXXFLAGS = -DNO_THREADS $(AM_CXXFLAGS)
template_nothreads_regtest_LDADD = libctemplate_nothreads_debug.la
TESTDATA += \
   src/tests/template_unittest_test_footer.in \
   src/tests/template_unittest_test_html.in \
   src/tests/template_unittest_test_invalid1.in \
   src/tests/template_unittest_test_invalid2.in \
   src/tests/template_unittest_test_markerdelim.in \
   src/tests/template_unittest_test_modifiers.in \
   src/tests/template_unittest_test_nul.in \
   src/tests/template_unittest_test_selective_css.in \
   src/tests/template_unittest_test_selective_html.in \
   src/tests/template_unittest_test_selective_js.in \
   src/tests/template_unittest_test_simple.in \
   src/tests/template_unittest_test_valid1.in \
   src/tests/template_unittest_test_footer_dict01.out \
   src/tests/template_unittest_test_footer_dict02.out \
   src/tests/template_unittest_test_html_dict01.out \
   src/tests/template_unittest_test_html_dict02.out \
   src/tests/template_unittest_test_markerdelim_dict01.out \
   src/tests/template_unittest_test_markerdelim_dict02.out \
   src/tests/template_unittest_test_modifiers_dict01.out \
   src/tests/template_unittest_test_nul_dict01.out \
   src/tests/template_unittest_test_selective_css_dict01.out \
   src/tests/template_unittest_test_selective_css_dict02.out \
   src/tests/template_unittest_test_selective_html_dict01.out \
   src/tests/template_unittest_test_selective_html_dict02.out \
   src/tests/template_unittest_test_selective_js_dict01.out \
   src/tests/template_unittest_test_selective_js_dict02.out \
   src/tests/template_unittest_test_simple_dict01.out \
   src/tests/template_unittest_test_simple_dict02.out \
   src/tests/template_unittest_test_simple_dict03.out \
   src/tests/template_unittest_test_valid1_dict01.out

TESTS += htmlparser_test htmlparser_nothreads_test
WINDOWS_PROJECTS += vsprojects/htmlparser_test/htmlparser_test.vcxproj
htmlparser_test_SOURCES = src/tests/htmlparser_cpp_test.cc
htmlparser_test_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)
htmlparser_test_LDFLAGS = $(PTHREAD_CFLAGS)
htmlparser_test_LDADD = libctemplate_debug.la $(PTHREAD_LIBS)
htmlparser_nothreads_test_SOURCES = $(htmlparser_test_SOURCES)
htmlparser_nothreads_test_CXXFLAGS = -DNO_THREADS $(AM_CXXFLAGS)
htmlparser_nothreads_test_LDADD = libctemplate_nothreads_debug.la
TESTDATA += \
   src/tests/htmlparser_testdata/cdata.html \
   src/tests/htmlparser_testdata/comments.html \
   src/tests/htmlparser_testdata/context.html \
   src/tests/htmlparser_testdata/google.html \
   src/tests/htmlparser_testdata/javascript_attribute.html \
   src/tests/htmlparser_testdata/javascript_block.html \
   src/tests/htmlparser_testdata/javascript_regexp.html \
   src/tests/htmlparser_testdata/position.html \
   src/tests/htmlparser_testdata/reset.html \
   src/tests/htmlparser_testdata/simple.html \
   src/tests/htmlparser_testdata/tags.html

# We want to compile statemachine.cc as a c file as well as a c++ file
statemachine.c: $(top_srcdir)/src/htmlparser/statemachine.cc
	cp $< $@

BUILT_SOURCES += statemachine.c
CLEANFILES = statemachine.c

TESTS += statemachine_test
WINDOWS_PROJECTS += vsprojects/statemachine_test/statemachine_test.vcxproj
statemachine_test_SOURCES = src/tests/statemachine_test.c \
                            statemachine.c
statemachine_test_CFLAGS = -I$(top_srcdir)/src/htmlparser

TESTS += generate_fsm_c_test
WINDOWS_PROJECTS += vsprojects/generate_fsm_c_test/generate_fsm_c_test.vcxproj
generate_fsm_c_test_SOURCES = src/tests/generate_fsm_c_test.c
generate_fsm_c_test_DEPENDENCIES = src/tests/htmlparser_testdata/sample_fsm.c

# This is something only maintainers need (since they're the only ones
# who generate .h or .c files from .config files).
check_SCRIPTS += generate_fsm_test_sh
noinst_SCRIPTS += src/tests/generate_fsm_test.sh
generate_fsm_test_sh: src/tests/generate_fsm_test.sh \
                      src/tests/htmlparser_testdata/sample_fsm.config \
                      src/tests/htmlparser_testdata/sample_fsm.c \
                      src/htmlparser/generate_fsm.py
	sh $(top_srcdir)/src/tests/generate_fsm_test.sh $(top_srcdir)
TESTDATA += \
   src/tests/htmlparser_testdata/sample_fsm.config \
   src/tests/htmlparser_testdata/sample_fsm.c

check_SCRIPTS += make_tpl_varnames_h_unittest_sh
noinst_SCRIPTS += src/tests/make_tpl_varnames_h_unittest.sh
make_tpl_varnames_h_unittest_sh: src/tests/make_tpl_varnames_h_unittest.sh \
                                 make_tpl_varnames_h
	sh $(top_srcdir)/src/tests/make_tpl_varnames_h_unittest.sh \
	   $(top_builddir)/make_tpl_varnames_h $(TMPDIR)/$@_dir

check_SCRIPTS += diff_tpl_auto_escape_unittest_sh
noinst_SCRIPTS += src/tests/diff_tpl_auto_escape_unittest.sh
diff_tpl_auto_escape_unittest_sh: src/tests/diff_tpl_auto_escape_unittest.sh \
                                  diff_tpl_auto_escape
	sh $(top_srcdir)/src/tests/diff_tpl_auto_escape_unittest.sh \
	   $(top_builddir)/diff_tpl_auto_escape $(TMPDIR)/$@_dir

## ^^^^ END OF RULES TO MAKE THE LIBRARIES, BINARIES, AND UNITTESTS

## This should always include $(TESTS), but may also include other
## binaries that you compile but don't want automatically installed.
check_PROGRAMS = $(TESTS)

rpm: dist-gzip packages/rpm.sh packages/rpm/rpm.spec
	@cd packages && ./rpm.sh ${PACKAGE} ${VERSION}

deb: dist-gzip packages/deb.sh packages/deb/*
	@cd packages && ./deb.sh ${PACKAGE} ${VERSION}

# http://linux.die.net/man/1/pkg-config, http://pkg-config.freedesktop.org/wiki
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = lib${PACKAGE}.pc lib${PACKAGE}_nothreads.pc
CLEANFILES += $(pkgconfig_DATA)

# I get the description and URL lines from the rpm spec. I use sed to
# try to rewrite exec_prefix, libdir, and includedir in terms of
# prefix, if possible.
lib${PACKAGE}.pc: Makefile packages/rpm/rpm.spec
	echo 'prefix=$(prefix)' > "$@".tmp
	echo 'exec_prefix='`echo '$(exec_prefix)' | sed 's@^$(prefix)@$${prefix}@'` >> "$@".tmp
	echo 'libdir='`echo '$(libdir)' | sed 's@^$(exec_prefix)@$${exec_prefix}@'` >> "$@".tmp
	echo 'includedir='`echo '$(includedir)' | sed 's@^$(prefix)@$${prefix}@'` >> "$@".tmp
	echo '' >> "$@".tmp
	echo 'Name: $(PACKAGE)' >> "$@".tmp
	echo 'Version: $(VERSION)' >> "$@".tmp
	-grep '^Summary:' $(top_srcdir)/packages/rpm/rpm.spec | sed s/^Summary:/Description:/ | head -n1 >> "$@".tmp
	-grep '^URL: ' $(top_srcdir)/packages/rpm/rpm.spec >> "$@".tmp
	echo 'Requires:' >> "$@".tmp
	echo 'Libs: -L$${libdir} -l$(PACKAGE)' >> "$@".tmp
	echo 'Libs.private: $(PTHREAD_CFLAGS) $(PTHREAD_LIBS)' >> "$@".tmp
	echo 'Cflags: -I$${includedir}' >> "$@".tmp
	mv -f "$@".tmp "$@"

# The nothreads version is mostly the same
lib${PACKAGE}_nothreads.pc: lib${PACKAGE}.pc
	grep -v Libs.private lib${PACKAGE}.pc | sed s/-l$(PACKAGE)/-l$(PACKAGE)_nothreads/ > "$@"

# Windows wants write permission to .vcxproj files and maybe even sln files.
dist-hook:
	test -e "$(distdir)/vsprojects" \
	   && chmod -R u+w $(distdir)/*.sln $(distdir)/vsprojects/

## If you're using libtool, add 'libtool' here.  Also add this rule:
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

# In addition to the normal stuff, we include all the windows-specific
# code, and also the code used to generate the html-parser fsm files.
# NOTE: BUILT_SOURCES should come after the .config files, so they
# come later (and thus end up with a later timestamp) when we tar/untar.
EXTRA_DIST = packages/rpm.sh packages/rpm/rpm.spec packages/deb.sh packages/deb \
	$(SCRIPTS) $(TESTDATA) libtool autogen.sh \
	src/htmlparser/generate_fsm.py src/htmlparser/fsm_config.py \
	src/tests/statemachine_test_fsm.config \
	src/htmlparser/htmlparser_fsm.config \
	src/htmlparser/jsparser_fsm.config \
	$(BUILT_SOURCES) \
	src/windows/config.h src/windows/preprocess.sh \
	src/windows/ctemplate $(WINDOWS_PROJECTS) \
	src/solaris/libstdc++.la contrib
