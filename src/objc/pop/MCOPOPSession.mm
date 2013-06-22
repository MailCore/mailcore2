//
//  MCOPOPSession.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOPOPSession.h"

#include "MCAsyncPOP.h"

#import "MCOUtils.h"
#import "MCOPOPOperation.h"
#import "MCOOperation+Private.h"
#import "MCOPOPFetchMessagesOperation.h"

@implementation MCOPOPSession {
    mailcore::POPAsyncSession * _session;
}

#define nativeType mailcore::POPAsyncSession

- (mailcore::Object *) mco_mcObject
{
    return _session;
}

- (NSString *) description
{
    return MCO_OBJC_BRIDGE_GET(description);
}

- (id)init {
    self = [super init];
    if (self) {
        _session = new mailcore::POPAsyncSession();
    }
    return self;
}

- (void)dealloc {
    _session->release();
    [super dealloc];
}

MCO_OBJC_SYNTHESIZE_STRING(setHostname, hostname)
MCO_OBJC_SYNTHESIZE_SCALAR(unsigned int, unsigned int, setPort, port)
MCO_OBJC_SYNTHESIZE_STRING(setUsername, username)
MCO_OBJC_SYNTHESIZE_STRING(setPassword, password)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOAuthType, mailcore::AuthType, setAuthType, authType)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOConnectionType, mailcore::ConnectionType, setConnectionType, connectionType)
MCO_OBJC_SYNTHESIZE_SCALAR(NSTimeInterval, time_t, setTimeout, timeout)
MCO_OBJC_SYNTHESIZE_BOOL(setCheckCertificateEnabled, isCheckCertificateEnabled)

#pragma mark - Operations

- (MCOPOPFetchMessagesOperation *) fetchMessagesOperation
{
    mailcore::POPFetchMessagesOperation * coreOp = MCO_NATIVE_INSTANCE->fetchMessagesOperation();
    return MCO_TO_OBJC(coreOp);
}

- (MCOPOPFetchHeaderOperation *) fetchHeaderOperationWithIndex:(unsigned int)index
{
    mailcore::POPFetchHeaderOperation * coreOp = MCO_NATIVE_INSTANCE->fetchHeaderOperation(index);
    return MCO_TO_OBJC(coreOp);
}

- (MCOPOPFetchMessageOperation *) fetchMessageOperationWithIndex:(unsigned int)index;
{
    mailcore::POPFetchMessageOperation * coreOp = MCO_NATIVE_INSTANCE->fetchMessageOperation(index);
    return MCO_TO_OBJC(coreOp);
}

// Will disconnect.
- (MCOPOPOperation *) deleteMessagesOperationWithIndexes:(MCOIndexSet *)indexes
{
    mailcore::POPOperation * coreOp = MCO_NATIVE_INSTANCE->deleteMessagesOperation(MCO_FROM_OBJC(mailcore::IndexSet, indexes));
    return [[[MCOPOPOperation alloc] initWithMCOperation:coreOp] autorelease];
}

- (MCOPOPOperation *) disconnectOperation
{
    mailcore::POPOperation * coreOp = MCO_NATIVE_INSTANCE->disconnectOperation();
    return [[[MCOPOPOperation alloc] initWithMCOperation:coreOp] autorelease];
}

- (MCOPOPOperation *) checkAccountOperation
{
    mailcore::POPOperation * coreOp = MCO_NATIVE_INSTANCE->checkAccountOperation();
    return [[[MCOPOPOperation alloc] initWithMCOperation:coreOp] autorelease];
}

@end
