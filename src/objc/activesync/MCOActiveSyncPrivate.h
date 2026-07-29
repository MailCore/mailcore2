#ifndef MAILCORE_MCOACTIVESYNCPRIVATE_H

#define MAILCORE_MCOACTIVESYNCPRIVATE_H

#import "MCOActiveSync.h"

#import "MCOUtils.h"

#include "MCActiveSync.h"

#define MCO_DEFINE_ACTIVE_SYNC_WRAPPER(className, nativeClass) \
@implementation className { \
    nativeClass * _nativeObject; \
} \
+ (void) load \
{ \
    MCORegisterClass(self, &typeid(nativeClass)); \
} \
- (instancetype) init \
{ \
    nativeClass * object = new nativeClass(); \
    self = [self initWithMCObject:object]; \
    object->release(); \
    return self; \
} \
- (instancetype) initWithMCObject:(nativeClass *)object \
{ \
    self = [super init]; \
    _nativeObject = object; \
    _nativeObject->retain(); \
    return self; \
} \
- (void) dealloc \
{ \
    MC_SAFE_RELEASE(_nativeObject); \
    [super dealloc]; \
} \
- (mailcore::Object *) mco_mcObject \
{ \
    return _nativeObject; \
} \
+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object \
{ \
    return [[[self alloc] initWithMCObject:(nativeClass *) object] autorelease]; \
} \
- (id) copyWithZone:(NSZone *)zone \
{ \
    nativeClass * nativeObject = (nativeClass *) _nativeObject->copy(); \
    id result = [[self class] mco_objectWithMCObject:nativeObject]; \
    MC_SAFE_RELEASE(nativeObject); \
    return [result retain]; \
}

#define MCO_END_ACTIVE_SYNC_WRAPPER @end

#endif
