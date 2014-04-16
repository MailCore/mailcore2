//
//  MCIterator.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 4/18/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef MAILCORE_ITERATOR_H

#define MAILCORE_ITERATOR_H

#include <MailCore/MCArray.h>
#include <MailCore/MCHashMap.h>
#include <MailCore/MCAutoreleasePool.h>
#include <string.h>

#ifdef __cplusplus

#define mc_foreacharray(type, __variable, __array) \
type * __variable; \
mailcore::ArrayIterator __variable##__iterator = mailcore::ArrayIteratorInit(__array); \
for (; NULL != (__variable = (type *) mailcore::ArrayIteratorNext(&__variable##__iterator)); )

#define mc_foreacharrayIndex(__index, type, __variable, __array) \
type * __variable; \
mailcore::ArrayIterator __variable##__iterator = mailcore::ArrayIteratorInit(__array); \
for (unsigned int __index = 0; NULL != (__variable = (type *) mailcore::ArrayIteratorNext(&__variable##__iterator)); __index++)

#define mc_foreachhashmapKey(keyType, __key, __hashmap) \
keyType * __key; \
HashMapIterator __key##__iterator = HashMapIteratorInit(__hashmap, true, false); \
while (HashMapIteratorRun(&__key##__iterator)) \
while (HashMapIteratorNext(&__key##__iterator, &__key, NULL))

#define mc_foreachhashmapValue(valueType, __value, __hashmap) \
valueType * __value; \
HashMapIterator __value##__iterator = HashMapIteratorInit(__hashmap, false, true); \
while (HashMapIteratorRun(&__value##__iterator)) \
while (HashMapIteratorNext(&__value##__iterator, NULL, (Object **) &__value))

#define mc_foreachhashmapKeyAndValue(keyType, __key, valueType, __value, __hashmap) \
keyType * __key; \
valueType * __value; \
HashMapIterator __key##__value##__iterator = HashMapIteratorInit(__hashmap, true, true); \
while (HashMapIteratorRun(&__key##__value##__iterator)) \
while (HashMapIteratorNext(&__key##__value##__iterator, (Object **) &__key, (Object **) &__value))

namespace mailcore {
    
    struct ArrayIterator {
        unsigned index;
        unsigned count;
        Array * array;
    };
    
    static inline ArrayIterator ArrayIteratorInit(Array * array)
    {
        ArrayIterator iterator = { 0, array != NULL ? array->count() : 0, array };
        return iterator;
    }
    
    static inline Object * ArrayIteratorNext(ArrayIterator * iterator)
    {
        if (iterator->index >= iterator->count) {
            return NULL;
        }
        
        Object * result = iterator->array->objectAtIndex(iterator->index);
        ++ iterator->index;
        return result;
    }
    
    
    struct HashMapIterator {
        bool cleanup;
        unsigned index;
        unsigned count;
        Array * keys;
        Array * values;
    };
    
    static inline HashMapIterator HashMapIteratorInit(HashMap * hashmap, bool useKeys, bool useValues)
    {
        AutoreleasePool * pool = new AutoreleasePool();
        Array * keys =  useKeys ? (hashmap != NULL ? hashmap->allKeys() : NULL) : NULL;
        Array * values = useValues ? (hashmap != NULL ? hashmap->allValues() : NULL) : NULL;
        if (keys != NULL) {
            keys->retain();
        }
        if (values != NULL) {
            values->retain();
        }
        HashMapIterator iterator = { false, 0, hashmap != NULL ? hashmap->count() : 0, keys, values };
        pool->release();
        
        return iterator;
    }
    
    static inline bool HashMapIteratorNext(HashMapIterator * iterator, Object ** keyp, Object ** valuep)
    {
        if (iterator->index >= iterator->count) {
            return false;
        }
        
        if (keyp != NULL) {
            * keyp = iterator->keys->objectAtIndex(iterator->index);
        }
        if (valuep != NULL) {
            * valuep = iterator->values->objectAtIndex(iterator->index);
        }
        iterator->index ++;
        return true;
    }
    
    
    static inline bool HashMapIteratorRun(HashMapIterator * iterator)
    {
        if (!iterator->cleanup) {
            iterator->cleanup = true;
            return true;
        } else {
            MC_SAFE_RELEASE(iterator->keys);
            MC_SAFE_RELEASE(iterator->values);
            return false;
        }
    }
    
};

#endif

#endif
