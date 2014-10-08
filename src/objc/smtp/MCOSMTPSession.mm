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
#import "MCOSMTPNoopOperation.h"
#import "MCOSMTPOperation.h"
#import "MCOOperation+Private.h"
#import "MCOAddress.h"
#import "MCOSMTPOperation+Private.h"

using namespace mailcore;

@interface MCOSMTPSession ()

- (void) _logWithSender:(void *)sender connectionType:(MCOConnectionLogType)logType data:(NSData *)data;

@end

class MCOSMTPConnectionLoggerBridge : public Object, public ConnectionLogger {
public:
    MCOSMTPConnectionLoggerBridge(MCOSMTPSession * session)
    {
        mSession = session;
    }
    
    virtual void log(void * sender, ConnectionLogType logType, Data * data)
    {
        [mSession _logWithSender:sender connectionType:(MCOConnectionLogType)logType data:MCO_TO_OBJC(data)];
    }
    
private:
    MCOSMTPSession * mSession;
};

@implementation MCOSMTPSession {
    mailcore::SMTPAsyncSession * _session;
    MCOConnectionLogger _connectionLogger;
    MCOSMTPConnectionLoggerBridge * _loggerBridge;
}

#define nativeType mailcore::SMTPAsyncSession

- (mailcore::Object *) mco_mcObject
{
    return _session;
}

- (id)init {
    self = [super init];
    
    _session = new mailcore::SMTPAsyncSession();
    _loggerBridge = new MCOSMTPConnectionLoggerBridge(self);
    
    return self;
}

- (void)dealloc {
    MC_SAFE_RELEASE(_loggerBridge);
    [_connectionLogger release];
    _session->setConnectionLogger(NULL);
    _session->release();
    [super dealloc];
}

MCO_OBJC_SYNTHESIZE_STRING(setHostname, hostname)
MCO_OBJC_SYNTHESIZE_SCALAR(unsigned int, unsigned int, setPort, port)
MCO_OBJC_SYNTHESIZE_STRING(setUsername, username)
MCO_OBJC_SYNTHESIZE_STRING(setPassword, password)
MCO_OBJC_SYNTHESIZE_STRING(setOAuth2Token, OAuth2Token)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOAuthType, mailcore::AuthType, setAuthType, authType)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOConnectionType, mailcore::ConnectionType, setConnectionType, connectionType)
MCO_OBJC_SYNTHESIZE_SCALAR(NSTimeInterval, time_t, setTimeout, timeout)
MCO_OBJC_SYNTHESIZE_BOOL(setCheckCertificateEnabled, isCheckCertificateEnabled)
MCO_OBJC_SYNTHESIZE_BOOL(setUseHeloIPEnabled, useHeloIPEnabled)
MCO_OBJC_SYNTHESIZE_SCALAR(dispatch_queue_t, dispatch_queue_t, setDispatchQueue, dispatchQueue);

- (void) setConnectionLogger:(MCOConnectionLogger)connectionLogger
{
    [_connectionLogger release];
    _connectionLogger = [connectionLogger copy];
    
    if (_connectionLogger != nil) {
        _session->setConnectionLogger(_loggerBridge);
    }
    else {
        _session->setConnectionLogger(NULL);
    }
}

- (MCOConnectionLogger) connectionLogger
{
    return _connectionLogger;
}

#pragma mark - Operations

- (MCOSMTPSendOperation *) sendOperationWithData:(NSData *)messageData
{
    mailcore::SMTPOperation * coreOp = MCO_NATIVE_INSTANCE->sendMessageOperation([messageData mco_mcData]);
    MCOSMTPSendOperation * result = [[[MCOSMTPSendOperation alloc] initWithMCOperation:coreOp] autorelease];
    [result setSession:self];
    return result;
}

- (MCOSMTPSendOperation *) sendOperationWithData:(NSData *)messageData
                                            from:(MCOAddress *)from
                                      recipients:(NSArray *)recipients
{
    mailcore::SMTPOperation * coreOp =
    MCO_NATIVE_INSTANCE->sendMessageOperation(MCO_FROM_OBJC(Address, from),
                                              MCO_FROM_OBJC(Array, recipients),
                                              [messageData mco_mcData]);
    MCOSMTPSendOperation * result = [[[MCOSMTPSendOperation alloc] initWithMCOperation:coreOp] autorelease];
    [result setSession:self];
    return result;
}

- (MCOSMTPOperation *) checkAccountOperationWithFrom:(MCOAddress *)from
{
    mailcore::SMTPOperation *coreOp = MCO_NATIVE_INSTANCE->checkAccountOperation(MCO_FROM_OBJC(mailcore::Address, from));
    MCOSMTPOperation * result = [[[MCOSMTPOperation alloc] initWithMCOperation:coreOp] autorelease];
    [result setSession:self];
    return result;
}

- (MCOSMTPOperation *) noopOperation
{
    mailcore::SMTPOperation * coreOp = MCO_NATIVE_INSTANCE->noopOperation();
    MCOSMTPNoopOperation * result = [[[MCOSMTPNoopOperation alloc] initWithMCOperation:coreOp] autorelease];
    [result setSession:self];
    return result;
}

- (void) _logWithSender:(void *)sender connectionType:(MCOConnectionLogType)logType data:(NSData *)data
{
    _connectionLogger(sender, logType, data);
}

@end
