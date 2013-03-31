//
//  MCOSMTPSession.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOSMTPSession.h"

#include "MCAsyncSMTP.h"

#import "MCOUtils.h"
#import "MCOSMTPSendOperation.h"
#import "MCOSMTPOperation.h"
#import "MCOOperation+Private.h"
#import "MCOAddress.h"

@implementation MCOSMTPSession {
    mailcore::SMTPAsyncSession * _session;
}

#define nativeType mailcore::SMTPAsyncSession

- (mailcore::Object *) mco_mcObject
{
    return _session;
}

- (id)init {
    self = [super init];
    if (self) {
        _session = new mailcore::SMTPAsyncSession();
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
MCO_OBJC_SYNTHESIZE_BOOL(setUseHeloIPEnabled, useHeloIPEnabled)

#pragma mark - Operations

- (MCOSMTPSendOperation *) sendOperationWithData:(NSData *)messageData
{
    mailcore::SMTPOperation * coreOp = MCO_NATIVE_INSTANCE->sendMessageOperation([messageData mco_mcData]);
    return [[[MCOSMTPSendOperation alloc] initWithMCOperation:coreOp] autorelease];
}

- (MCOOperation *) checkAccountOperationWithFrom:(MCOAddress *)from
{
    mailcore::SMTPOperation *coreOp = MCO_NATIVE_INSTANCE->checkAccountOperation(MCO_FROM_OBJC(mailcore::Address, from));
    return [[[MCOSMTPOperation alloc] initWithMCOperation:coreOp] autorelease];
}

@end
