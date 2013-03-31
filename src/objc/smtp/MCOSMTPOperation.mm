//
//  MCOSMTPCheckAccountOperation.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOSMTPOperation.h"

#include "MCAsyncSMTP.h"

#import "MCOUtils.h"
#import "MCOOperation+Private.h"

typedef void (^CompletionType)(NSError *error);

@implementation MCOSMTPOperation {
    CompletionType _completionBlock;
}

#define nativeType mailcore::SMTPOperation

- (void) dealloc
{
    [_completionBlock release];
    [super dealloc];
}

- (void)start:(void (^)(NSError *error))completionBlock {
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void)operationCompleted {
    NSError * error = [NSError mco_errorWithErrorCode:MCO_NATIVE_INSTANCE->error()];
    _completionBlock(error);
}

@end
