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
#import "MCOSMTPOperation+Private.h"
#import "MCOSMTPSession.h"

typedef void (^CompletionType)(NSError *error);

NSString *const kMCOSMTPOperationResponseKey = @"MCOSMTPOperationResponse";
NSString *const kMCOSMTPOperationResponseCodeKey = @"MCOSMTPOperationResponseCode";

@implementation MCOSMTPOperation {
    CompletionType _completionBlock;
    MCOSMTPSession * _session;
}

#define nativeType mailcore::SMTPOperation

- (void) dealloc
{
    [_session release];
    [_completionBlock release];
    [super dealloc];
}

- (void)start:(void (^)(NSError *error))completionBlock {
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void)operationCompleted {
    if (_completionBlock == NULL)
        return;
    
    NSMutableDictionary *userInfo = [NSMutableDictionary dictionary];
    MCOSMTPSession *session = [self session];
    if (session.lastSMTPResponse) {
        userInfo[kMCOSMTPOperationResponseKey] = session.lastSMTPResponse;
    }
    if (session.lastSMTPResponseCode) {
        userInfo[kMCOSMTPOperationResponseCodeKey] = @(session.lastSMTPResponseCode);
    }

    NSError * error = [NSError mco_errorWithErrorCode:MCO_NATIVE_INSTANCE->error() userInfo:userInfo];
    _completionBlock(error);
    [_completionBlock release];
    _completionBlock = NULL;
}

- (void) setSession:(MCOSMTPSession *)session
{
    [_session release];
    _session = [session retain];
}

- (MCOSMTPSession *) session
{
    return _session;
}

@end
