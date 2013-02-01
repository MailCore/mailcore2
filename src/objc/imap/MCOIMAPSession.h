//
//  MCOIMAPSession.h
//  mailcore2
//
//  Created by Matt Ronge on 1/31/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <mailcore/MCOConstants.h>
#import <mailcore/MCOOperation.h>

@class MCOCheckAccountOperation;

@interface MCOIMAPSession : NSObject
@property (nonatomic, strong) NSString *hostname;
@property (nonatomic, assign) unsigned int port;
@property (nonatomic, strong) NSString *username;
@property (nonatomic, strong) NSString *password;
@property (nonatomic, assign) MCOAuthType authType;
@property (nonatomic, assign) MCOConnectionType connectionType;

@property (nonatomic, assign) NSTimeInterval timeout;
@property (nonatomic, assign) BOOL checkCertificateEnabled;
@property (nonatomic, assign) BOOL VoIPEnabled;
@property (nonatomic, strong) NSString *delimiter;

- (MCOCheckAccountOperation *)checkAccount:(MCOCompletionBlock)completionBlock;

@end
