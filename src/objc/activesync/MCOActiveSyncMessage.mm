#import "MCOActiveSyncPrivate.h"

@implementation MCOActiveSyncMessage

#define nativeType mailcore::ActiveSyncMessage

+ (void) load
{
    MCORegisterClass(self, &typeid(nativeType));
}

- (instancetype) init
{
    nativeType * message = new nativeType();
    self = [self initWithMCMessage:message];
    message->release();
    return self;
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCMessage:(nativeType *) object] autorelease];
}

- (id) copyWithZone:(NSZone *)zone
{
    nativeType * nativeObject = (nativeType *) [self mco_mcObject]->copy();
    id result = [[self class] mco_objectWithMCObject:nativeObject];
    MC_SAFE_RELEASE(nativeObject);
    return [result retain];
}

MCO_OBJC_SYNTHESIZE_STRING(setServerID, serverID)
MCO_OBJC_SYNTHESIZE_STRING(setMessageClass, messageClass)
MCO_OBJC_SYNTHESIZE_SCALAR(uint32_t, uint32_t, setEstimatedSize, estimatedSize)
MCO_OBJC_SYNTHESIZE_BOOL(setRead, isRead)
MCO_OBJC_SYNTHESIZE_BOOL(setFlagged, isFlagged)
MCO_OBJC_SYNTHESIZE_DATA(setMIMEData, MIMEData)
MCO_OBJC_SYNTHESIZE(ActiveSyncBody, setBody, body)

@end

#undef nativeType
