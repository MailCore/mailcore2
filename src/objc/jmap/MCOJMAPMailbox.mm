//
//  MCOJMAPMailbox.mm
//  mailcore2
//

#import "MCOJMAPMailbox.h"

#include "MCJMAP.h"
#import "MCOUtils.h"

@implementation MCOJMAPMailbox {
    mailcore::JMAPMailbox * _nativeMailbox;
}

#define nativeType mailcore::JMAPMailbox

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
    return [[[self alloc] initWithMCMailbox:(nativeType *) object] autorelease];
}

- (instancetype) init
{
    nativeType * mailbox = new nativeType();
    self = [self initWithMCMailbox:mailbox];
    mailbox->release();
    return self;
}

- (instancetype) initWithMCMailbox:(nativeType *)mailbox
{
    self = [super init];
    _nativeMailbox = mailbox;
    _nativeMailbox->retain();
    return self;
}

- (void) dealloc
{
    MC_SAFE_RELEASE(_nativeMailbox);
    [super dealloc];
}

- (mailcore::Object *) mco_mcObject
{
    return _nativeMailbox;
}

MCO_OBJC_SYNTHESIZE_STRING(setIdentifier, identifier)
MCO_OBJC_SYNTHESIZE_STRING(setName, name)
MCO_OBJC_SYNTHESIZE_STRING(setParentID, parentID)
MCO_OBJC_SYNTHESIZE_STRING(setRole, role)
MCO_OBJC_SYNTHESIZE_SCALAR(int, int, setSortOrder, sortOrder)
MCO_OBJC_SYNTHESIZE_BOOL(setSubscribed, isSubscribed)
MCO_OBJC_SYNTHESIZE_SCALAR(int, int, setTotalEmails, totalEmails)
MCO_OBJC_SYNTHESIZE_SCALAR(int, int, setUnreadEmails, unreadEmails)
MCO_OBJC_SYNTHESIZE_SCALAR(int, int, setTotalThreads, totalThreads)
MCO_OBJC_SYNTHESIZE_SCALAR(int, int, setUnreadThreads, unreadThreads)
MCO_OBJC_SYNTHESIZE_HASHMAP(setRights, rights)

@end
