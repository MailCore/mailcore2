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
#import <MailCore/MCOConstants.h>

@class MCOIMAPFetchFoldersOperation;
@class MCOIMAPOperation;
@class MCOIMAPNamespace;
@class MCOIMAPFolderInfoOperation;
@class MCOIMAPAppendMessageOperation;
@class MCOIMAPCopyMessagesOperation;
@class MCOIndexSet;
@class MCOIMAPFetchMessagesOperation;
@class MCOIMAPFetchContentOperation;
@class MCOIMAPSearchOperation;
@class MCOIMAPIdleOperation;
@class MCOIMAPFetchNamespaceOperation;
@class MCOIMAPSearchExpression;
@class MCOIMAPIdentityOperation;
@class MCOIMAPCapabilityOperation;

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
@property (nonatomic, assign) char delimiter;

@property (nonatomic, strong) MCOIMAPNamespace * defaultNamespace;
@property (nonatomic, assign) BOOL allowsFolderConcurrentAccessEnabled;
@property (nonatomic, assign) unsigned int maximumConnectons;

- (MCOIMAPFolderInfoOperation *) folderInfoOperation:(NSString *)folder;
- (MCOIMAPFetchFoldersOperation *) fetchSubscribedFoldersOperation;
- (MCOIMAPFetchFoldersOperation *) fetchAllFoldersOperation;

- (MCOIMAPOperation *) renameFolderOperation:(NSString *)folder otherName:(NSString *)otherName;
- (MCOIMAPOperation *) deleteFolderOperation:(NSString *)folder;
- (MCOIMAPOperation *) createFolderOperation:(NSString *)folder;

- (MCOIMAPOperation *) subscribeFolderOperation:(NSString *)folder;
- (MCOIMAPOperation *) unsubscribeFolderOperation:(NSString *)folder;

- (MCOIMAPAppendMessageOperation *)appendMessageOperationWithFolder:(NSString *)folder
                                                        messageData:(NSData *)messageData
                                                              flags:(MCOMessageFlag)flags;

- (MCOIMAPCopyMessagesOperation *)copyMessagesOperationWithFolder:(NSString *)folder
                                                             uids:(MCOIndexSet *)uids
                                                       destFolder:(NSString *)destFolder;


- (MCOIMAPOperation *) expungeOperation:(NSString *)folder;

- (MCOIMAPFetchMessagesOperation *) fetchMessagesByUIDOperationWithFolder:(NSString *)folder
                                                              requestKind:(MCOIMAPMessagesRequestKind)requestKind
                                                                     uids:(MCOIndexSet *)uids;

- (MCOIMAPFetchMessagesOperation *) fetchMessagesByNumberOperationWithFolder:(NSString *)folder
                                                                 requestKind:(MCOIMAPMessagesRequestKind)requestKind
                                                                     numbers:(MCOIndexSet *)numbers;

- (MCOIMAPFetchMessagesOperation *) syncMessagesByUIDWithFolder:(NSString *)folder
                                                    requestKind:(MCOIMAPMessagesRequestKind)requestKind
                                                           uids:(MCOIndexSet *)uids
                                                         modSeq:(uint64_t)modSeq;

- (MCOIMAPFetchContentOperation *) fetchMessageByUIDOperationWithFolder:(NSString *)folder
                                                                    uid:(uint32_t)uid
                                                                 urgent:(BOOL)urgent;

- (MCOIMAPFetchContentOperation *) fetchMessageByUIDOperationWithFolder:(NSString *)folder
                                                                    uid:(uint32_t)uid;

- (MCOIMAPFetchContentOperation *) fetchMessageAttachmentByUIDOperationWithFolder:(NSString *)folder
                                                                              uid:(uint32_t)uid
                                                                           partID:(NSString *)partID
                                                                         encoding:(MCOEncoding)encoding
                                                                           urgent:(BOOL)urgent;

- (MCOIMAPFetchContentOperation *) fetchMessageAttachmentByUIDOperationWithFolder:(NSString *)folder
                                                                              uid:(uint32_t)uid
                                                                           partID:(NSString *)partID
                                                                         encoding:(MCOEncoding)encoding;

- (MCOIMAPOperation *) storeFlagsOperationWithFolder:(NSString *)folder
                                                uids:(MCOIndexSet *)uids
                                                kind:(MCOIMAPStoreFlagsRequestKind)kind
                                               flags:(MCOMessageFlag)flags;

- (MCOIMAPOperation *) storeLabelsOperationWithFolder:(NSString *)folder
                                                 uids:(MCOIndexSet *)uids
                                                 kind:(MCOIMAPStoreFlagsRequestKind)kind
                                               labels:(NSArray *)labels;

- (MCOIMAPSearchOperation *) searchOperationWithFolder:(NSString *)folder
                                                  kind:(MCOIMAPSearchKind)kind
                                          searchString:(NSString *)searchString;

- (MCOIMAPSearchOperation *) searchExpressionOperationWithFolder:(NSString *)folder
                                                      expression:(MCOIMAPSearchExpression *)expression;

- (MCOIMAPIdleOperation *) idleOperationWithFolder:(NSString *)folder
                                      lastKnownUID:(uint32_t)lastKnownUID;

- (MCOIMAPFetchNamespaceOperation *) fetchNamespaceOperation;

- (MCOIMAPIdentityOperation *) identityOperationWithVendor:(NSString *)vendor
                                                      name:(NSString *)name
                                                   version:(NSString *)version;

- (MCOIMAPOperation *) checkAccountOperation;

- (MCOIMAPCapabilityOperation *) capabilityOperation;

@end

#endif
