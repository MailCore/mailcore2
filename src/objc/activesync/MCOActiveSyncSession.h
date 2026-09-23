#ifndef MAILCORE_MCOACTIVESYNCSESSION_H

#define MAILCORE_MCOACTIVESYNCSESSION_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncOptions.h>
#import <MailCore/MCOActiveSyncFolderSyncResult.h>
#import <MailCore/MCOActiveSyncFolderMutationResult.h>
#import <MailCore/MCOActiveSyncSyncRequest.h>
#import <MailCore/MCOActiveSyncSyncResult.h>
#import <MailCore/MCOActiveSyncProvisionResult.h>
#import <MailCore/MCOActiveSyncItemEstimateResult.h>
#import <MailCore/MCOActiveSyncMessage.h>
#import <MailCore/MCOActiveSyncMoveResult.h>
#import <MailCore/MCOActiveSyncAttachmentData.h>
#import <MailCore/MCOActiveSyncPingResult.h>

@interface MCOActiveSyncSession : NSObject
@property (nonatomic, copy) NSString * serverURL;
@property (nonatomic, copy) NSString * username;
@property (nonatomic, copy) NSString * password;
@property (nonatomic, copy) NSString * OAuth2Token;
@property (nonatomic, copy) NSString * deviceID;
@property (nonatomic, readonly) NSString * lastRedirectURL;
@property (nonatomic, readonly) NSString * lastAuthenticateHeader;

- (BOOL) connectWithError:(NSError **)error;
- (BOOL) loginWithError:(NSError **)error;
- (BOOL) loginOAuth2WithError:(NSError **)error;
- (BOOL) setOAuth2TokenOnConnectionWithError:(NSError **)error;
- (MCOActiveSyncOptions *) optionsWithError:(NSError **)error;
- (MCOActiveSyncFolderSyncResult *) folderSyncWithSyncKey:(NSString *)syncKey error:(NSError **)error;
- (MCOActiveSyncFolderSyncResult *) folderResyncWithError:(NSError **)error;
- (MCOActiveSyncFolderMutationResult *) folderCreateWithSyncKey:(NSString *)syncKey parentID:(NSString *)parentID displayName:(NSString *)displayName error:(NSError **)error;
- (MCOActiveSyncFolderMutationResult *) folderUpdateWithSyncKey:(NSString *)syncKey folderID:(NSString *)folderID parentID:(NSString *)parentID displayName:(NSString *)displayName error:(NSError **)error;
- (MCOActiveSyncFolderMutationResult *) folderDeleteWithSyncKey:(NSString *)syncKey folderID:(NSString *)folderID error:(NSError **)error;
- (MCOActiveSyncSyncResult *) syncWithRequest:(MCOActiveSyncSyncRequest *)request error:(NSError **)error;
- (MCOActiveSyncSyncResult *) syncMessagesInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey error:(NSError **)error;
- (MCOActiveSyncSyncResult *) markMessagesReadInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageIDs:(NSArray *)messageIDs read:(BOOL)read error:(NSError **)error;
- (MCOActiveSyncSyncResult *) setMessagesFlaggedInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageIDs:(NSArray *)messageIDs flagged:(BOOL)flagged error:(NSError **)error;
- (MCOActiveSyncSyncResult *) deleteMessagesInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageIDs:(NSArray *)messageIDs deletesAsMoves:(BOOL)deletesAsMoves error:(NSError **)error;
- (MCOActiveSyncSyncResult *) markMessageReadInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageID:(NSString *)messageID read:(BOOL)read error:(NSError **)error;
- (MCOActiveSyncSyncResult *) setMessageFlaggedInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageID:(NSString *)messageID flagged:(BOOL)flagged error:(NSError **)error;
- (MCOActiveSyncSyncResult *) deleteMessageInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageID:(NSString *)messageID deletesAsMoves:(BOOL)deletesAsMoves error:(NSError **)error;
- (MCOActiveSyncMoveResult *) moveMessages:(NSArray *)moves error:(NSError **)error;
- (MCOActiveSyncProvisionResult *) provisionWithError:(NSError **)error;
- (MCOActiveSyncItemEstimateResult *) itemEstimateForCollectionID:(NSString *)collectionID syncKey:(NSString *)syncKey error:(NSError **)error;
- (MCOActiveSyncMessage *) fetchMessageInFolderID:(NSString *)folderID messageID:(NSString *)messageID error:(NSError **)error;
- (MCOActiveSyncMessage *) fetchMessageBodyPartInFolderID:(NSString *)folderID messageID:(NSString *)messageID bodyType:(MCOActiveSyncBodyType)bodyType truncationSize:(uint32_t)truncationSize error:(NSError **)error;
- (MCOActiveSyncAttachmentData *) fetchAttachmentWithFileReference:(NSString *)fileReference range:(NSString *)range error:(NSError **)error;
- (BOOL) sendMessageWithData:(NSData *)messageData saveInSent:(BOOL)saveInSent error:(NSError **)error;
- (BOOL) smartReplyInFolderID:(NSString *)folderID messageID:(NSString *)messageID messageData:(NSData *)messageData saveInSent:(BOOL)saveInSent error:(NSError **)error;
- (BOOL) smartForwardInFolderID:(NSString *)folderID messageID:(NSString *)messageID messageData:(NSData *)messageData saveInSent:(BOOL)saveInSent error:(NSError **)error;
- (MCOActiveSyncPingResult *) pingCollectionIDs:(NSArray *)collectionIDs heartbeatInterval:(uint32_t)heartbeatInterval error:(NSError **)error;
@end

#endif
