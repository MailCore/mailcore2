#import "MCOActiveSyncPrivate.h"

@implementation MCOActiveSyncAttachment

#define nativeType mailcore::ActiveSyncAttachment

+ (void) load
{
    MCORegisterClass(self, &typeid(nativeType));
}

- (instancetype) init
{
    nativeType * attachment = new nativeType();
    self = [self initWithMCPart:attachment];
    attachment->release();
    return self;
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCPart:(nativeType *) object] autorelease];
}

- (id) copyWithZone:(NSZone *)zone
{
    nativeType * nativeObject = (nativeType *) [self mco_mcObject]->copy();
    id result = [[self class] mco_objectWithMCObject:nativeObject];
    MC_SAFE_RELEASE(nativeObject);
    return [result retain];
}

MCO_OBJC_SYNTHESIZE_SCALAR(MCOActiveSyncAttachmentMethod, mailcore::ActiveSyncAttachmentMethod, setMethod, method)
MCO_OBJC_SYNTHESIZE_SCALAR(uint32_t, uint32_t, setEstimatedDataSize, estimatedDataSize)

@end

#undef nativeType
