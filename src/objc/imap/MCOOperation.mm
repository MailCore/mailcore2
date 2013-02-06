//
//  MCOOperation.m
//  mailcore2
//
//  Created by Matt Ronge on 01/31/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "MCOOperation.h"
#import "MCOperation.h"
#import "MCOperationCallback.h"
#import "MCOObjectWrapper.h"

#import <Foundation/Foundation.h>

using namespace mailcore;

@interface MCOOperation ()
- (void)_operationCompleted;
@end

class CompletionCallback : public Object, public OperationCallback {
public:
    CompletionCallback(MCOOperation *op) {
        mOperation = op;
    }

    virtual void operationFinished(Operation * op)
    {
        [mOperation _operationCompleted];
    }

    private:
        MCOOperation *mOperation;
};

@implementation MCOOperation {
    Operation * _operation;
    CompletionCallback * _callback;
    BOOL _started;
}

- (id)initWithOperation:(Operation *)op
{
    self = [super init];
    
    _operation = op;
    _operation->retain();
    
    _callback = new CompletionCallback(self);
    _operation->setCallback(_callback);
    
    return self;
}

- (void)dealloc
{
    _operation->release();
    _callback->release();
    [super dealloc];
}

- (BOOL)isCancelled
{
    return _operation->isCancelled();
}

- (void)cancel
{
    if (_started) {
        _started = NO;
        [self release];
    }
    _operation->cancel();
}

- (void)start
{
    _started = YES;
    [self retain];
    _operation->start();
}

- (mailcore::Operation *)operation
{
    return _operation;
}

- (void)_operationCompleted
{
    _started = NO;
    [self operationCompleted];
    [self release];
}

- (void)operationCompleted
{
}

@end
