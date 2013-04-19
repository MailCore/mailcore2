//
//  MCIterator.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 4/18/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_ITERATOR_H_
#define __MAILCORE_ITERATOR_H_

#include <MailCore/MCArray.h>
#include <MailCore/MCHashMap.h>
#include <MailCore/MCAutoreleasePool.h>
#include <string.h>

#define mc_foreacharray(type, __variable, __array) \
type * __variable; \
mailcore::ArrayIterator __variable##__iterator = mailcore::ArrayIteratorInit(__array); \
for (; NULL != (__variable = (type *) mailcore::ArrayIteratorNext(&__variable##__iterator)); )

#define mc_foreacharrayIndex(__index, type, __variable, __array) \
type * __variable; \
mailcore::ArrayIterator __variable##__iterator = mailcore::ArrayIteratorInit(__array); \
for (unsigned int __index = 0; NULL != (__variable = mailcore::ArrayIteratorNext(&__variable##__iterator)); __index++)

#define mc_foreachdictionaryKey(keyType, __key, __dictionary) \
keyType * __key; \
DictionaryIterator __key##__iterator = DictionaryIteratorInit(__dictionary, true, false); \
while (DictionaryIteratorRun(&__key##__iterator)) \
while (DictionaryIteratorNext(&__key##__iterator, &__key, NULL))

#define mc_foreachdictionaryValue(valueType, __value, __dictionary) \
valueType * __value; \
DictionaryIterator __value##__iterator = DictionaryIteratorInit(__dictionary, false, true); \
while (DictionaryIteratorRun(&__value##__iterator)) \
while (DictionaryIteratorNext(&__value##__iterator, NULL, &__value))

#define mc_foreachdictionaryKeyAndValue(__key, __value, __dictionary) \
keyType * __key; \
valueType * __value; \
DictionaryIterator __key##__value##__iterator = DictionaryIteratorInit(__dictionary, true, true); \
while (DictionaryIteratorRun(&__key##__value##__iterator)) \
while (DictionaryIteratorNext(&__key##__value##__iterator, &__key, &__value))

namespace mailcore {
    
    struct ArrayIterator {
        unsigned index;
        unsigned count;
        Array * array;
    };
    
    static inline ArrayIterator ArrayIteratorInit(Array * array)
    {
        ArrayIterator iterator = { 0, array->count(), array };
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
    
    static inline HashMapIterator FastDictionaryIteratorInit(HashMap * hashmap, bool useKeys, bool useValues)
    {
        AutoreleasePool * pool = new AutoreleasePool();
        Array * keys =  useKeys ? hashmap->allKeys() : NULL;
        Array * values = useValues ? hashmap->allValues() : NULL;
        keys->retain();
        values->retain();
        HashMapIterator iterator = { false, 0, hashmap->count(), keys, values };
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
    
    
    static inline bool DictionaryIteratorRun(HashMapIterator * iterator)
    {
        if (iterator->cleanup) {
            iterator->cleanup = true;
            return true;
        } else {
            iterator->keys->release();
            iterator->values->release();
            return false;
        }
    }

};

#endif
