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

@interface MCOPOPSession : NSObject

@property (nonatomic, copy) NSString * hostname;
@property (nonatomic, assign) unsigned int port;
@property (nonatomic, copy) NSString * username;
@property (nonatomic, copy) NSString * password;
@property (nonatomic, assign) MCOAuthType authType;
@property (nonatomic, assign) MCOConnectionType connectionType;
@property (nonatomic, assign) NSTimeInterval timeout;
@property (nonatomic, assign, getter=isCheckCertificateEnabled) BOOL checkCertificateEnabled;

- (MCOPOPFetchMessagesOperation *) fetchMessagesOperation;
- (MCOPOPFetchHeaderOperation *) fetchHeaderOperationWithIndex:(unsigned int)index;
- (MCOPOPFetchMessageOperation *) fetchMessageOperationWithIndex:(unsigned int)index;

// Will disconnect.
- (MCOPOPOperation *) deleteMessagesWithIndexes:(MCOIndexSet *)indexes;

@end

#endif
