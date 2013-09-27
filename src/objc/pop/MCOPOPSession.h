//
//  MCOPOPSession.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOPOPSESSION_H_

#define __MAILCORE_MCOPOPSESSION_H_

#import <Foundation/Foundation.h>

#import <MailCore/MCOConstants.h>

@class MCOPOPFetchMessagesOperation;
@class MCOPOPFetchHeaderOperation;
@class MCOPOPFetchMessageOperation;
@class MCOPOPOperation;
@class MCOIndexSet;

/** This class implements asynchronous access to the POP3 protocol.*/

@interface MCOPOPSession : NSObject

/** This is the hostname of the POP3 server to connect to.*/
@property (nonatomic, copy) NSString * hostname;

/** This is the port of the POP3 server to connect to.*/
@property (nonatomic, assign) unsigned int port;

/** This is the username of the account.*/
@property (nonatomic, copy) NSString * username;

/** This is the password of the account.*/
@property (nonatomic, copy) NSString * password;

/** 
 This is the authentication type to use to connect.
 `MCOAuthTypeSASLNone` means that it uses the clear-text is used (and is the default).
 @warning *Important*: Over an encrypted connection like TLS, the password will still be secure
*/
@property (nonatomic, assign) MCOAuthType authType;

/** This is the encryption type to use.
See MCOConnectionType for more information.*/
@property (nonatomic, assign) MCOConnectionType connectionType;

/** This is the timeout of the connection.*/
@property (nonatomic, assign) NSTimeInterval timeout;

/** When set to YES, the connection will fail if the certificate is incorrect.*/
@property (nonatomic, assign, getter=isCheckCertificateEnabled) BOOL checkCertificateEnabled;

/**
 Sets logger callback. The network traffic will be sent to this block.
 
 [session setConnectionLogger:^(void * connectionID, MCOConnectionLogType type, NSData * data) {
   ...
 }];
 */
@property (nonatomic, copy) MCOConnectionLogger connectionLogger;

/** @name Operations */

/**
 Returns an operation that will fetch the list of messages.

     MCOPOPFetchMessagesOperation * op = [session fetchMessagesOperation];
     [op start:^(NSError * error, NSArray * messages) {
        // messages is an array of MCOPOPMessageInfo
        // [info index] can be used as reference for a given message in the other operations.
     }];
*/
- (MCOPOPFetchMessagesOperation *) fetchMessagesOperation;

/**
 Returns an operation that will fetch the header of the given message.

     MCOPOPFetchHeaderOperation * op = [session fetchHeaderOperationWithIndex:idx];
     [op start:^(NSError * error, MCOMessageHeader * header) {
          // header is the parsed header of the message.
     }];
*/  
- (MCOPOPFetchHeaderOperation *) fetchHeaderOperationWithIndex:(unsigned int)index;

/**
 Returns an operation that will fetch the content of the given message.

     MCOPOPFetchMessageOperation * op = [session fetchMessageOperationWithIndex:idx];
     [op start:^(NSError * error, NSData * messageData) {
          // messageData is the RFC 822 formatted message data.
     }];
*/
- (MCOPOPFetchMessageOperation *) fetchMessageOperationWithIndex:(unsigned int)index;

/**
 Returns an operation that will delete the given messages.
 Will disconnect when finished.

     MCOIndexSet * indexes = [MCOIndexSet indexSet];
     [indexes addIndex:1];
     [indexes addIndex:2];
     [indexes addIndex:3];
     MCOPOPOperation * op = [session deleteMessagesOperationWithIndexes:indexes];
     [op start:^(NSError * error) {
          ...
     }];
*/
- (MCOPOPOperation *) deleteMessagesOperationWithIndexes:(MCOIndexSet *)indexes;

/**
 Returns an operation that will disconnect the session.
 
 MCOPOPOperation * op = [session disconnectOperation];
 [op start:^(NSError * error) {
 ...
 }];
 */
- (MCOPOPOperation *) disconnectOperation;

/**
 Returns an operation that will check whether the POP account is valid.

     MCOPOPOperation * op = [session checkAccountOperation];
     [op start:^(NSError * error) {
          ...
     }];
*/
- (MCOPOPOperation *) checkAccountOperation;

/**
 Returns an operation that will perform a No-Op operation.
 
 MCOPOPOperation * op = [session noopOperation];
 [op start:^(NSError * error) {
 ...
 }];
 */
- (MCOPOPOperation *) noopOperation;

@end

#endif
