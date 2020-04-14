// Copyright (c) 2006, Google Inc.
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
//
// Author: kenton@google.com (Kenton Varda)
//
// small_map is a drop-in replacement for map or hash_map.  It uses a fixed
// array to store a certain number of elements, then reverts to using a
// map or hash_map when it runs out of space.  For maps that are typically
// small, this can be considerably faster than using something like hash_map
// directly, as hash_map is optimized for large data sets.  Of course, in
// order for this to be a significant win, you have to have a situation where
// you are using lots and lots of these small maps.  One such situation is
// MessageSet:  A set of search results may contain thousands of MessageSets,
// each containing only a couple items.
//
// TODO(kenton):  This is very minimal, and was originally written for a
//   very specific use (MessageSet).  It only implements a few core methods
//   of the STL associative container interface, though you are welcome to
//   extend it.

#ifndef UTIL_GTL_SMALL_MAP_H_
#define UTIL_GTL_SMALL_MAP_H_

#include <config.h>
#include <assert.h>
#include <utility>   // for make_pair()
#include "base/manual_constructor.h"

namespace ctemplate {

template <bool> struct CompileAssert { };
#define COMPILE_ASSERT(expr, msg) \
  typedef CompileAssert<(bool(expr))> msg[bool(expr) ? 1 : -1]

// An STL-like associative container which starts out backed by a simple
// array but switches to some other container type if it grows beyond a
// fixed size.
//
// NormalMap:  The map type to fall back to.  This also defines the key
//             and value types for the small_map.
// kArraySize:  The size of the initial array of results.  Once the map
//              grows beyond this size, the map type will be used instead.
// EqualKey:  A functor which tests two keys for equality.  If the wrapped
//            map type has a "key_equal" member (hash_map does), then that
//            will be used by default.  Otherwise you must specify this
//            manually.
// MapInit: A functor that takes a ManualConstructor<NormalMap>* and uses it to
//          initialize the map. This functor will be called at most once per
//          small_map, when the map exceeds the threshold of kArraySize and we
//          are about to copy values from the array to the map. The functor
//          *must* call one of the Init() methods provided by
//          ManualConstructor, since after it runs we assume that the NormalMap
//          has been initialized.
//
// example:
//   small_map<hash_map<string, int> > days;
//   days["sunday"   ] = 0;
//   days["monday"   ] = 1;
//   days["tuesday"  ] = 2;
//   days["wednesday"] = 3;
//   days["thursday" ] = 4;
//   days["friday"   ] = 5;
//   days["saturday" ] = 6;
//
// You should assume that small_map might invalidate all the iterators
// on any call to erase(), insert() and operator[].
template <typename NormalMap>
class small_map_default_init {
 public:
  void operator ()(ManualConstructor<NormalMap>* map) const {
    map->Init();
  }
};

template <typename NormalMap,
          int kArraySize = 4,
          typename EqualKey = typename NormalMap::key_equal,
          typename MapInit = small_map_default_init<NormalMap> >
class small_map {
  // We cannot rely on the compiler to reject array of size 0.  In
  // particular, gcc 2.95.3 does it but later versions allow 0-length
  // arrays.  Therefore, we explicitly reject non-positive kArraySize
  // here.
  COMPILE_ASSERT(kArraySize > 0, default_initial_size_should_be_positive);

 public:
  typedef typename NormalMap::key_type key_type;
  typedef typename NormalMap::mapped_type data_type;
  typedef typename NormalMap::mapped_type mapped_type;
  typedef typename NormalMap::value_type value_type;
  typedef EqualKey key_equal;

  small_map() : size_(0), functor_(MapInit()) {}

  explicit small_map(const MapInit& functor) : size_(0), functor_(functor) {}

  // Allow copy-constructor and assignment, since STL allows them too.
  small_map(const small_map& src) {
    // size_ and functor_ are initted in InitFrom()
    InitFrom(src);
  }
  void operator=(const small_map& src) {
    if (&src == this) return;

    // This is not optimal. If src and dest are both using the small
    // array, we could skip the teardown and reconstruct. One problem
    // to be resolved is that the value_type itself is pair<const K,
    // V>, and const K is not assignable.
    Destroy();
    InitFrom(src);
  }
  ~small_map() {
    Destroy();
  }

  class const_iterator;

  class iterator {
   public:
    typedef typename NormalMap::iterator::iterator_category iterator_category;
    typedef typename NormalMap::iterator::value_type value_type;
    typedef typename NormalMap::iterator::difference_type difference_type;
    typedef typename NormalMap::iterator::pointer pointer;
    typedef typename NormalMap::iterator::reference reference;

    inline iterator(): array_iter_(NULL) {}

    inline iterator& operator++() {
      if (array_iter_ != NULL) {
        ++array_iter_;
      } else {
        ++hash_iter_;
      }
      return *this;
    }
    inline iterator operator++(int) {
      iterator result(*this);
      ++(*this);
      return result;
    }
    inline iterator& operator--() {
      if (array_iter_ != NULL) {
        --array_iter_;
      } else {
        --hash_iter_;
      }
      return *this;
    }
    inline iterator operator--(int) {
      iterator result(*this);
      --(*this);
      return result;
    }
    inline value_type* operator->() const {
      if (array_iter_ != NULL) {
        return array_iter_->get();
      } else {
        return hash_iter_.operator->();
      }
    }

    inline value_type& operator*() const {
      if (array_iter_ != NULL) {
        return *array_iter_->get();
      } else {
        return *hash_iter_;
      }
    }

    inline bool operator==(const iterator& other) const {
      if (array_iter_ != NULL) {
        return array_iter_ == other.array_iter_;
      } else {
        return other.array_iter_ == NULL && hash_iter_ == other.hash_iter_;
      }
    }

    inline bool operator!=(const iterator& other) const {
      return !(*this == other);
    }

    bool operator==(const const_iterator& other) const;
    bool operator!=(const const_iterator& other) const;

   private:
    friend class small_map;
    friend class const_iterator;
    inline explicit iterator(ManualConstructor<value_type>* init)
      : array_iter_(init) {}
    inline explicit iterator(const typename NormalMap::iterator& init)
      : array_iter_(NULL), hash_iter_(init) {}

    ManualConstructor<value_type>* array_iter_;
    typename NormalMap::iterator hash_iter_;
  };

  class const_iterator {
   public:
    typedef typename NormalMap::const_iterator::iterator_category iterator_category;
    typedef typename NormalMap::const_iterator::value_type value_type;
    typedef typename NormalMap::const_iterator::difference_type difference_type;
    typedef typename NormalMap::const_iterator::pointer pointer;
    typedef typename NormalMap::const_iterator::reference reference;

    inline const_iterator(): array_iter_(NULL) {}
    inline const_iterator(const iterator& other)
      : array_iter_(other.array_iter_), hash_iter_(other.hash_iter_) {}

    inline const_iterator& operator++() {
      if (array_iter_ != NULL) {
        ++array_iter_;
      } else {
        ++hash_iter_;
      }
      return *this;
    }
    inline const_iterator operator++(int) {
      const_iterator result(*this);
      ++(*this);
      return result;
    }

    inline const_iterator& operator--() {
      if (array_iter_ != NULL) {
        --array_iter_;
      } else {
        --hash_iter_;
      }
      return *this;
    }
    inline const_iterator operator--(int) {
      const_iterator result(*this);
      --(*this);
      return result;
    }

    inline const value_type* operator->() const {
      if (array_iter_ != NULL) {
        return array_iter_->get();
      } else {
        return hash_iter_.operator->();
      }
    }

    inline const value_type& operator*() const {
      if (array_iter_ != NULL) {
        return *array_iter_->get();
      } else {
        return *hash_iter_;
      }
    }

    inline bool operator==(const const_iterator& other) const {
      if (array_iter_ != NULL) {
        return array_iter_ == other.array_iter_;
      } else {
        return other.array_iter_ == NULL && hash_iter_ == other.hash_iter_;
      }
    }

    inline bool operator!=(const const_iterator& other) const {
      return !(*this == other);
    }

   private:
    friend class small_map;
    inline explicit const_iterator(
        const ManualConstructor<value_type>* init)
      : array_iter_(init) {}
    inline explicit const_iterator(
        const typename NormalMap::const_iterator& init)
      : array_iter_(NULL), hash_iter_(init) {}

    const ManualConstructor<value_type>* array_iter_;
    typename NormalMap::const_iterator hash_iter_;
  };

  iterator find(const key_type& key) {
    key_equal compare;
    if (size_ >= 0) {
      for (int i = 0; i < size_; i++) {
        if (compare(array_[i]->first, key)) {
          return iterator(array_ + i);
        }
      }
      return iterator(array_ + size_);
    } else {
      return iterator(map()->find(key));
    }
  }

  const_iterator find(const key_type& key) const {
    key_equal compare;
    if (size_ >= 0) {
      for (int i = 0; i < size_; i++) {
        if (compare(array_[i]->first, key)) {
          return const_iterator(array_ + i);
        }
      }
      return const_iterator(array_ + size_);
    } else {
      return const_iterator(map()->find(key));
    }
  }

  // Invalidates iterators.
  data_type& operator[](const key_type& key) {
    key_equal compare;

    if (size_ >= 0) {
      // operator[] searches backwards, favoring recently-added
      // elements.
      for (int i = size_-1; i >= 0; --i) {
        if (compare(array_[i]->first, key)) {
          return array_[i]->second;
        }
      }
      if (size_ == kArraySize) {
        ConvertToRealMap();
        return (*map_)[key];
      } else {
        array_[size_].Init(key, data_type());
        return array_[size_++]->second;
      }
    } else {
      return (*map_)[key];
    }
  }

  // Invalidates iterators.
  std::pair<iterator, bool> insert(const value_type& x) {
    key_equal compare;

    if (size_ >= 0) {
      for (int i = 0; i < size_; i++) {
        if (compare(array_[i]->first, x.first)) {
          return std::make_pair(iterator(array_ + i), false);
        }
      }
      if (size_ == kArraySize) {
        ConvertToRealMap();  // Invalidates all iterators!
        std::pair<typename NormalMap::iterator, bool> ret = map_->insert(x);
        return std::make_pair(iterator(ret.first), ret.second);
      } else {
        array_[size_].Init(x);
        return std::make_pair(iterator(array_ + size_++), true);
      }
    } else {
      std::pair<typename NormalMap::iterator, bool> ret = map_->insert(x);
      return std::make_pair(iterator(ret.first), ret.second);
    }
  }

  // Invalidates iterators.
  template <class InputIterator>
  void insert(InputIterator f, InputIterator l) {
    while (f != l) {
      insert(*f);
      ++f;
    }
  }

  iterator begin() {
    if (size_ >= 0) {
      return iterator(array_);
    } else {
      return iterator(map_->begin());
    }
  }
  const_iterator begin() const {
    if (size_ >= 0) {
      return const_iterator(array_);
    } else {
      return const_iterator(map_->begin());
    }
  }

  iterator end() {
    if (size_ >= 0) {
      return iterator(array_ + size_);
    } else {
      return iterator(map_->end());
    }
  }
  const_iterator end() const {
    if (size_ >= 0) {
      return const_iterator(array_ + size_);
    } else {
      return const_iterator(map_->end());
    }
  }

  void clear() {
    if (size_ >= 0) {
      for (int i = 0; i < size_; i++) {
        array_[i].Destroy();
      }
    } else {
      map_.Destroy();
    }
    size_ = 0;
  }

  // Invalidates iterators.
  void erase(const iterator& position) {
    if (size_ >= 0) {
      int i = position.array_iter_ - array_;
      array_[i].Destroy();
      --size_;
      if (i != size_) {
        array_[i].Init(*array_[size_]);
        array_[size_].Destroy();
      }
    } else {
      map_->erase(position.hash_iter_);
    }
  }

  int erase(const key_type& key) {
    iterator iter = find(key);
    if (iter == end()) return 0;
    erase(iter);
    return 1;
  }

  int count(const key_type& key) const {
    return (find(key) == end()) ? 0 : 1;
  }

  int size() const {
    if (size_ >= 0) {
      return size_;
    } else {
      return map_->size();
    }
  }

  bool empty() const {
    if (size_ >= 0) {
      return (size_ == 0);
    } else {
      return map_->empty();
    }
  }

  // Returns true if we have fallen back to using the underlying map
  // representation.
  bool using_full_map() const {
    return size_ < 0;
  }

  inline NormalMap* map() {
    assert(using_full_map());
    return map_.get();
  }
  inline const NormalMap* map() const {
    assert(using_full_map());
    return map_.get();
  }

 private:
  int size_;  // negative = using hash_map

  MapInit functor_;

  // We want to call constructors and destructors manually, but we don't
  // want to allocate and deallocate the memory used for them separately.
  // So, we use this crazy ManualConstructor class.
  //
  // Since array_ and map_ are mutually exclusive, we'll put them in a
  // union, too.  We add in a dummy_ value which quiets MSVC (both
  // 7.1 and 8.0) from otherwise giving an erroneous "union member has
  // copy constructor" error message (C2621).  This dummy member has
  // to come before array_ to quiet the compiler.  Shrug.
  union {
    ManualConstructor<value_type> dummy_;
    ManualConstructor<value_type> array_[kArraySize];
    ManualConstructor<NormalMap> map_;
  };

  void ConvertToRealMap() {
    // Move the current elements into a temporary array.
    ManualConstructor<value_type> temp_array[kArraySize];

    for (int i = 0; i < kArraySize; i++) {
      temp_array[i].Init(*array_[i]);
      array_[i].Destroy();
    }

    // Initialize the map.
    size_ = -1;
    functor_(&map_);

    // Insert elements into it.
    for (int i = 0; i < kArraySize; i++) {
      map_->insert(*temp_array[i]);
      temp_array[i].Destroy();
    }
  }

  // Helpers for constructors and destructors.
  void InitFrom(const small_map& src) {
    functor_ = src.functor_;
    size_ = src.size_;
    if (src.size_ >= 0) {
      for (int i = 0; i < size_; i++) {
        array_[i].Init(*src.array_[i]);
      }
    } else {
      functor_(&map_);
      (*map_.get()) = (*src.map_.get());
    }
  }
  void Destroy() {
    if (size_ >= 0) {
      for (int i = 0; i < size_; i++) {
        array_[i].Destroy();
      }
    } else {
      map_.Destroy();
    }
  }
};

template <typename NormalMap, int kArraySize, typename EqualKey,
          typename Functor>
inline bool small_map<NormalMap, kArraySize, EqualKey,
                      Functor>::iterator::operator==(
    const const_iterator& other) const {
  return other == *this;
}
template <typename NormalMap, int kArraySize, typename EqualKey,
          typename Functor>
inline bool small_map<NormalMap, kArraySize, EqualKey,
                      Functor>::iterator::operator!=(
    const const_iterator& other) const {
  return other != *this;
}

}

#endif  // UTIL_GTL_SMALL_MAP_H_
