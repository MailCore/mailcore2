//
//  MCONNTPFetchMessageOperation.m
//  mailcore2
//
//  Created by Robert Widmann on 8/13/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#import "MCONNTPFetchMessageOperation.h"

#import "MCAsyncNNTP.h"

#import "MCOUtils.h"
#import "MCOOperation+Private.h"

#define nativeType mailcore::NNTPFetchMessageOperation

typedef void (^CompletionType)(NSError *error, NSData * messageData);

@interface MCONNTPFetchMessageOperation ()

- (void) bodyProgress:(unsigned int)current maximum:(unsigned int)maximum;

@end

class MCONNTPFetchMessageOperationCallback : public mailcore::NNTPOperationCallback {
public:
    MCONNTPFetchMessageOperationCallback(MCONNTPFetchMessageOperation * op)
    {
        mOperation = op;
    }
    virtual ~MCONNTPFetchMessageOperationCallback()
    {
    }
    
    virtual void bodyProgress(mailcore::NNTPOperation * session, unsigned int current, unsigned int maximum) {
        [mOperation bodyProgress:current maximum:maximum];
    }
    
private:
    MCONNTPFetchMessageOperation * mOperation;
};

@implementation MCONNTPFetchMessageOperation {
    CompletionType _completionBlock;
    MCONNTPFetchMessageOperationCallback * _popCallback;
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

- (id)initWithMCOperation:(mailcore::Operation *)op
{
    self = [super initWithMCOperation:op];
    
    _popCallback = new MCONNTPFetchMessageOperationCallback(self);
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

- (void) start:(void (^)(NSError *error, NSData * messageData))completionBlock
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
        _completionBlock(nil, MCO_TO_OBJC(op->data()));
    } else {
        _completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
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
