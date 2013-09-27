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

/** 
 This class is used to create an SMTP connection and send messages

 After calling a method that returns an operation you must call start: on the instance
 to begin the operation.
*/

@class MCOSMTPSendOperation;
@class MCOSMTPOperation;
@class MCOAddress;

@interface MCOSMTPSession : NSObject

/** This is the hostname of the SMTP server to connect to. */
@property (nonatomic, copy) NSString * hostname;

/** This is the port of the POP3 server to connect to. */
@property (nonatomic, assign) unsigned int port;

/** This is the username of the account. */
@property (nonatomic, copy) NSString * username;

/** This is the password of the account. */
@property (nonatomic, copy) NSString * password;

/** This is the OAuth2 token. */
@property (nonatomic, copy) NSString *OAuth2Token;

/** 
 This is the authentication type to use to connect.
 `MCOAuthTypeSASLNone` means that it uses the clear-text is used (and is the default).
 @warning *Important*: Over an encrypted connection like TLS, the password will still be secure
*/
@property (nonatomic, assign) MCOAuthType authType;

/**
 This is the encryption type to use.
 See MCOConnectionType for more information.
*/
@property (nonatomic, assign) MCOConnectionType connectionType;

/** This is the timeout of the connection. */
@property (nonatomic, assign) NSTimeInterval timeout;

/** When set to YES, the connection will fail if the certificate is incorrect. */
@property (nonatomic, assign, getter=isCheckCertificateEnabled) BOOL checkCertificateEnabled;

/**
 If set to YES, when sending the EHLO or HELO command, use IP address instead of hostname.
 Default is NO.
*/
@property (nonatomic, assign, getter=isUseHeloIPEnabled) BOOL useHeloIPEnabled;

/**
 Sets logger callback. The network traffic will be sent to this block.
 
 [session setConnectionLogger:^(void * connectionID, MCOConnectionLogType type, NSData * data) {
   ...
 }];
 */
@property (nonatomic, copy) MCOConnectionLogger connectionLogger;

/** @name Operations */

/**
 Returns an operation that will send the given message through SMTP.
 It will use the recipient set in the message data (To, Cc and Bcc).
 It will also filter out Bcc from the content of the message.

 Generate RFC 822 data using MCOMessageBuilder

     MCOPOPOperation * op = [session sendOperationWithData:rfc822Data];
     [op start:^(NSError * error) {
          ...
     }];
*/
- (MCOSMTPSendOperation *) sendOperationWithData:(NSData *)messageData;

/**
 Returns an operation that will send the given message through SMTP.
 It will use the sender and recipient set from the parameters.
 It will also filter out Bcc from the content of the message.
 
 Generate RFC 822 data using MCOMessageBuilder
 
 MCOPOPOperation * op = [session sendOperationWithData:rfc822Data
                                                  from:[MCOAddress addressWithMailbox:@"hoa@etpan.org"]
                                            recipients:[NSArray arrayWithObject:[MCOAddress addressWithMailbox:@"laura@etpan.org"]]];
 [op start:^(NSError * error) {
 ...
 }];
 */
- (MCOSMTPSendOperation *) sendOperationWithData:(NSData *)messageData
                                            from:(MCOAddress *)from
                                      recipients:(NSArray *)recipients;

/**
 Returns an operation that will check whether the SMTP account is valid.

     MCOPOPOperation * op = [session checkAccountOperationWithFrom:[MCOAddress addressWithMailbox:@"hoa@etpan.org"]];
     [op start:^(NSError * error) {
          ...
     }];
*/
- (MCOSMTPOperation *) checkAccountOperationWithFrom:(MCOAddress *)from;

/**
 Returns an operation that will perform a No-Op.
 
 MCOPOPOperation * op = [session noopOperation];
 [op start:^(NSError * error) {
 ...
 }];
 */
- (MCOSMTPOperation *) noopOperation;

@end

#endif
