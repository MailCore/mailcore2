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

@property (nonatomic, copy) NSString * hostname;
@property (nonatomic, assign) unsigned int port;
@property (nonatomic, copy) NSString * username;
@property (nonatomic, copy) NSString * password;
@property (nonatomic, assign) MCOAuthType authType;
@property (nonatomic, assign) MCOConnectionType connectionType;
@property (nonatomic, assign) NSTimeInterval timeout;
@property (nonatomic, assign, getter=isCheckCertificateEnabled) BOOL checkCertificateEnabled;
@property (nonatomic, assign, getter=isUseHeloIPEnabled) BOOL useHeloIPEnabled;

- (MCOSMTPSendOperation *) sendOperationWithData:(NSData *)messageData;
- (MCOSMTPOperation *) checkAccountOperationWithFrom:(MCOAddress *)from;

@end

#endif
