#include "MCObject.h"

#include <stdlib.h>
#include <typeinfo>
#include <cxxabi.h>

#include "MCAutoreleasePool.h"
#include "MCString.h"
#include "MCHash.h"
#include "MCLog.h"
#include "MCUtils.h"
#include "MCAssert.h"
#include "MCMainThread.h"
#include "MCLog.h"

using namespace mailcore;

bool mailcore::zombieEnabled = 0;

Object::Object()
{
    init();
}

Object::~Object()
{
}

void Object::init()
{
    pthread_mutex_init(&mLock, NULL);
    mCounter = 1;
}

int Object::retainCount()
{
    pthread_mutex_lock(&mLock);
    int value = mCounter;
    pthread_mutex_unlock(&mLock);
    
    return value;
}

Object * Object::retain()
{
    pthread_mutex_lock(&mLock);
    mCounter ++;
    pthread_mutex_unlock(&mLock);
    return this;
}

void Object::release()
{
    bool shouldRelease = false;
    
    pthread_mutex_lock(&mLock);
    mCounter --;
    if (mCounter == 0) {
        shouldRelease = true;
    }
    if (mCounter < 0) {
        MCLog("release too much %p %s", this, MCUTF8(className()));
        MCAssert(0);
    }
    pthread_mutex_unlock(&mLock);
    
    if (shouldRelease && !zombieEnabled) {
        //MCLog("dealloc %s", className()->description()->UTF8Characters());
        delete this;
    }
}

Object * Object::autorelease()
{
    AutoreleasePool::autorelease(this);
    return this;
}

String * Object::className()
{
    int status;
    char * unmangled = abi::__cxa_demangle(typeid(* this).name(), NULL, NULL, &status);
	return mailcore::String::uniquedStringWithUTF8Characters(unmangled);
}

String * Object::description()
{
    return String::stringWithUTF8Format("<%s:%p>", className()->UTF8Characters(), this);
}

bool Object::isEqual(Object * otherObject)
{
    return this == otherObject;
}

unsigned int Object::hash()
{
    return hashCompute((const char *) this, sizeof(this));
}

Object * Object::copy()
{
    MCAssert(0);
    return NULL;
}

void Object::performMethod(Object::Method method, void * context)
{
    (this->*method)(context);
}

struct mainThreadCallData {
    Object * obj;
    void * context;
    Object::Method method;
};

static void performOnMainThread(void * info)
{
    struct mainThreadCallData * data;
    void * context;
    Object * obj;
    Object::Method method;
    
    data = (struct mainThreadCallData *) info;
    obj = data->obj;
    context = data->context;
    method = data->method;
    
    (obj->*method)(context);
    
    free(data);
}

static void callAfterDelay(void * info)
{
    struct mainThreadCallData * data;
    void * context;
    Object * obj;
    Object::Method method;
    
    data = (struct mainThreadCallData *) info;
    obj = data->obj;
    context = data->context;
    method = data->method;
    
    (obj->*method)(context);
    
    free(data);
}

void Object::performMethodOnMainThread(Method method, void * context, bool waitUntilDone)
{
    struct mainThreadCallData * data;
    
    data = (struct mainThreadCallData *) calloc(sizeof(* data), 1);
    data->obj = this;
    data->context = context;
    data->method = method;
    
    if (waitUntilDone) {
        callOnMainThreadAndWait(performOnMainThread, data);
    }
    else {
        callOnMainThread(performOnMainThread, data);
    }
}

void Object::performMethodAfterDelay(Method method, void * context, double delay)
{
    struct mainThreadCallData * data;
    
    data = (struct mainThreadCallData *) calloc(sizeof(* data), 1);
    data->obj = this;
    data->context = context;
    data->method = method;
    
    callAfterDelay(performOnMainThread, data, delay);
}
