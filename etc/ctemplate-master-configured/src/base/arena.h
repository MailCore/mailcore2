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
// "Handles" by Ilan Horn
//
// Sometimes it is necessary to allocate a large number of small
// objects.  Doing this the usual way (malloc, new) is slow,
// especially for multithreaded programs.  A BaseArena provides a
// mark/release method of memory management: it asks for a large chunk
// from the operating system and doles it out bit by bit as required.
// Then you free all the memory at once by calling BaseArena::Reset().
//
// Use SafeArena for multi-threaded programs where multiple threads
// could access the same arena at once.  Use UnsafeArena otherwise.
// Usually you'll want UnsafeArena.
//
// There are four ways to use the arena.  arena.h and arena.cc are
// sufficient for the MALLOC and STRINGS uses.  For NEW and STL you'll
// also need to include arena-inl.h in the appropriate .cc file.
// However, we do *declare* (but not define) the template types here.
//
// LIKE MALLOC: --Uses UnsafeArena (or SafeArena)--
//    This is the simplest way.  Just create an arena, and whenever you
//    need a block of memory to put something in, call BaseArena::Alloc().  eg
//        s = arena.Alloc(100);
//        snprintf(s, 100, "%s:%d", host, port);
//        arena.Shrink(strlen(s)+1);     // optional; see below for use
//
//    You'll probably use the convenience routines more often:
//        s = arena.Strdup(host);        // a copy of host lives in the arena
//        s = arena.Strndup(host, 100);  // we guarantee to NUL-terminate!
//        s = arena.Memdup(protobuf, sizeof(protobuf);
//
//    If you go the Alloc() route, you'll probably allocate too-much-space.
//    You can reclaim the extra space by calling Shrink() before the next
//    Alloc() (or Strdup(), or whatever), with the #bytes you actually used.
//       If you use this method, memory management is easy: just call Alloc()
//    and friends a lot, and call Reset() when you're done with the data.
//
// FOR STRINGS: --Uses UnsafeArena (or SafeArena)--
//    This is a special case of STL (below), but is simpler.  Use an
//    astring, which acts like a string but allocates from the passed-in
//    arena:
//       astring s(arena);             // or "sastring" to use a SafeArena
//       s.assign(host);
//       astring s2(host, hostlen, arena);
//
// WITH NEW: --Uses BaseArena, Gladiator (or ArenaOnlyGladiator)--
//    Use this to allocate a C++ class object (or any other object you
//    have to get via new/delete rather than malloc/free).
//    There are several things you have to do in this case:
//       1) Your class (the one you new) must inherit from Gladiator.
//       2) To actually allocate this class on the arena, use
//             myclass = new (AllocateInArena, arena) MyClass(constructor, args)
//
//    Note that MyClass doesn't need to have the arena passed in.
//    But if it, in turn, wants to call "new" on some of its member
//    variables, and you want those member vars to be on the arena
//    too, you better pass in an arena so it can call new(0,arena).
//
//    If you can guarantee that everyone who ever calls new on
//    MyClass uses the new(0,arena) form (ie nobody ever just says
//    new), you can have MyClass subclass from ArenaOnlyGladiator
//    rather than from Gladiator.  ArenaOnlyGladiator is a bit more
//    efficient (faster and smaller), but is otherwise identical.
//
//    If you allocate myclass using new(0,arena), and MyClass only
//    does memory management in the destructor, it's not necessary
//    to even call "delete myclass;", you can just call arena.Reset();
//    If the destructor does something else (closes a file, logs
//    a message, whatever), you'll have to call destructor and Reset()
//    both: "delete myclass; arena.Reset();"
//
//    Note that you can not allocate an array of classes this way:
//         noway = new (AllocateInArena, arena) MyClass[5];   // not supported!
//    It's not difficult to program, we just haven't done it.  Arrays
//    are typically big and so there's little point to arena-izing them.
//
// WITH NEW: --Uses UnsafeArena--
//    There are cases where you can't inherit the class from Gladiator,
//    or inheriting would be too expensive.  Examples of this include
//    plain-old-data (allocated using new) and third-party classes (such
//    as STL containers).  arena-inl.h provides a global operator new
//    that can be used as follows:
//
//    #include "base/arena-inl.h"
//
//      UnsafeArena arena(1000);
//      Foo* foo = new (AllocateInArena, &arena) Foo;
//      Foo* foo_array = new (AllocateInArena, &arena) Foo[10];
//
// IN STL: --Uses BaseArena, ArenaAllocator--
//    All STL containers (vector, hash_map, etc) take an allocator.
//    You can use the arena as an allocator.  Then whenever the vector
//    (or whatever) wants to allocate memory, it will take it from the
//    arena.  To use, you just indicate in the type that you want to use the
//    arena, and then actually give a pointer to the arena as the last
//    constructor arg:
//       vector<int, ArenaAllocator<int, UnsafeArena> > v(&arena);
//       v.push_back(3);
//
// WARNING: Careless use of STL within an arena-allocated object can
//    result in memory leaks if you rely on arena.Reset() to free
//    memory and do not call the object destructor.  This is actually
//    a subclass of a more general hazard: If an arena-allocated
//    object creates (and owns) objects that are not also
//    arena-allocated, then the creating object must have a
//    destructor that deletes them, or they will not be deleted.
//    However, since the outer object is arena allocated, it's easy to
//    forget to call delete on it, and needing to do so may seem to
//    negate much of the benefit of arena allocation.  A specific
//    example is use of vector<string> in an arena-allocated object,
//    since type string is not atomic and is always allocated by the
//    default runtime allocator.  The arena definition provided here
//    allows for much flexibility, but you ought to carefully consider
//    before defining arena-allocated objects which in turn create
//    non-arena allocated objects.
//
// WITH HANDLES:
//    The various arena classes can supply compact handles to data kept
//    in the arena. These handles consume only 4 bytes each, and are thus
//    more efficient than pointers - this may be interesting in cases
//    where a very large number of references to memory in the arena need
//    to be kept.
//    Note that handles are limited in the amount of data that can be reference
//    in the arena, typically to 4GB*the number given to set_handle_alignment()
//    (which defaults to 1). The number of allocations that can have handles
//    is, of course, smaller than 4G (that's what's representable by 32 bits).
//    It does depend on their sizes, however. In a worst-case scenario each
//    allocation consumes a page of its own, and we will run out of handles
//    after approximately (4G/block_size)*handle_alignment allocations.
//    When we run out of handles or allocate data over the amount of memory
//    that handles can reference, an invalid handle will be returned (but
//    the requested memory will still be allocated in the arena).
//    Handles memory use is most efficient when the arena block size is a power
//    of two. When this is not the case, we can run out of handles when at
//    most half of the addressable space (as described above) is not in use.
//    At worst handles can reference at least 2GB*handle_alignment.
//    Example use:
//      UnsafeArena arena(16384);
//      arena.set_handle_alignment(4);
//      // Assume you want to keep the string s in the arena.
//      Handle h = arena.MemdupWithHandle(s.c_str(), s.length());
//      // Later, to get the memory from the handle, use:
//      void* p = arena.HandleToPointer(h);
//      // Note that there's no way to retrieve the size from the handle.
//      // It probably makes sense to encode the size into the buffer saved,
//      // unless the size is known/fixed.
//  Internal machinery of handles:
//    The handle consists of the block index in the arena and the offset
//    inside the block, encoded into a single unsigned uint32 value.
//    Note that, the rightmost alignment bits (controlled by
//    set_handle_alignment()) are shaved off the saved offset in the Handle,
//    to give some extra capacity :)
//    set_handle_alignment() can only be called when the arena is empty,
//    as changing it invalidates any handles that are still in flight.
//
//
// PUTTING IT ALL TOGETHER
//    Here's a program that uses all of the above.  Note almost all the
//    examples are the various ways to use "new" and STL.  Using the
//    malloc-like features and the string type are much easier!
//
// Class A : public Gladiator {
//  public:
//   int i;
//   vector<int> v1;
//   vector<int, ArenaAllocator<int, UnsafeArena> >* v3;
//   vector<int, ArenaAllocator<int, UnsafeArena> >* v4;
//   vector<int>* v5;
//   vector<string> vs;
//   vector<astring> va;
//   char *s;
//   A() : v1(), v3(NULL), v4(NULL), vs(), va(), s(NULL) {
//     // v1 is allocated on the arena whenever A is.  Its ints never are.
//     v5 = new vector<int>;
//     // v5 is not allocated on the arena, and neither are any of its ints.
//   }
//   ~A() {
//     delete v5;          // needed since v5 wasn't allocated on the arena
//     printf("I'm done!\n");
//   }
// };
//
// class B : public A {    // we inherit from Gladiator, but indirectly
//  public:
//   UnsafeArena* arena_;
//   vector<int, ArenaAllocator<int, UnsafeArena> > v2;
//   vector<A> va1;
//   vector<A, ArenaAllocator<A, UnsafeArena> > va2;
//   vector<A>* pva;
//   vector<A, ArenaAllocator<A, UnsafeArena> >* pva2;
//   astring a;
//
//   B(UnsafeArena * arena)
//     : arena_(arena), v2(arena_), va1(), va2(arena_), a("initval", arena_) {
//     v3 = new vector<int, ArenaAllocator<int, UnsafeArena> >(arena_);
//     v4 = new (AllocateInArena, arena_) vector<int, ArenaAllocator<int, UnsafeArena> >(arena_);
//     v5 = new (AllocateInArena, arena_) vector<int>;
//     // v2 is allocated on the arena whenever B is.  Its ints always are.
//     // v3 is not allocated on the arena, but the ints you give it are
//     // v4 is allocated on the arena, and so are the ints you give it
//     // v5 is allocated on the arena, but the ints you give it are not
//     // va1 is allocated on the arena whenever B is.  No A ever is.
//     // va2 is allocated on the arena whenever B is.  Its A's always are.
//     pva = new (AllocateInArena, arena_) vector<A>;
//     pva2 = new (AllocateInArena, arena_) vector<A, ArenaAllocator<A, UnsafeArena> >(arena_);
//     // pva is allocated on the arena, but its A's are not
//     // pva2 is allocated on the arena, and so are its A's.
//     // a's value "initval" is stored on the arena.  If we reassign a,
//     // the new value will be stored on the arena too.
//   }
//   ~B() {
//      delete v3;   // necessary to free v3's memory, though not its ints'
//      // don't need to delete v4: arena_.Reset() will do as good
//      delete v5;   // necessary to free v5's ints memory, though not v5 itself
//      delete pva;  // necessary to make sure you reclaim space used by A's
//      delete pva2; // safe to call this; needed if you want to see the printfs
//      // pva2->clear() -- not necessary, arena_.Reset() will do just as good
//   }
// };
//
// main() {
//   UnsafeArena arena(1000);
//   A a1;                               // a1 is not on the arena
//   a1.vs.push_back(string("hello"));   // hello is not copied onto the arena
//   a1.va.push_back(astring("hello", &arena));      // hello is on the arena,
//                                                   // astring container isn't
//   a1.s = arena.Strdup("hello");       // hello is on the arena
//
//   A* a2 = new (AllocateInArena, arena) A;         // a2 is on the arena
//   a2.vs.push_back(string("hello"));   // hello is *still* not on the arena
//   a2.s = arena.Strdup("world");       // world is on the arena.  a1.s is ok
//
//   B b1(&arena);                       // B is not allocated on the arena
//   b1.a.assign("hello");               // hello is on the arena
//   b1.pva2.push_back(a1);              // our copy of a1 will be stored on
//                                       // the arena, though a1 itself wasn't
//   arena.Reset();                      // all done with our memory!
// }

#ifndef BASE_ARENA_H_
#define BASE_ARENA_H_

#include <config.h>
#include "base/mutex.h"   // must go first to get _XOPEN_SOURCE
#include <assert.h>
#include <cstdint>
#include <string.h>
#include <vector>
#include "base/thread_annotations.h"
#include "base/util.h"    // for CHECK, etc

namespace ctemplate {

// Annoying stuff for windows -- make sure clients (in this case
// unittests) can import the class definitions and variables.
#ifndef CTEMPLATE_DLL_DECL
# ifdef _MSC_VER
#   define CTEMPLATE_DLL_DECL  __declspec(dllimport)
# else
#   define CTEMPLATE_DLL_DECL  /* should be the empty string for non-windows */
# endif
#endif

// This class is "thread-compatible": different threads can access the
// arena at the same time without locking, as long as they use only
// const methods.
class CTEMPLATE_DLL_DECL BaseArena {
 protected:         // You can't make an arena directly; only a subclass of one
  BaseArena(char* first_block, const size_t block_size, bool align_to_page);
 public:
  virtual ~BaseArena();

  virtual void Reset();

  // A handle to a pointer in an arena. An opaque type, with default
  // copy and assignment semantics.
  class Handle {
   public:
    static const uint32_t kInvalidValue = 0xFFFFFFFF;   // int32-max

    Handle() : handle_(kInvalidValue) { }
    // Default copy constructors are fine here.
    bool operator==(const Handle& h) const { return handle_ == h.handle_; }
    bool operator!=(const Handle& h) const { return handle_ != h.handle_; }

    uint32_t hash() const { return handle_; }
    bool valid() const { return handle_ != kInvalidValue; }

   private:
    // Arena needs to be able to access the internal data.
    friend class BaseArena;

    explicit Handle(uint32_t handle) : handle_(handle) { }

    uint32_t handle_;
  };

  // they're "slow" only 'cause they're virtual (subclasses define "fast" ones)
  virtual char* SlowAlloc(size_t size) = 0;
  virtual void  SlowFree(void* memory, size_t size) = 0;
  virtual char* SlowRealloc(char* memory, size_t old_size, size_t new_size) = 0;
  virtual char* SlowAllocWithHandle(const size_t size, Handle* handle) = 0;

  // Set the alignment to be used when Handles are requested. This can only
  // be set for an arena that is empty - it cannot be changed on the fly.
  // The alignment must be a power of 2 that the block size is divisable by.
  // The default alignment is 1.
  // Trying to set an alignment that does not meet the above constraints will
  // cause a CHECK-failure.
  void set_handle_alignment(int align);

  // Retrieve the memory pointer that the supplied handle refers to.
  // Calling this with an invalid handle will CHECK-fail.
  void* HandleToPointer(const Handle& h) const;


  class Status {
   private:
    friend class BaseArena;
    size_t bytes_allocated_;
   public:
    Status() : bytes_allocated_(0) { }
    size_t bytes_allocated() const {
      return bytes_allocated_;
    }
  };

  // Accessors and stats counters
  // This accessor isn't so useful here, but is included so we can be
  // type-compatible with ArenaAllocator (in arena-inl.h).  That is,
  // we define arena() because ArenaAllocator does, and that way you
  // can template on either of these and know it's safe to call arena().
  virtual BaseArena* arena()  { return this; }
  size_t block_size() const   { return block_size_; }
  int block_count() const;
  bool is_empty() const {
    // must check block count in case we allocated a block larger than blksize
    return freestart_ == freestart_when_empty_ && 1 == block_count();
  }

  // This should be the worst-case alignment for any type.  This is
  // good for IA-32, SPARC version 7 (the last one I know), and
  // supposedly Alpha.  i386 would be more time-efficient with a
  // default alignment of 8, but ::operator new() uses alignment of 4,
  // and an assertion will fail below after the call to MakeNewBlock()
  // if you try to use a larger alignment.
#ifdef __i386__
  static const int kDefaultAlignment = 4;
#else
  static const int kDefaultAlignment = 8;
#endif

 protected:
  void MakeNewBlock();
  void* GetMemoryFallback(const size_t size, const int align);
  void* GetMemory(const size_t size, const int align) {
    assert(remaining_ <= block_size_);          // an invariant
    if ( size > 0 && size < remaining_ && align == 1 ) {       // common case
      last_alloc_ = freestart_;
      freestart_ += size;
      remaining_ -= size;
      return reinterpret_cast<void*>(last_alloc_);
    }
    return GetMemoryFallback(size, align);
  }

  // This doesn't actually free any memory except for the last piece allocated
  void ReturnMemory(void* memory, const size_t size) {
    if ( memory == last_alloc_ && size == freestart_ - last_alloc_ ) {
      remaining_ += size;
      freestart_ = last_alloc_;
    }
  }

  // This is used by Realloc() -- usually we Realloc just by copying to a
  // bigger space, but for the last alloc we can realloc by growing the region.
  bool AdjustLastAlloc(void* last_alloc, const size_t newsize);

  // Since using different alignments for different handles would make
  // the handles incompatible (e.g., we could end up with the same handle
  // value referencing two different allocations, the alignment is not passed
  // as an argument to GetMemoryWithHandle, and handle_alignment_ is used
  // automatically for all GetMemoryWithHandle calls.
  void* GetMemoryWithHandle(const size_t size, Handle* handle);

  Status status_;
  size_t remaining_;

 private:
  struct AllocatedBlock {
    char *mem;
    size_t size;
  };

  // The returned AllocatedBlock* is valid until the next call to AllocNewBlock
  // or Reset (i.e. anything that might affect overflow_blocks_).
  AllocatedBlock *AllocNewBlock(const size_t block_size);

  const AllocatedBlock *IndexToBlock(int index) const;

  const int first_block_we_own_;   // 1 if they pass in 1st block, 0 else
  const size_t block_size_;
  char* freestart_;         // beginning of the free space in most recent block
  char* freestart_when_empty_;  // beginning of the free space when we're empty
  char* last_alloc_;         // used to make sure ReturnBytes() is safe
  // STL vector isn't as efficient as it could be, so we use an array at first
  int blocks_alloced_;       // how many of the first_blocks_ have been alloced
  AllocatedBlock first_blocks_[16];   // the length of this array is arbitrary
  // if the first_blocks_ aren't enough, expand into overflow_blocks_.
  std::vector<AllocatedBlock>* overflow_blocks_;
  const bool page_aligned_;  // when true, all blocks need to be page aligned
  int handle_alignment_;  // Alignment to be used when Handles are requested.
  int handle_alignment_bits_;  // log2(handle_alignment_).
  // The amount of bits required to keep block_size_ (ceil(log2(block_size_))).
  size_t block_size_bits_;

  void FreeBlocks();         // Frees all except first block

  // This subclass needs to alter permissions for all allocated blocks.
  friend class ProtectableUnsafeArena;

  DISALLOW_COPY_AND_ASSIGN(BaseArena);
};

class CTEMPLATE_DLL_DECL UnsafeArena : public BaseArena {
 public:
  // Allocates a thread-compatible arena with the specified block size.
  explicit UnsafeArena(const size_t block_size)
    : BaseArena(NULL, block_size, false) { }
  UnsafeArena(const size_t block_size, bool align)
    : BaseArena(NULL, block_size, align) { }

  // Allocates a thread-compatible arena with the specified block
  // size. "first_block" must have size "block_size". Memory is
  // allocated from "first_block" until it is exhausted; after that
  // memory is allocated by allocating new blocks from the heap.
  UnsafeArena(char* first_block, const size_t block_size)
    : BaseArena(first_block, block_size, false) { }
  UnsafeArena(char* first_block, const size_t block_size, bool align)
    : BaseArena(first_block, block_size, align) { }

  char* Alloc(const size_t size) {
    return reinterpret_cast<char*>(GetMemory(size, 1));
  }
  void* AllocAligned(const size_t size, const int align) {
    return GetMemory(size, align);
  }
  char* Calloc(const size_t size) {
    void* return_value = Alloc(size);
    memset(return_value, 0, size);
    return reinterpret_cast<char*>(return_value);
  }
  void* CallocAligned(const size_t size, const int align) {
    void* return_value = AllocAligned(size, align);
    memset(return_value, 0, size);
    return return_value;
  }
  // Free does nothing except for the last piece allocated.
  void Free(void* memory, size_t size) {
    ReturnMemory(memory, size);
  }
  typedef BaseArena::Handle Handle;
  char* AllocWithHandle(const size_t size, Handle* handle) {
    return reinterpret_cast<char*>(GetMemoryWithHandle(size, handle));
  }
  virtual char* SlowAlloc(size_t size) {  // "slow" 'cause it's virtual
    return Alloc(size);
  }
  virtual void SlowFree(void* memory, size_t size) {  // "slow" 'cause it's virt
    Free(memory, size);
  }
  virtual char* SlowRealloc(char* memory, size_t old_size, size_t new_size) {
    return Realloc(memory, old_size, new_size);
  }
  virtual char* SlowAllocWithHandle(const size_t size, Handle* handle) {
    return AllocWithHandle(size, handle);
  }

  char* Memdup(const char* s, size_t bytes) {
    char* newstr = Alloc(bytes);
    memcpy(newstr, s, bytes);
    return newstr;
  }
  char* MemdupPlusNUL(const char* s, size_t bytes) {  // like "string(s, len)"
    char* newstr = Alloc(bytes+1);
    memcpy(newstr, s, bytes);
    newstr[bytes] = '\0';
    return newstr;
  }
  Handle MemdupWithHandle(const char* s, size_t bytes) {
    Handle handle;
    char* newstr = AllocWithHandle(bytes, &handle);
    memcpy(newstr, s, bytes);
    return handle;
  }
  char* Strdup(const char* s) {
    return Memdup(s, strlen(s) + 1);
  }
  // Unlike libc's strncpy, I always NUL-terminate.  libc's semantics are dumb.
  // This will allocate at most n+1 bytes (+1 is for the NULL terminator).
  char* Strndup(const char* s, size_t n) {
    // Use memchr so we don't walk past n.
    // We can't use the one in //strings since this is the base library,
    // so we have to reinterpret_cast from the libc void *.
    const char* eos = reinterpret_cast<const char*>(memchr(s, '\0', n));
    // if no null terminator found, use full n
    const size_t bytes = (eos == NULL) ? n + 1 : eos - s + 1;
    char* ret = Memdup(s, bytes);
    ret[bytes-1] = '\0';           // make sure the string is NUL-terminated
    return ret;
  }

  // You can realloc a previously-allocated string either bigger or smaller.
  // We can be more efficient if you realloc a string right after you allocate
  // it (eg allocate way-too-much space, fill it, realloc to just-big-enough)
  char* Realloc(char* s, size_t oldsize, size_t newsize);
  // If you know the new size is smaller (or equal), you don't need to know
  // oldsize.  We don't check that newsize is smaller, so you'd better be sure!
  char* Shrink(char* s, size_t newsize) {
    AdjustLastAlloc(s, newsize);       // reclaim space if we can
    return s;                          // never need to move if we go smaller
  }

  // We make a copy so you can keep track of status at a given point in time
  Status status() const { return status_; }

  // Number of bytes remaining before the arena has to allocate another block.
  size_t bytes_until_next_allocation() const { return remaining_; }

 private:
  DISALLOW_COPY_AND_ASSIGN(UnsafeArena);
};



// we inherit from BaseArena instead of UnsafeArena so that we don't need
// virtual methods for allocation/deallocation.  This means, however,
// I have to copy the definitions of strdup, strndup, etc. :-(

class CTEMPLATE_DLL_DECL SafeArena : public BaseArena {
 public:
  // Allocates a thread-safe arena with the specified block size.
  explicit SafeArena(const size_t block_size)
    : BaseArena(NULL, block_size, false) { }

  // Allocates a thread-safe arena with the specified block size.
  // "first_block" must have size "block_size".  Memory is allocated
  // from "first_block" until it is exhausted; after that memory is
  // allocated by allocating new blocks from the heap.
  SafeArena(char* first_block, const size_t block_size)
    : BaseArena(first_block, block_size, false) { }

  virtual void Reset() LOCKS_EXCLUDED(mutex_) {
    MutexLock lock(&mutex_);      // in case two threads Reset() at same time
    BaseArena::Reset();
  }

  char* Alloc(const size_t size) LOCKS_EXCLUDED(mutex_) {
    MutexLock lock(&mutex_);
    return reinterpret_cast<char*>(GetMemory(size, 1));
  }
  void* AllocAligned(const size_t size, const int align)
      LOCKS_EXCLUDED(mutex_) {
    MutexLock lock(&mutex_);
    return GetMemory(size, align);
  }
  char* Calloc(const size_t size) {
    void* return_value = Alloc(size);
    memset(return_value, 0, size);
    return reinterpret_cast<char*>(return_value);
  }
  void* CallocAligned(const size_t size, const int align) {
    void* return_value = AllocAligned(size, align);
    memset(return_value, 0, size);
    return return_value;
  }
  // Free does nothing except for the last piece allocated.
  void Free(void* memory, size_t size) LOCKS_EXCLUDED(mutex_) {
    MutexLock lock(&mutex_);
    ReturnMemory(memory, size);
  }
  typedef BaseArena::Handle Handle;
  char* AllocWithHandle(const size_t size, Handle* handle)
      LOCKS_EXCLUDED(mutex_) {
    MutexLock lock(&mutex_);
    return reinterpret_cast<char*>(GetMemoryWithHandle(size, handle));
  }
  virtual char* SlowAlloc(size_t size) {  // "slow" 'cause it's virtual
    return Alloc(size);
  }
  virtual void SlowFree(void* memory, size_t size) {  // "slow" 'cause it's virt
    Free(memory, size);
  }
  virtual char* SlowRealloc(char* memory, size_t old_size, size_t new_size) {
    return Realloc(memory, old_size, new_size);
  }
  virtual char* SlowAllocWithHandle(const size_t size, Handle* handle) {
    return AllocWithHandle(size, handle);
  }

  char* Memdup(const char* s, size_t bytes) {
    char* newstr = Alloc(bytes);
    memcpy(newstr, s, bytes);
    return newstr;
  }
  char* MemdupPlusNUL(const char* s, size_t bytes) {  // like "string(s, len)"
    char* newstr = Alloc(bytes+1);
    memcpy(newstr, s, bytes);
    newstr[bytes] = '\0';
    return newstr;
  }
  Handle MemdupWithHandle(const char* s, size_t bytes) {
    Handle handle;
    char* newstr = AllocWithHandle(bytes, &handle);
    memcpy(newstr, s, bytes);
    return handle;
  }
  char* Strdup(const char* s) {
    return Memdup(s, strlen(s) + 1);
  }
  // Unlike libc's strncpy, I always NUL-terminate.  libc's semantics are dumb.
  // This will allocate at most n+1 bytes (+1 is for the NULL terminator).
  char* Strndup(const char* s, size_t n) {
    // Use memchr so we don't walk past n.
    // We can't use the one in //strings since this is the base library,
    // so we have to reinterpret_cast from the libc void *.
    const char* eos = reinterpret_cast<const char*>(memchr(s, '\0', n));
    // if no null terminator found, use full n
    const size_t bytes = (eos == NULL) ? n + 1 : eos - s + 1;
    char* ret = Memdup(s, bytes);
    ret[bytes-1] = '\0';           // make sure the string is NUL-terminated
    return ret;
  }

  // You can realloc a previously-allocated string either bigger or smaller.
  // We can be more efficient if you realloc a string right after you allocate
  // it (eg allocate way-too-much space, fill it, realloc to just-big-enough)
  char* Realloc(char* s, size_t oldsize, size_t newsize)
      LOCKS_EXCLUDED(mutex_);
  // If you know the new size is smaller (or equal), you don't need to know
  // oldsize.  We don't check that newsize is smaller, so you'd better be sure!
  char* Shrink(char* s, size_t newsize) LOCKS_EXCLUDED(mutex_) {
    MutexLock lock(&mutex_);
    AdjustLastAlloc(s, newsize);   // reclaim space if we can
    return s;                      // we never need to move if we go smaller
  }

  Status status() LOCKS_EXCLUDED(mutex_) {
    MutexLock lock(&mutex_);
    return status_;
  }

  // Number of bytes remaining before the arena has to allocate another block.
  size_t bytes_until_next_allocation() LOCKS_EXCLUDED(mutex_) {
    MutexLock lock(&mutex_);
    return remaining_;
  }

 protected:
  Mutex mutex_;

 private:
  DISALLOW_COPY_AND_ASSIGN(SafeArena);
};

}

#endif  // BASE_ARENA_H_
