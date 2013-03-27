//
//  MCOIMAPFetchContentOperation.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPFetchContentOperation.h"

#include "MCAsyncIMAP.h"

#import "MCOOperation+Private.h"
#import "MCOUtils.h"

typedef void (^completionType)(NSError *error, NSData * data);

@implementation MCOIMAPFetchContentOperation {
    completionType _completionBlock;
    MCOIMAPBaseOperationProgressBlock _progress;
}

@synthesize progress = _progress;

#define nativeType mailcore::IMAPFetchContentOperation

+ (void) load
{
    MCORegisterClass(self, &typeid(nativeType));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    nativeType * op = (nativeType *) object;
    return [[[self alloc] initWithMCOperation:op] autorelease];
}

- (void) dealloc
{
    [_progress release];
    [_completionBlock release];
    [super dealloc];
}

- (void)start:(void (^)(NSError *error, NSData * data))completionBlock {
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void)operationCompleted {
    nativeType *op = MCO_NATIVE_INSTANCE;
    if (op->error() == mailcore::ErrorNone) {
        _completionBlock(nil, MCO_TO_OBJC(op->data()));
    } else {
        _completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
    }
}

- (void) bodyProgress:(unsigned int)current maximum:(unsigned int)maximum
{
    if (_progress != NULL) {
        _progress(current, maximum);
    }
}

@end
