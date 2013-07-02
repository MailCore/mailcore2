#include "MCHashMap.h"

#include <stdlib.h>
#include <string.h>

#include "MCArray.h"
#include "MCString.h"
#include "MCUtils.h"
#include "MCLog.h"

using namespace mailcore;

namespace mailcore {
    struct HashMapCell {
        unsigned int func;
        Object * key;
        Object * value;
        HashMapCell * next;
    };
}

#define CHASH_DEFAULTSIZE 13
#define CHASH_MAXDEPTH    3

void HashMap::init()
{
    mCount = 0;
    mCells = (void **) (HashMapCell **) calloc(CHASH_DEFAULTSIZE, sizeof(HashMapCell *));
    mAllocated = CHASH_DEFAULTSIZE;
}

HashMap::HashMap()
{
    init();
}

HashMap::HashMap(HashMap * other)
{
    init();
    Array * keys = other->allKeys();
    for(unsigned int i = 0 ; i < keys->count() ; i ++) {
        Object * key = keys->objectAtIndex(i);
        Object * value = other->objectForKey(key);
        setObjectForKey(key, value);
    }
}

HashMap::~HashMap()
{
    for(unsigned int indx = 0; indx < mAllocated; indx++) {
        HashMapIter * iter, * next;
        iter = (HashMapIter *) mCells[indx];
        while (iter) {
            next = iter->next;
            iter->key->release();
            iter->value->release();
            free(iter);
            iter = next;
        }
    }
    free(mCells);
}

void HashMap::allocate(unsigned int size)
{
    HashMapCell ** cells;
    unsigned int indx, nindx;
    HashMapIter * iter, * next;

    if (mAllocated == size)
      return;

    cells = (HashMapCell **) calloc(size, sizeof(HashMapCell *));
    /* iterate over initial hash and copy items in second hash */
    for(indx = 0 ; indx < mAllocated ; indx ++) {
      iter = (HashMapIter *) mCells[indx];
      while (iter) {
        next = iter->next;
        nindx = iter->func % size;
        iter->next = cells[nindx];
        cells[nindx] = iter;
        iter = next;
      }
    }
    free(mCells);
    mAllocated = size;
    mCells = (void **) cells;
}

HashMap * HashMap::hashMap()
{
    HashMap * result = new HashMap();
    return (HashMap *) result->autorelease();
}

String * HashMap::description()
{
    String * result = String::string();
    Array * keys = allKeys();
    result->appendUTF8Characters("{");
    for(unsigned int i = 0 ; i < keys->count() ; i ++) {
        Object * key = keys->objectAtIndex(i);
        if (i != 0) {
            result->appendUTF8Characters(",");
        }
        result->appendString(key->description());
        result->appendUTF8Characters(":");
        Object * value = objectForKey(key);
        result->appendString(value->description());
    }
    result->appendUTF8Characters("}");
    
    return result;
}

Object * HashMap::copy()
{
    return new HashMap(this);
}

unsigned int HashMap::count()
{
    return mCount;
}

void HashMap::setObjectForKey(Object * key, Object * value)
{
    unsigned int func, indx;
    HashMapIter * iter, * cell;

    if (mCount > mAllocated * CHASH_MAXDEPTH) {
        allocate((mCount / CHASH_MAXDEPTH) * 2 + 1);
    }

    func = key->hash();
    indx = func % mAllocated;

     /* look for the key in existing cells */
    iter = (HashMapIter *) mCells[indx];
    while (iter) {
        if (iter->func == func && iter->key->isEqual(key)) {
         /* found, replacing entry */
            value->retain();
            iter->value->release();
            iter->value = value;
            return;
        }
        iter = iter->next;
    }

     /* not found, adding entry */
    cell = (HashMapCell *) malloc(sizeof(HashMapCell));
    cell->key = key->copy();
    cell->value = value->retain();
    cell->func = func;
    cell->next = (HashMapCell *) mCells[indx];
    mCells[indx] = cell;
    mCount ++;
}

void HashMap::removeObjectForKey(Object * key)
{
    unsigned int func, indx;
    HashMapIter * iter, * old;

    func = key->hash();;
    indx = func % mAllocated;

  /* look for the key in existing cells */
    old = NULL;
    iter = (HashMapIter *) mCells[indx];
    while (iter) {
        if (iter->func == func && iter->key->isEqual(key)) {
            /* found, deleting */
            if (old)
                old->next = iter->next;
            else
                mCells[indx] = iter->next;
            iter->key->release();
            iter->value->release();
            free(iter);
            mCount --;
            return;
        }
        old = iter;
        iter = iter->next;
    }
    // Not found.
}

Object * HashMap::objectForKey(Object * key)
{
    unsigned int func;
    HashMapIter * iter;

    func = key->hash();

    /* look for the key in existing cells */
    iter = (HashMapIter *) mCells[func % mAllocated];
    while (iter) {
        if (iter->func == func && key->isEqual(iter->key)) {
            return iter->value; /* found */
        }
        iter = iter->next;
    }
    return NULL;
}

HashMapIter * HashMap::iteratorBegin()
{
  HashMapIter * iter;
  unsigned int indx = 0;
  
  iter = (HashMapIter *) mCells[0];
  while (!iter) {
    indx ++;
    if (indx >= mAllocated)
      return NULL;
    iter = (HashMapIter *) mCells[indx];
  }
  return iter;
}

HashMapIter * HashMap::iteratorNext(HashMapIter * iter)
{
  unsigned int indx;

  if (!iter)
    return NULL;

  indx = iter->func % mAllocated;
  iter = iter->next;

  while(!iter) {
    indx++;
    if (indx >= mAllocated)
      return NULL;
    iter = (HashMapIter *) mCells[indx];
  }
  return iter;
}

Array * HashMap::allKeys()
{
    Array * keys = Array::array();
    for(HashMapIter * iter = iteratorBegin() ; iter != NULL ; iter = iteratorNext(iter)) {
        keys->addObject(iter->key);
    }
    return keys;
}

Array * HashMap::allValues()
{
    Array * values = Array::array();
    for(HashMapIter * iter = iteratorBegin() ; iter != NULL ; iter = iteratorNext(iter)) {
        values->addObject(iter->value);
    }
    return values;
}

void HashMap::removeAllObjects()
{
    for(unsigned int indx = 0 ; indx < mAllocated ; indx++) {
        HashMapIter * iter, * next;
        iter = (HashMapIter *) mCells[indx];
        while (iter) {
            next = iter->next;
            iter->key->release();
            iter->value->release();
            free(iter);
            iter = next;
        }
    }
    memset(mCells, 0, mAllocated * sizeof(* mCells));
    mCount = 0;
}
