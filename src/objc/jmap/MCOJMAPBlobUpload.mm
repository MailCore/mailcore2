//
//  MCOJMAPBlobUpload.mm
//  mailcore2
//

#import "MCOJMAPBlobUpload.h"

#include "MCJMAP.h"
#import "MCOUtils.h"

@implementation MCOJMAPBlobUpload {
    mailcore::JMAPBlobUpload * _nativeUpload;
}

#define nativeType mailcore::JMAPBlobUpload

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
    return [[[self alloc] initWithMCUpload:(nativeType *) object] autorelease];
}

- (instancetype) init
{
    nativeType * upload = new nativeType();
    self = [self initWithMCUpload:upload];
    upload->release();
    return self;
}

- (instancetype) initWithMCUpload:(nativeType *)upload
{
    self = [super init];
    _nativeUpload = upload;
    _nativeUpload->retain();
    return self;
}

- (void) dealloc
{
    MC_SAFE_RELEASE(_nativeUpload);
    [super dealloc];
}

- (mailcore::Object *) mco_mcObject
{
    return _nativeUpload;
}

MCO_OBJC_SYNTHESIZE_STRING(setAccountID, accountID)
MCO_OBJC_SYNTHESIZE_STRING(setBlobID, blobID)
MCO_OBJC_SYNTHESIZE_STRING(setType, type)
MCO_OBJC_SYNTHESIZE_STRING(setName, name)
MCO_OBJC_SYNTHESIZE_SCALAR(NSUInteger, size_t, setSize, size)

@end
