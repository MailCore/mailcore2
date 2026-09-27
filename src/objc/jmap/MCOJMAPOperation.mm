//
//  MCOJMAPOperation.mm
//  mailcore2
//

#import "MCOJMAPOperation.h"

#include "MCAsyncJMAP.h"
#import "MCOOperation+Private.h"
#import "MCOUtils.h"

typedef void (^CompletionType)(NSError * error);

@implementation MCOJMAPOperation {
    CompletionType _completionBlock;
}

#define nativeType mailcore::JMAPOperation

+ (void) load
{
    MCORegisterClass(self, &typeid(nativeType));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCOperation:(nativeType *) object] autorelease];
}

- (void) dealloc
{
    [_completionBlock release];
    [super dealloc];
}

- (void) start:(void (^)(NSError * error))completionBlock
{
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void) cancel
{
    [_completionBlock release];
    _completionBlock = nil;
    [super cancel];
}

- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;

    NSError * error = [NSError mco_errorWithErrorCode:MCO_NATIVE_INSTANCE->error()];
    _completionBlock(error);
    [_completionBlock release];
    _completionBlock = nil;
}

@end
