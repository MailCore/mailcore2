//
//  MCOIMAPFolderInfoOperation.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPFolderInfoOperation.h"

#include "MCAsyncIMAP.h"

#import "MCOOperation+Private.h"
#import "MCOIMAPFolderInfo.h"
#import "MCOUtils.h"

typedef void (^CompletionType)(NSError *error, MCOIMAPFolderInfo *info);

@implementation MCOIMAPFolderInfoOperation {
    CompletionType _completionBlock;
}

#define nativeType mailcore::IMAPFolderInfoOperation

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

- (void)start:(void (^)(NSError * error, MCOIMAPFolderInfo * info))completionBlock
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
        MCOIMAPFolderInfo * info = [MCOIMAPFolderInfo info];
        [info setUidNext:MCO_NATIVE_INSTANCE->uidNext()];
        [info setUidValidity:MCO_NATIVE_INSTANCE->uidValidity()];
        [info setModSequenceValue:MCO_NATIVE_INSTANCE->modSequenceValue()];
        [info setMessageCount:MCO_NATIVE_INSTANCE->messageCount()];
        [info setFirstUnseenUid:MCO_NATIVE_INSTANCE->firstUnseenUid()];
        [info setAllowsNewPermanentFlags:MCO_NATIVE_INSTANCE->allowsNewPermanentFlags()];
        
        _completionBlock(nil, info);
    }
    else {
        _completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
    }
    [_completionBlock release];
    _completionBlock = nil;
}

@end
