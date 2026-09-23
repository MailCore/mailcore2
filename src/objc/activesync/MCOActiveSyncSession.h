#ifndef MAILCORE_MCOACTIVESYNCSESSION_H

#define MAILCORE_MCOACTIVESYNCSESSION_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOConstants.h>
#import <MailCore/MCOActiveSyncTypes.h>
#import <MailCore/MCOActiveSyncOperation.h>

@class MCOActiveSyncSyncRequest;

@interface MCOActiveSyncSession : NSObject

@property (nonatomic, copy) NSString * serverURL;
@property (nonatomic, copy) NSString * username;
@property (nonatomic, copy) NSString * password;
@property (nonatomic, copy) NSString * OAuth2Token;
@property (nonatomic, copy) NSString * deviceID;
@property (nonatomic, readonly) NSString * lastRedirectURL;
@property (nonatomic, readonly) NSString * lastAuthenticateHeader;

#if OS_OBJECT_USE_OBJC
@property (nonatomic, retain) dispatch_queue_t dispatchQueue;
#else
@property (nonatomic, assign) dispatch_queue_t dispatchQueue;
#endif

@property (nonatomic, assign, readonly, getter=isOperationQueueRunning) BOOL operationQueueRunning;
@property (nonatomic, copy) MCOOperationQueueRunningChangeBlock operationQueueRunningChangeBlock;

- (void) cancelAllOperations;

- (MCOActiveSyncOperation *) connectOperation;
- (MCOActiveSyncOperation *) loginOperation;
- (MCOActiveSyncOperation *) loginOAuth2Operation;
- (MCOActiveSyncOperation *) setOAuth2TokenOnConnectionOperation;
- (MCOActiveSyncOptionsOperation *) optionsOperation;
- (MCOActiveSyncFolderSyncOperation *) folderSyncOperationWithSyncKey:(NSString *)syncKey;
- (MCOActiveSyncFolderSyncOperation *) folderResyncOperation;
- (MCOActiveSyncProvisionOperation *) provisionOperation;
- (MCOActiveSyncItemEstimateOperation *) itemEstimateOperationForCollectionID:(NSString *)collectionID syncKey:(NSString *)syncKey;
- (MCOActiveSyncFolderMutationOperation *) folderCreateOperationWithSyncKey:(NSString *)syncKey parentID:(NSString *)parentID displayName:(NSString *)displayName;
- (MCOActiveSyncFolderMutationOperation *) folderUpdateOperationWithSyncKey:(NSString *)syncKey folderID:(NSString *)folderID parentID:(NSString *)parentID displayName:(NSString *)displayName;
- (MCOActiveSyncFolderMutationOperation *) folderDeleteOperationWithSyncKey:(NSString *)syncKey folderID:(NSString *)folderID;
- (MCOActiveSyncSyncOperation *) syncOperationWithRequest:(MCOActiveSyncSyncRequest *)request;
- (MCOActiveSyncSyncOperation *) syncMessagesOperationInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey;
- (MCOActiveSyncSyncOperation *) markMessagesReadOperationInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageIDs:(NSArray *)messageIDs read:(BOOL)read;
- (MCOActiveSyncSyncOperation *) setMessagesFlaggedOperationInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageIDs:(NSArray *)messageIDs flagged:(BOOL)flagged;
- (MCOActiveSyncSyncOperation *) deleteMessagesOperationInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageIDs:(NSArray *)messageIDs deletesAsMoves:(BOOL)deletesAsMoves;
- (MCOActiveSyncSyncOperation *) markMessageReadOperationInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageID:(NSString *)messageID read:(BOOL)read;
- (MCOActiveSyncSyncOperation *) setMessageFlaggedOperationInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageID:(NSString *)messageID flagged:(BOOL)flagged;
- (MCOActiveSyncSyncOperation *) deleteMessageOperationInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageID:(NSString *)messageID deletesAsMoves:(BOOL)deletesAsMoves;
- (MCOActiveSyncMoveOperation *) moveMessagesOperation:(NSArray *)moves;
- (MCOActiveSyncFetchMessageOperation *) fetchMessageOperationInFolderID:(NSString *)folderID messageID:(NSString *)messageID;
- (MCOActiveSyncFetchMessageOperation *) fetchMessageBodyPartOperationInFolderID:(NSString *)folderID messageID:(NSString *)messageID bodyType:(MCOActiveSyncBodyType)bodyType truncationSize:(uint32_t)truncationSize;
- (MCOActiveSyncFetchAttachmentOperation *) fetchAttachmentOperationWithFileReference:(NSString *)fileReference range:(NSString *)range;
- (MCOActiveSyncOperation *) sendMessageOperationWithData:(NSData *)messageData saveInSent:(BOOL)saveInSent;
- (MCOActiveSyncOperation *) smartReplyOperationInFolderID:(NSString *)folderID messageID:(NSString *)messageID messageData:(NSData *)messageData saveInSent:(BOOL)saveInSent;
- (MCOActiveSyncOperation *) smartForwardOperationInFolderID:(NSString *)folderID messageID:(NSString *)messageID messageData:(NSData *)messageData saveInSent:(BOOL)saveInSent;
- (MCOActiveSyncPingOperation *) pingOperationWithCollectionIDs:(NSArray *)collectionIDs heartbeatInterval:(uint32_t)heartbeatInterval;

@end

#endif
