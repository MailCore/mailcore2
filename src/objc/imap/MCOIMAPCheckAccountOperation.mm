//
//  MCOIMAPCheckAccountOperation.m
//  mailcore2
//
//  Created by Hoa V. DINH on 3/7/16.
//  Copyright © 2016 MailCore. All rights reserved.
//

#import "MCOIMAPCheckAccountOperation.h"

#include "MCIMAPCheckAccountOperation.h"

#import "MCOOperation+Private.h"
#import "MCOUtils.h"
#import "MCOConstants.h"

typedef void (^CompletionType)(NSError *error);

@implementation MCOIMAPCheckAccountOperation {
    CompletionType _completionBlock;
}

#define nativeType mailcore::IMAPCheckAccountOperation

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
    [_completionBlock release];
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
        NSError * error = [NSError mco_errorWithErrorCode:op->error()];
        if (op->loginResponse() != NULL) {
            NSMutableDictionary * userInfo = [[error userInfo] mutableCopy];
            userInfo[MCOIMAPResponseKey] = MCO_TO_OBJC(op->loginResponse());
            error = [NSError errorWithDomain:[error domain] code:[error code] userInfo:userInfo];
        }
        _completionBlock(error);
    }
    [_completionBlock release];
    _completionBlock = nil;
}

@end
