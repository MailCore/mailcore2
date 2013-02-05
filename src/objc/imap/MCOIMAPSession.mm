//
//  MCOIMAPSession.m
//  mailcore2
//
//  Created by Matt Ronge on 1/31/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPSession.h"
#import "MCOOperation+Internals.h"
#import "MCOObjectWrapper.h"
#import "MCOCheckAccountOperation.h"
#import "MCOFetchFoldersOperation.h"

#import "NSString+MCO.h"

#import <mailcore/MCAsync.h>


using namespace mailcore;

@implementation MCOIMAPSession {
    IMAPAsyncSession * _session;
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

- (IMAPAsyncSession *)session {
    return _session;
}

- (NSString *)hostname {
    return [NSString mco_stringWithMCString:self.session->hostname()];
}

- (void)setHostname:(NSString *)hostname {
    self.session->setHostname([hostname mco_mcString]);
}

- (unsigned int)port {
    return self.session->port();
}

- (void)setPort:(unsigned int)port {
    self.session->setPort(port);
}

- (NSString *)username {
    return [NSString mco_stringWithMCString:self.session->username()];
}

- (void)setUsername:(NSString *)username {
    self.session->setUsername([username mco_mcString]);
}

- (NSString *)password {
    return [NSString mco_stringWithMCString:self.session->password()];
}

- (void)setPassword:(NSString *)password {
    self.session->setPassword([password mco_mcString]);
}

- (void)setAuthType:(MCOAuthType)authType {
    self.session->setAuthType((AuthType)authType);
}

- (MCOAuthType)authType {
    return (MCOAuthType)self.session->authType();
}

- (void)setConnectionType:(MCOConnectionType)connectionType {
    self.session->setConnectionType((ConnectionType)connectionType);
}

- (MCOConnectionType)connectionType{
    return (MCOConnectionType)self.session->connectionType();
}

- (void)setTimeout:(NSTimeInterval)timeout {
    self.session->setTimeout((time_t)timeout);
}

- (NSTimeInterval)timeout {
    return (NSTimeInterval)self.session->timeout();
}

- (void)setCheckCertificateEnabled:(BOOL)checkCertificateEnabled {
    self.session->setCheckCertificateEnabled(checkCertificateEnabled);
}

- (BOOL)checkCertificateEnabled {
    return (BOOL)self.session->isCheckCertificateEnabled();
}

- (void)setVoIPEnabled:(BOOL)VoIPEnabled {
    self.session->setVoIPEnabled(VoIPEnabled);
}

- (BOOL)VoIPEnabled {
    return (BOOL)self.session->isVoIPEnabled();
}

- (NSString *)delimiter {
    char delim = self.session->delimiter();
    return [NSString stringWithFormat:@"%c", delim];
}

- (void)setDelimiter:(NSString *)delimiter {
    NSAssert(delimiter.length == 1, @"Delimiter has to be a single char");
    char delim = [delimiter characterAtIndex:0];
    self.session->setDelimiter(delim);
}

#pragma mark - Operations

- (MCOCheckAccountOperation *)checkAccountOperation {
    IMAPOperation *coreOp = self.session->checkAccountOperation();
    MCOCheckAccountOperation *op = [[MCOCheckAccountOperation alloc] initWithOperation:coreOp];
    return [op autorelease];

}

- (MCOFetchFoldersOperation *)fetchAllFoldersOperation {
    IMAPOperation *coreOp = self.session->fetchAllFoldersOperation();
    MCOFetchFoldersOperation *op = [[MCOFetchFoldersOperation alloc] initWithOperation:coreOp];
    return [op autorelease];
    
}
@end
