#ifndef MAILCORE_MCOACTIVESYNCSESSION_H

#define MAILCORE_MCOACTIVESYNCSESSION_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncOptions.h>
#import <MailCore/MCOActiveSyncFolderSyncResult.h>
#import <MailCore/MCOActiveSyncSyncRequest.h>
#import <MailCore/MCOActiveSyncSyncResult.h>
#import <MailCore/MCOActiveSyncProvisionResult.h>
#import <MailCore/MCOActiveSyncSettingsResult.h>
#import <MailCore/MCOActiveSyncItemEstimateResult.h>
#import <MailCore/MCOActiveSyncMessage.h>
#import <MailCore/MCOActiveSyncPingResult.h>

@interface MCOActiveSyncSession : NSObject
@property (nonatomic, copy) NSString * serverURL;
@property (nonatomic, copy) NSString * username;
@property (nonatomic, copy) NSString * password;
@property (nonatomic, copy) NSString * OAuth2Token;
@property (nonatomic, copy) NSString * deviceID;
@property (nonatomic, copy) NSString * deviceType;
@property (nonatomic, copy) NSString * protocolVersion;
@property (nonatomic, copy) NSString * policyKey;
@property (nonatomic, copy) NSString * userAgent;
@property (nonatomic, assign, getter=isCached) BOOL cached;
@property (nonatomic, copy) NSString * cacheDirectory;
@property (nonatomic, readonly) NSString * lastRedirectURL;
@property (nonatomic, readonly) NSString * lastAuthenticateHeader;

- (BOOL) connectWithError:(NSError **)error;
- (BOOL) loginWithError:(NSError **)error;
- (BOOL) loginOAuth2WithError:(NSError **)error;
- (BOOL) setOAuth2TokenOnConnectionWithError:(NSError **)error;
- (MCOActiveSyncOptions *) optionsWithError:(NSError **)error;
- (MCOActiveSyncFolderSyncResult *) folderSyncWithSyncKey:(NSString *)syncKey error:(NSError **)error;
- (MCOActiveSyncSyncResult *) syncWithRequest:(MCOActiveSyncSyncRequest *)request error:(NSError **)error;
- (MCOActiveSyncProvisionResult *) provisionWithError:(NSError **)error;
- (MCOActiveSyncSettingsResult *) setDeviceInformation:(NSDictionary *)deviceInformation error:(NSError **)error;
- (MCOActiveSyncItemEstimateResult *) itemEstimateForCollectionID:(NSString *)collectionID syncKey:(NSString *)syncKey error:(NSError **)error;
- (MCOActiveSyncMessage *) fetchItemInCollectionID:(NSString *)collectionID serverID:(NSString *)serverID error:(NSError **)error;
- (BOOL) sendMailWithMIMEData:(NSData *)MIMEData saveInSent:(BOOL)saveInSent error:(NSError **)error;
- (BOOL) smartReplyInCollectionID:(NSString *)collectionID serverID:(NSString *)serverID MIMEData:(NSData *)MIMEData saveInSent:(BOOL)saveInSent error:(NSError **)error;
- (BOOL) smartForwardInCollectionID:(NSString *)collectionID serverID:(NSString *)serverID MIMEData:(NSData *)MIMEData saveInSent:(BOOL)saveInSent error:(NSError **)error;
- (MCOActiveSyncPingResult *) pingCollectionIDs:(NSArray *)collectionIDs heartbeatInterval:(uint32_t)heartbeatInterval error:(NSError **)error;
@end

#endif
