//
//  MCONNTPSendOperation.mm
//  mailcore2
//
//  Created by Daryle Walker on 2/21/16.
//  Copyright © 2016 MailCore. All rights reserved.
//

#import "MCONNTPSendOperation.h"

#import "MCAsyncNNTP.h"

#import "MCOUtils.h"
#import "MCOOperation+Private.h"

#define nativeType mailcore::NNTPSendOperation

typedef void (^CompletionType)(NSError *error);

@interface MCONNTPSendOperation ()

- (void) bodyProgress:(unsigned int)current maximum:(unsigned int)maximum;

@end

class MCONNTPSendOperationCallback : public mailcore::NNTPOperationCallback {
public:
    MCONNTPSendOperationCallback(MCONNTPSendOperation * op)
    {
        mOperation = op;
    }
    virtual ~MCONNTPSendOperationCallback()
    {
    }
    
    virtual void bodyProgress(mailcore::NNTPOperation * session, unsigned int current, unsigned int maximum) {
        [mOperation bodyProgress:current maximum:maximum];
    }
    
private:
    MCONNTPSendOperation * mOperation;
};

@implementation MCONNTPSendOperation {
    CompletionType _completionBlock;
    MCONNTPSendOperationCallback * _popCallback;
    MCONNTPOperationProgressBlock _progress;
}

@synthesize progress = _progress;

+ (void) load
{
    MCORegisterClass(self, &typeid(nativeType));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    nativeType * op = (nativeType *) object;
    return [[[self alloc] initWithMCOperation:op] autorelease];
}

- (instancetype) initWithMCOperation:(mailcore::Operation *)op
{
    self = [super initWithMCOperation:op];
    
    _popCallback = new MCONNTPSendOperationCallback(self);
    ((mailcore::NNTPOperation *) op)->setNNTPCallback(_popCallback);
    
    return self;
}

- (void) dealloc
{
    [_progress release];
    [_completionBlock release];
    delete _popCallback;
    [super dealloc];
}

- (void) start:(void (^)(NSError *error))completionBlock
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
    
    nativeType *op = MCO_NATIVE_INSTANCE;
    if (op->error() == mailcore::ErrorNone) {
        _completionBlock(nil);
    } else {
        _completionBlock([NSError mco_errorWithErrorCode:op->error()]);
    }
    [_completionBlock release];
    _completionBlock = nil;
}

- (void) bodyProgress:(unsigned int)current maximum:(unsigned int)maximum
{
    if (_progress != NULL) {
        _progress(current, maximum);
    }
}

@end
