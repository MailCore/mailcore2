// Copyright (c) 2008, Google Inc.
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
// Authors: jcrim@google.com (Jay Crim)
//          csilvers@google.com (Craig Silverstein)

#include <config.h>
#include "base/mutex.h"   // This has to come first to get _XOPEN_SOURCE
#include <ctemplate/find_ptr.h>
#include <ctemplate/template_string.h>
#include HASH_SET_H
#include "base/arena.h"
#include "base/thread_annotations.h"
#include <assert.h>
#include "base/macros.h"
#include "base/util.h"

using HASH_NAMESPACE::unordered_set;

namespace ctemplate {

// Based on public domain MurmurHashUnaligned2, by Austin Appleby.
// http://murmurhash.googlepages.com/
// This variation:
//   - interleaves odd/even 32-bit words to improve performance and
//     to generate more random bits,
//   - has a more complex final mix to combine the 32-bit hashes into
//     64-bits,
//   - uses a fixed seed.
// This is not static because template_string_test accesses it directly.
uint64_t MurmurHash64(const char* ptr, size_t len) {
  const uint32_t kMultiplyVal = 0x5bd1e995;
  const int kShiftVal = 24;
  const uint32_t kHashSeed1 = 0xc86b14f7;
  const uint32_t kHashSeed2 = 0x650f5c4d;

  uint32_t h1 = kHashSeed1 ^ len, h2 = kHashSeed2;
  while (len >= 8) {
    uint32_t k1 = UNALIGNED_LOAD32(ptr);
    k1 *= kMultiplyVal;
    k1 ^= k1 >> kShiftVal;
    k1 *= kMultiplyVal;

    h1 *= kMultiplyVal;
    h1 ^= k1;
    ptr += 4;

    uint32_t k2 = UNALIGNED_LOAD32(ptr);
    k2 *= kMultiplyVal;
    k2 ^= k2 >> kShiftVal;
    k2 *= kMultiplyVal;

    h2 *= kMultiplyVal;
    h2 ^= k2;
    ptr += 4;

    len -= 8;
  }

  if (len >= 4) {
    uint32_t k1 = UNALIGNED_LOAD32(ptr);
    k1 *= kMultiplyVal;
    k1 ^= k1 >> kShiftVal;
    k1 *= kMultiplyVal;

    h1 *= kShiftVal;
    h1 ^= k1;

    ptr += 4;
    len -= 4;
  }

  switch(len) {
    case 3:
      h2 ^= ptr[2] << 16;  // fall through.
    case 2:
      h2 ^= ptr[1] << 8;  // fall through.
    case 1:
      h2 ^= ptr[0];  // fall through.
    default:
      h2 *= kMultiplyVal;
  }

  h1 ^= h2 >> 18;
  h1 *= kMultiplyVal;
  h2 ^= h1 >> 22;
  h2 *= kMultiplyVal;
  h1 ^= h2 >> 17;
  h1 *= kMultiplyVal;

  uint64_t h = h1;
  h = (h << 32) | h2;
  return h;
}

// Unlike StaticTemplateString, it is not a good idea to have a
// default TemplateString::Hasher because TemplateString does not
// provide any lifetime guarantees.  The global template_string_set is
// an obvious exception.
struct TemplateStringHasher {
  size_t operator()(const TemplateString& ts) const {
    TemplateId id = ts.GetGlobalId();
    DCHECK(IsTemplateIdInitialized(id));
    return hasher(id);
  }
  // Less operator for MSVC's hash containers.
  bool operator()(const TemplateString& a, const TemplateString& b) const {
    const TemplateId id_a = a.GetGlobalId();
    const TemplateId id_b = b.GetGlobalId();
    assert(IsTemplateIdInitialized(id_a));
    assert(IsTemplateIdInitialized(id_b));
    return hasher(id_a, id_b);
  }
  TemplateIdHasher hasher;
  // These two public members are required by msvc.  4 and 8 are defaults.
  static const size_t bucket_size = 4;
  static const size_t min_buckets = 8;
};

namespace {
Mutex mutex(base::LINKER_INITIALIZED);

typedef unordered_set<TemplateString, TemplateStringHasher> TemplateStringSet;

TemplateStringSet* template_string_set
GUARDED_BY(mutex) PT_GUARDED_BY(mutex) = NULL;

UnsafeArena* arena
GUARDED_BY(mutex) PT_GUARDED_BY(mutex) = NULL;
}  // unnamed namespace


size_t StringHash::Hash(const char* s, size_t slen) const {
  return static_cast<size_t>(MurmurHash64(s, slen));
}

void TemplateString::AddToGlobalIdToNameMap() LOCKS_EXCLUDED(mutex) {
  // shouldn't be calling this if we don't have an id.
  CHECK(IsTemplateIdInitialized(id_));
  {
    // Check to see if it's already here.
    ReaderMutexLock reader_lock(&mutex);
    if (template_string_set) {
      const TemplateString* iter =
          find_ptr0(*template_string_set, *this);
      if (iter) {
        DCHECK_EQ(TemplateString(ptr_, length_),
                  TemplateString(iter->ptr_, iter->length_))
            << "TemplateId collision!";
        return;
      }
    }
  }
  WriterMutexLock writer_lock(&mutex);
  // First initialize our data structures if we need to.
  if (!template_string_set) {
    template_string_set = new TemplateStringSet;
  }

  if (!arena) {
    arena = new UnsafeArena(1024);  // 1024 was picked out of a hat.
  }

  if (template_string_set->count(*this)) {
    return;
  }
  // If we are immutable, we can store ourselves directly in the map.
  // Otherwise, we need to make an immutable copy.
  if (is_immutable()) {
    template_string_set->insert(*this);
  } else {
    const char* immutable_copy = arena->Memdup(ptr_, length_);
    template_string_set->insert(
        TemplateString(immutable_copy, length_, true, id_));
  }
}

TemplateId TemplateString::GetGlobalId() const {
  if (IsTemplateIdInitialized(id_)) {
    return id_;
  }
  // Initialize the id and sets the "initialized" flag.
  return static_cast<TemplateId>(MurmurHash64(ptr_, length_) |
                                 kTemplateStringInitializedFlag);
}

// static
TemplateString TemplateString::IdToString(TemplateId id) LOCKS_EXCLUDED(mutex) {
  ReaderMutexLock reader_lock(&mutex);
  if (!template_string_set)
    return TemplateString(kStsEmpty);
  // To search the set by TemplateId, we must first construct a dummy
  // TemplateString.  This may seem weird, but it lets us use a
  // hash_set instead of a hash_map.
  TemplateString id_as_template_string(NULL, 0, false, id);
  const TemplateString* iter = find_ptr0(*template_string_set, id_as_template_string);
  return iter ? *iter : TemplateString(kStsEmpty);
}

StaticTemplateStringInitializer::StaticTemplateStringInitializer(
    const StaticTemplateString* sts) {
  // Compute the sts's id if it wasn't specified at static-init
  // time.  If it was specified at static-init time, verify it's
  // correct.  This is necessary because static-init id's are, by
  // nature, pre-computed, and the id-generating algorithm may have
  // changed between the time they were computed and now.
  if (sts->do_not_use_directly_.id_ == 0) {
    sts->do_not_use_directly_.id_ = TemplateString(*sts).GetGlobalId();
  } else {
    // Don't use the TemplateString(const StaticTemplateString& sts)
    // constructor below, since if we do, GetGlobalId will just return
    // sts->do_not_use_directly_.id_ and the check will be pointless.
    DCHECK_EQ(TemplateString(sts->do_not_use_directly_.ptr_,
                             sts->do_not_use_directly_.length_).GetGlobalId(),
              sts->do_not_use_directly_.id_);
  }

  // Now add this id/name pair to the backwards map from id to name.
  TemplateString ts_copy_of_sts(*sts);
  ts_copy_of_sts.AddToGlobalIdToNameMap();
}

}
