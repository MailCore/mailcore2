#ifndef MAILCORE_MCOACTIVESYNCSESSION_H

#define MAILCORE_MCOACTIVESYNCSESSION_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncOptions.h>
#import <MailCore/MCOActiveSyncFolderSyncResult.h>
#import <MailCore/MCOActiveSyncSyncRequest.h>
#import <MailCore/MCOActiveSyncSyncResult.h>
#import <MailCore/MCOActiveSyncProvisionResult.h>
#import <MailCore/MCOActiveSyncItemEstimateResult.h>
#import <MailCore/MCOActiveSyncMessage.h>
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
- (MCOActiveSyncSyncResult *) syncWithRequest:(MCOActiveSyncSyncRequest *)request error:(NSError **)error;
- (MCOActiveSyncSyncResult *) syncMessagesInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey error:(NSError **)error;
- (MCOActiveSyncProvisionResult *) provisionWithError:(NSError **)error;
- (MCOActiveSyncItemEstimateResult *) itemEstimateForCollectionID:(NSString *)collectionID syncKey:(NSString *)syncKey error:(NSError **)error;
- (MCOActiveSyncMessage *) fetchMessageInFolderID:(NSString *)folderID messageID:(NSString *)messageID error:(NSError **)error;
- (BOOL) sendMessageWithData:(NSData *)messageData saveInSent:(BOOL)saveInSent error:(NSError **)error;
- (BOOL) smartReplyInFolderID:(NSString *)folderID messageID:(NSString *)messageID messageData:(NSData *)messageData saveInSent:(BOOL)saveInSent error:(NSError **)error;
- (BOOL) smartForwardInFolderID:(NSString *)folderID messageID:(NSString *)messageID messageData:(NSData *)messageData saveInSent:(BOOL)saveInSent error:(NSError **)error;
- (MCOActiveSyncPingResult *) pingCollectionIDs:(NSArray *)collectionIDs heartbeatInterval:(uint32_t)heartbeatInterval error:(NSError **)error;
@end

#endif
