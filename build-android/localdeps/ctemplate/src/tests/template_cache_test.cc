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
// Author: csilvers@google.com (Craig Silverstein)
//

#include "config_for_unittests.h"
#include <ctemplate/template_cache.h>
#include <assert.h>      // for assert()
#include <stdio.h>       // for printf()
#include <stdlib.h>      // for exit()
#include <string.h>      // for strcmp()
#include <sys/types.h>   // for mode_t
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif      // for unlink()
#include <ctemplate/template.h>  // for Template
#include <ctemplate/template_dictionary.h>  // for TemplateDictionary
#include <ctemplate/template_enums.h>  // for DO_NOT_STRIP, etc
#include <ctemplate/template_pathops.h>  // for PathJoin(), kCWD
#include <ctemplate/template_string.h>  // for TemplateString
#include "tests/template_test_util.h"  // for AssertExpandIs(), etc
using std::string;
using ctemplate::FLAGS_test_tmpdir;
using ctemplate::AssertExpandIs;
using ctemplate::CreateOrCleanTestDir;
using ctemplate::CreateOrCleanTestDirAndSetAsTmpdir;
using ctemplate::DO_NOT_STRIP;
using ctemplate::PathJoin;
using ctemplate::STRIP_BLANK_LINES;
using ctemplate::STRIP_WHITESPACE;
using ctemplate::StaticTemplateString;
using ctemplate::StringToFile;
using ctemplate::StringToTemplateCache;
using ctemplate::StringToTemplateFile;
using ctemplate::Template;
using ctemplate::TemplateCache;
using ctemplate::TemplateCachePeer;
using ctemplate::TemplateDictionary;
using ctemplate::kCWD;

#define ASSERT(cond)  do {                                      \
  if (!(cond)) {                                                \
    printf("ASSERT FAILED, line %d: %s\n", __LINE__, #cond);    \
    assert(cond);                                               \
    exit(1);                                                    \
  }                                                             \
} while (0)

#define ASSERT_STREQ(a, b)  ASSERT(strcmp(a, b) == 0)

static const StaticTemplateString kKey = STS_INIT(kKey, "MY_KEY");
static const StaticTemplateString kContent = STS_INIT(kContent, "content");

// It would be nice to use the TEST framework, but it makes friendship
// more difficult.  (TemplateCache befriends TemplateCacheUnittest.)
class TemplateCacheUnittest {
 public:
  static void TestGetTemplate() {
    // Tests the cache
    TemplateCache cache1;
    const char* text = "{This is perfectly valid} yay!";
    TemplateDictionary empty_dict("dict");

    string filename = StringToTemplateFile(text);
    const Template* tpl1 = cache1.GetTemplate(filename, DO_NOT_STRIP);
    const Template* tpl2 = cache1.GetTemplate(filename.c_str(), DO_NOT_STRIP);
    const Template* tpl3 = cache1.GetTemplate(filename, STRIP_WHITESPACE);
    ASSERT(tpl1 && tpl2 && tpl3);
    ASSERT(tpl1 == tpl2);
    ASSERT(tpl1 != tpl3);
    AssertExpandIs(tpl1, &empty_dict, text, true);
    AssertExpandIs(tpl2, &empty_dict, text, true);
    AssertExpandIs(tpl3, &empty_dict, text, true);

    // Tests that a nonexistent template returns NULL
    const Template* tpl4 = cache1.GetTemplate("/yakakak", STRIP_WHITESPACE);
    ASSERT(!tpl4);

    // Make sure we get different results if we use a different cache.
    TemplateCache cache2;
    const Template* tpl5 = cache2.GetTemplate(filename, DO_NOT_STRIP);
    ASSERT(tpl5);
    ASSERT(tpl5 != tpl1);
    AssertExpandIs(tpl5, &empty_dict, text, true);

    // And different results yet if we use the default cache.
    const Template* tpl6 = Template::GetTemplate(filename, DO_NOT_STRIP);
    ASSERT(tpl6);
    ASSERT(tpl6 != tpl1);
    AssertExpandIs(tpl6, &empty_dict, text, true);
  }

  static void TestLoadTemplate() {
    // Tests the cache
    TemplateCache cache1;
    const char* text = "{This is perfectly valid} yay!";
    TemplateDictionary empty_dict("dict");
    string filename = StringToTemplateFile(text);

    ASSERT(cache1.LoadTemplate(filename, DO_NOT_STRIP));

    // Tests that a nonexistent template returns false
    ASSERT(!cache1.LoadTemplate("/yakakak", STRIP_WHITESPACE));
  }

  static void TestStringGetTemplate() {
    // If you use these same cache keys somewhere else,
    // call Template::ClearCache first.
    const string cache_key_a = "cache key a";
    const string text = "Test template 1";
    TemplateDictionary empty_dict("dict");

    TemplateCache cache1;
    const Template *tpl1;
    ASSERT(cache1.StringToTemplateCache(cache_key_a, text, DO_NOT_STRIP));
    tpl1 = cache1.GetTemplate(cache_key_a, DO_NOT_STRIP);
    AssertExpandIs(tpl1, &empty_dict, text, true);

    // A different cache should give different templates.
    TemplateCache cache2;
    const Template *tpl3;
    ASSERT(cache2.StringToTemplateCache(cache_key_a, text, DO_NOT_STRIP));
    tpl3 = cache2.GetTemplate(cache_key_a, DO_NOT_STRIP);
    ASSERT(tpl3 != tpl1);
    AssertExpandIs(tpl3, &empty_dict, text, true);

    // And the main cache different still
    const Template *tpl4;
    ASSERT(StringToTemplateCache(cache_key_a, text, DO_NOT_STRIP));
    tpl4 = Template::GetTemplate(cache_key_a, DO_NOT_STRIP);
    ASSERT(tpl4 != tpl1);
    AssertExpandIs(tpl4, &empty_dict, text, true);

    // If we register a new string with the same text, it should be ignored.
    ASSERT(!cache1.StringToTemplateCache(cache_key_a, "new text",
                                         DO_NOT_STRIP));

    Template::ClearCache();
  }

  static void TestStringToTemplateCacheWithStrip() {
    const string cache_key_a = "cache key a";
    const string text = "Test template 1";
    TemplateDictionary empty_dict("dict");

    TemplateCache cache;
    ASSERT(cache.StringToTemplateCache(cache_key_a, text, DO_NOT_STRIP));

    TemplateCachePeer cache_peer(&cache);
    TemplateCachePeer::TemplateCacheKey cache_key1(cache_key_a, DO_NOT_STRIP);
    ASSERT(cache_peer.TemplateIsCached(cache_key1));
    const Template* tpl1 = cache_peer.GetTemplate(cache_key_a, DO_NOT_STRIP);
    ASSERT(tpl1);
    AssertExpandIs(tpl1, &empty_dict, text, true);

    // Different strip: when a string template is registered via
    // StringToTemplateCache with a strip, we cannot use a different
    // strip later to fetch the template.
    TemplateCachePeer::TemplateCacheKey cache_key2(cache_key_a,
                                                   STRIP_WHITESPACE);
    ASSERT(!cache_peer.TemplateIsCached(cache_key2));
  }

  static void TestExpandNoLoad() {
    TemplateCache cache;
    string filename = StringToTemplateFile("alone");
    string top_filename = StringToTemplateFile("Hello, {{>WORLD}}");
    string inc_filename = StringToTemplateFile("world");

    TemplateDictionary dict("ExpandNoLoad");
    dict.AddIncludeDictionary("WORLD")->SetFilename(inc_filename);
    string out;

    // This should fail because the cache is empty.
    cache.Freeze();
    ASSERT(!cache.ExpandNoLoad(filename, DO_NOT_STRIP, &dict, NULL, &out));

    cache.ClearCache();   // also clears the "frozen" state
    // This should succeed -- it loads inc_filename from disk.
    ASSERT(cache.ExpandWithData(filename, DO_NOT_STRIP, &dict, NULL, &out));
    ASSERT(out == "alone");
    out.clear();
    // Now this should succeed -- it's in the cache.
    cache.Freeze();
    ASSERT(cache.ExpandNoLoad(filename, DO_NOT_STRIP, &dict, NULL, &out));
    ASSERT(out == "alone");
    out.clear();

    // This should fail because neither top nor inc are in the cache.
    cache.ClearCache();
    cache.Freeze();
    ASSERT(!cache.ExpandNoLoad(top_filename, DO_NOT_STRIP, &dict, NULL, &out));
    cache.ClearCache();
    ASSERT(cache.LoadTemplate(top_filename, DO_NOT_STRIP));
    // This *should* fail, but because inc_filename isn't in the cache.
    cache.Freeze();
    ASSERT(!cache.ExpandNoLoad(top_filename, DO_NOT_STRIP, &dict, NULL, &out));
    // TODO(csilvers): this should not be necessary.  But expand writes
    //                 to its output even before it fails.
    out.clear();
    cache.ClearCache();
    ASSERT(cache.LoadTemplate(top_filename, DO_NOT_STRIP));
    ASSERT(cache.LoadTemplate(inc_filename, DO_NOT_STRIP));
    cache.Freeze();
    // *Now* it should succeed, with everything it needs loaded.
    ASSERT(cache.ExpandNoLoad(top_filename, DO_NOT_STRIP, &dict, NULL, &out));
    ASSERT(out == "Hello, world");
    out.clear();
    // This should succeed too, of course.
    ASSERT(cache.ExpandWithData(top_filename, DO_NOT_STRIP, &dict, NULL, &out));
    ASSERT(out == "Hello, world");
    out.clear();

    cache.ClearCache();
    ASSERT(cache.ExpandWithData(top_filename, DO_NOT_STRIP, &dict, NULL, &out));
    ASSERT(out == "Hello, world");
    out.clear();
    // Now everything NoLoad needs should be in the cache again.
    cache.Freeze();
    ASSERT(cache.ExpandNoLoad(top_filename, DO_NOT_STRIP, &dict, NULL, &out));
    ASSERT(out == "Hello, world");
    out.clear();

    cache.ClearCache();
    ASSERT(cache.LoadTemplate(top_filename, DO_NOT_STRIP));
    cache.Freeze();
    // This fails, of course, because we're frozen.
    ASSERT(!cache.LoadTemplate(inc_filename, DO_NOT_STRIP));
    // And thus, this fails too.
    ASSERT(!cache.ExpandNoLoad(top_filename, DO_NOT_STRIP, &dict, NULL, &out));
  }

  static void TestTemplateSearchPath() {
    TemplateCache cache1;

    const string pathA = PathJoin(FLAGS_test_tmpdir, "a/");
    const string pathB = PathJoin(FLAGS_test_tmpdir, "b/");
    CreateOrCleanTestDir(pathA);
    CreateOrCleanTestDir(pathB);

    TemplateDictionary dict("");
    cache1.SetTemplateRootDirectory(pathA);
    cache1.AddAlternateTemplateRootDirectory(pathB);
    ASSERT(cache1.template_root_directory() == pathA);

    // 1. Show that a template in the secondary path can be found.
    const string path_b_bar = PathJoin(pathB, "template_bar");
    StringToFile("b/template_bar", path_b_bar);
    ASSERT_STREQ(path_b_bar.c_str(),
                 cache1.FindTemplateFilename("template_bar").c_str());
    const Template* b_bar = cache1.GetTemplate("template_bar", DO_NOT_STRIP);
    ASSERT(b_bar);
    AssertExpandIs(b_bar, &dict, "b/template_bar", true);

    // 2. Show that the search stops once the first match is found.
    //    Create two templates in separate directories with the same name.
    const string path_a_foo = PathJoin(pathA, "template_foo");
    const string path_b_foo = PathJoin(pathB, "template_foo");
    StringToFile("a/template_foo", path_a_foo);
    StringToFile("b/template_foo", path_b_foo);
    ASSERT_STREQ(path_a_foo.c_str(),
                 cache1.FindTemplateFilename("template_foo").c_str());
    const Template* a_foo = cache1.GetTemplate("template_foo", DO_NOT_STRIP);
    ASSERT(a_foo);
    AssertExpandIs(a_foo, &dict, "a/template_foo", true);

    // 3. Show that attempting to find a non-existent template gives an
    //    empty path.
    ASSERT(cache1.FindTemplateFilename("baz").empty());

    // 4. If we make a new cache, its path will be followed.
    TemplateCache cache2;
    cache2.SetTemplateRootDirectory(pathB);
    ASSERT_STREQ(path_b_foo.c_str(),
                 cache2.FindTemplateFilename("template_foo").c_str());
    const Template* b_foo = cache2.GetTemplate("template_foo", DO_NOT_STRIP);
    ASSERT(b_foo);
    AssertExpandIs(b_foo, &dict, "b/template_foo", true);

    // 5. Neither path will work for the default cache, which has no path.
    ASSERT(Template::template_root_directory() == kCWD);
    ASSERT(Template::FindTemplateFilename("template_foo").empty());
    ASSERT(!Template::GetTemplate("template_foo", DO_NOT_STRIP));

    CreateOrCleanTestDir(pathA);
    CreateOrCleanTestDir(pathB);
  }

  static void TestDelete() {
    Template::ClearCache();   // just for exercise.
    const string cache_key = "TestRemoveStringFromTemplateCache";
    const string text = "<html>here today...</html>";
    const string text2 = "<html>on disk tomorrow</html>";

    TemplateDictionary dict("test");
    TemplateCache cache1;

    ASSERT(cache1.StringToTemplateCache(cache_key, text, DO_NOT_STRIP));
    const Template* tpl = cache1.GetTemplate(cache_key, DO_NOT_STRIP);
    ASSERT(tpl);
    AssertExpandIs(tpl, &dict, text, true);

    cache1.Delete(cache_key);
    tpl = cache1.GetTemplate(cache_key, DO_NOT_STRIP);
    ASSERT(!tpl);
    tpl = cache1.GetTemplate(cache_key, STRIP_WHITESPACE);
    ASSERT(!tpl);
    tpl = cache1.GetTemplate(cache_key, STRIP_BLANK_LINES);
    ASSERT(!tpl);

    // Try delete on a file-based template as well.
    string filename = StringToTemplateFile(text2);
    tpl = cache1.GetTemplate(filename, DO_NOT_STRIP);
    ASSERT(tpl);
    AssertExpandIs(tpl, &dict, text2, true);
    cache1.Delete(filename);
    tpl = cache1.GetTemplate(filename, DO_NOT_STRIP);
    ASSERT(tpl);
    AssertExpandIs(tpl, &dict, text2, true);

    // Try re-adding a cache key after deleting it.
    ASSERT(cache1.StringToTemplateCache(cache_key, text, DO_NOT_STRIP));
    tpl = cache1.GetTemplate(cache_key, DO_NOT_STRIP);
    ASSERT(tpl);
    AssertExpandIs(tpl, &dict, text, true);

    // Try ClearCache while we're at it.
    cache1.ClearCache();
    tpl = cache1.GetTemplate(cache_key, STRIP_BLANK_LINES);
    ASSERT(!tpl);

    // Test on the Template class, which has a different function name.
    ASSERT(StringToTemplateCache(cache_key, text, DO_NOT_STRIP));
    tpl = Template::GetTemplate(cache_key, DO_NOT_STRIP);
    ASSERT(tpl);
    AssertExpandIs(tpl, &dict, text, true);

    Template::RemoveStringFromTemplateCache(cache_key);
    tpl = Template::GetTemplate(cache_key, DO_NOT_STRIP);
    ASSERT(!tpl);
    tpl = Template::GetTemplate(cache_key, STRIP_WHITESPACE);
    ASSERT(!tpl);
    tpl = Template::GetTemplate(cache_key, STRIP_BLANK_LINES);
    ASSERT(!tpl);
  }

  static void TestTemplateCache() {
    const string filename_a = StringToTemplateFile("Test template 1");
    const string filename_b = StringToTemplateFile("Test template 2.");

    TemplateCache cache1;
    const Template *tpl, *tpl2;
    ASSERT(tpl = cache1.GetTemplate(filename_a, DO_NOT_STRIP));

    ASSERT(tpl2 = cache1.GetTemplate(filename_b, DO_NOT_STRIP));
    ASSERT(tpl2 != tpl);  // different filenames.
    ASSERT(tpl2 = cache1.GetTemplate(filename_a, STRIP_BLANK_LINES));
    ASSERT(tpl2 != tpl);  // different strip.
    ASSERT(tpl2 = cache1.GetTemplate(filename_b, STRIP_BLANK_LINES));
    ASSERT(tpl2 != tpl);  // different filenames and strip.
    ASSERT(tpl2 = cache1.GetTemplate(filename_a, DO_NOT_STRIP));
    ASSERT(tpl2 == tpl);  // same filename and strip.
  }

  static void TestReloadAllIfChangedLazyLoad() {
    TemplateDictionary dict("empty");
    TemplateCache cache1;

    string filename = StringToTemplateFile("{valid template}");
    string nonexistent = StringToTemplateFile("dummy");
    unlink(nonexistent.c_str());

    const Template* tpl = cache1.GetTemplate(filename, STRIP_WHITESPACE);
    assert(tpl);
    const Template* tpl2 = cache1.GetTemplate(nonexistent, STRIP_WHITESPACE);
    assert(!tpl2);

    StringToFile("exists now!", nonexistent);
    tpl2 = cache1.GetTemplate(nonexistent, STRIP_WHITESPACE);
    ASSERT(!tpl2);
    cache1.ReloadAllIfChanged(TemplateCache::LAZY_RELOAD);
    tpl = cache1.GetTemplate(filename, STRIP_WHITESPACE);  // force the reload
    tpl2 = cache1.GetTemplate(nonexistent, STRIP_WHITESPACE);
    ASSERT(tpl2);                     // file exists now

    unlink(nonexistent.c_str());      // here today...
    cache1.ReloadAllIfChanged(TemplateCache::LAZY_RELOAD);
    ASSERT(cache1.GetTemplate(filename, STRIP_WHITESPACE));
    ASSERT(!cache1.GetTemplate(nonexistent, STRIP_WHITESPACE));

    StringToFile("lazarus", nonexistent);
    StringToFile("{new template}", filename);
    tpl = cache1.GetTemplate(filename, STRIP_WHITESPACE);
    AssertExpandIs(tpl, &dict, "{valid template}", true);   // haven't reloaded
    // But a different cache (say, the default) should load the new content.
    const Template* tpl3 = Template::GetTemplate(filename, STRIP_WHITESPACE);
    AssertExpandIs(tpl3, &dict, "{new template}", true);

    cache1.ReloadAllIfChanged(TemplateCache::LAZY_RELOAD);
    tpl = cache1.GetTemplate(filename, STRIP_WHITESPACE);  // needed
    AssertExpandIs(tpl, &dict, "{new template}", true);
    tpl2 = cache1.GetTemplate(nonexistent, STRIP_WHITESPACE);
    ASSERT(tpl2);
    AssertExpandIs(tpl2, &dict, "lazarus", true);

    // Ensure that string templates don't reload
    const string cache_key_a = "cache key a";
    const string text = "Test template 1";
    const Template *str_tpl;
    ASSERT(cache1.StringToTemplateCache(cache_key_a, text, DO_NOT_STRIP));
    str_tpl = cache1.GetTemplate(cache_key_a, DO_NOT_STRIP);
    AssertExpandIs(str_tpl, &dict, text, true);
    cache1.ReloadAllIfChanged(TemplateCache::LAZY_RELOAD);
    ASSERT(cache1.GetTemplate(cache_key_a, DO_NOT_STRIP) == str_tpl);

    cache1.ClearCache();
  }

  static void TestReloadAllIfChangedImmediateLoad() {
    TemplateDictionary dict("empty");
    TemplateCache cache1;
    TemplateCachePeer cache_peer(&cache1);

    // Add templates
    string filename1 = StringToTemplateFile("{valid template}");
    string filename2 = StringToTemplateFile("{another valid template}");

    const Template* tpl1 = cache1.GetTemplate(filename1,
                                              STRIP_WHITESPACE);
    assert(tpl1);
    const Template* tpl2 = cache1.GetTemplate(filename2,
                                              STRIP_WHITESPACE);
    assert(tpl2);

    StringToFile("{file1 contents changed}", filename1);
    cache1.ReloadAllIfChanged(TemplateCache::IMMEDIATE_RELOAD);

    TemplateCachePeer::TemplateCacheKey cache_key1(filename1, STRIP_WHITESPACE);
    ASSERT(cache_peer.TemplateIsCached(cache_key1));
    const Template* tpl1_post_reload = cache_peer.GetTemplate(filename1,
                                                              STRIP_WHITESPACE);
    ASSERT(tpl1_post_reload != tpl1);
    // Check that cache1's tpl1 has the new contents
    AssertExpandIs(tpl1_post_reload, &dict, "{file1 contents changed}",
                   true);

    // Ensure tpl2 is unchanged
    TemplateCachePeer::TemplateCacheKey cache_key2(filename2, STRIP_WHITESPACE);
    ASSERT(cache_peer.TemplateIsCached(cache_key2));
    const Template* tpl2_post_reload = cache_peer.GetTemplate(filename2,
                                                              STRIP_WHITESPACE);
    ASSERT(tpl2_post_reload == tpl2);

    // Test delete & re-add: delete tpl2, and reload.
    unlink(filename2.c_str());
    cache1.ReloadAllIfChanged(TemplateCache::IMMEDIATE_RELOAD);
    ASSERT(!cache_peer.GetTemplate(filename2, STRIP_WHITESPACE));
    // Re-add tpl2 and ensure it reloads.
    StringToFile("{re-add valid template contents}", filename2);
    cache1.ReloadAllIfChanged(TemplateCache::IMMEDIATE_RELOAD);
    ASSERT(cache_peer.GetTemplate(filename2, STRIP_WHITESPACE));

    // Ensure that string templates don't reload
    const string cache_key_a = "cache key a";
    const string text = "Test template 1";
    const Template *str_tpl;
    ASSERT(cache1.StringToTemplateCache(cache_key_a, text, DO_NOT_STRIP));
    str_tpl = cache1.GetTemplate(cache_key_a, DO_NOT_STRIP);
    AssertExpandIs(str_tpl, &dict, text, true);
    cache1.ReloadAllIfChanged(TemplateCache::IMMEDIATE_RELOAD);
    ASSERT(cache1.GetTemplate(cache_key_a, DO_NOT_STRIP) == str_tpl);

    cache1.ClearCache();
  }

  static void TestReloadImmediateWithDifferentSearchPaths() {
    TemplateDictionary dict("empty");
    TemplateCache cache1;
    TemplateCachePeer cache_peer(&cache1);

    const string pathA = PathJoin(FLAGS_test_tmpdir, "a/");
    const string pathB = PathJoin(FLAGS_test_tmpdir, "b/");
    CreateOrCleanTestDir(pathA);
    CreateOrCleanTestDir(pathB);

    cache1.SetTemplateRootDirectory(pathA);
    cache1.AddAlternateTemplateRootDirectory(pathB);
    ASSERT(cache1.template_root_directory() == pathA);

    // Add b/foo
    const string path_b_foo = PathJoin(pathB, "template_foo");
    StringToFile("b/template_foo", path_b_foo);
    ASSERT_STREQ(path_b_foo.c_str(),
                 cache1.FindTemplateFilename("template_foo").c_str());
    // Add b/foo to the template cache.
    cache1.GetTemplate("template_foo", DO_NOT_STRIP);

    // Add a/foo
    const string path_a_foo = PathJoin(pathA, "template_foo");
    StringToFile("a/template_foo", path_a_foo);
    ASSERT_STREQ(path_a_foo.c_str(),
                 cache1.FindTemplateFilename("template_foo").c_str());

    // Now, on reload we pick up foo from the earlier search path: a/foo
    cache1.ReloadAllIfChanged(TemplateCache::IMMEDIATE_RELOAD);
    const Template* foo_post_reload = cache_peer.GetTemplate("template_foo",
                                                             STRIP_WHITESPACE);
    AssertExpandIs(foo_post_reload, &dict, "a/template_foo",
                   true);

    // Delete a/foo and reload. Now we pick up the next available foo: b/foo
    unlink(path_a_foo.c_str());
    cache1.ReloadAllIfChanged(TemplateCache::IMMEDIATE_RELOAD);
    foo_post_reload = cache_peer.GetTemplate("template_foo",
                                             STRIP_WHITESPACE);
    AssertExpandIs(foo_post_reload, &dict, "b/template_foo",
                   true);
  }

  static void TestReloadLazyWithDifferentSearchPaths() {
    // Identical test as above with but with LAZY_RELOAD
    TemplateDictionary dict("empty");
    TemplateCache cache1;
    TemplateCachePeer cache_peer(&cache1);

    const string pathA = PathJoin(FLAGS_test_tmpdir, "a/");
    const string pathB = PathJoin(FLAGS_test_tmpdir, "b/");
    CreateOrCleanTestDir(pathA);
    CreateOrCleanTestDir(pathB);

    cache1.SetTemplateRootDirectory(pathA);
    cache1.AddAlternateTemplateRootDirectory(pathB);
    ASSERT(cache1.template_root_directory() == pathA);

    // Add b/foo
    const string path_b_foo = PathJoin(pathB, "template_foo");
    StringToFile("b/template_foo", path_b_foo);
    ASSERT_STREQ(path_b_foo.c_str(),
                 cache1.FindTemplateFilename("template_foo").c_str());
    // Add b/foo to the template cache.
    cache1.GetTemplate("template_foo", DO_NOT_STRIP);

    // Add a/foo
    const string path_a_foo = PathJoin(pathA, "template_foo");
    StringToFile("a/template_foo", path_a_foo);
    ASSERT_STREQ(path_a_foo.c_str(),
                 cache1.FindTemplateFilename("template_foo").c_str());

    // Now, on reload we pick up foo from the earlier search path: a/foo
    cache1.ReloadAllIfChanged(TemplateCache::LAZY_RELOAD);
    const Template* foo_post_reload = cache_peer.GetTemplate("template_foo",
                                                             STRIP_WHITESPACE);
    AssertExpandIs(foo_post_reload, &dict, "a/template_foo",
                   true);

    // Delete a/foo and reload. Now we pick up the next available foo: b/foo
    unlink(path_a_foo.c_str());
    cache1.ReloadAllIfChanged(TemplateCache::LAZY_RELOAD);
    foo_post_reload = cache_peer.GetTemplate("template_foo",
                                             STRIP_WHITESPACE);
    AssertExpandIs(foo_post_reload, &dict, "b/template_foo",
                   true);
  }

  static void TestRefcounting() {
    TemplateCache cache1;
    TemplateCachePeer cache_peer(&cache1);
    TemplateDictionary dict("dict");

    // Add templates
    string filename1 = StringToTemplateFile("{valid template}");
    string filename2 = StringToTemplateFile("{another valid template}");

    const Template* cache1_tpl1 = cache1.GetTemplate(filename1,
                                                     STRIP_WHITESPACE);
    assert(cache1_tpl1);
    const Template* cache1_tpl2 = cache1.GetTemplate(filename2,
                                                     STRIP_WHITESPACE);
    assert(cache1_tpl2);

    // Check refcount.  It should be 2 -- one for the originalvalue
    // when it's constructed, and one for the call to GetTemplate.
    TemplateCachePeer::TemplateCacheKey cache_key1(filename1, STRIP_WHITESPACE);
    ASSERT(cache_peer.Refcount(cache_key1) == 2);
    TemplateCachePeer::TemplateCacheKey cache_key2(filename2, STRIP_WHITESPACE);
    ASSERT(cache_peer.Refcount(cache_key2) == 2);

    // Clone cache2 from cache1
    TemplateCache* cache2 = cache1.Clone();
    TemplateCachePeer cache_peer2(cache2);

    // Check refcount was incremented.  It should be the same for both caches.
    ASSERT(cache_peer.Refcount(cache_key1) == 3);
    ASSERT(cache_peer2.Refcount(cache_key1) == 3);
    ASSERT(cache_peer.Refcount(cache_key2) == 3);
    ASSERT(cache_peer2.Refcount(cache_key2) == 3);

    // Check that the template ptrs in both caches are the same.
    const Template* cache2_tpl1 = cache2->GetTemplate(filename1,
                                                      STRIP_WHITESPACE);
    const Template* cache2_tpl2 = cache2->GetTemplate(filename2,
                                                      STRIP_WHITESPACE);
    ASSERT(cache2_tpl1 == cache1_tpl1);
    ASSERT(cache2_tpl2 == cache1_tpl2);

    // GetTemplate should have augmented the refcount.
    ASSERT(cache_peer.Refcount(cache_key1) == 4);
    ASSERT(cache_peer2.Refcount(cache_key1) == 4);
    ASSERT(cache_peer.Refcount(cache_key2) == 4);
    ASSERT(cache_peer2.Refcount(cache_key2) == 4);

    // Change tpl1 file contents and reload.
    StringToFile("{file1 contents changed}", filename1);
    cache2->ReloadAllIfChanged(TemplateCache::LAZY_RELOAD);
    // Since the template will be reloaded into a new instance,
    // GetTemplate will return new pointers. The older template
    // pointer was moved to the freelist.
    const Template* cache2_tpl1_post_reload = cache2->GetTemplate(
        filename1, STRIP_WHITESPACE);
    ASSERT(cache2_tpl1_post_reload != cache2_tpl1);
    // Check that cache1's tpl1 has the new contents
    AssertExpandIs(cache2_tpl1_post_reload, &dict, "{file1 contents changed}",
                   true);

    // Ensure tpl2 is unchanged
    const Template* cache2_tpl2_post_reload = cache2->GetTemplate(
        filename2, STRIP_WHITESPACE);
    ASSERT(cache2_tpl2_post_reload == cache2_tpl2);

    // Now key1 points to different templates in cache1 and cache2.
    // cache1's version should have a refcount of 3 (was 4, went down
    // by 1 when cache2 dropped its reference to it).  cache2's
    // version should be 2 (one for the new file, 1 for the call to
    // GetTemplate() that followed it), while key2 should have a
    // refcount of 5 in both caches (due to the new call, above, to
    // GetTemplate()).
    ASSERT(cache_peer.Refcount(cache_key1) == 3);
    ASSERT(cache_peer2.Refcount(cache_key1) == 2);
    ASSERT(cache_peer.Refcount(cache_key2) == 5);
    ASSERT(cache_peer2.Refcount(cache_key2) == 5);

    const int old_delete_count = cache_peer.NumTotalTemplateDeletes();

    // Clear up the cache2's freelist, this should drop all refcounts,
    // due to the calls cache_peer2 made to
    // GetTemplate(the-old-filename1), GetTemplate(the-new-filename1),
    // and GetTemplate(filename2) (twice!)
    cache_peer2.DoneWithGetTemplatePtrs();
    ASSERT(cache_peer.Refcount(cache_key1) == 2);
    ASSERT(cache_peer2.Refcount(cache_key1) == 1);
    ASSERT(cache_peer.Refcount(cache_key2) == 3);
    ASSERT(cache_peer2.Refcount(cache_key2) == 3);

    // Make sure that deleting from the cache causes deletion.
    // ClearCache() on peer1 should finally get rid of the old filename1.
    cache_peer.ClearCache();
    ASSERT(cache_peer.NumTotalTemplateDeletes() == old_delete_count + 1);
    cache_peer2.ClearCache();
    // Delete-count should go up by 2 as both the new tpl1, and tpl2, go away.
    ASSERT(cache_peer.NumTotalTemplateDeletes() == old_delete_count + 3);

    delete cache2;
  }

  static void TestDoneWithGetTemplatePtrs() {
    TemplateCache cache1;
    TemplateCachePeer cache_peer1(&cache1);
    TemplateDictionary dict("dict");

    // Add templates
    string fname = StringToTemplateFile("{valid template}");
    TemplateCachePeer::TemplateCacheKey cache_key(fname, STRIP_WHITESPACE);
    string out;

    int old_delete_count = cache_peer1.NumTotalTemplateDeletes();

    // OK, let's get the templates in the cache.
    ASSERT(cache1.ExpandWithData(fname, STRIP_WHITESPACE, &dict, NULL, &out));
    // This should not have changed the delete-count.
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == old_delete_count);
    // And the refcount should be 1.
    ASSERT(cache_peer1.Refcount(cache_key) == 1);
    // Same holds if we expand again.
    ASSERT(cache1.ExpandWithData(fname, STRIP_WHITESPACE, &dict, NULL, &out));
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == old_delete_count);
    ASSERT(cache_peer1.Refcount(cache_key) == 1);

    // Now we delete from the cache.  Should up the delete_count.
    ASSERT(cache1.Delete(fname));
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);

    // Calling DoneWithGetTemplatePtrs() should be a noop -- we
    // haven't called GetTemplate() yet.
    cache1.DoneWithGetTemplatePtrs();
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == old_delete_count);

    // Now do the same thing, but throw in a GetTemplate().  Now
    // DoneWithGetTemplatePtrs() should still cause a delete, but only
    // after a call to Delete() deletes the cache's refcount too.
    ASSERT(cache1.ExpandWithData(fname, STRIP_WHITESPACE, &dict, NULL, &out));
    cache1.GetTemplate(fname, STRIP_WHITESPACE);
    cache1.DoneWithGetTemplatePtrs();
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == old_delete_count);
    ASSERT(cache1.Delete(fname));
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);
    cache1.ClearCache();
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == old_delete_count);

    // Now load in a replacement.  The loading itself should cause a
    // delete (no GetTemplate calls, so no need to involve the freelist).
    ASSERT(cache1.ExpandWithData(fname, STRIP_WHITESPACE, &dict, NULL, &out));
    StringToFile("{file1 contents changed}", fname);
    cache1.ReloadAllIfChanged(TemplateCache::IMMEDIATE_RELOAD);
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);
    // DoneWithGetTemplatePtrs() should just be a noop.
    cache1.DoneWithGetTemplatePtrs();
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == old_delete_count);
    // Delete the new version of fname too!
    cache1.Delete(fname);
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);

    // Now load in a replacement, but having done a GetTemplate() first.
    // We need DoneWithGetTemplatePtrs() to delete, in this case.
    ASSERT(cache1.ExpandWithData(fname, STRIP_WHITESPACE, &dict, NULL, &out));
    cache1.GetTemplate(fname, STRIP_WHITESPACE);
    ASSERT(cache_peer1.Refcount(cache_key) == 2);
    StringToFile("{file1 contents changed}", fname);
    cache1.ReloadAllIfChanged(TemplateCache::IMMEDIATE_RELOAD);
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == old_delete_count);
    cache1.DoneWithGetTemplatePtrs();
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);
    // Delete the new version of fname too!
    cache1.Delete(fname);
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);

    // Add a Clone() into the mix.  Now Delete() calls, even from both
    // caches, won't up the delete-count until we DoneWithGetTemplatePtrs()
    // -- but only from the cache that called GetTemplate().
    ASSERT(cache1.ExpandWithData(fname, STRIP_WHITESPACE, &dict, NULL, &out));
    cache1.GetTemplate(fname, STRIP_WHITESPACE);
    ASSERT(cache_peer1.Refcount(cache_key) == 2);
    {
      TemplateCache* cache2 = cache1.Clone();
      TemplateCachePeer cache_peer2(cache2);
      ASSERT(cache_peer1.Refcount(cache_key) == 3);
      ASSERT(cache_peer2.Refcount(cache_key) == 3);
      // Do all sorts of Delete()s.
      StringToFile("{file1 contents changed}", fname);
      cache1.ReloadAllIfChanged(TemplateCache::IMMEDIATE_RELOAD);
      ASSERT(cache_peer1.Refcount(cache_key) == 1);  // the new file
      ASSERT(cache_peer2.Refcount(cache_key) == 2);  // the old file
      cache2->ReloadAllIfChanged(TemplateCache::IMMEDIATE_RELOAD);
      // Each cache has a different copy of the new file.
      ASSERT(cache_peer1.Refcount(cache_key) == 1);  // the new file
      ASSERT(cache_peer2.Refcount(cache_key) == 1);  // the new file
      ASSERT(cache1.Delete(fname));   // should delete the new file
      ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);
      ASSERT(cache2->Delete(fname));
      ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);
      cache2->DoneWithGetTemplatePtrs();
      ASSERT(cache_peer1.NumTotalTemplateDeletes() == old_delete_count);
      cache1.DoneWithGetTemplatePtrs();
      ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);
      cache1.ClearCache();
      ASSERT(cache_peer1.NumTotalTemplateDeletes() == old_delete_count);
      delete cache2;
    }

    // If we call DoneWithGetTemplatePtrs() while a clone points to the
    // template, it won't delete the template yet.
    ASSERT(cache1.ExpandWithData(fname, STRIP_WHITESPACE, &dict, NULL, &out));
    {
      TemplateCache* cache2 = cache1.Clone();
      TemplateCachePeer cache_peer2(cache2);
      StringToFile("{file1 contents changed}", fname);
      cache1.ReloadAllIfChanged(TemplateCache::IMMEDIATE_RELOAD);
      ASSERT(cache_peer1.NumTotalTemplateDeletes() == old_delete_count);
      delete cache2;
      ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);
    }
    cache1.ClearCache();
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);

    // If we throw an explicit GetTemplate() in, we still need
    // DoneWithGetTemplatePtrs().
    ASSERT(cache1.ExpandWithData(fname, STRIP_WHITESPACE, &dict, NULL, &out));
    cache1.GetTemplate(fname, STRIP_WHITESPACE);
    {
      TemplateCache* cache2 = cache1.Clone();
      TemplateCachePeer cache_peer2(cache2);
      StringToFile("{file1 contents changed}", fname);
      cache1.ReloadAllIfChanged(TemplateCache::IMMEDIATE_RELOAD);
      cache1.DoneWithGetTemplatePtrs();
      ASSERT(cache_peer1.NumTotalTemplateDeletes() == old_delete_count);
      delete cache2;
      ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);
    }
    cache1.ClearCache();
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);

    // Multiple GetTemplate()s should still all be cleared by
    // DoneWithGetTemplatePtrs().
    ASSERT(cache1.ExpandWithData(fname, STRIP_WHITESPACE, &dict, NULL, &out));
    ASSERT(cache1.ExpandWithData(fname, STRIP_WHITESPACE, &dict, NULL, &out));
    cache1.GetTemplate(fname, STRIP_WHITESPACE);
    cache1.GetTemplate(fname, STRIP_WHITESPACE);
    ASSERT(cache_peer1.Refcount(cache_key) == 3);
    StringToFile("{file1 contents changed}", fname);
    cache1.ReloadAllIfChanged(TemplateCache::IMMEDIATE_RELOAD);
    cache1.DoneWithGetTemplatePtrs();
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);
    cache1.ClearCache();
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);

    // Calling ClearCache() deletes old templates too -- we don't even
    // need to change the content.
    ASSERT(cache1.ExpandWithData(fname, STRIP_WHITESPACE, &dict, NULL, &out));
    cache1.GetTemplate(fname, STRIP_WHITESPACE);
    cache1.GetTemplate(fname, STRIP_WHITESPACE);
    cache1.ClearCache();
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);

    // So does deleting the cache object.
    ASSERT(cache1.ExpandWithData(fname, STRIP_WHITESPACE, &dict, NULL, &out));
    {
      TemplateCache* cache2 = cache1.Clone();
      TemplateCachePeer cache_peer2(cache2);
      ASSERT(cache_peer1.Refcount(cache_key) == 2);
      cache2->GetTemplate(fname, STRIP_WHITESPACE);
      ASSERT(cache_peer1.Refcount(cache_key) == 3);
      ASSERT(cache_peer2.Refcount(cache_key) == 3);
      ASSERT(cache1.Delete(fname));
      ASSERT(cache_peer1.NumTotalTemplateDeletes() == old_delete_count);
      ASSERT(cache_peer2.Refcount(cache_key) == 2);
      delete cache2;
    }
    ASSERT(cache_peer1.NumTotalTemplateDeletes() == ++old_delete_count);
  }

  static void TestCloneStringTemplates() {
    TemplateCache cache1;

    // Create & insert a string template
    const string cache_key_a = "cache key a";
    const string text = "Test template 1";
    TemplateDictionary empty_dict("dict");

    ASSERT(cache1.StringToTemplateCache(cache_key_a, text, DO_NOT_STRIP));

    // Clone cache2 from cache1
    TemplateCache* cache2 = cache1.Clone();

    // Check that the string template was copied into cache2
    const Template* cache2_tpl = cache2->GetTemplate(cache_key_a,
                                                     DO_NOT_STRIP);
    ASSERT(cache2_tpl);
    AssertExpandIs(cache2_tpl, &empty_dict, text, true);

    delete cache2;
  }

  static void TestInclude() {
    TemplateCache cache;
    string incname = StringToTemplateFile("include & print file\n");
    string tpl_file = StringToTemplateFile("hi {{>INC:h}} bar\n");
    const Template* tpl = cache.GetTemplate(tpl_file, DO_NOT_STRIP);
    ASSERT(tpl);

    TemplateDictionary dict("dict");
    AssertExpandWithCacheIs(&cache, tpl_file, DO_NOT_STRIP, &dict, NULL,
                            "hi  bar\n", true);
    dict.AddIncludeDictionary("INC")->SetFilename(incname);
    AssertExpandWithCacheIs(&cache, tpl_file, DO_NOT_STRIP, &dict, NULL,
                            "hi include &amp; print file  bar\n",
                            true);
  }

  // Make sure we don't deadlock when a template includes itself.
  // This also tests we handle recursive indentation properly.
  static void TestRecursiveInclude() {
    TemplateCache cache;
    string incname = StringToTemplateFile("hi {{>INC}} bar\n  {{>INC}}!");
    const Template* tpl = cache.GetTemplate(incname, DO_NOT_STRIP);
    ASSERT(tpl);
    TemplateDictionary dict("dict");
    dict.AddIncludeDictionary("INC")->SetFilename(incname);
    // Note the last line is indented 4 spaces instead of 2.  This is
    // because the last sub-include is indented.
    AssertExpandWithCacheIs(&cache, incname, DO_NOT_STRIP, &dict, NULL,
                            "hi hi  bar\n  ! bar\n  hi  bar\n    !!",
                            true);
  }

  static void TestStringTemplateInclude() {
    const string cache_key = "TestStringTemplateInclude";
    const string cache_key_inc = "TestStringTemplateInclude-inc";
    const string text = "<html>{{>INC}}</html>";
    const string text_inc = "<div>\n<p>\nUser {{USER}}\n</div>";

    TemplateCache cache;
    ASSERT(cache.StringToTemplateCache(cache_key, text, DO_NOT_STRIP));
    ASSERT(cache.StringToTemplateCache(cache_key_inc, text_inc, DO_NOT_STRIP));

    const Template *tpl = cache.GetTemplate(cache_key, DO_NOT_STRIP);
    ASSERT(tpl);

    TemplateDictionary dict("dict");
    TemplateDictionary* sub_dict = dict.AddIncludeDictionary("INC");
    sub_dict->SetFilename(cache_key_inc);

    sub_dict->SetValue("USER", "John<>Doe");
    string expected = "<html><div>\n<p>\nUser John<>Doe\n</div></html>";
    AssertExpandWithCacheIs(&cache, cache_key, DO_NOT_STRIP, &dict, NULL,
                            expected, true);
  }

  static void TestTemplateString() {
    TemplateCache cache;
    ASSERT(cache.StringToTemplateCache(kKey, kContent, DO_NOT_STRIP));
    const Template *tpl = cache.GetTemplate(kKey, DO_NOT_STRIP);
    ASSERT(tpl);

    TemplateDictionary dict("dict");
    AssertExpandWithCacheIs(&cache, "MY_KEY", DO_NOT_STRIP, &dict, NULL,
                            "content", true);

    // Try retrieving with a char* rather than a TemplateString*.
    tpl = cache.GetTemplate("MY_KEY", DO_NOT_STRIP);
    ASSERT(tpl);
    AssertExpandWithCacheIs(&cache, "MY_KEY", DO_NOT_STRIP, &dict, NULL,
                            "content", true);

    // Delete with a char* rather than a TemplateString*.
    cache.Delete("MY_KEY");
    tpl = cache.GetTemplate("MY_KEY", DO_NOT_STRIP);
    ASSERT(!tpl);

    ASSERT(cache.StringToTemplateCache("MY_KEY", "content", DO_NOT_STRIP));
    tpl = cache.GetTemplate(kKey, DO_NOT_STRIP);
    ASSERT(tpl);
    cache.Delete(kKey);
    tpl = cache.GetTemplate("MY_KEY", DO_NOT_STRIP);
    ASSERT(!tpl);
  }

  static void TestFreeze() {
    TemplateCache cache;
     TemplateDictionary dict("dict");

    // Load some templates
    string filename1 = StringToTemplateFile("{valid template}");
    string filename2 = StringToTemplateFile("hi {{>INC:h}} bar\n");

    const Template* cache_tpl1 = cache.GetTemplate(filename1, STRIP_WHITESPACE);
    assert(cache_tpl1);
    AssertExpandIs(cache_tpl1, &dict, "{valid template}", true);
    const Template* cache_tpl2 = cache.GetTemplate(filename2, DO_NOT_STRIP);
    assert(cache_tpl2);
    static_cast<void>(cache_tpl2);  // avoid unused var warning in opt mode
    AssertExpandWithCacheIs(&cache, filename2, DO_NOT_STRIP, &dict, NULL,
                            "hi  bar\n", true);

    // Set the root directory
    const string pathA = PathJoin(FLAGS_test_tmpdir, "a/");
    CreateOrCleanTestDir(pathA);
    cache.SetTemplateRootDirectory(pathA);
    ASSERT(cache.template_root_directory() == pathA);

    // Freeze the cache now, and test its impact.
    cache.Freeze();

    // 1. Loading new templates fails.
    string filename3 = StringToTemplateFile("{yet another valid template}");
    const Template* cache_tpl3 = cache.GetTemplate(filename3, STRIP_WHITESPACE);
    assert(!cache_tpl3);
    static_cast<void>(cache_tpl3);  // avoid unused var warning in opt mode

    // 2. Reloading existing templates fails.
    StringToFile("{file1 contents changed}", filename1);
    cache.ReloadAllIfChanged(TemplateCache::LAZY_RELOAD);
    const Template* cache_tpl1_post_reload = cache.GetTemplate(
        filename1, STRIP_WHITESPACE);
    ASSERT(cache_tpl1_post_reload == cache_tpl1);
    // Check that cache's tpl1 has the same old contents
    AssertExpandIs(cache_tpl1_post_reload, &dict, "{valid template}",
                   true);
    // 3. Cannot delete from a frozen cache.
    cache.Delete(filename1);
    ASSERT(cache.GetTemplate(filename1, STRIP_WHITESPACE));

    // 4. Expand won't load an included template on-demand.
    string incname = StringToTemplateFile("include & print file\n");
    dict.AddIncludeDictionary("INC")->SetFilename(incname);
    AssertExpandWithCacheIs(&cache, filename2, DO_NOT_STRIP, &dict, NULL,
                            "hi  bar\n", false);

    // 5. Cannot change template root directory.
    const string pathB = PathJoin(FLAGS_test_tmpdir, "b/");
    CreateOrCleanTestDir(pathB);
    cache.SetTemplateRootDirectory(pathB);
    ASSERT(cache.template_root_directory() == pathA);  // Still the old path

    CreateOrCleanTestDir(pathA);
    CreateOrCleanTestDir(pathB);
  }
};


int main(int argc, char** argv) {

  CreateOrCleanTestDirAndSetAsTmpdir(FLAGS_test_tmpdir);

  TemplateCacheUnittest::TestGetTemplate();
  TemplateCacheUnittest::TestLoadTemplate();
  TemplateCacheUnittest::TestStringGetTemplate();
  TemplateCacheUnittest::TestStringToTemplateCacheWithStrip();
  TemplateCacheUnittest::TestExpandNoLoad();
  TemplateCacheUnittest::TestTemplateSearchPath();
  TemplateCacheUnittest::TestDelete();
  TemplateCacheUnittest::TestTemplateCache();
  TemplateCacheUnittest::TestReloadAllIfChangedLazyLoad();
  TemplateCacheUnittest::TestReloadAllIfChangedImmediateLoad();
  TemplateCacheUnittest::TestReloadImmediateWithDifferentSearchPaths();
  TemplateCacheUnittest::TestReloadLazyWithDifferentSearchPaths();
  TemplateCacheUnittest::TestRefcounting();
  TemplateCacheUnittest::TestDoneWithGetTemplatePtrs();
  TemplateCacheUnittest::TestCloneStringTemplates();
  TemplateCacheUnittest::TestInclude();
  TemplateCacheUnittest::TestRecursiveInclude();
  TemplateCacheUnittest::TestStringTemplateInclude();
  TemplateCacheUnittest::TestTemplateString();
  TemplateCacheUnittest::TestFreeze();

  printf("DONE\n");
  return 0;
}
