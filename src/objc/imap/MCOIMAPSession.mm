//
//  MCOIMAPSession.m
//  mailcore2
//
//  Created by Matt Ronge on 1/31/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPSession.h"

#import "MCOOperation.h"
#import "MCOObjectWrapper.h"
#import "MCOIMAPOperation.h"
#import "MCOIMAPFetchFoldersOperation.h"

#import "MCOUtils.h"

#import <mailcore/MCAsync.h>


using namespace mailcore;

@implementation MCOIMAPSession {
    IMAPAsyncSession * _session;
}

#define nativeType mailcore::IMAPAsyncSession

- (mailcore::Object *) mco_mcObject
{
    return _session;
}

- (id)init {
    self = [super init];
    if (self) {
        _session = new IMAPAsyncSession();
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
MCO_OBJC_SYNTHESIZE_BOOL(setVoIPEnabled, isVoIPEnabled)
MCO_OBJC_SYNTHESIZE_SCALAR(char, char, setDelimiter, delimiter)

#pragma mark - Operations

- (MCOIMAPOperation *)checkAccountOperation {
    IMAPOperation *coreOp = MCO_NATIVE_INSTANCE->checkAccountOperation();
    return MCO_TO_OBJC(coreOp);

}

- (MCOIMAPFetchFoldersOperation *)fetchAllFoldersOperation {
    IMAPOperation *coreOp = MCO_NATIVE_INSTANCE->fetchAllFoldersOperation();
    return MCO_TO_OBJC(coreOp);
    
}
@end
