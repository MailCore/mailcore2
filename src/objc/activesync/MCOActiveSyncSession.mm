#import "MCOActiveSyncPrivate.h"

static BOOL MCOSetError(NSError ** error, mailcore::ErrorCode errorCode)
{
    if (error != NULL)
        * error = [NSError mco_errorWithErrorCode:errorCode];
    return errorCode == mailcore::ErrorNone;
}

@implementation MCOActiveSyncSession {
    mailcore::ActiveSyncSession * _session;
}

#define nativeType mailcore::ActiveSyncSession

- (instancetype) init
{
    self = [super init];
    _session = new mailcore::ActiveSyncSession();
    return self;
}

- (void) dealloc
{
    MC_SAFE_RELEASE(_session);
    [super dealloc];
}

- (mailcore::Object *) mco_mcObject
{
    return _session;
}

MCO_OBJC_SYNTHESIZE_STRING(setServerURL, serverURL)
MCO_OBJC_SYNTHESIZE_STRING(setUsername, username)
MCO_OBJC_SYNTHESIZE_STRING(setPassword, password)
MCO_OBJC_SYNTHESIZE_STRING(setOAuth2Token, OAuth2Token)
MCO_OBJC_SYNTHESIZE_STRING(setDeviceID, deviceID)

- (NSString *) lastRedirectURL
{
    return MCO_OBJC_BRIDGE_GET(lastRedirectURL);
}

- (NSString *) lastAuthenticateHeader
{
    return MCO_OBJC_BRIDGE_GET(lastAuthenticateHeader);
}

- (BOOL) connectWithError:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    _session->connect(&errorCode);
    return MCOSetError(error, errorCode);
}

- (BOOL) loginWithError:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    _session->login(&errorCode);
    return MCOSetError(error, errorCode);
}

- (BOOL) loginOAuth2WithError:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    _session->loginOAuth2(&errorCode);
    return MCOSetError(error, errorCode);
}

- (BOOL) setOAuth2TokenOnConnectionWithError:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    _session->setOAuth2TokenOnConnection(&errorCode);
    return MCOSetError(error, errorCode);
}

- (MCOActiveSyncOptions *) optionsWithError:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncOptions * result = _session->options(&errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncOptions *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncFolderSyncResult *) folderSyncWithSyncKey:(NSString *)syncKey error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncFolderSyncResult * result = _session->folderSync([syncKey mco_mcString], &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncFolderSyncResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncFolderSyncResult *) folderResyncWithError:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncFolderSyncResult * result = _session->folderResync(&errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncFolderSyncResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncFolderMutationResult *) folderCreateWithSyncKey:(NSString *)syncKey parentID:(NSString *)parentID displayName:(NSString *)displayName error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncFolderMutationResult * result = _session->folderCreate([syncKey mco_mcString], [parentID mco_mcString], [displayName mco_mcString], &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncFolderMutationResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncFolderMutationResult *) folderUpdateWithSyncKey:(NSString *)syncKey folderID:(NSString *)folderID parentID:(NSString *)parentID displayName:(NSString *)displayName error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncFolderMutationResult * result = _session->folderUpdate([syncKey mco_mcString], [folderID mco_mcString], [parentID mco_mcString], [displayName mco_mcString], &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncFolderMutationResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncFolderMutationResult *) folderDeleteWithSyncKey:(NSString *)syncKey folderID:(NSString *)folderID error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncFolderMutationResult * result = _session->folderDelete([syncKey mco_mcString], [folderID mco_mcString], &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncFolderMutationResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncSyncResult *) syncWithRequest:(MCOActiveSyncSyncRequest *)request error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncSyncResult * result = _session->sync((mailcore::ActiveSyncSyncRequest *) [request mco_mcObject], &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncSyncResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncSyncResult *) syncMessagesInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncSyncResult * result = _session->syncMessages([folderID mco_mcString], [syncKey mco_mcString], &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncSyncResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncSyncResult *) markMessagesReadInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageIDs:(NSArray *)messageIDs read:(BOOL)read error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncSyncResult * result = _session->markMessagesRead([folderID mco_mcString], [syncKey mco_mcString], (mailcore::Array *) [messageIDs mco_mcObject], read, &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncSyncResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncSyncResult *) setMessagesFlaggedInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageIDs:(NSArray *)messageIDs flagged:(BOOL)flagged error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncSyncResult * result = _session->setMessagesFlagged([folderID mco_mcString], [syncKey mco_mcString], (mailcore::Array *) [messageIDs mco_mcObject], flagged, &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncSyncResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncSyncResult *) deleteMessagesInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageIDs:(NSArray *)messageIDs deletesAsMoves:(BOOL)deletesAsMoves error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncSyncResult * result = _session->deleteMessages([folderID mco_mcString], [syncKey mco_mcString], (mailcore::Array *) [messageIDs mco_mcObject], deletesAsMoves, &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncSyncResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncSyncResult *) markMessageReadInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageID:(NSString *)messageID read:(BOOL)read error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncSyncResult * result = _session->markMessageRead([folderID mco_mcString], [syncKey mco_mcString], [messageID mco_mcString], read, &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncSyncResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncSyncResult *) setMessageFlaggedInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageID:(NSString *)messageID flagged:(BOOL)flagged error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncSyncResult * result = _session->setMessageFlagged([folderID mco_mcString], [syncKey mco_mcString], [messageID mco_mcString], flagged, &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncSyncResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncSyncResult *) deleteMessageInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageID:(NSString *)messageID deletesAsMoves:(BOOL)deletesAsMoves error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncSyncResult * result = _session->deleteMessage([folderID mco_mcString], [syncKey mco_mcString], [messageID mco_mcString], deletesAsMoves, &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncSyncResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncMoveResult *) moveMessages:(NSArray *)moves error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncMoveResult * result = _session->moveMessages((mailcore::Array *) [moves mco_mcObject], &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncMoveResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncProvisionResult *) provisionWithError:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncProvisionResult * result = _session->provision(&errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncProvisionResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncItemEstimateResult *) itemEstimateForCollectionID:(NSString *)collectionID syncKey:(NSString *)syncKey error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncItemEstimateResult * result = _session->itemEstimate([collectionID mco_mcString], [syncKey mco_mcString], &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncItemEstimateResult *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncMessage *) fetchMessageInFolderID:(NSString *)folderID messageID:(NSString *)messageID error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncMessage * result = _session->fetchMessage([folderID mco_mcString], [messageID mco_mcString], &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncMessage *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncMessage *) fetchMessageBodyPartInFolderID:(NSString *)folderID messageID:(NSString *)messageID bodyType:(MCOActiveSyncBodyType)bodyType truncationSize:(uint32_t)truncationSize error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncMessage * result = _session->fetchMessageBodyPart([folderID mco_mcString], [messageID mco_mcString], (mailcore::ActiveSyncBodyType) bodyType, truncationSize, &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncMessage *) MCO_TO_OBJC(result);
}

- (MCOActiveSyncAttachmentData *) fetchAttachmentWithFileReference:(NSString *)fileReference range:(NSString *)range error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncAttachmentData * result = _session->fetchAttachment([fileReference mco_mcString], [range mco_mcString], &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncAttachmentData *) MCO_TO_OBJC(result);
}

- (BOOL) sendMessageWithData:(NSData *)messageData saveInSent:(BOOL)saveInSent error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    _session->sendMessage([messageData mco_mcData], saveInSent, &errorCode);
    return MCOSetError(error, errorCode);
}

- (BOOL) smartReplyInFolderID:(NSString *)folderID messageID:(NSString *)messageID messageData:(NSData *)messageData saveInSent:(BOOL)saveInSent error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    _session->smartReply([folderID mco_mcString], [messageID mco_mcString], [messageData mco_mcData], saveInSent, &errorCode);
    return MCOSetError(error, errorCode);
}

- (BOOL) smartForwardInFolderID:(NSString *)folderID messageID:(NSString *)messageID messageData:(NSData *)messageData saveInSent:(BOOL)saveInSent error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    _session->smartForward([folderID mco_mcString], [messageID mco_mcString], [messageData mco_mcData], saveInSent, &errorCode);
    return MCOSetError(error, errorCode);
}

- (MCOActiveSyncPingResult *) pingCollectionIDs:(NSArray *)collectionIDs heartbeatInterval:(uint32_t)heartbeatInterval error:(NSError **)error
{
    mailcore::ErrorCode errorCode = mailcore::ErrorNone;
    mailcore::ActiveSyncPingResult * result = _session->ping((mailcore::Array *) [collectionIDs mco_mcObject], heartbeatInterval, &errorCode);
    MCOSetError(error, errorCode);
    return (MCOActiveSyncPingResult *) MCO_TO_OBJC(result);
}

@end

#undef nativeType
