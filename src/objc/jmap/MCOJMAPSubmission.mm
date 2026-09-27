//
//  MCOJMAPSubmission.mm
//  mailcore2
//

#import "MCOJMAPSubmission.h"

#include "MCJMAP.h"
#import "MCOUtils.h"

@implementation MCOJMAPSubmission {
    mailcore::JMAPSubmission * _nativeSubmission;
}

#define nativeType mailcore::JMAPSubmission

+ (void) load
{
    MCORegisterClass(self, &typeid(nativeType));
}

- (id) copyWithZone:(NSZone *)zone
{
    nativeType * nativeObject = (nativeType *) [self mco_mcObject]->copy();
    id result = [[self class] mco_objectWithMCObject:nativeObject];
    MC_SAFE_RELEASE(nativeObject);
    return [result retain];
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCSubmission:(nativeType *) object] autorelease];
}

- (instancetype) init
{
    nativeType * submission = new nativeType();
    self = [self initWithMCSubmission:submission];
    submission->release();
    return self;
}

- (instancetype) initWithMCSubmission:(nativeType *)submission
{
    self = [super init];
    _nativeSubmission = submission;
    _nativeSubmission->retain();
    return self;
}

- (void) dealloc
{
    MC_SAFE_RELEASE(_nativeSubmission);
    [super dealloc];
}

- (mailcore::Object *) mco_mcObject
{
    return _nativeSubmission;
}

MCO_OBJC_SYNTHESIZE_STRING(setIdentifier, identifier)
MCO_OBJC_SYNTHESIZE_STRING(setEmailID, emailID)
MCO_OBJC_SYNTHESIZE_STRING(setThreadID, threadID)
MCO_OBJC_SYNTHESIZE_STRING(setIdentityID, identityID)
MCO_OBJC_SYNTHESIZE_STRING(setSendAt, sendAt)
MCO_OBJC_SYNTHESIZE_STRING(setUndoStatus, undoStatus)
MCO_OBJC_SYNTHESIZE_HASHMAP(setDeliveryStatus, deliveryStatus)

@end
