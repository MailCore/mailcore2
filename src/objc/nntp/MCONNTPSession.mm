//
//  MCONNTPSession.m
//  mailcore2
//
//  Created by Robert Widmann on 8/13/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#import "MCONNTPSession.h"

#include "MCAsyncNNTP.h"

#import "MCOUtils.h"
#import "MCONNTPOperation.h"
#import "MCOOperation+Private.h"
#import "MCONNTPFetchArticlesOperation.h"
#import "MCONNTPOperation+Private.h"

using namespace mailcore;

@interface MCONNTPSession ()

- (void) _logWithSender:(void *)sender connectionType:(MCOConnectionLogType)logType data:(NSData *)data;

@end

class MCONNTPConnectionLoggerBridge : public Object, public ConnectionLogger {
public:
    MCONNTPConnectionLoggerBridge(MCONNTPSession * session)
    {
        mSession = session;
    }
    
    virtual void log(void * sender, ConnectionLogType logType, Data * data)
    {
        [mSession _logWithSender:sender connectionType:(MCOConnectionLogType)logType data:MCO_TO_OBJC(data)];
    }
    
private:
    MCONNTPSession * mSession;
};

@implementation MCONNTPSession {
    mailcore::NNTPAsyncSession * _session;
    MCOConnectionLogger _connectionLogger;
    MCONNTPConnectionLoggerBridge * _loggerBridge;
}

#define nativeType mailcore::NNTPAsyncSession

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
    
    _session = new mailcore::NNTPAsyncSession();
    _loggerBridge = new MCONNTPConnectionLoggerBridge(self);
    
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
MCO_OBJC_SYNTHESIZE_SCALAR(MCOConnectionType, mailcore::ConnectionType, setConnectionType, connectionType)
MCO_OBJC_SYNTHESIZE_SCALAR(NSTimeInterval, time_t, setTimeout, timeout)
MCO_OBJC_SYNTHESIZE_BOOL(setCheckCertificateEnabled, isCheckCertificateEnabled)
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


#define MCO_TO_OBJC_OP(op) [self _objcOperationFromNativeOp:op];
#define OPAQUE_OPERATION(op) [self _objcOpaqueOperationFromNativeOp:op]

- (id) _objcOperationFromNativeOp:(mailcore::NNTPOperation *)op
{
    MCONNTPOperation * result = MCO_TO_OBJC(op);
    [result setSession:self];
    return result;
}

- (id) _objcOpaqueOperationFromNativeOp:(mailcore::NNTPOperation *)op
{
    MCONNTPOperation * result = [[[MCONNTPOperation alloc] initWithMCOperation:op] autorelease];
    [result setSession:self];
    return result;
}

- (MCONNTPFetchArticlesOperation *) fetchArticlesOperation:(NSString *)group
{
    mailcore::MCNNTPFetchArticlesOperation * coreOp = MCO_NATIVE_INSTANCE->fetchArticlesOperation(MCO_FROM_OBJC(mailcore::String, group));
    return MCO_TO_OBJC_OP(coreOp);
}

- (MCONNTPFetchHeaderOperation *) fetchHeaderOperationWithIndex:(unsigned int)index inGroup:(NSString *)group
{
    mailcore::NNTPFetchHeaderOperation * coreOp = MCO_NATIVE_INSTANCE->fetchHeaderOperation(MCO_FROM_OBJC(mailcore::String, group), index);
    return MCO_TO_OBJC_OP(coreOp);
}

- (MCONNTPFetchArticleOperation *) fetchArticleOperationWithIndex:(unsigned int)index inGroup:(NSString *)group
{
    mailcore::NNTPFetchArticleOperation * coreOp = MCO_NATIVE_INSTANCE->fetchArticleOperation(MCO_FROM_OBJC(mailcore::String, group), index);
    return MCO_TO_OBJC_OP(coreOp);
}

- (MCONNTPListNewsgroupsOperation *) listAllNewsgroupsOperation {
    mailcore::NNTPListNewsgroupsOperation * coreOp = MCO_NATIVE_INSTANCE->listAllNewsgroupsOperation();
    return MCO_TO_OBJC_OP(coreOp);
}

- (MCONNTPListNewsgroupsOperation *) listSubscribedNewsgroupsOperation {
    mailcore::NNTPListNewsgroupsOperation * coreOp = MCO_NATIVE_INSTANCE->listSubscribedNewsgroupsOperation();
    return MCO_TO_OBJC_OP(coreOp);
}

- (MCONNTPOperation *) disconnectOperation
{
    mailcore::NNTPOperation * coreOp = MCO_NATIVE_INSTANCE->disconnectOperation();
    return OPAQUE_OPERATION(coreOp);
}

- (MCONNTPOperation *) checkAccountOperation
{
    mailcore::NNTPOperation * coreOp = MCO_NATIVE_INSTANCE->checkAccountOperation();
    return OPAQUE_OPERATION(coreOp);
}

- (void) _logWithSender:(void *)sender connectionType:(MCOConnectionLogType)logType data:(NSData *)data
{
    _connectionLogger(sender, logType, data);
}

@end
