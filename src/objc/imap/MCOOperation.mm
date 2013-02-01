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
- (void)operationCompleted;
@end

class CompletionCallback : public Object, public OperationCallback {
public:
    CompletionCallback(MCOOperation *op) {
        mOperation = op;
    }

    virtual void operationFinished(Operation * op)
    {
        [mOperation operationCompleted];
    }

    private:
        MCOOperation *mOperation;
};

@implementation MCOOperation {
    MCOObjectWrapper *_wrapper; // Operation
    MCOObjectWrapper *_completionWrapper; // CompletionCallback
}

- (id)initWithOperation:(Operation *)op
{
    self = [super init];
    if (self) {
        _wrapper = [MCOObjectWrapper objectWrapperWithObject:op];
        
        CompletionCallback *callback = new CompletionCallback(self);
        _completionWrapper = [MCOObjectWrapper objectWrapperWithObject:callback];
        self.operation->setCallback(callback);
    }
    return self;
}

- (BOOL)isCancelled
{
    return self.operation->isCancelled();
}

- (void)cancel
{
    self.operation->cancel();
}

- (void)start
{
    self.operation->start();
}

- (Operation *)operation {
    return dynamic_cast<Operation *>([_wrapper object]);
}

- (void)operationCompleted {
}

@end
