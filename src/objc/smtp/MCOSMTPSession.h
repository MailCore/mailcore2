//
//  MCOSMTPSession.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOSMTPSESSION_H_

#define __MAILCORE_MCOSMTPSESSION_H_

#import <Foundation/Foundation.h>

#import <MailCore/MCOConstants.h>

// This class implements asynchronous SMTP protocol.

@class MCOSMTPSendOperation;
@class MCOSMTPOperation;
@class MCOAddress;

@interface MCOSMTPSession : NSObject

// This is the hostname of the POP3 server to connect to.
@property (nonatomic, copy) NSString * hostname;

// This is the port of the POP3 server to connect to.
@property (nonatomic, assign) unsigned int port;

// This is the username of the account.
@property (nonatomic, copy) NSString * username;

// This is the password of the account.
@property (nonatomic, copy) NSString * password;

// This is the authentication type to use to connect.
// MCOAuthTypeSASLNone means that it uses the clear-text password authentication.
// It's the default.
// Note: However, over a encrypted connection, the password will be safe.
@property (nonatomic, assign) MCOAuthType authType;

// This is the encryption type to use.
// See MCOConnectionType for more information.
@property (nonatomic, assign) MCOConnectionType connectionType;

// This is the timeout of the connection.
@property (nonatomic, assign) NSTimeInterval timeout;

// When set to YES, the connection will fail if the certificate is incorrect.
@property (nonatomic, assign, getter=isCheckCertificateEnabled) BOOL checkCertificateEnabled;

// If set to YES, when sending the EHLO or HELO command, use IP address instead of hostname.
// Default is NO.
@property (nonatomic, assign, getter=isUseHeloIPEnabled) BOOL useHeloIPEnabled;

// Returns an operation that will send the given message through SMTP.
// It will use the recipient set in the message data (To, Cc and Bcc).
// It will also filter out Bcc from the content of the message.
//
// {
//   ...
//   // Generate RFC 822 data using MCOMessageBuilder
//   MCOPOPOperation * op [session sendOperationWithData:rfc822Data];
//   [op start:^(NSError *error) {
//        ...
//   }]];
//
- (MCOSMTPSendOperation *) sendOperationWithData:(NSData *)messageData;

// Returns an operation that will check whether the SMTP account is valid.
//
// {
//   ...
//   MCOPOPOperation * op [session checkAccountOperationWithFrom:[MCOAddress addressWithMailbox:@"hoa@etpan.org"]];
//   [op start:^(NSError *error) {
//        ...
//   }]];
//
- (MCOSMTPOperation *) checkAccountOperationWithFrom:(MCOAddress *)from;

@end

#endif
