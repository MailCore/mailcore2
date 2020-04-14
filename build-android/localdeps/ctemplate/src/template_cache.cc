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

#include <config.h>
#include "base/mutex.h"  // This must go first so we get _XOPEN_SOURCE
#include <ctemplate/template_cache.h>
#include <assert.h>      // for assert()
#include <errno.h>
#include <stddef.h>      // for size_t
#include <stdlib.h>      // for strerror()
#include <sys/stat.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif      // for getcwd()
#include HASH_MAP_H      // for hash_map<>::iterator, hash_map<>, etc
#include <utility>       // for pair<>, make_pair()
#include <vector>        // for vector<>::size_type, vector<>, etc
#include "base/thread_annotations.h"  // for GUARDED_BY
#include <ctemplate/find_ptr.h>
#include <ctemplate/template.h>  // for Template, TemplateState
#include <ctemplate/template_enums.h>  // for Strip, DO_NOT_STRIP
#include <ctemplate/template_pathops.h>  // for PathJoin(), IsAbspath(), etc
#include <ctemplate/template_string.h>  // for StringHash
#include "base/fileutil.h"
#include <iostream>      // for cerr

#ifndef PATH_MAX
#ifdef MAXPATHLEN
#define PATH_MAX        MAXPATHLEN
#else
#define PATH_MAX        4096         // seems conservative for max filename len!
#endif
#endif

using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using HASH_NAMESPACE::unordered_map;

static int kVerbosity = 0;   // you can change this by hand to get vlogs
#define LOG(level)   std::cerr << #level ": "
#define PLOG(level)   std::cerr << #level ": [" << strerror(errno) << "] "
#define VLOG(level)  if (kVerbosity >= level)  std::cerr << "V" #level ": "

namespace ctemplate {

// ----------------------------------------------------------------------
// TemplateCache::RefcountedTemplate
//    A simple refcounting class to keep track of templates, which
//    might be shared between caches.  It also owns the pointer to
//    the template itself.
// ----------------------------------------------------------------------

class TemplateCache::RefcountedTemplate {
 public:
  explicit RefcountedTemplate(const Template* ptr) : ptr_(ptr), refcount_(1) { }
  void IncRef() {
    MutexLock ml(&mutex_);
    assert(refcount_ > 0);
    ++refcount_;
  }
  void DecRefN(int n) {
    bool refcount_is_zero;
    {
      MutexLock ml(&mutex_);
      assert(refcount_ >= n);
      refcount_ -= n;
      refcount_is_zero = (refcount_ == 0);
    }
    // We can't delete this within the MutexLock, because when the
    // MutexLock tries to unlock Mutex at function-exit, the mutex
    // will have been deleted!  This is just as safe as doing the
    // delete within the lock -- in either case, if anyone tried to do
    // anything to this class after the refcount got to 0, bad things
    // would happen.
    if (refcount_is_zero)
      delete this;
  }
  void DecRef() {
    DecRefN(1);
  }
  int refcount() const {
    MutexLock ml(&mutex_);   // could be ReaderMutexLock, but whatever
    return refcount_;
  }
  const Template* tpl() const { return ptr_; }

 private:
  ~RefcountedTemplate() { delete ptr_; }
  const Template* const ptr_;
  int refcount_  GUARDED_BY(mutex_);
  mutable Mutex mutex_;
};

// ----------------------------------------------------------------------
// TemplateCache::RefTplPtrHash
// TemplateCache::TemplateCacheHash
// TemplateCache::CachedTemplate
//    These are used for the cache-map.  CachedTemplate is what is
//    actually stored in the map: the Template* and some information
//    about it (whether we need to reload it, etc.).  Refcount is
//    a simple refcounting class, used to keep track of templates.
// ----------------------------------------------------------------------

// This is needed just because many STLs (eg FreeBSD's) are unable to
// hash pointers by default.
class TemplateCache::RefTplPtrHash {
 public:
  size_t operator()(const RefcountedTemplate* p) const {
    return reinterpret_cast<size_t>(p);
  }
  // Less operator for MSVC's hash containers.
  bool operator()(const RefcountedTemplate* a,
                  const RefcountedTemplate* b) const {
    return a < b;
  }
  // These two public members are required by msvc.  4 and 8 are defaults.
  static const size_t bucket_size = 4;
  static const size_t min_buckets = 8;
};

class TemplateCache::TemplateCacheHash {
 public:
  size_t operator()(const TemplateCacheKey& p) const {
    // Using + here is silly, but should work ok in practice.
    return p.first + p.second;
}
  // Less operator for MSVC's hash containers.
  bool operator()(const TemplateCacheKey& a,
                  const TemplateCacheKey& b) const {
    return (a.first == b.first
            ? a.second < b.second
            : a.first < b.first);
  }
  // These two public members are required by msvc.  4 and 8 are defaults.
  static const size_t bucket_size = 4;
  static const size_t min_buckets = 8;
};

struct TemplateCache::CachedTemplate {
  enum TemplateType { UNUSED, FILE_BASED, STRING_BASED };
  CachedTemplate()
      : refcounted_tpl(NULL),
        should_reload(false),
        template_type(UNUSED) {
  }
  CachedTemplate(const Template* tpl_ptr, TemplateType type)
      : refcounted_tpl(new TemplateCache::RefcountedTemplate(tpl_ptr)),
        should_reload(false),
        template_type(type) {
  }

  // we won't remove the template from the cache until refcount drops to 0
  TemplateCache::RefcountedTemplate* refcounted_tpl;   // shared across Clone()
  // reload status
  bool should_reload;
  // indicates if the template is string-based or file-based
  TemplateType template_type;
};


// ----------------------------------------------------------------------
// TemplateCache::TemplateCache()
// TemplateCache::~TemplateCache()
// ----------------------------------------------------------------------

TemplateCache::TemplateCache()
    : parsed_template_cache_(new TemplateMap),
      is_frozen_(false),
      search_path_(),
      get_template_calls_(new TemplateCallMap),
      mutex_(new Mutex),
      search_path_mutex_(new Mutex) {
}

TemplateCache::~TemplateCache() {
  ClearCache();
  delete parsed_template_cache_;
  delete get_template_calls_;
  delete mutex_;
  delete search_path_mutex_;
}


// ----------------------------------------------------------------------
// HasTemplateChangedOnDisk
//    Indicates whether the template has changed, based on the
//    backing file's last modtime.
// ----------------------------------------------------------------------

bool HasTemplateChangedOnDisk(const char* resolved_filename,
                              time_t mtime,
                              FileStat* statbuf) {
  if (!File::Stat(resolved_filename, statbuf)) {
    LOG(WARNING) << "Unable to stat file " << resolved_filename << endl;
    // If we can't Stat the file then the file may have been deleted,
    // so reload the template.
    return true;
  }
  if (statbuf->mtime == mtime && mtime > 0) {
    // No need to reload yet.
    return false;
  }
  return true;
}


// ----------------------------------------------------------------------
// TemplateCache::LoadTemplate()
// TemplateCache::GetTemplate()
// TemplateCache::GetTemplateLocked()
// TemplateCache::StringToTemplateCache()
//    The routines for adding a template to the cache.  LoadTemplate
//    loads the template into the cache and returns true if the
//    template was successfully loaded or if it already exists in the
//    cache.  GetTemplate loads the template into the cache from disk
//    and returns the parsed template.  StringToTemplateCache parses
//    and loads the template from the given string into the parsed
//    cache, or returns false if an older version already exists in
//    the cache.
// ----------------------------------------------------------------------

bool TemplateCache::LoadTemplate(const TemplateString& filename, Strip strip) {
  TemplateCacheKey cache_key = TemplateCacheKey(filename.GetGlobalId(), strip);
  WriterMutexLock ml(mutex_);
  return GetTemplateLocked(filename, strip, cache_key) != NULL;
}

const Template *TemplateCache::GetTemplate(const TemplateString& filename,
                                           Strip strip) {
  // No need to have the cache-mutex acquired for this step
  TemplateCacheKey cache_key = TemplateCacheKey(filename.GetGlobalId(), strip);
  CachedTemplate retval;
  WriterMutexLock ml(mutex_);
  RefcountedTemplate* refcounted_tpl =
      GetTemplateLocked(filename, strip, cache_key);
  if (!refcounted_tpl)
    return NULL;

  refcounted_tpl->IncRef();   // DecRef() is in DoneWithGetTemplatePtrs()
  (*get_template_calls_)[refcounted_tpl]++;   // set up for DoneWith...()
  return refcounted_tpl->tpl();
}

TemplateCache::RefcountedTemplate* TemplateCache::GetTemplateLocked(
    const TemplateString& filename,
    Strip strip,
    const TemplateCacheKey& template_cache_key) {
  // NOTE: A write-lock must be held on mutex_ when this method is called.
  CachedTemplate* it = find_ptr(*parsed_template_cache_, template_cache_key);
  if (!it) {
    // If the cache is frozen and the template doesn't already exist in cache,
    // do not load the template, return NULL.
    if (is_frozen_) {
      return NULL;
    }
    // TODO(panicker): Validate the filename here, and if the file can't be
    // resolved then insert a NULL in the cache.
    // If validation succeeds then pass in resolved filename, mtime &
    // file length (from statbuf) to the constructor.
    const Template* tpl = new Template(filename, strip, this);
    it = &(*parsed_template_cache_)[template_cache_key];
    *it = CachedTemplate(tpl, CachedTemplate::FILE_BASED);
    assert(it);
  }
  if (it->should_reload) {
    // check if the template has changed on disk or if a new template with the
    // same name has been added earlier in the search path:
    const string resolved = FindTemplateFilename(
        it->refcounted_tpl->tpl()->original_filename());
    FileStat statbuf;
    if (it->template_type == CachedTemplate::FILE_BASED &&
        (resolved != it->refcounted_tpl->tpl()->template_file() ||
         HasTemplateChangedOnDisk(
             it->refcounted_tpl->tpl()->template_file(),
             it->refcounted_tpl->tpl()->mtime(),
             &statbuf))) {
      // Create a new template, insert it into the cache under
      // template_cache_key, and DecRef() the old one to indicate
      // the cache no longer has a reference to it.
      const Template* tpl = new Template(filename, strip, this);
      // DecRef after creating the new template since DecRef may free up
      // the storage for filename,
      it->refcounted_tpl->DecRef();
      *it = CachedTemplate(tpl, CachedTemplate::FILE_BASED);
    }
    it->should_reload = false;
  }

  // If the state is TS_ERROR, we leave the state as is, but return
  // NULL.  We won't try to load the template file again until the
  // reload status is set to true by another call to ReloadAllIfChanged.
  return it->refcounted_tpl->tpl()->state() == TS_READY ? it->refcounted_tpl : NULL;
}

bool TemplateCache::StringToTemplateCache(const TemplateString& key,
                                          const TemplateString& content,
                                          Strip strip) {
  TemplateCacheKey template_cache_key = TemplateCacheKey(key.GetGlobalId(), strip);
  {
    ReaderMutexLock ml(mutex_);
    if (is_frozen_) {
      return false;
    }
    // If the key is already in the parsed-cache, we just return false.
    CachedTemplate* it = find_ptr(*parsed_template_cache_, template_cache_key);
    if (it && it->refcounted_tpl->tpl()->state() != TS_ERROR) {
      return false;
    }
  }
  Template* tpl = Template::StringToTemplate(content, strip);
  if (tpl == NULL) {
    return false;
  }
  if (tpl->state() != TS_READY) {
    delete tpl;
    return false;
  }

  WriterMutexLock ml(mutex_);
  // Double-check it wasn't just inserted.
  CachedTemplate* it = find_ptr(*parsed_template_cache_, template_cache_key);
  if (it) {
    if (it->refcounted_tpl->tpl()->state() == TS_ERROR) {
      // replace the old entry with the new one
      it->refcounted_tpl->DecRef();
    } else {
      delete tpl;
      return false;
    }
  }
  // Insert into cache.
  (*parsed_template_cache_)[template_cache_key] =
      CachedTemplate(tpl, CachedTemplate::STRING_BASED);
  return true;
}

// ----------------------------------------------------------------------
// TemplateCache::ExpandWithData()
// TemplateCache::ExpandFrozen()
// TemplateCache::ExpandLocked()
//    ExpandWithData gets the template from the parsed-cache, possibly
//    loading the template on-demand, and then expands the template.
//    ExpandFrozen is for frozen caches only -- if the filename isn't
//    in the cache, the routine fails (returns false) rather than trying
//    to fetch the template.  ExpandLocked is used for recursive
//    sub-template includes, and just tells template.cc it doesn't
//    need to recursively acquire any locks.
// ----------------------------------------------------------------------

bool TemplateCache::ExpandWithData(const TemplateString& filename,
                                   Strip strip,
                                   const TemplateDictionaryInterface *dict,
                                   PerExpandData *per_expand_data,
                                   ExpandEmitter *expand_emitter) {
  TemplateCacheKey template_cache_key(filename.GetGlobalId(), strip);
  // We make a local copy of this struct so we don't have to worry about
  // what happens to our cache while we don't hold the lock (during Expand).
  RefcountedTemplate* refcounted_tpl = NULL;
  {
    WriterMutexLock ml(mutex_);
    // Optionally load the template (depending on whether the cache is frozen,
    // the reload bit is set etc.)
    refcounted_tpl = GetTemplateLocked(filename, strip, template_cache_key);
    if (!refcounted_tpl)
      return false;
    refcounted_tpl->IncRef();
  }
  const bool result = refcounted_tpl->tpl()->ExpandWithDataAndCache(
      expand_emitter, dict, per_expand_data, this);
  {
    WriterMutexLock ml(mutex_);
    refcounted_tpl->DecRef();
  }
  return result;
}

bool TemplateCache::ExpandNoLoad(
    const TemplateString& filename,
    Strip strip,
    const TemplateDictionaryInterface *dict,
    PerExpandData *per_expand_data,
    ExpandEmitter *expand_emitter) const {
  TemplateCacheKey template_cache_key(filename.GetGlobalId(), strip);
  CachedTemplate cached_tpl;
  {
    ReaderMutexLock ml(mutex_);
    if (!is_frozen_) {
      LOG(DFATAL) << ": ExpandNoLoad() only works on frozen caches.";
      return false;
    }
    CachedTemplate* it = find_ptr(*parsed_template_cache_, template_cache_key);
    if (!it) {
      return false;
    }
    cached_tpl = *it;
    cached_tpl.refcounted_tpl->IncRef();
  }
  const bool result = cached_tpl.refcounted_tpl->tpl()->ExpandWithDataAndCache(
      expand_emitter, dict, per_expand_data, this);
  {
    WriterMutexLock ml(mutex_);
    cached_tpl.refcounted_tpl->DecRef();
  }
  return result;
}

// Note: "Locked" in this name refers to the template object, not to
// use; we still need to acquire our locks as per normal.
bool TemplateCache::ExpandLocked(const TemplateString& filename,
                                 Strip strip,
                                 ExpandEmitter *expand_emitter,
                                 const TemplateDictionaryInterface *dict,
                                 PerExpandData *per_expand_data) {
  TemplateCacheKey template_cache_key(filename.GetGlobalId(), strip);
  RefcountedTemplate* refcounted_tpl = NULL;
  {
    WriterMutexLock ml(mutex_);
    refcounted_tpl = GetTemplateLocked(filename, strip, template_cache_key);
    if (!refcounted_tpl)
      return false;
    refcounted_tpl->IncRef();
  }
  const bool result = refcounted_tpl->tpl()->ExpandLocked(
      expand_emitter, dict, per_expand_data, this);
  {
    WriterMutexLock ml(mutex_);
    refcounted_tpl->DecRef();
  }
  return result;
}

// ----------------------------------------------------------------------
// TemplateCache::SetTemplateRootDirectory()
// TemplateCache::AddAlternateTemplateRootDirectory()
// TemplateCache::template_root_directory()
// TemplateCache::FindTemplateFilename()
//    The template-root-directory is where we look for template
//    files (in GetTemplate and include templates) when they're
//    given with a relative rather than absolute name.  You can
//    set a 'main' root directory (where we look first), as well
//    as alternates.
// ----------------------------------------------------------------------

bool TemplateCache::AddAlternateTemplateRootDirectoryHelper(
    const string& directory,
    bool clear_template_search_path) {
  {
    ReaderMutexLock ml(mutex_);
    if (is_frozen_) {  // Cannot set root-directory on a frozen cache.
      return false;
    }
  }
  string normalized = directory;
  // make sure it ends with '/'
  NormalizeDirectory(&normalized);
  // Make the directory absolute if it isn't already.  This makes code
  // safer if client later does a chdir.
  if (!IsAbspath(normalized)) {
    char* cwdbuf = new char[PATH_MAX];   // new to avoid stack overflow
    const char* cwd = getcwd(cwdbuf, PATH_MAX);
    if (!cwd) {   // probably not possible, but best to be defensive
      PLOG(WARNING) << "Unable to convert '" << normalized
                    << "' to an absolute path, with cwd=" << cwdbuf;
    } else {
      normalized = PathJoin(cwd, normalized);
    }
    delete[] cwdbuf;
  }

  VLOG(2) << "Setting Template directory to " << normalized << endl;
  {
    WriterMutexLock ml(search_path_mutex_);
    if (clear_template_search_path) {
      search_path_.clear();
    }
    search_path_.push_back(normalized);
  }

  // NOTE(williasr): The template root is not part of the template
  // cache key, so we need to invalidate the cache contents.
  ReloadAllIfChanged(LAZY_RELOAD);
  return true;
}

bool TemplateCache::SetTemplateRootDirectory(const string& directory) {
  return AddAlternateTemplateRootDirectoryHelper(directory, true);
}

bool TemplateCache::AddAlternateTemplateRootDirectory(
    const string& directory) {
  return AddAlternateTemplateRootDirectoryHelper(directory, false);
}

string TemplateCache::template_root_directory() const {
  ReaderMutexLock ml(search_path_mutex_);
  if (search_path_.empty()) {
    return kCWD;
  }
  return search_path_[0];
}

// Given an unresolved filename, look through the template search path
// to see if the template can be found. If so, resolved contains the
// resolved filename, statbuf contains the stat structure for the file
// (to avoid double-statting the file), and the function returns
// true. Otherwise, the function returns false.
bool TemplateCache::ResolveTemplateFilename(const string& unresolved,
                                            string* resolved,
                                            FileStat* statbuf) const {
  ReaderMutexLock ml(search_path_mutex_);
  if (search_path_.empty() || IsAbspath(unresolved)) {
    *resolved = unresolved;
    if (File::Stat(*resolved, statbuf)) {
      VLOG(1) << "Resolved " << unresolved << " to " << *resolved << endl;
      return true;
    }
  } else {
    for (TemplateSearchPath::const_iterator path = search_path_.begin();
         path != search_path_.end();
         ++path) {
      *resolved = PathJoin(*path, unresolved);
      if (File::Stat(*resolved, statbuf)) {
        VLOG(1) << "Resolved " << unresolved << " to " << *resolved << endl;
        return true;
      }
    }
  }

  resolved->clear();
  return false;
}

string TemplateCache::FindTemplateFilename(const string& unresolved)
    const {
  string resolved;
  FileStat statbuf;
  if (!ResolveTemplateFilename(unresolved, &resolved, &statbuf))
    resolved.clear();
  return resolved;
}


// ----------------------------------------------------------------------
// TemplateCache::Delete()
// TemplateCache::ClearCache()
//    Delete deletes one entry from the cache.
// ----------------------------------------------------------------------

bool TemplateCache::Delete(const TemplateString& key) {
  WriterMutexLock ml(mutex_);
  if (is_frozen_) {  // Cannot delete from a frozen cache.
    return false;
  }
  vector<TemplateCacheKey> to_erase;
  const TemplateId key_id = key.GetGlobalId();
  for (TemplateMap::iterator it = parsed_template_cache_->begin();
       it != parsed_template_cache_->end();  ++it) {
    if (it->first.first == key_id) {
      // We'll delete the content pointed to by the entry here, since
      // it's handy, but we won't delete the entry itself quite yet.
      it->second.refcounted_tpl->DecRef();
      to_erase.push_back(it->first);
    }
  }
  for (vector<TemplateCacheKey>::iterator it = to_erase.begin();
       it != to_erase.end(); ++it) {
    parsed_template_cache_->erase(*it);
  }
  return !to_erase.empty();
}

void TemplateCache::ClearCache() {
  // NOTE: We allow a frozen cache to be cleared with this method, although
  // no other changes can be made to the cache.
  // We clear the cache by swapping it with an empty cache.  This lets
  // us delete the items in the cache at our leisure without needing
  // to hold mutex_.
  TemplateMap tmp_cache;
  {
    WriterMutexLock ml(mutex_);
    parsed_template_cache_->swap(tmp_cache);
    is_frozen_ = false;
  }
  for (TemplateMap::iterator it = tmp_cache.begin();
       it != tmp_cache.end();
       ++it) {
    it->second.refcounted_tpl->DecRef();
  }

  // Do a decref for all templates ever returned by GetTemplate().
  DoneWithGetTemplatePtrs();
}

// ----------------------------------------------------------------------
// TemplateCache::DoneWithGetTemplatePtrs()
//    DoneWithGetTemplatePtrs() DecRefs every template in the
//    get_template_calls_ list.  This is because the user of
//    GetTemplate() didn't have a pointer to the refcounted Template
//    to do this themselves.  Note we only provide this as a batch
//    operation, so the user should be careful to only call this when
//    they are no longer using *any* template ever retrieved by
//    this cache's GetTemplate().
// ----------------------------------------------------------------------

void TemplateCache::DoneWithGetTemplatePtrs() {
  WriterMutexLock ml(mutex_);
  for (TemplateCallMap::iterator it = get_template_calls_->begin();
       it != get_template_calls_->end(); ++it) {
    it->first->DecRefN(it->second);  // it.second: # of times GetTpl was called
  }
  get_template_calls_->clear();
}

// ----------------------------------------------------------------------
// TemplateCache::ReloadAllIfChanged()
//    IMMEDIATE_RELOAD attempts to immediately reload and parse
//    all templates if the corresponding template files have changed.
//    LAZY_RELOAD just sets the reload bit in the cache so that the next
//    GetTemplate will reload and parse the template, if it changed.

//    NOTE: Suppose the search path is "dira:dirb", and a template is
//    created with name "foo", which resolves to "dirb/foo" because
//    dira/foo does not exist.  Then suppose dira/foo is created and then
//    ReloadAllIfChanged() is called. Then ReloadAllIfChanged() will replace
//    the contents of the template with dira/foo, *not* dirb/foo, even if
//    dirb/foo hasn't changed.
// ----------------------------------------------------------------------

void TemplateCache::ReloadAllIfChanged(ReloadType reload_type) {
  WriterMutexLock ml(mutex_);
  if (is_frozen_) {  // do not reload a frozen cache.
    return;
  }
  for (TemplateMap::iterator it = parsed_template_cache_->begin();
       it != parsed_template_cache_->end();
       ++it) {
    it->second.should_reload = true;
    if (reload_type == IMMEDIATE_RELOAD) {
      const Template* tpl = it->second.refcounted_tpl->tpl();
      // Reload should always use the original filename.
      // For instance on reload, we may replace an existing template with a
      // new one that came earlier on the search path.
      GetTemplateLocked(tpl->original_filename(), tpl->strip(), it->first);
    }
  }
}

// ----------------------------------------------------------------------
// TemplateCache::Freeze()
//    This method marks the cache as 'frozen'. After this method is called,
//    the cache is immutable, and cannot be modified. New templates cannot be
//    loaded and existing templates cannot be reloaded.
// ----------------------------------------------------------------------

void TemplateCache::Freeze() {
  {
    ReaderMutexLock ml(mutex_);
    if (is_frozen_) {  // if already frozen, then this is a no-op.
      return;
    }
  }
  // A final reload before freezing the cache.
  ReloadAllIfChanged(IMMEDIATE_RELOAD);
  {
    WriterMutexLock ml(mutex_);
    is_frozen_ = true;
  }
}

// ----------------------------------------------------------------------
// TemplateCache::Clone()
//    Clone makes a shallow copy of the parsed cache by incrementing
//    templates' refcount.
//    The caller is responsible for deallocating the returned TemplateCache.
// ----------------------------------------------------------------------

TemplateCache* TemplateCache::Clone() const {
  ReaderMutexLock ml(mutex_);
  TemplateCache* new_cache = new TemplateCache();
  *(new_cache->parsed_template_cache_) = *parsed_template_cache_;
  for (TemplateMap::iterator it = parsed_template_cache_->begin();
       it != parsed_template_cache_->end(); ++it) {
    it->second.refcounted_tpl->IncRef();
  }

  return new_cache;
}

// ----------------------------------------------------------------------
// TemplateCache::Refcount()
//    This routine is DEBUG-only. It returns the refcount of a template,
//    given the TemplateCacheKey.
// ----------------------------------------------------------------------

int TemplateCache::Refcount(const TemplateCacheKey template_cache_key) const {
  ReaderMutexLock ml(mutex_);
  CachedTemplate* it = find_ptr(*parsed_template_cache_, template_cache_key);
  return it ? it->refcounted_tpl->refcount() : 0;
}

// ----------------------------------------------------------------------
// TemplateCache::TemplateIsCached()
//    This routine is for testing only -- is says whether a given
//    template is already in the cache or not.
// ----------------------------------------------------------------------

bool TemplateCache::TemplateIsCached(const TemplateCacheKey template_cache_key)
    const {
  ReaderMutexLock ml(mutex_);
  return parsed_template_cache_->count(template_cache_key);
}

// ----------------------------------------------------------------------
// TemplateCache::ValidTemplateFilename
//    Validates the filename before constructing the template.
// ----------------------------------------------------------------------

bool TemplateCache::IsValidTemplateFilename(const string& filename,
                                            string* resolved_filename,
                                            FileStat* statbuf) const {
  if (!ResolveTemplateFilename(filename,
                               resolved_filename,
                               statbuf)) {
    LOG(WARNING) << "Unable to locate file " << filename << endl;
    return false;
  }
  if (statbuf->IsDirectory()) {
    LOG(WARNING) << *resolved_filename
                 << "is a directory and thus not readable" << endl;
    return false;
  }
  return true;
}

}
