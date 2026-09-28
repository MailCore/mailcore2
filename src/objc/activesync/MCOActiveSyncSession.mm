#import "MCOActiveSyncPrivate.h"
#import "MCOActiveSyncOperation+Private.h"

#include "MCAsyncActiveSync.h"
#include "MCOperationQueueCallback.h"

using namespace mailcore;

@interface MCOActiveSyncSession ()
- (void) _queueRunningChanged;
@end

class MCOActiveSyncCallbackBridge : public Object, public OperationQueueCallback {
public:
    MCOActiveSyncCallbackBridge(MCOActiveSyncSession * session)
    {
        mSession = session;
    }

    virtual void queueStartRunning()
    {
        [mSession _queueRunningChanged];
    }

    virtual void queueStoppedRunning()
    {
        [mSession _queueRunningChanged];
    }

private:
    MCOActiveSyncSession * mSession;
};

@implementation MCOActiveSyncSession {
    mailcore::ActiveSyncAsyncSession * _session;
    MCOActiveSyncCallbackBridge * _callbackBridge;
    MCOOperationQueueRunningChangeBlock _operationQueueRunningChangeBlock;
}

#define nativeType mailcore::ActiveSyncAsyncSession

- (instancetype) init
{
    self = [super init];
    _session = new mailcore::ActiveSyncAsyncSession();
    _callbackBridge = new MCOActiveSyncCallbackBridge(self);
    return self;
}

- (void) dealloc
{
    [_operationQueueRunningChangeBlock release];
    _session->setOperationQueueCallback(NULL);
    MC_SAFE_RELEASE(_callbackBridge);
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
MCO_OBJC_SYNTHESIZE_SCALAR(dispatch_queue_t, dispatch_queue_t, setDispatchQueue, dispatchQueue)

- (NSString *) lastRedirectURL
{
    return MCO_OBJC_BRIDGE_GET(lastRedirectURL);
}

- (NSString *) lastAuthenticateHeader
{
    return MCO_OBJC_BRIDGE_GET(lastAuthenticateHeader);
}

- (void) setOperationQueueRunningChangeBlock:(MCOOperationQueueRunningChangeBlock)operationQueueRunningChangeBlock
{
    [_operationQueueRunningChangeBlock release];
    _operationQueueRunningChangeBlock = [operationQueueRunningChangeBlock copy];

    if (_operationQueueRunningChangeBlock != nil) {
        _session->setOperationQueueCallback(_callbackBridge);
    }
    else {
        _session->setOperationQueueCallback(NULL);
    }
}

- (MCOOperationQueueRunningChangeBlock) operationQueueRunningChangeBlock
{
    return _operationQueueRunningChangeBlock;
}

- (BOOL) isOperationQueueRunning
{
    return _session->isOperationQueueRunning();
}

- (void) cancelAllOperations
{
    _session->cancelAllOperations();
}

- (void) _queueRunningChanged
{
    if (_operationQueueRunningChangeBlock == NULL)
        return;

    _operationQueueRunningChangeBlock();
}

#pragma mark - Operations

- (id) _objcOperationFromNativeOp:(mailcore::ActiveSyncOperation *)op
{
    MCOActiveSyncOperation * result = MCO_TO_OBJC(op);
    [result setSession:self];
    return result;
}

- (id) _objcOpaqueOperationFromNativeOp:(mailcore::ActiveSyncOperation *)op
{
    MCOActiveSyncOperation * result = [[[MCOActiveSyncOperation alloc] initWithMCOperation:op] autorelease];
    [result setSession:self];
    return result;
}

- (MCOActiveSyncOperation *) connectOperation
{
    return [self _objcOpaqueOperationFromNativeOp:_session->connectOperation()];
}

- (MCOActiveSyncOperation *) loginOperation
{
    return [self _objcOpaqueOperationFromNativeOp:_session->loginOperation()];
}

- (MCOActiveSyncOperation *) loginOAuth2Operation
{
    return [self _objcOpaqueOperationFromNativeOp:_session->loginOAuth2Operation()];
}

- (MCOActiveSyncOperation *) setOAuth2TokenOnConnectionOperation
{
    return [self _objcOpaqueOperationFromNativeOp:_session->setOAuth2TokenOnConnectionOperation()];
}

- (MCOActiveSyncOptionsOperation *) optionsOperation
{
    return [self _objcOperationFromNativeOp:_session->optionsOperation()];
}

- (MCOActiveSyncFolderSyncOperation *) folderSyncOperationWithSyncKey:(NSString *)syncKey
{
    return [self _objcOperationFromNativeOp:_session->folderSyncOperation([syncKey mco_mcString])];
}

- (MCOActiveSyncFolderSyncOperation *) folderResyncOperation
{
    return [self _objcOperationFromNativeOp:_session->folderResyncOperation()];
}

- (MCOActiveSyncProvisionOperation *) provisionOperation
{
    return [self _objcOperationFromNativeOp:_session->provisionOperation()];
}

- (MCOActiveSyncItemEstimateOperation *) itemEstimateOperationForCollectionID:(NSString *)collectionID syncKey:(NSString *)syncKey
{
    return [self _objcOperationFromNativeOp:_session->itemEstimateOperation([collectionID mco_mcString], [syncKey mco_mcString])];
}

- (MCOActiveSyncItemEstimateOperation *) itemEstimateOperationForFolderID:(NSString *)folderID syncKey:(NSString *)syncKey
{
    return [self _objcOperationFromNativeOp:_session->itemEstimateOperationForFolderID([folderID mco_mcString], [syncKey mco_mcString])];
}

- (MCOActiveSyncFolderMutationOperation *) folderCreateOperationWithSyncKey:(NSString *)syncKey parentID:(NSString *)parentID displayName:(NSString *)displayName
{
    return [self _objcOperationFromNativeOp:_session->folderCreateOperation([syncKey mco_mcString], [parentID mco_mcString], [displayName mco_mcString])];
}

- (MCOActiveSyncFolderMutationOperation *) folderUpdateOperationWithSyncKey:(NSString *)syncKey folderID:(NSString *)folderID parentID:(NSString *)parentID displayName:(NSString *)displayName
{
    return [self _objcOperationFromNativeOp:_session->folderUpdateOperation([syncKey mco_mcString], [folderID mco_mcString], [parentID mco_mcString], [displayName mco_mcString])];
}

- (MCOActiveSyncFolderMutationOperation *) folderDeleteOperationWithSyncKey:(NSString *)syncKey folderID:(NSString *)folderID
{
    return [self _objcOperationFromNativeOp:_session->folderDeleteOperation([syncKey mco_mcString], [folderID mco_mcString])];
}

- (MCOActiveSyncSyncOperation *) syncMessagesOperationInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey
{
    return [self _objcOperationFromNativeOp:_session->syncMessagesOperation([folderID mco_mcString], [syncKey mco_mcString])];
}

- (MCOActiveSyncSyncOperation *) markMessagesReadOperationInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageIDs:(NSArray *)messageIDs read:(BOOL)read
{
    return [self _objcOperationFromNativeOp:_session->markMessagesReadOperation([folderID mco_mcString], [syncKey mco_mcString], (mailcore::Array *) [messageIDs mco_mcObject], read)];
}

- (MCOActiveSyncSyncOperation *) setMessagesFlaggedOperationInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageIDs:(NSArray *)messageIDs flagged:(BOOL)flagged
{
    return [self _objcOperationFromNativeOp:_session->setMessagesFlaggedOperation([folderID mco_mcString], [syncKey mco_mcString], (mailcore::Array *) [messageIDs mco_mcObject], flagged)];
}

- (MCOActiveSyncSyncOperation *) deleteMessagesOperationInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageIDs:(NSArray *)messageIDs deletesAsMoves:(BOOL)deletesAsMoves
{
    return [self _objcOperationFromNativeOp:_session->deleteMessagesOperation([folderID mco_mcString], [syncKey mco_mcString], (mailcore::Array *) [messageIDs mco_mcObject], deletesAsMoves)];
}

- (MCOActiveSyncSyncOperation *) markMessageReadOperationInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageID:(NSString *)messageID read:(BOOL)read
{
    return [self _objcOperationFromNativeOp:_session->markMessageReadOperation([folderID mco_mcString], [syncKey mco_mcString], [messageID mco_mcString], read)];
}

- (MCOActiveSyncSyncOperation *) setMessageFlaggedOperationInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageID:(NSString *)messageID flagged:(BOOL)flagged
{
    return [self _objcOperationFromNativeOp:_session->setMessageFlaggedOperation([folderID mco_mcString], [syncKey mco_mcString], [messageID mco_mcString], flagged)];
}

- (MCOActiveSyncSyncOperation *) deleteMessageOperationInFolderID:(NSString *)folderID syncKey:(NSString *)syncKey messageID:(NSString *)messageID deletesAsMoves:(BOOL)deletesAsMoves
{
    return [self _objcOperationFromNativeOp:_session->deleteMessageOperation([folderID mco_mcString], [syncKey mco_mcString], [messageID mco_mcString], deletesAsMoves)];
}

- (MCOActiveSyncMoveOperation *) moveMessagesOperation:(NSArray *)moves
{
    return [self _objcOperationFromNativeOp:_session->moveMessagesOperation((mailcore::Array *) [moves mco_mcObject])];
}

- (MCOActiveSyncFetchMessageOperation *) fetchMessageOperationInFolderID:(NSString *)folderID messageID:(NSString *)messageID
{
    return [self _objcOperationFromNativeOp:_session->fetchMessageOperation([folderID mco_mcString], [messageID mco_mcString])];
}

- (MCOActiveSyncFetchMessageOperation *) fetchMessageBodyPartOperationInFolderID:(NSString *)folderID messageID:(NSString *)messageID bodyType:(MCOActiveSyncBodyType)bodyType truncationSize:(uint32_t)truncationSize
{
    return [self _objcOperationFromNativeOp:_session->fetchMessageBodyPartOperation([folderID mco_mcString], [messageID mco_mcString], (mailcore::ActiveSyncBodyType) bodyType, truncationSize)];
}

- (MCOActiveSyncFetchAttachmentOperation *) fetchAttachmentOperationWithFileReference:(NSString *)fileReference range:(NSString *)range
{
    return [self _objcOperationFromNativeOp:_session->fetchAttachmentOperation([fileReference mco_mcString], [range mco_mcString])];
}

- (MCOActiveSyncOperation *) sendMessageOperationWithData:(NSData *)messageData saveInSent:(BOOL)saveInSent
{
    return [self _objcOpaqueOperationFromNativeOp:_session->sendMessageOperation([messageData mco_mcData], saveInSent)];
}

- (MCOActiveSyncOperation *) smartReplyOperationInFolderID:(NSString *)folderID messageID:(NSString *)messageID messageData:(NSData *)messageData saveInSent:(BOOL)saveInSent
{
    return [self _objcOpaqueOperationFromNativeOp:_session->smartReplyOperation([folderID mco_mcString], [messageID mco_mcString], [messageData mco_mcData], saveInSent)];
}

- (MCOActiveSyncOperation *) smartForwardOperationInFolderID:(NSString *)folderID messageID:(NSString *)messageID messageData:(NSData *)messageData saveInSent:(BOOL)saveInSent
{
    return [self _objcOpaqueOperationFromNativeOp:_session->smartForwardOperation([folderID mco_mcString], [messageID mco_mcString], [messageData mco_mcData], saveInSent)];
}

- (MCOActiveSyncPingOperation *) pingOperationWithCollectionIDs:(NSArray *)collectionIDs heartbeatInterval:(uint32_t)heartbeatInterval
{
    return [self _objcOperationFromNativeOp:_session->pingOperation((mailcore::Array *) [collectionIDs mco_mcObject], heartbeatInterval)];
}

- (MCOActiveSyncPingOperation *) pingOperationWithFolderIDs:(NSArray *)folderIDs heartbeatInterval:(uint32_t)heartbeatInterval
{
    return [self _objcOperationFromNativeOp:_session->pingOperationWithFolderIDs((mailcore::Array *) [folderIDs mco_mcObject], heartbeatInterval)];
}

@end

#undef nativeType
