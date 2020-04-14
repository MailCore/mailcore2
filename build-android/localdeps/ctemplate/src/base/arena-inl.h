// Copyright (c) 2000, Google Inc.
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
// Reorganized by Craig Silverstein
//
// In this file we define the arena template code.  This includes the
// ArenaAllocator, which is meant only to be used with STL, and also
// the Gladiator (which needs to know how to new and delete various
// types of objects).
//
// If you're only using the MALLOC-LIKE functionality of the arena,
// you don't need to include this file at all!  You do need to include
// it (in your own .cc file) if you want to use the STRING, STL, or
// NEW aspects of the arena.  See arena.h for details on these types.
//
// ArenaAllocator is an STL allocator, but because it relies on unequal
// instances, it may not work with all standards-conforming STL
// implementations.  But it works with SGI STL so we're happy.
//
// Here's an example of how the ArenaAllocator would be used.
// Say we have a vector of ints that we want to have use the arena
// for memory allocation.  Here's one way to do it:
//    UnsafeArena* arena = new UnsafeArena(1000); // or SafeArena(), or 10000
//    vector<int, ArenaAllocator<int, UnsafeArena> > v(arena);
//
// Note that every STL type always allows the allocator (in this case,
// the arena, which is automatically promoted to an allocator) as the last
// arg to the constructor.  So if you would normally do
//    vector<...> v(foo, bar),
// with the arena you can do
//    vector<...> v(foo, bar, arena);

#ifndef BASE_ARENA_INL_H_
#define BASE_ARENA_INL_H_

#include <config.h>
#include "base/arena.h"
#include <assert.h>
#include <stddef.h>
#include <new>
#include <memory>

namespace ctemplate {

// T is the type we want to allocate, and C is the type of the arena.
// ArenaAllocator has the thread-safety characteristics of C.
template <class T, class C> class ArenaAllocator {
 public:
  typedef T value_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  pointer address(reference r) const  { return &r; }
  const_pointer address(const_reference r) const  { return &r; }
  size_type max_size() const  { return size_t(-1) / sizeof(T); }

  // DO NOT USE! The default constructor is for gcc3 compatibility only.
  ArenaAllocator() : arena_(0) { }
  // This is not an explicit constructor!  So you can pass in an arena*
  // to functions needing an ArenaAllocator (like the astring constructor)
  // and everything will work ok.
  ArenaAllocator(C* arena) : arena_(arena) { }  // NOLINT
  ~ArenaAllocator() { }

  pointer allocate(size_type n,
                   std::allocator<void>::const_pointer /*hint*/ = 0) {
    assert(arena_ && "No arena to allocate from!");
    return reinterpret_cast<T*>(arena_->AllocAligned(n * sizeof(T),
                                                     kAlignment));
  }
  void deallocate(pointer p, size_type n) {
    arena_->Free(p, n * sizeof(T));
  }
  void construct(pointer p, const T & val) {
    new(reinterpret_cast<void*>(p)) T(val);
  }
  void construct(pointer p) {
    new(reinterpret_cast<void*>(p)) T();
  }
  void destroy(pointer p) { p->~T(); }

  C* arena(void) const { return arena_; }

  template<class U> struct rebind {
    typedef ArenaAllocator<U, C> other;
  };

  template<class U> ArenaAllocator(const ArenaAllocator<U, C>& other)
    : arena_(other.arena()) { }

  template<class U> bool operator==(const ArenaAllocator<U, C>& other) const {
    return arena_ == other.arena();
  }

  template<class U> bool operator!=(const ArenaAllocator<U, C>& other) const {
    return arena_ != other.arena();
  }

 protected:
  static const int kAlignment;
  C* arena_;
};

template<class T, class C> const int ArenaAllocator<T, C>::kAlignment =
    (1 == sizeof(T) ? 1 : BaseArena::kDefaultAlignment);


// 'new' must be in the global namespace.
}
using ctemplate::UnsafeArena;


// Operators for allocation on the arena
// Syntax: new (AllocateInArena, arena) MyClass;
//         new (AllocateInArena, arena) MyClass[num];
// Useful for classes you can't descend from Gladiator, such as POD,
// STL containers, etc.
enum AllocateInArenaType { AllocateInArena };

inline void* operator new(size_t size,
                          AllocateInArenaType /* unused */,
                          UnsafeArena *arena) {
  return arena->Alloc(size);
}

inline void* operator new[](size_t size,
                             AllocateInArenaType /* unused */,
                             UnsafeArena *arena) {
  return arena->Alloc(size);
}

namespace ctemplate {

// Ordinarily in C++, one allocates all instances of a class from an
// arena.  If that's what you want to do, you don't need Gladiator.
// (However you may find ArenaOnlyGladiator useful.)
//
// However, for utility classes that are used by multiple clients, the
// everything-in-one-arena model may not work.  Some clients may wish
// not to use an arena at all.  Or perhaps a composite structure
// (tree) will contain multiple objects (nodes) and some of those
// objects will be created by a factory, using an arena, while other
// objects will be created on-the-fly by an unsuspecting user who
// doesn't know anything about the arena.
//
// To support that, have the arena-allocated class inherit from
// Gladiator.  The ordinary operator new will continue to allocate
// from the heap.  To allocate from an arena, do
//     Myclass * m = new (AllocateInArena, a) Myclass (args, to, constructor);
// where a is either an arena or an allocator.  Now you can call
// delete on all the objects, whether they are allocated from an arena
// or on the heap.  Heap memory will be released, while arena memory will
// not be.
//
// If a client knows that no objects were allocated on the heap, it
// need not delete any objects (but it may if it wishes).  The only
// objects that must be deleted are those that were actually allocated
// from the heap.
//
// NOTE: an exception to the google C++ style guide rule for "No multiple
// implementation inheritance" is granted for this class: you can treat this
// class as an "Interface" class, and use it in a multiple inheritence context,
// even though it implements operator new/delete.

class Gladiator {
 public:
  Gladiator() { }
  virtual ~Gladiator() { }

  // We do not override the array allocators, so array allocation and
  // deallocation will always be from the heap.  Typically, arrays are
  // larger, and thus the costs of arena allocation are higher and the
  // benefits smaller.  Since arrays are typically allocated and deallocated
  // very differently from scalars, this may not interfere too much with
  // the arena concept.  If it does pose a problem, flesh out the
  // ArrayGladiator class below.

  void* operator new(size_t size) {
    void* ret = ::operator new(1 + size);
    static_cast<char *>(ret)[size] = 1;     // mark as heap-allocated
    return ret;
  }
  // the ignored parameter keeps us from stepping on placement new
  template<class T> void* operator new(size_t size, const int ignored,
                                       T* allocator) {
    if (allocator) {
      void* ret = allocator->AllocAligned(1 + size,
                                          BaseArena::kDefaultAlignment);
      static_cast<char*>(ret)[size] = 0;  // mark as arena-allocated
      return ret;
    } else {
      return operator new(size);          // this is the function above
    }
  }
  void operator delete(void* memory, size_t size) {
    if (static_cast<char*>(memory)[size]) {
      assert (1 == static_cast<char *>(memory)[size]);
      ::operator delete(memory);
    } else {
      // We never call the allocator's Free method.  If we need to do
      // that someday, we can store a pointer to the arena instead of
      // the Boolean marker flag.
    }
  }
  template<class T> void operator delete(void* memory, size_t size,
                                         const int ign, T* allocator) {
    // This "placement delete" can only be called if the constructor
    // throws an exception.
    if (allocator) {
      allocator->Free(memory, 1 + size);
    } else {
      ::operator delete(memory);
    }
  }
};

// This avoids the space overhead of Gladiator if you just want to
// override new and delete.  It helps avoid some of the more common
// problems that can occur when overriding new and delete.

class ArenaOnlyGladiator {
 public:
  ArenaOnlyGladiator() { }
  // No virtual destructor is needed because we ignore the size
  // parameter in all the delete functions.
  // virtual ~ArenaOnlyGladiator() { }

  // can't just return NULL here -- compiler gives a warning. :-|
  void* operator new(size_t /*size*/) {
    assert(0);
    return reinterpret_cast<void *>(1);
  }
  void* operator new[](size_t /*size*/) {
    assert(0);
    return reinterpret_cast<void *>(1);
  }

  // the ignored parameter keeps us from stepping on placement new
  template<class T> void* operator new(size_t size, const int ignored,
                                       T* allocator) {
    assert(allocator);
    return allocator->AllocAligned(size, BaseArena::kDefaultAlignment);
  }
  template<class T> void* operator new[](size_t size,
                                         const int ignored, T* allocator) {
    assert(allocator);
    return allocator->AllocAligned (size, BaseArena::kDefaultAlignment);
  }
  void operator delete(void* /*memory*/, size_t /*size*/) { }
  template<class T> void operator delete(void* memory, size_t size,
                                         const int ign, T* allocator) { }
  void operator delete [](void* /*memory*/) { }
  template<class T> void operator delete(void* memory,
                                         const int ign, T* allocator) { }
};

#if 0  // ********** for example purposes only; 100% untested.

// Note that this implementation incurs an overhead of kHeaderSize for
// every array that is allocated.  *Before* the space is returned to the
// user, we store the address of the Arena that owns the space, and
// the length of th space itself.

class ArrayGladiator : public Gladiator {
 public:
  void * operator new[] (size_t size) {
    const int sizeplus = size + kHeaderSize;
    void * const ret = ::operator new(sizeplus);
    *static_cast<Arena **>(ret) = NULL;  // mark as heap-allocated
    *static_cast<size_t *>(ret + sizeof(Arena *)) = sizeplus;
    return ret + kHeaderSize;
  }
  // the ignored parameter keeps us from stepping on placement new
  template<class T> void * operator new[] (size_t size,
                                           const int ignored, T * allocator) {
    if (allocator) {
      const int sizeplus = size + kHeaderSize;
      void * const ret =
          allocator->AllocAligned(sizeplus, BaseArena::kDefaultAlignment);
      *static_cast<Arena **>(ret) = allocator->arena();
      *static_cast<size_t *>(ret + sizeof(Arena *)) = sizeplus;
      return ret + kHeaderSize;
    } else {
      return operator new[](size);  // this is the function above
    }
  }
  void operator delete [] (void * memory) {
    memory -= kHeaderSize;
    Arena * const arena = *static_cast<Arena **>(memory);
    size_t sizeplus = *static_cast<size_t *>(memory + sizeof(arena));
    if (arena) {
      arena->SlowFree(memory, sizeplus);
    } else {
      ::operator delete (memory);
    }
  }
  template<class T> void * operator delete (void * memory,
                                            const int ign, T * allocator) {
    // This "placement delete" can only be called if the constructor
    // throws an exception.
    memory -= kHeaderSize;
    size_t sizeplus = *static_cast<size_t *>(memory + sizeof(Arena *));
    if (allocator) {
      allocator->Free(memory, 1 + size);
    } else {
      operator delete (memory);
    }
  }

 protected:
  static const int kMinSize = sizeof size_t + sizeof(Arena *);
  static const int kHeaderSize = kMinSize > BaseArena::kDefaultAlignment ?
    2 * BaseArena::kDefaultAlignment : BaseArena::kDefaultAlignment;
};

#endif  // ********** example

}

#endif  // BASE_ARENA_INL_H_
