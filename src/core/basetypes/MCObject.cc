#include "MCObject.h"

#include <stdlib.h>
#include <typeinfo>
#include <cxxabi.h>
#include <libetpan/libetpan.h>
#include <string.h>

#include "MCAutoreleasePool.h"
#include "MCString.h"
#include "MCHash.h"
#include "MCLog.h"
#include "MCUtils.h"
#include "MCAssert.h"
#include "MCMainThread.h"
#include "MCLog.h"
#include "MCHashMap.h"

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
        //int status;
        //char * unmangled = abi::__cxa_demangle(typeid(* this).name(), NULL, NULL, &status);
        //MCLog("dealloc %p %s", this, unmangled);
        delete this;
    }
}

#ifndef __APPLE__
Object * Object::autorelease()
{
    AutoreleasePool::autorelease(this);
    return this;
}
#endif

String * Object::className()
{
    int status;
    char * unmangled = abi::__cxa_demangle(typeid(* this).name(), NULL, NULL, &status);
    String * result = String::uniquedStringWithUTF8Characters(unmangled);
    free(unmangled);
    return result;
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
    void * caller;
};

static pthread_once_t delayedPerformOnce = PTHREAD_ONCE_INIT;
static chash * delayedPerformHash = NULL;

static void reallyInitDelayedPerform()
{
    delayedPerformHash = chash_new(CHASH_DEFAULTSIZE, CHASH_COPYKEY);
}

static void initDelayedPerform()
{
    pthread_once(&delayedPerformOnce, reallyInitDelayedPerform);
}

struct mainThreadCallKeyData {
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

static void performAfterDelay(void * info)
{
    struct mainThreadCallData * data;
    void * context;
    Object * obj;
    Object::Method method;
    
    data = (struct mainThreadCallData *) info;
    obj = data->obj;
    context = data->context;
    method = data->method;
    
    chashdatum key;
    struct mainThreadCallKeyData keyData;
    keyData.obj = obj;
    keyData.context = context;
    keyData.method = method;
    key.data = &keyData;
    key.len = sizeof(keyData);
    chash_delete(delayedPerformHash, &key, NULL);
    
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
    data->caller = NULL;
    
    if (waitUntilDone) {
        callOnMainThreadAndWait(performOnMainThread, data);
    }
    else {
        callOnMainThread(performOnMainThread, data);
    }
}

void Object::performMethodAfterDelay(Method method, void * context, double delay)
{
    initDelayedPerform();
    
    struct mainThreadCallData * data;
    
    data = (struct mainThreadCallData *) calloc(sizeof(* data), 1);
    data->obj = this;
    data->context = context;
    data->method = method;
    data->caller = callAfterDelay(performAfterDelay, data, delay);
    
    chashdatum key;
    chashdatum value;
    struct mainThreadCallKeyData keyData;
    keyData.obj = this;
    keyData.context = context;
    keyData.method = method;
    key.data = &keyData;
    key.len = sizeof(keyData);
    value.data = (void *) data;
    value.len = 0;
    chash_set(delayedPerformHash, &key, &value, NULL);
}

void Object::cancelDelayedPerformMethod(Method method, void * context)
{
    initDelayedPerform();
    
    int r;
    chashdatum key;
    chashdatum value;
    struct mainThreadCallKeyData keyData;
    keyData.obj = this;
    keyData.context = context;
    keyData.method = method;
    key.data = &keyData;
    key.len = sizeof(keyData);
    r = chash_get(delayedPerformHash, &key, &value);
    if (r < 0)
        return;
    
    chash_delete(delayedPerformHash, &key, NULL);
    struct mainThreadCallData * data = (struct mainThreadCallData *) value.data;
    cancelDelayedCall(data->caller);
    free(data);
}

HashMap * Object::serializable()
{
    HashMap * result = HashMap::hashMap();
    result->setObjectForKey(MCSTR("class"), className());
    return result;
}

void Object::importSerializable(HashMap * serializable)
{
    MCAssert(0);
}

static chash * constructors = NULL;

void Object::initObjectConstructors()
{
    constructors = chash_new(CHASH_DEFAULTSIZE, CHASH_COPYKEY);
}

void Object::registerObjectConstructor(const char * className, void * (* objectConstructor)(void))
{
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    pthread_once(&once, initObjectConstructors);
    
    chashdatum key;
    chashdatum value;
    key.data = (void *) className;
    key.len = strlen(className);
    value.data = (void *) objectConstructor;
    value.len = 0;
    chash_set(constructors, &key, &value, NULL);
}

Object * Object::objectWithSerializable(HashMap * serializable)
{
    if (serializable == NULL)
        return NULL;
    
    void * (* objectConstructor)(void) = NULL;
    
    chashdatum key;
    chashdatum value;
    const char * className = ((String *) serializable->objectForKey(MCSTR("class")))->UTF8Characters();
    key.data = (void *) className;
    key.len = strlen(className);
    int r = chash_get(constructors, &key, &value);
    if (r < 0)
        return NULL;
    
    objectConstructor = (void * (*)()) value.data;
    Object * obj = (Object *) objectConstructor();
    obj->importSerializable(serializable);
    return obj->autorelease();
}

