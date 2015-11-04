//
//  MCOIMAPCustomCommandOperation.m
//  mailcore2
//
//  Created by Libor Huspenina on 29/10/2015.
//  Copyright © 2015 MailCore. All rights reserved.
//

#import "MCOIMAPCustomCommandOperation.h"

#include "MCAsyncIMAP.h"

#import "MCOOperation+Private.h"
#import "MCOUtils.h"

typedef void (^CompletionType)(NSError *error);

@implementation MCOIMAPCustomCommandOperation {
    CompletionType _completionBlock;
}

#define nativeType mailcore::IMAPCustomCommandOperation

+ (void)load
{
    MCORegisterClass(self, &typeid(nativeType));
}

+ (NSObject *)mco_objectWithMCObject:(mailcore::Object *)object
{
    nativeType * op = (nativeType *)object;
    return [[[self alloc] initWithMCOperation:op] autorelease];
}

- (void)dealloc
{
    [_completionBlock release];
    [super dealloc];
}

- (void)start:(void(^)(NSError * __nullable error))completionBlock
{
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void)cancel
{
    [_completionBlock release];
    _completionBlock = nil;
    [super cancel];
}

- (void)operationCompleted
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

@end
