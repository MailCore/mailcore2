//
//  MCOIMAPSession.h
//  mailcore2
//
//  Created by Matt Ronge on 1/31/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPSESSION_H_

#define __MAILCORE_MCOIMAPSESSION_H_

#import <Foundation/Foundation.h>
#import <mailcore/MCOConstants.h>
#import <mailcore/MCOOperation.h>

@class MCOIMAPCheckAccountOperation;
@class MCOIMAPFetchFoldersOperation;

@interface MCOIMAPSession : NSObject
@property (nonatomic, strong) NSString *hostname;
@property (nonatomic, assign) unsigned int port;
@property (nonatomic, strong) NSString *username;
@property (nonatomic, strong) NSString *password;
@property (nonatomic, assign) MCOAuthType authType;
@property (nonatomic, assign) MCOConnectionType connectionType;

@property (nonatomic, assign) NSTimeInterval timeout;
@property (nonatomic, assign) BOOL checkCertificateEnabled;
@property (nonatomic, assign, getter=isVoIPEnabled) BOOL VoIPEnabled;
@property (nonatomic, strong) NSString *delimiter;

- (MCOIMAPCheckAccountOperation *)checkAccountOperation;
- (MCOIMAPFetchFoldersOperation *)fetchAllFoldersOperation;
@end

#endif
