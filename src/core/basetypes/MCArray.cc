#include "MCArray.h"

#include <string.h>
#include <stdlib.h>
#include <libetpan/libetpan.h>

#include "MCAssert.h"
#include "MCString.h"
#include "MCLog.h"
#include "MCUtils.h"

using namespace mailcore;

Array::Array()
{
    init();
}

Array::Array(Array * other) : Object()
{
    init();
    for(unsigned int i = 0 ; i < other->count() ; i ++) {
        Object * obj = other->objectAtIndex(i);
        addObject(obj);
    }
}

void Array::init()
{
    mArray = carray_new(4);
}

Array::~Array()
{
    removeAllObjects();
    carray_free(mArray);
}

Array * Array::array()
{
    Array * result = new Array();
    return (Array *) result->autorelease();
}

Array * Array::arrayWithObject(Object * obj)
{
    Array * result = new Array();
    result->addObject(obj);
    return (Array *) result->autorelease();
}

String * Array::description()
{
    String * result = String::string();
    
    result->appendUTF8Characters("[");
    for(unsigned int i = 0 ; i < count() ; i ++) {
        if (i != 0) {
            result->appendUTF8Characters(",");
        }
        Object * obj = objectAtIndex(i);
        result->appendString(obj->description());
    }    
    result->appendUTF8Characters("]");
    
    return result;
}

Object * Array::copy()
{
    return new Array(this);
}

unsigned int Array::count()
{
    return carray_count(mArray);
}

void Array::addObject(Object * obj)
{
    unsigned int idx;
    obj->retain();
    carray_add(mArray, obj, &idx);
}

void Array::removeObjectAtIndex(unsigned int idx)
{
    Object * obj = (Object *) carray_get(mArray, idx);
    obj->release();
    carray_delete_slow(mArray, idx);
}

void Array::removeObject(Object * obj)
{
    int idx = indexOfObject(obj);
    if (idx == -1)
        return;
    
    removeObjectAtIndex(idx);
}

int Array::indexOfObject(Object * obj)
{
    for(unsigned int i = 0 ; i < count() ; i ++) {
        Object * currentObj = objectAtIndex(i);
        if (currentObj->isEqual(obj)) {
            return i;
        }
    }
    
    return -1;
}

Object * Array::objectAtIndex(unsigned int idx)
{
    Object * obj = (Object *) carray_get(mArray, idx);
    return obj;
}

void Array::replaceObject(unsigned int idx, Object * obj)
{
    if (idx < count()) {
        Object * previousObject = (Object *) carray_get(mArray, idx);
        previousObject->release();
        obj->retain();
        carray_set(mArray, idx, obj);
    }
    else if (idx == count()) {
        addObject(obj);
    }
    else {
        MCAssert(0);
    }
}

void Array::insertObject(unsigned int idx, Object * obj)
{
    if (idx < count()) {
        int count = carray_count(mArray) - idx;
        carray_set_size(mArray, count + 1);
        void ** p = carray_data(mArray);
        memmove(p + idx + 1, p + idx, count * sizeof(* p));
        obj->retain();
        carray_set(mArray, idx, obj);
    }
    else if (idx == count()) {
        addObject(obj);
    }
    else {
        MCAssert(0);
    }
}

void Array::removeAllObjects()
{
    for(unsigned int i = 0 ; i < count() ; i ++) {
        Object * obj = objectAtIndex(i);
        obj->release();
    }
    carray_set_size(mArray, 0);
}

void Array::addObjectsFromArray(Array * array)
{
    if (array == NULL)
        return;
    
    for(unsigned int i = 0 ; i < array->count() ; i ++) {
        Object * obj = array->objectAtIndex(i);
        addObject(obj);
    }
}

Object * Array::lastObject()
{
    if (count() == 0)
        return NULL;
    
    return objectAtIndex(count() - 1);
}

bool Array::containsObject(Object * obj)
{
    return (indexOfObject(obj) != -1);
}

struct sortData {
    int (* compare)(void *, void *, void *);
    void * context;
};

#ifdef __MACH__
static int sortCompare(struct sortData * data, Object ** pa, Object ** pb)
#else
static int sortCompare(Object ** pa, Object ** pb, struct sortData * data)
#endif
{
  Object * a;
  Object * b;
  
  a = * pa;
  b = * pb;
  
  return data->compare(a, b, data->context);
}

Array * Array::sortedArray(int (* compare)(void *, void *, void *), void * context)
{
    struct sortData data;
    Array * result = (Array *) this->copy()->autorelease();
    data.compare = compare;
    data.context = context;
#ifdef __MACH__
    qsort_r(carray_data(result->mArray), carray_count(result->mArray),
        sizeof(* carray_data(result->mArray)), this,
        (int (*)(void *, const void *, const void *)) sortCompare);
#else
    qsort_r(carray_data(result->mArray), carray_count(result->mArray),
        sizeof(* carray_data(result->mArray)),
        (int (*)(const void *, const void *, void *)) sortCompare,
        this);
#endif
    return result;
}

String * Array::componentsJoinedByString(String * delimiter)
{
    String * result = String::string();
    for(unsigned int i = 0 ; i < count() ; i ++) {
        Object * obj = objectAtIndex(i);
        if (result != 0) {
            result->appendString(delimiter);
        }
        result->appendString(obj->description());
    }
    return result;
}
