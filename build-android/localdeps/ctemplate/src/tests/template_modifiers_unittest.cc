// Copyright (c) 2007, Google Inc.
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
// This code is written to not use the google testing framework
// as much as possible, to make it easier to opensource.

#include "config_for_unittests.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <vector>
#include <ctemplate/template.h>
#include <ctemplate/template_dictionary.h>
#include <ctemplate/template_emitter.h>
#include <ctemplate/template_modifiers.h>
#include "template_modifiers_internal.h"
#include "tests/template_test_util.h"
#include "base/util.h"
TEST_INIT               // defines RUN_ALL_TESTS

using std::string;
using std::vector;

// Rather than put all these tests in the ctemplate namespace, or use
// using-declarations, for this test I've decided to manually prepend
// ctemplate:: everywhere it's needed.  This test can serve as an
// example of how that approach looks.

TEST(TemplateModifiers, HtmlEscape) {
  ctemplate::TemplateDictionary dict("TestHtmlEscape", NULL);
  dict.SetEscapedValue("easy HTML", "foo",
                       ctemplate::html_escape);
  dict.SetEscapedValue("harder HTML", "foo & bar",
                       ctemplate::html_escape);
  dict.SetEscapedValue("hardest HTML",
                       "<A HREF='foo'\nid=\"bar\t\t&&\vbaz\">",
                       ctemplate::html_escape);

  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  // TODO(csilvers): change this (and all other expect_*'s in all files
  // in this directory) to take the expected value first, not second.
  EXPECT_STREQ(peer.GetSectionValue("easy HTML"), "foo");
  EXPECT_STREQ(peer.GetSectionValue("harder HTML"), "foo &amp; bar");
  EXPECT_STREQ(peer.GetSectionValue("hardest HTML"),
               "&lt;A HREF=&#39;foo&#39; id=&quot;bar  &amp;&amp; "
               "baz&quot;&gt;");
}

TEST(TemplateModifiers, SnippetEscape) {
  ctemplate::TemplateDictionary dict("TestSnippetEscape", NULL);
  dict.SetEscapedValue("easy snippet", "foo",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("valid snippet",
                       "<b>foo<br> &amp; b<wbr>&shy;ar</b>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("invalid snippet",
                       "<b><A HREF='foo'\nid=\"bar\t\t&&{\vbaz\">",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("snippet with italics",
                       "<i>foo<br> &amp; b<wbr>&shy;ar</i>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("unclosed snippet",
                       "<b>foo",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("snippet with interleaving",
                       "<b><i>foo</b></i>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("unclosed interleaving",
                       "<b><i><b>foo</b>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("unclosed",
                       "<b><i>foo",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("unterminated 1",
                       "foo<",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("unterminated 2",
                       "foo<b",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("unterminated 3",
                       "foo</",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("unterminated 4",
                       "foo</b",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("unterminated 5",
                       "<b>foo</b",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("close b i",
                       "<i><b>foo",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("close i b",
                       "<b><i>foo",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("em",
                       "<em>foo</em>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("nested em",
                       "<b>This is foo<em>...</em></b>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("unclosed em",
                       "<em>foo",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("wrongly closed em",
                       "foo</em>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("misnested em",
                       "<i><em>foo</i></em>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("span ltr",
                       "<span dir=ltr>bidi text</span>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("span rtl",
                       "<span dir=rtl>bidi text</span>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("span garbage dir attr",
                       "<span dir=foo>bidi text</span>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("span no dir",
                       "<span>bidi text</span>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("span bad attribute",
                       "<span onclick=alert('foo')>bidi text</span>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("span quotes",
                       "<span dir=\"rtl\">bidi text</span>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("nested span",
                       "<b>This is <span dir=rtl>bidi text</span></b>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("doubly-nested span",
                       "<span dir=rtl>This is <span dir=rtl>"
                       "bidi text</span></span>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("two spans",
                       "<b>This is <span dir=rtl>text</span> that is "
                       "<span dir=rtl>bidi.</span></b>",
                       ctemplate::snippet_escape);
  dict.SetEscapedValue("unclosed span",
                       "<b>This is <span dir=rtl>bidi text",
                       ctemplate::snippet_escape);


  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("easy snippet"), "foo");
  EXPECT_STREQ(peer.GetSectionValue("valid snippet"),
               "<b>foo<br> &amp; b<wbr>&shy;ar</b>");
  EXPECT_STREQ(peer.GetSectionValue("invalid snippet"),
               "<b>&lt;A HREF=&#39;foo&#39; id=&quot;bar  &&amp;{ "
               "baz&quot;&gt;</b>");
  EXPECT_STREQ(peer.GetSectionValue("snippet with italics"),
               "<i>foo<br> &amp; b<wbr>&shy;ar</i>");
  EXPECT_STREQ(peer.GetSectionValue("unclosed snippet"),
               "<b>foo</b>");
  EXPECT_STREQ(peer.GetSectionValue("snippet with interleaving"),
               "<b><i>foo</b></i>");
  EXPECT_STREQ(peer.GetSectionValue("unclosed interleaving"),
               "<b><i>&lt;b&gt;foo</b></i>");
  EXPECT_STREQ(peer.GetSectionValue("unclosed"),
               "<b><i>foo</i></b>");
  EXPECT_STREQ(peer.GetSectionValue("unterminated 1"), "foo&lt;");
  EXPECT_STREQ(peer.GetSectionValue("unterminated 2"), "foo&lt;b");
  EXPECT_STREQ(peer.GetSectionValue("unterminated 3"), "foo&lt;/");
  EXPECT_STREQ(peer.GetSectionValue("unterminated 4"), "foo&lt;/b");
  EXPECT_STREQ(peer.GetSectionValue("unterminated 5"), "<b>foo&lt;/b</b>");
  EXPECT_STREQ(peer.GetSectionValue("close b i"), "<i><b>foo</b></i>");
  EXPECT_STREQ(peer.GetSectionValue("close i b"), "<b><i>foo</i></b>");
  EXPECT_STREQ(peer.GetSectionValue("em"), "<em>foo</em>");
  EXPECT_STREQ(peer.GetSectionValue("nested em"),
               "<b>This is foo<em>...</em></b>");
  EXPECT_STREQ(peer.GetSectionValue("unclosed em"), "<em>foo</em>");
  EXPECT_STREQ(peer.GetSectionValue("wrongly closed em"), "foo&lt;/em&gt;");
  EXPECT_STREQ(peer.GetSectionValue("misnested em"), "<i><em>foo</i></em>");
  EXPECT_STREQ(peer.GetSectionValue("span ltr"),
               "<span dir=ltr>bidi text</span>");
  EXPECT_STREQ(peer.GetSectionValue("span rtl"),
               "<span dir=rtl>bidi text</span>");
  EXPECT_STREQ(peer.GetSectionValue("span garbage dir attr"),
               "&lt;span dir=foo&gt;bidi text&lt;/span&gt;");
  EXPECT_STREQ(peer.GetSectionValue("span no dir"),
               "&lt;span&gt;bidi text&lt;/span&gt;");
  EXPECT_STREQ(peer.GetSectionValue("span bad attribute"),
               "&lt;span onclick=alert(&#39;foo&#39;)&gt;bidi text&lt;/span&gt;");
  EXPECT_STREQ(peer.GetSectionValue("span quotes"),
               "&lt;span dir=&quot;rtl&quot;&gt;bidi text&lt;/span&gt;");
  EXPECT_STREQ(peer.GetSectionValue("nested span"),
               "<b>This is <span dir=rtl>bidi text</span></b>");
  EXPECT_STREQ(peer.GetSectionValue("doubly-nested span"),
               "<span dir=rtl>This is &lt;span dir=rtl&gt;bidi text"
               "</span>&lt;/span&gt;");
  EXPECT_STREQ(peer.GetSectionValue("two spans"),
               "<b>This is <span dir=rtl>text</span> that is "
               "<span dir=rtl>bidi.</span></b>");
  EXPECT_STREQ(peer.GetSectionValue("unclosed span"),
               "<b>This is <span dir=rtl>bidi text</span></b>");
}

TEST(TemplateModifiers, PreEscape) {
  ctemplate::TemplateDictionary dict("TestPreEscape", NULL);
  dict.SetEscapedValue("easy PRE", "foo",
                       ctemplate::pre_escape);
  dict.SetEscapedValue("harder PRE", "foo & bar",
                       ctemplate::pre_escape);
  dict.SetEscapedValue("hardest PRE",
                       " \"--\v--\f--\n--\t--&--<-->--'--\"",
                       ctemplate::pre_escape);

  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("easy PRE"), "foo");
  EXPECT_STREQ(peer.GetSectionValue("harder PRE"), "foo &amp; bar");
  EXPECT_STREQ(peer.GetSectionValue("hardest PRE"),
               " &quot;--\v--\f--\n--\t--&amp;--&lt;--&gt;--&#39;--&quot;");
}

TEST(TemplateModifiers, XmlEscape) {
  ctemplate::TemplateDictionary dict("TestXmlEscape", NULL);
  dict.SetEscapedValue("no XML", "",
                       ctemplate::xml_escape);
  dict.SetEscapedValue("easy XML", "xoo",
                       ctemplate::xml_escape);
  dict.SetEscapedValue("harder XML-1", "<>&'\"",
                       ctemplate::xml_escape);
  dict.SetEscapedValue("harder XML-2", "Hello<script>alert('&')</script>",
                       ctemplate::xml_escape);
  dict.SetEscapedValue("hardest XML", "<<b>>&!''\"\"foo",
                       ctemplate::xml_escape);
  // Characters 0x00-0x1F (except \t, \r and \n) are not valid for XML and
  // compliant parsers are allowed to die when they encounter them. They
  // should be replaced with spaces.
  dict.SetEscapedValue("Spacey XML", " \r\n\f",
                       ctemplate::xml_escape);
  dict.SetEscapedValue("XML with control chars",
                       "\x01\x02\x03\x09\x0A\x0B\x0D\x15\x16\x1F",
                       ctemplate::xml_escape);

  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("no XML"), "");
  EXPECT_STREQ(peer.GetSectionValue("easy XML"), "xoo");
  EXPECT_STREQ(peer.GetSectionValue("harder XML-1"),
               "&lt;&gt;&amp;&#39;&quot;");
  EXPECT_STREQ(peer.GetSectionValue("harder XML-2"),
               "Hello&lt;script&gt;alert(&#39;&amp;&#39;)&lt;/script&gt;");
  EXPECT_STREQ(peer.GetSectionValue("hardest XML"),
               "&lt;&lt;b&gt;&gt;&amp;!&#39;&#39;&quot;&quot;foo");
  EXPECT_STREQ(peer.GetSectionValue("Spacey XML"),
               " \r\n ");
  EXPECT_STREQ(peer.GetSectionValue("XML with control chars"),
               "   \t\n \r   ");
}

TEST(TemplateModifiers, ValidateUrlHtmlEscape) {
  ctemplate::TemplateDictionary dict("TestValidateUrlHtmlEscape", NULL);
  dict.SetEscapedValue("easy http URL", "http://www.google.com",
                       ctemplate::validate_url_and_html_escape);
  dict.SetEscapedValue("harder https URL",
                       "https://www.google.com/search?q=f&hl=en",
                       ctemplate::validate_url_and_html_escape);
  dict.SetEscapedValue("easy javascript URL",
                       "javascript:alert(document.cookie)",
                       ctemplate::validate_url_and_html_escape);
  dict.SetEscapedValue("harder javascript URL",
                       "javascript:alert(10/5)",
                       ctemplate::validate_url_and_html_escape);
  dict.SetEscapedValue("easy relative URL",
                       "foobar.html",
                       ctemplate::validate_url_and_html_escape);
  dict.SetEscapedValue("harder relative URL",
                       "/search?q=green flowers&hl=en",
                       ctemplate::validate_url_and_html_escape);
  dict.SetEscapedValue("ftp URL",
                       "ftp://ftp.example.org/pub/file.txt",
                       ctemplate::validate_url_and_html_escape);

  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("easy http URL"),
               "http://www.google.com");
  EXPECT_STREQ(peer.GetSectionValue("harder https URL"),
               "https://www.google.com/search?q=f&amp;hl=en");
  EXPECT_STREQ(peer.GetSectionValue("easy javascript URL"),
               ctemplate::ValidateUrl::kUnsafeUrlReplacement);
  EXPECT_STREQ(peer.GetSectionValue("harder javascript URL"),
               ctemplate::ValidateUrl::kUnsafeUrlReplacement);
  EXPECT_STREQ(peer.GetSectionValue("easy relative URL"),
               "foobar.html");
  EXPECT_STREQ(peer.GetSectionValue("harder relative URL"),
               "/search?q=green flowers&amp;hl=en");
  EXPECT_STREQ(peer.GetSectionValue("ftp URL"),
               "ftp://ftp.example.org/pub/file.txt");
}

TEST(TemplateModifiers, ValidateImgSrcUrlHtmlEscape) {
  ctemplate::TemplateDictionary dict("TestValidateImgSrcUrlHtmlEscape", NULL);
  dict.SetEscapedValue("easy http URL", "http://www.google.com",
                       ctemplate::validate_img_src_url_and_html_escape);
  dict.SetEscapedValue("harder https URL",
                       "https://www.google.com/search?q=f&hl=en",
                       ctemplate::validate_img_src_url_and_html_escape);
  dict.SetEscapedValue("easy javascript URL",
                       "javascript:alert(document.cookie)",
                       ctemplate::validate_img_src_url_and_html_escape);
  dict.SetEscapedValue("harder javascript URL",
                       "javascript:alert(10/5)",
                       ctemplate::validate_img_src_url_and_html_escape);
  dict.SetEscapedValue("easy relative URL",
                       "foobar.html",
                       ctemplate::validate_img_src_url_and_html_escape);
  dict.SetEscapedValue("harder relative URL",
                       "/search?q=green flowers&hl=en",
                       ctemplate::validate_img_src_url_and_html_escape);
  dict.SetEscapedValue("ftp URL",
                       "ftp://ftp.example.org/pub/file.txt",
                       ctemplate::validate_img_src_url_and_html_escape);

  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("easy http URL"),
               "http://www.google.com");
  EXPECT_STREQ(peer.GetSectionValue("harder https URL"),
               "https://www.google.com/search?q=f&amp;hl=en");
  EXPECT_STREQ(peer.GetSectionValue("easy javascript URL"),
               ctemplate::ValidateUrl::kUnsafeImgSrcUrlReplacement);
  EXPECT_STREQ(peer.GetSectionValue("harder javascript URL"),
               ctemplate::ValidateUrl::kUnsafeImgSrcUrlReplacement);
  EXPECT_STREQ(peer.GetSectionValue("easy relative URL"),
               "foobar.html");
  EXPECT_STREQ(peer.GetSectionValue("harder relative URL"),
               "/search?q=green flowers&amp;hl=en");
  EXPECT_STREQ(peer.GetSectionValue("ftp URL"),
               "ftp://ftp.example.org/pub/file.txt");
}

TEST(TemplateModifiers, ValidateUrlJavascriptEscape) {
  ctemplate::TemplateDictionary dict("TestValidateUrlJavascriptEscape", NULL);
  dict.SetEscapedValue(
      "easy http URL", "http://www.google.com",
      ctemplate::validate_url_and_javascript_escape);
  dict.SetEscapedValue(
      "harder https URL",
      "https://www.google.com/search?q=f&hl=en",
      ctemplate::validate_url_and_javascript_escape);
  dict.SetEscapedValue(
      "mangled http URL", "HTTP://www.google.com",
      ctemplate::validate_url_and_javascript_escape);
  dict.SetEscapedValue(
      "easy javascript URL",
      "javascript:alert(document.cookie)",
      ctemplate::validate_url_and_javascript_escape);
  dict.SetEscapedValue(
      "harder javascript URL",
      "javascript:alert(10/5)",
      ctemplate::validate_url_and_javascript_escape);
  dict.SetEscapedValue(
      "easy relative URL",
      "foobar.html",
      ctemplate::validate_url_and_javascript_escape);
  dict.SetEscapedValue(
      "harder relative URL",
      "/search?q=green flowers&hl=en",
      ctemplate::validate_url_and_javascript_escape);
  dict.SetEscapedValue(
      "data URL",
      "data: text/html",
      ctemplate::validate_url_and_javascript_escape);
  dict.SetEscapedValue(
      "mangled javascript URL",
      "javaSCRIPT:alert(5)",
      ctemplate::validate_url_and_javascript_escape);
  dict.SetEscapedValue(
      "harder mangled javascript URL",
      "java\nSCRIPT:alert(5)",
      ctemplate::validate_url_and_javascript_escape);


  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("easy http URL"),
               "http://www.google.com");
  EXPECT_STREQ(peer.GetSectionValue("harder https URL"),
               "https://www.google.com/search?q\\x3df\\x26hl\\x3den");
  EXPECT_STREQ(peer.GetSectionValue("mangled http URL"),
               "HTTP://www.google.com");
  EXPECT_STREQ(peer.GetSectionValue("easy javascript URL"), "#");
  EXPECT_STREQ(peer.GetSectionValue("harder javascript URL"),
               ctemplate::ValidateUrl::kUnsafeUrlReplacement);
  EXPECT_STREQ(peer.GetSectionValue("easy relative URL"),
               "foobar.html");
  EXPECT_STREQ(peer.GetSectionValue("harder relative URL"),
               "/search?q\\x3dgreen flowers\\x26hl\\x3den");
  EXPECT_STREQ(peer.GetSectionValue("data URL"),
               ctemplate::ValidateUrl::kUnsafeUrlReplacement);
  EXPECT_STREQ(peer.GetSectionValue("mangled javascript URL"),
               ctemplate::ValidateUrl::kUnsafeUrlReplacement);
  EXPECT_STREQ(peer.GetSectionValue("harder mangled javascript URL"),
               ctemplate::ValidateUrl::kUnsafeUrlReplacement);
}

TEST(TemplateModifiers, ValidateImgSrcUrlJavascriptEscape) {
  ctemplate::TemplateDictionary dict("TestValidateImgSrcUrlJavascriptEscape", NULL);
  dict.SetEscapedValue(
      "easy http URL", "http://www.google.com",
      ctemplate::validate_img_src_url_and_javascript_escape);
  dict.SetEscapedValue(
      "harder https URL",
      "https://www.google.com/search?q=f&hl=en",
      ctemplate::validate_img_src_url_and_javascript_escape);
  dict.SetEscapedValue(
      "mangled http URL", "HTTP://www.google.com",
      ctemplate::validate_img_src_url_and_javascript_escape);
  dict.SetEscapedValue(
      "easy javascript URL",
      "javascript:alert(document.cookie)",
      ctemplate::validate_img_src_url_and_javascript_escape);
  dict.SetEscapedValue(
      "harder javascript URL",
      "javascript:alert(10/5)",
      ctemplate::validate_img_src_url_and_javascript_escape);
  dict.SetEscapedValue(
      "easy relative URL",
      "foobar.html",
      ctemplate::validate_img_src_url_and_javascript_escape);
  dict.SetEscapedValue(
      "harder relative URL",
      "/search?q=green flowers&hl=en",
      ctemplate::validate_img_src_url_and_javascript_escape);
  dict.SetEscapedValue(
      "data URL",
      "data: text/html",
      ctemplate::validate_img_src_url_and_javascript_escape);
  dict.SetEscapedValue(
      "mangled javascript URL",
      "javaSCRIPT:alert(5)",
      ctemplate::validate_img_src_url_and_javascript_escape);
  dict.SetEscapedValue(
      "harder mangled javascript URL",
      "java\nSCRIPT:alert(5)",
      ctemplate::validate_img_src_url_and_javascript_escape);


  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("easy http URL"),
               "http://www.google.com");
  EXPECT_STREQ(peer.GetSectionValue("harder https URL"),
               "https://www.google.com/search?q\\x3df\\x26hl\\x3den");
  EXPECT_STREQ(peer.GetSectionValue("mangled http URL"),
               "HTTP://www.google.com");
  EXPECT_STREQ(peer.GetSectionValue("easy javascript URL"),
               ctemplate::ValidateUrl::kUnsafeImgSrcUrlReplacement);
  EXPECT_STREQ(peer.GetSectionValue("harder javascript URL"),
               ctemplate::ValidateUrl::kUnsafeImgSrcUrlReplacement);
  EXPECT_STREQ(peer.GetSectionValue("easy relative URL"),
               "foobar.html");
  EXPECT_STREQ(peer.GetSectionValue("harder relative URL"),
               "/search?q\\x3dgreen flowers\\x26hl\\x3den");
  EXPECT_STREQ(peer.GetSectionValue("data URL"),
               "/images/cleardot.gif");
  EXPECT_STREQ(peer.GetSectionValue("mangled javascript URL"),
               ctemplate::ValidateUrl::kUnsafeImgSrcUrlReplacement);
  EXPECT_STREQ(peer.GetSectionValue("harder mangled javascript URL"),
               ctemplate::ValidateUrl::kUnsafeImgSrcUrlReplacement);
}

TEST(TemplateModifiers, ValidateUrlCssEscape) {
  ctemplate::TemplateDictionary dict("TestValidateUrlCssEscape", NULL);
  dict.SetEscapedValue("easy http URL", "http://www.google.com",
                       ctemplate::validate_url_and_css_escape);
  dict.SetEscapedValue("harder https URL",
                       "https://www.google.com/search?q=f&hl=en",
                       ctemplate::validate_url_and_css_escape);
  dict.SetEscapedValue("javascript URL",
                       "javascript:alert(document.cookie)",
                       ctemplate::validate_url_and_css_escape);
  dict.SetEscapedValue("relative URL", "/search?q=green flowers&hl=en",
                       ctemplate::validate_url_and_css_escape);
  dict.SetEscapedValue("hardest URL", "http://www.google.com/s?q='bla'"
                       "&a=\"\"&b=(<tag>)&c=*\r\n\\\\bla",
                       ctemplate::validate_url_and_css_escape);

  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("easy http URL"),
               "http://www.google.com");
  EXPECT_STREQ(peer.GetSectionValue("harder https URL"),
               "https://www.google.com/search?q=f&hl=en");
  EXPECT_STREQ(peer.GetSectionValue("javascript URL"),
               ctemplate::ValidateUrl::kUnsafeUrlReplacement);
  EXPECT_STREQ(peer.GetSectionValue("relative URL"),
               "/search?q=green flowers&hl=en");
  EXPECT_STREQ(peer.GetSectionValue("hardest URL"),
               "http://www.google.com/s?q=%27bla%27"
               "&a=%22%22&b=%28%3Ctag%3E%29&c=%2A%0D%0A%5C%5Cbla");
}

TEST(TemplateModifiers, ValidateImgSrcUrlCssEscape) {
  ctemplate::TemplateDictionary dict("TestValidateImgSrcUrlCssEscape", NULL);
  dict.SetEscapedValue("easy http URL", "http://www.google.com",
                       ctemplate::validate_img_src_url_and_css_escape);
  dict.SetEscapedValue("harder https URL",
                       "https://www.google.com/search?q=f&hl=en",
                       ctemplate::validate_img_src_url_and_css_escape);
  dict.SetEscapedValue("javascript URL",
                       "javascript:alert(document.cookie)",
                       ctemplate::validate_img_src_url_and_css_escape);
  dict.SetEscapedValue("relative URL", "/search?q=green flowers&hl=en",
                       ctemplate::validate_img_src_url_and_css_escape);
  dict.SetEscapedValue("hardest URL", "http://www.google.com/s?q='bla'"
                       "&a=\"\"&b=(<tag>)&c=*\r\n\\\\bla",
                       ctemplate::validate_img_src_url_and_css_escape);

  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("easy http URL"),
               "http://www.google.com");
  EXPECT_STREQ(peer.GetSectionValue("harder https URL"),
               "https://www.google.com/search?q=f&hl=en");
  EXPECT_STREQ(peer.GetSectionValue("javascript URL"),
               ctemplate::ValidateUrl::kUnsafeImgSrcUrlReplacement);
  EXPECT_STREQ(peer.GetSectionValue("relative URL"),
               "/search?q=green flowers&hl=en");
  EXPECT_STREQ(peer.GetSectionValue("hardest URL"),
               "http://www.google.com/s?q=%27bla%27"
               "&a=%22%22&b=%28%3Ctag%3E%29&c=%2A%0D%0A%5C%5Cbla");
}

TEST(TemplateModifiers, CleanseAttribute) {
  ctemplate::TemplateDictionary dict("TestCleanseAttribute", NULL);
  dict.SetEscapedValue("easy attribute", "top",
                       ctemplate::cleanse_attribute);
  dict.SetEscapedValue("harder attribute", "foo & bar",
                       ctemplate::cleanse_attribute);
  dict.SetEscapedValue("hardest attribute",
                       "top onclick='alert(document.cookie)'",
                       ctemplate::cleanse_attribute);
  dict.SetEscapedValue("equal in middle", "foo = bar",
                       ctemplate::cleanse_attribute);
  dict.SetEscapedValue("leading equal", "=foo",
                       ctemplate::cleanse_attribute);
  dict.SetEscapedValue("trailing equal", "foo=",
                       ctemplate::cleanse_attribute);
  dict.SetEscapedValue("all equals", "===foo===bar===",
                       ctemplate::cleanse_attribute);
  dict.SetEscapedValue("just equals", "===",
                       ctemplate::cleanse_attribute);

  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("easy attribute"), "top");
  EXPECT_STREQ(peer.GetSectionValue("harder attribute"), "foo___bar");
  EXPECT_STREQ(peer.GetSectionValue("hardest attribute"),
               "top_onclick=_alert_document.cookie__");

  EXPECT_STREQ(peer.GetSectionValue("equal in middle"), "foo_=_bar");
  EXPECT_STREQ(peer.GetSectionValue("leading equal"), "_foo");
  EXPECT_STREQ(peer.GetSectionValue("trailing equal"), "foo_");
  EXPECT_STREQ(peer.GetSectionValue("just equals"), "_=_");
  EXPECT_STREQ(peer.GetSectionValue("all equals"), "_==foo===bar==_");
}

TEST(TemplateModifiers, CleanseCss) {
  ctemplate::TemplateDictionary dict("TestCleanseCss", NULL);
  dict.SetEscapedValue("easy css", "top",
                       ctemplate::cleanse_css);
  dict.SetEscapedValue("harder css", "foo & bar",
                       ctemplate::cleanse_css);
  dict.SetEscapedValue("hardest css",
                       ";width:expression(document.cookie)",
                       ctemplate::cleanse_css);

  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("easy css"),
               "top");
  EXPECT_STREQ(peer.GetSectionValue("harder css"),
               "foo  bar");
  EXPECT_STREQ(peer.GetSectionValue("hardest css"),
               "widthexpressiondocument.cookie");
}

TEST(TemplateModifiers, JavascriptEscape) {
  ctemplate::TemplateDictionary dict("TestJavascriptEscape", NULL);
  dict.SetEscapedValue("easy JS", "joo",
                       ctemplate::javascript_escape);
  dict.SetEscapedValue("harder JS", "f = 'joo';",
                       ctemplate::javascript_escape);
  dict.SetEscapedValue("hardest JS",
                       ("f = 'foo\f';\r\n\tprint \"\\&foo = \b\", \"foo\""),
                       ctemplate::javascript_escape);
  dict.SetEscapedValue("close script JS",
                       "//--></script><script>alert(123);</script>",
                       ctemplate::javascript_escape);
  dict.SetEscapedValue("unicode codepoints",
                       ("line1" "\xe2\x80\xa8" "line2" "\xe2\x80\xa9" "line3"
                        /* \u2027 */ "\xe2\x80\xa7"
                        /* \u202A */ "\xe2\x80\xaa"
                        /* malformed */ "\xe2" "\xe2\x80\xa8"
                        /* truncated */ "\xe2\x80"),
                       ctemplate::javascript_escape);

  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("easy JS"), "joo");
  EXPECT_STREQ(peer.GetSectionValue("harder JS"), "f \\x3d \\x27joo\\x27;");
  EXPECT_STREQ(peer.GetSectionValue("hardest JS"),
               "f \\x3d \\x27foo\\f\\x27;\\r\\n\\tprint \\x22\\\\\\x26foo "
               "\\x3d \\b\\x22, \\x22foo\\x22");
  EXPECT_STREQ(peer.GetSectionValue("close script JS"),
               "//--\\x3e\\x3c/script\\x3e\\x3cscript\\x3e"
               "alert(123);\\x3c/script\\x3e");
  EXPECT_STREQ(peer.GetSectionValue("unicode codepoints"),
               "line1" "\\u2028" "line2" "\\u2029" "line3"
               "\xe2\x80\xa7"
               "\xe2\x80\xaa"
               "\xe2" "\\u2028"
               "\xe2\x80");
}

TEST(TemplateModifiers, JavascriptNumber) {
  ctemplate::TemplateDictionary dict("TestJavascriptNumber", NULL);
  dict.SetEscapedValue("empty string", "",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("boolean true", "true",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("boolean false", "false",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("bad boolean 1", "tfalse",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("bad boolean 2", "tru",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("bad boolean 3", "truee",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("bad boolean 4", "invalid",
                       ctemplate::javascript_number);

  // Check that our string comparisons for booleans do not
  // assume input is null terminated.
  dict.SetEscapedValue("good boolean 5", ctemplate::TemplateString("truee", 4),
                       ctemplate::javascript_number);
  dict.SetEscapedValue("bad boolean 6", ctemplate::TemplateString("true", 3),
                       ctemplate::javascript_number);

  dict.SetEscapedValue("hex number 1", "0x123456789ABCDEF",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("hex number 2", "0X123456789ABCDEF",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("bad hex number 1", "0x123GAC",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("bad hex number 2", "0x",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("number zero", "0",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("invalid number", "A9",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("decimal zero", "0.0",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("octal number", "01234567",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("decimal number", "799.123",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("negative number", "-244",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("positive number", "+244",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("valid float 1", ".55",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("valid float 2", "8.55e-12",
                       ctemplate::javascript_number);
  dict.SetEscapedValue("invalid float", "8.55ABC",
                       ctemplate::javascript_number);

  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("empty string"), "");
  EXPECT_STREQ(peer.GetSectionValue("boolean true"), "true");
  EXPECT_STREQ(peer.GetSectionValue("boolean false"), "false");
  EXPECT_STREQ(peer.GetSectionValue("bad boolean 1"), "null");
  EXPECT_STREQ(peer.GetSectionValue("bad boolean 2"), "null");
  EXPECT_STREQ(peer.GetSectionValue("bad boolean 3"), "null");
  EXPECT_STREQ(peer.GetSectionValue("bad boolean 4"), "null");
  EXPECT_STREQ(peer.GetSectionValue("good boolean 5"), "true");
  EXPECT_STREQ(peer.GetSectionValue("bad boolean 6"), "null");
  EXPECT_STREQ(peer.GetSectionValue("hex number 1"), "0x123456789ABCDEF");
  EXPECT_STREQ(peer.GetSectionValue("hex number 2"), "0X123456789ABCDEF");
  EXPECT_STREQ(peer.GetSectionValue("bad hex number 1"), "null");
  EXPECT_STREQ(peer.GetSectionValue("bad hex number 2"), "null");
  EXPECT_STREQ(peer.GetSectionValue("number zero"), "0");
  EXPECT_STREQ(peer.GetSectionValue("invalid number"), "null");
  EXPECT_STREQ(peer.GetSectionValue("decimal zero"), "0.0");
  EXPECT_STREQ(peer.GetSectionValue("octal number"), "01234567");
  EXPECT_STREQ(peer.GetSectionValue("decimal number"), "799.123");
  EXPECT_STREQ(peer.GetSectionValue("negative number"), "-244");
  EXPECT_STREQ(peer.GetSectionValue("positive number"), "+244");
  EXPECT_STREQ(peer.GetSectionValue("valid float 1"), ".55");
  EXPECT_STREQ(peer.GetSectionValue("valid float 2"), "8.55e-12");
  EXPECT_STREQ(peer.GetSectionValue("invalid float"), "null");
}

TEST(TemplateModifiers, JsonEscape) {
  ctemplate::TemplateDictionary dict("TestJsonEscape", NULL);
  dict.SetEscapedValue("easy JSON", "joo",
                       ctemplate::json_escape);
  dict.SetEscapedValue("harder JSON", "f = \"joo\"; e = 'joo';",
                       ctemplate::json_escape);
  dict.SetEscapedValue("hardest JSON",
                       "f = 'foo<>';\r\n\t\fprint \"\\&foo = /\b\", \"foo\"",
                       ctemplate::json_escape);
  dict.SetEscapedValue("html in JSON", "<html>&nbsp;</html>",
                       ctemplate::json_escape);

  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("easy JSON"), "joo");
  EXPECT_STREQ(peer.GetSectionValue("harder JSON"), "f = \\\"joo\\\"; "
               "e = 'joo';");
  EXPECT_STREQ(peer.GetSectionValue("html in JSON"),
               "\\u003Chtml\\u003E\\u0026nbsp;\\u003C\\/html\\u003E");
  // There's a bug in MSVC 7.1 where you can't pass a literal string
  // with more than one \" in it to a macro (!) -- see
  //    http://marc.info/?t=110853662500001&r=1&w=2
  // We work around this by assigning the string to a variable first.
  const char* expected = ("f = 'foo\\u003C\\u003E';\\r\\n\\t\\fprint \\\""
                          "\\\\\\u0026foo = \\/\\b\\\", \\\"foo\\\"");
  EXPECT_STREQ(peer.GetSectionValue("hardest JSON"), expected);
}

TEST(TemplateModifiers, UrlQueryEscape) {
  ctemplate::TemplateDictionary dict("TestUrlQueryEscape", NULL);
  // The first three tests do not need escaping.
  dict.SetEscapedValue("query escape 0", "",
                       ctemplate::url_query_escape);
  dict.SetEscapedValue("query escape 1", "noop",
                       ctemplate::url_query_escape);
  dict.SetEscapedValue("query escape 2",
                       "0123456789abcdefghjijklmnopqrstuvwxyz"
                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ.-_*/~!(),",
                       ctemplate::url_query_escape);
  dict.SetEscapedValue("query escape 3", " ?a=b;c#d ",
                       ctemplate::url_query_escape);
  dict.SetEscapedValue("query escape 4", "#$%&+<=>?@[\\]^`{|}",
                       ctemplate::url_query_escape);
  dict.SetEscapedValue("query escape 5", "\xDE\xAD\xCA\xFE",
                       ctemplate::url_query_escape);
  dict.SetEscapedValue("query escape 6", "\"':",
                       ctemplate::url_query_escape);

  ctemplate::TemplateDictionaryPeer peer(&dict);  // peer can look inside dicts
  EXPECT_STREQ(peer.GetSectionValue("query escape 0"), "");
  EXPECT_STREQ(peer.GetSectionValue("query escape 1"), "noop");
  EXPECT_STREQ(peer.GetSectionValue("query escape 2"),
               "0123456789abcdefghjijklmnopqrstuvwxyz"
               "ABCDEFGHIJKLMNOPQRSTUVWXYZ.-_*/~!(),");
  EXPECT_STREQ(peer.GetSectionValue("query escape 3"), "+%3Fa%3Db%3Bc%23d+");
  EXPECT_STREQ(peer.GetSectionValue("query escape 4"),
               "%23%24%25%26%2B%3C%3D%3E%3F%40%5B%5C%5D%5E%60%7B%7C%7D");
  EXPECT_STREQ(peer.GetSectionValue("query escape 5"), "%DE%AD%CA%FE");
  EXPECT_STREQ(peer.GetSectionValue("query escape 6"), "%22%27%3A");
}

TEST(TemplateModifiers, PrefixLine) {
  ctemplate::TemplateDictionary dict("TestPrefixLine", NULL);
  // These don't escape: we don't put the prefix before the first line
  EXPECT_STREQ(ctemplate::prefix_line("pt 1", "   ").c_str(),
               "pt 1");
  EXPECT_STREQ(ctemplate::prefix_line("pt 1", "::").c_str(),
               "pt 1");

  EXPECT_STREQ(ctemplate::prefix_line("pt 1\npt 2", ":").c_str(),
               "pt 1\n:pt 2");
  EXPECT_STREQ(ctemplate::prefix_line("pt 1\npt 2", " ").c_str(),
               "pt 1\n pt 2");
  EXPECT_STREQ(ctemplate::prefix_line("pt 1\npt 2", "\n").c_str(),
               "pt 1\n\npt 2");
  EXPECT_STREQ(ctemplate::prefix_line("pt 1\npt 2\n", "  ").c_str(),
               "pt 1\n  pt 2\n  ");

  EXPECT_STREQ(ctemplate::prefix_line("pt 1\rpt 2\n", ":").c_str(),
               "pt 1\r:pt 2\n:");
  EXPECT_STREQ(ctemplate::prefix_line("pt 1\npt 2\r", ":").c_str(),
               "pt 1\n:pt 2\r:");
  EXPECT_STREQ(ctemplate::prefix_line("pt 1\r\npt 2\r", ":").c_str(),
               "pt 1\r\n:pt 2\r:");
}

TEST(TemplateModifiers, FindModifier) {
  const ctemplate::ModifierInfo* info;
  EXPECT_TRUE((info = ctemplate::FindModifier("html_escape", 11, "", 0)));
  EXPECT_EQ(info->modifier, &ctemplate::html_escape);
  EXPECT_TRUE((info = ctemplate::FindModifier("h", 1, "", 0)));
  EXPECT_EQ(info->modifier, &ctemplate::html_escape);

  EXPECT_TRUE((info = ctemplate::FindModifier("html_escape_with_arg", 20,
                                              "=pre", 4)));
  EXPECT_EQ(info->modifier, &ctemplate::pre_escape);
  EXPECT_TRUE((info = ctemplate::FindModifier("H", 1, "=pre", 4)));
  EXPECT_EQ(info->modifier, &ctemplate::pre_escape);

  EXPECT_TRUE((info = ctemplate::FindModifier("javascript_escape_with_arg",
                                              26, "=number", 7)));
  EXPECT_TRUE((info = ctemplate::FindModifier("J", 1, "=number", 7)));
  EXPECT_EQ(info->modifier, &ctemplate::javascript_number);

  // html_escape_with_arg doesn't have a default value, so these should fail.
  EXPECT_FALSE(ctemplate::FindModifier("H", 1, "=pre", 2));  // "=p"
  EXPECT_FALSE(ctemplate::FindModifier("H", 1, "=pree", 5));
  EXPECT_FALSE(ctemplate::FindModifier("H", 1, "=notpresent", 11));

  // If we don't have a modifier-value when we ought, we should fail.
  EXPECT_FALSE(ctemplate::FindModifier("html_escape", 11, "=p", 2));
  EXPECT_FALSE(ctemplate::FindModifier("h", 1, "=p", 2));

  EXPECT_FALSE(ctemplate::FindModifier("html_escape_with_arg", 20,
                                       "", 0));
  EXPECT_FALSE(ctemplate::FindModifier("H", 1, "", 0));

  // Test with added modifiers as well.
  ctemplate::NullModifier foo_modifier1;
  ctemplate::NullModifier foo_modifier2;
  ctemplate::NullModifier foo_modifier3;
  ctemplate::NullModifier foo_modifier4;
  EXPECT_TRUE(ctemplate::AddModifier("x-test", &foo_modifier1));
  EXPECT_TRUE(ctemplate::AddModifier("x-test-arg=", &foo_modifier2));
  EXPECT_TRUE(ctemplate::AddModifier("x-test-arg=h", &foo_modifier3));
  EXPECT_TRUE(ctemplate::AddModifier("x-test-arg=json", &foo_modifier4));

  EXPECT_TRUE((info = ctemplate::FindModifier("x-test", 6, "", 0)));
  EXPECT_TRUE(info->is_registered);
  EXPECT_EQ(info->modifier, &foo_modifier1);
  EXPECT_EQ(info->xss_class, ctemplate::XSS_UNIQUE);
  EXPECT_TRUE((info = ctemplate::FindModifier("x-test", 6, "=h", 2)));
  EXPECT_FALSE(info->is_registered);
  // This tests default values
  EXPECT_TRUE((info = ctemplate::FindModifier("x-test-arg", 10, "=p", 2)));
  EXPECT_TRUE(info->is_registered);
  EXPECT_EQ(info->modifier, &foo_modifier2);
  EXPECT_TRUE((info = ctemplate::FindModifier("x-test-arg", 10, "=h", 2)));
  EXPECT_TRUE(info->is_registered);
  EXPECT_EQ(info->modifier, &foo_modifier3);
  EXPECT_TRUE((info = ctemplate::FindModifier("x-test-arg", 10, "=html", 5)));
  EXPECT_TRUE(info->is_registered);
  EXPECT_EQ(info->modifier, &foo_modifier2);
  EXPECT_TRUE((info = ctemplate::FindModifier("x-test-arg", 10, "=json", 5)));
  EXPECT_TRUE(info->is_registered);
  EXPECT_EQ(info->modifier, &foo_modifier4);
  // The value is required to start with an '=' to match the
  // specialization.  If it doesn't, it will match the default.
  EXPECT_TRUE((info = ctemplate::FindModifier("x-test-arg", 10, "json", 4)));
  EXPECT_TRUE(info->is_registered);
  EXPECT_EQ(info->modifier, &foo_modifier2);
  EXPECT_TRUE((info = ctemplate::FindModifier("x-test-arg", 10,
                                              "=jsonnabbe", 5)));
  EXPECT_TRUE(info->is_registered);
  EXPECT_EQ(info->modifier, &foo_modifier4);
  EXPECT_TRUE((info = ctemplate::FindModifier("x-test-arg", 10,
                                              "=jsonnabbe", 6)));
  EXPECT_TRUE(info->is_registered);
  EXPECT_EQ(info->modifier, &foo_modifier2);
  EXPECT_TRUE((info = ctemplate::FindModifier("x-test-arg", 10,
                                              "=jsonnabbe", 4)));
  EXPECT_TRUE(info->is_registered);
  EXPECT_EQ(info->modifier, &foo_modifier2);

  // If we try to find an x- modifier that wasn't added, we should get
  // a legit but "unknown" modifier back.
  EXPECT_TRUE((info = ctemplate::FindModifier("x-foo", 5, "", 0)));
  EXPECT_FALSE(info->is_registered);
  EXPECT_TRUE((info = ctemplate::FindModifier("x-bar", 5, "=p", 2)));
  EXPECT_FALSE(info->is_registered);

  // Basic test with added XssSafe modifier.
  ctemplate::NullModifier foo_modifier5;
  EXPECT_TRUE(ctemplate::AddXssSafeModifier("x-safetest",
                                            &foo_modifier5));
  EXPECT_TRUE((info = ctemplate::FindModifier("x-safetest", 10, "", 0)));
  EXPECT_TRUE(info->is_registered);
  EXPECT_EQ(info->xss_class, ctemplate::XSS_SAFE);
  EXPECT_EQ(info->modifier, &foo_modifier5);
}

TEST(TemplateModifiers, AddModifier) {
  EXPECT_TRUE(ctemplate::AddModifier("x-atest", &ctemplate::html_escape));
  EXPECT_TRUE(ctemplate::AddModifier("x-atest-arg=", &ctemplate::html_escape));
  EXPECT_TRUE(ctemplate::AddModifier("x-atest-arg=h", &ctemplate::html_escape));
  EXPECT_TRUE(ctemplate::AddModifier("x-atest-arg=html", &ctemplate::html_escape));
  EXPECT_TRUE(ctemplate::AddModifier("x-atest-arg=json", &ctemplate::json_escape));
  EXPECT_TRUE(ctemplate::AddModifier("x-atest-arg=j", &ctemplate::json_escape));
  EXPECT_TRUE(ctemplate::AddModifier("x-atest-arg=J", &ctemplate::json_escape));

  // Make sure AddModifier fails with an invalid name.
  EXPECT_FALSE(ctemplate::AddModifier("test", &ctemplate::html_escape));

  // Make sure AddModifier fails with a duplicate name.
  EXPECT_FALSE(ctemplate::AddModifier("x-atest", &ctemplate::html_escape));
  EXPECT_FALSE(ctemplate::AddModifier("x-atest-arg=", &ctemplate::html_escape));
  EXPECT_FALSE(ctemplate::AddModifier("x-atest-arg=h", &ctemplate::html_escape));
  EXPECT_FALSE(ctemplate::AddModifier("x-atest-arg=html", &ctemplate::html_escape));

  const ctemplate::ModifierInfo* info;
  EXPECT_TRUE((info = ctemplate::FindModifier("x-atest", 7, "", 0)));
  EXPECT_FALSE(info->modval_required);

  // Make sure we can still add a modifier after having already
  // searched for it.
  EXPECT_TRUE((info = ctemplate::FindModifier("x-foo", 5, "", 0)));
  EXPECT_FALSE(info->is_registered);

  ctemplate::NullModifier foo_modifier;
  EXPECT_TRUE(ctemplate::AddModifier("x-foo", &foo_modifier));
  EXPECT_TRUE((info = ctemplate::FindModifier("x-foo", 5, "", 0)));
  EXPECT_EQ(info->modifier, &foo_modifier);
}

TEST(TemplateModifiers, AddXssSafeModifier) {
  // For shorter lines.
  const ctemplate::TemplateModifier* esc_fn =
      &ctemplate::html_escape;

  EXPECT_TRUE(ctemplate::AddXssSafeModifier("x-asafetest", esc_fn));
  EXPECT_TRUE(ctemplate::AddXssSafeModifier("x-asafetest-arg=", esc_fn));
  EXPECT_TRUE(ctemplate::AddXssSafeModifier("x-asafetest-arg=h", esc_fn));

  // Make sure AddXssSafeModifier fails with an invalid name.
  EXPECT_FALSE(ctemplate::AddXssSafeModifier("test", esc_fn));

  // Make sure AddXssSafeModifier fails with a duplicate name.
  EXPECT_FALSE(ctemplate::AddXssSafeModifier("x-asafetest", esc_fn));
  EXPECT_FALSE(ctemplate::AddXssSafeModifier("x-asafetest-arg=", esc_fn));
  EXPECT_FALSE(ctemplate::AddXssSafeModifier("x-asafetest-arg=h",
                                             esc_fn));

  // Make sure AddXssSafeModifier fails if the same modifier was
  // previously added via AddModifier.
  EXPECT_TRUE(ctemplate::AddModifier("x-safetest2", esc_fn));
  EXPECT_TRUE(ctemplate::AddModifier("x-safetest2-arg=", esc_fn));
  EXPECT_TRUE(ctemplate::AddModifier("x-safetest2-arg=h", esc_fn));
  EXPECT_FALSE(ctemplate::AddXssSafeModifier("x-safetest2", esc_fn));
  EXPECT_FALSE(ctemplate::AddXssSafeModifier("x-safetest2-arg=", esc_fn));
  EXPECT_FALSE(ctemplate::AddXssSafeModifier("x-safetest2-arg=h", esc_fn));

  // and vice versa.
  EXPECT_FALSE(ctemplate::AddModifier("x-asafetest", esc_fn));
  EXPECT_FALSE(ctemplate::AddModifier("x-asafetest-arg=", esc_fn));
  EXPECT_FALSE(ctemplate::AddModifier("x-asafetest-arg=h", esc_fn));
}

// Helper function. Determines whether the Modifier specified by
// alt_modname/alt_modval is a safe XSS alternative to
// the Modifier specified by modname/modval.
static bool CheckXSSAlternative(const string& modname, const string& modval,
                                const string& alt_modname,
                                const string& alt_modval) {
  const ctemplate::ModifierInfo *mod, *alt_mod;
  mod = ctemplate::FindModifier(modname.c_str(), modname.length(),
                                modval.c_str(), modval.length());
  alt_mod = ctemplate::FindModifier(alt_modname.c_str(),
                                    alt_modname.length(),
                                    alt_modval.c_str(),
                                    alt_modval.length());
  EXPECT_TRUE(mod != NULL && alt_mod != NULL);
  return IsSafeXSSAlternative(*mod, *alt_mod);
}

TEST(TemplateModifiers, XSSAlternatives) {
  // A modifier is always a safe replacement to itself, even non built-in.
  EXPECT_TRUE(CheckXSSAlternative("h", "", "h", ""));
  EXPECT_TRUE(CheckXSSAlternative("url_escape_with_arg", "=javascript",
                                  "url_escape_with_arg", "=javascript"));
  EXPECT_TRUE(CheckXSSAlternative("x-bla", "", "x-bla", ""));

  // A built-in modifier is always a safe replacement to
  // another with the same function.
  EXPECT_TRUE(CheckXSSAlternative("H", "=pre", "p", ""));
  EXPECT_TRUE(CheckXSSAlternative("url_query_escape", "",
                                  "url_escape_with_arg", "=query"));

  // H=(pre|snippet|attribute), p, u, U=query, U=html (a.k.a H=url)
  // and I=html are all alternatives to h.
  EXPECT_TRUE(CheckXSSAlternative("h", "", "H", "=pre"));
  EXPECT_TRUE(CheckXSSAlternative("h", "", "H", "=snippet"));
  EXPECT_TRUE(CheckXSSAlternative("h", "", "H", "=attribute"));
  EXPECT_TRUE(CheckXSSAlternative("h", "", "H", "=url"));
  EXPECT_TRUE(CheckXSSAlternative("h", "", "p", ""));
  EXPECT_TRUE(CheckXSSAlternative("h", "", "u", ""));
  EXPECT_TRUE(CheckXSSAlternative("h", "", "U", "=query"));
  EXPECT_TRUE(CheckXSSAlternative("h", "", "U", "=html"));
  EXPECT_TRUE(CheckXSSAlternative("h", "", "I", "=html"));

  // But h is not an alternative to H=attribute and I=html,
  // nor is json_escape an alternative to h.
  EXPECT_FALSE(CheckXSSAlternative("H", "=attribute", "h", ""));
  EXPECT_FALSE(CheckXSSAlternative("I", "=html", "h", ""));
  EXPECT_FALSE(CheckXSSAlternative("h", "", "json_escape", ""));

  // H=snippet and H=attribute are alternatives to H=pre
  // But H=pre is not an alternative to H=attribute.
  EXPECT_TRUE(CheckXSSAlternative("H", "=pre", "H", "=snippet"));
  EXPECT_TRUE(CheckXSSAlternative("H", "=pre", "H", "=attribute"));
  EXPECT_FALSE(CheckXSSAlternative("H", "=attribute", "H", "=pre"));

  // javascript_escape is an alternative to json_escape and vice versa
  EXPECT_TRUE(CheckXSSAlternative("json_escape", "", "javascript_escape", ""));
  EXPECT_TRUE(CheckXSSAlternative("javascript_escape", "", "json_escape", ""));

  // I=javascript is an alternative to :j and :U=javascript but not
  // vice versa
  EXPECT_TRUE(CheckXSSAlternative("javascript_escape", "", "I", "=javascript"));
  EXPECT_TRUE(CheckXSSAlternative("U", "=javascript", "I", "=javascript"));
  EXPECT_FALSE(CheckXSSAlternative("I", "=javascript", "javascript_escape", ""));
  EXPECT_FALSE(CheckXSSAlternative("I", "=javascript", "U", "=javascript"));

  // U=css and I=css are alternatives to :c but not vice versa
  EXPECT_TRUE(CheckXSSAlternative("c", "", "U", "=css"));
  EXPECT_TRUE(CheckXSSAlternative("c", "", "I", "=css"));
  EXPECT_FALSE(CheckXSSAlternative("U", "=css", "c", ""));
  EXPECT_FALSE(CheckXSSAlternative("I", "=css", "c", ""));

  // Extended modifier should not match any other except itself.
  EXPECT_FALSE(CheckXSSAlternative("x-bla", "", "x-foo", ""));
}

// This is a basic sanity check for the GetDefaultModifierForXXX() functions.
// More testing happens in AutoEscaper code which uses them.
TEST(TemplateModifiers, DefaultModifiersForContext) {
  const ctemplate::ModifierAndValue* modval;
  string print_mods;

  const vector<const ctemplate::ModifierAndValue*> modvals_html =
      ctemplate::GetDefaultModifierForHtml();
  EXPECT_EQ(1, modvals_html.size());
  print_mods = ctemplate::PrettyPrintModifiers(modvals_html, ";");
  EXPECT_STREQ(":h", print_mods.c_str());
  modval = modvals_html.front();
  EXPECT_EQ(modval->modifier_info->modifier, &ctemplate::html_escape);

  const vector<const ctemplate::ModifierAndValue*> modvals_js =
      ctemplate::GetDefaultModifierForJs();
  EXPECT_EQ(1, modvals_js.size());
  print_mods = ctemplate::PrettyPrintModifiers(modvals_js, ";");
  EXPECT_STREQ(":j", print_mods.c_str());
  modval = modvals_js.front();
  EXPECT_EQ(modval->modifier_info->modifier, &ctemplate::javascript_escape);

  const vector<const ctemplate::ModifierAndValue*> modvals_xml =
      ctemplate::GetDefaultModifierForXml();
  EXPECT_EQ(1, modvals_xml.size());
  print_mods = ctemplate::PrettyPrintModifiers(modvals_xml, ";");
  EXPECT_STREQ(":xml_escape", print_mods.c_str());
  modval = modvals_xml.front();
  EXPECT_EQ(modval->modifier_info->modifier, &ctemplate::xml_escape);

  const vector<const ctemplate::ModifierAndValue*> modvals_json =
      ctemplate::GetDefaultModifierForJson();
  EXPECT_EQ(1, modvals_json.size());
  print_mods = ctemplate::PrettyPrintModifiers(modvals_json, ";");
  EXPECT_STREQ(":j", print_mods.c_str());
  modval = modvals_json.front();
  EXPECT_EQ(modval->modifier_info->modifier, &ctemplate::javascript_escape);
}

// This tests for a bug we had where we were returning a pointer into
// a vector that became invalid after the vector was resized.
TEST(TemplateModifiers, ManyUnknownModifiers) {
  string tpl_str1 = "{{from_name:x-test=4}} sent you a message";
  const ctemplate::Template* tpl1 = ctemplate::Template::StringToTemplate(
      tpl_str1, ctemplate::DO_NOT_STRIP);

  string tpl_str2 = "{{from_name:x-test=4}} sent you a message:";
  string expected_out = "me sent you a message:";
  // All those new unknown varnames should cause g_unknown_modifiers
  // to resize.  1111 is an arbitrary large number.
  for (int i = 0; i < 1111; i++) {
    tpl_str2.append("{{from_name:x-" + string(i, 't') + "=4}}");
    expected_out.append("me");
  }
  const ctemplate::Template* tpl2 = ctemplate::Template::StringToTemplate(
      tpl_str2, ctemplate::DO_NOT_STRIP);

  // Even after the resizing, the references to the unknown
  // modifiers in tpl1 and tpl2 should still be valid.
  ctemplate::TemplateDictionary dict("test");
  dict.SetValue("from_name", "me");
  string out;

  out.clear();
  tpl1->Expand(&out, &dict);
  EXPECT_STREQ("me sent you a message", out.c_str());
  delete tpl1;

  out.clear();
  tpl2->Expand(&out, &dict);
  EXPECT_STREQ(expected_out.c_str(), out.c_str());
  delete tpl2;
}


int main(int argc, char** argv) {

  return RUN_ALL_TESTS();
}
