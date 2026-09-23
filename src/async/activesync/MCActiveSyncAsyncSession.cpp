#include "MCActiveSyncAsyncSession.h"

#include "MCActiveSyncOperation.h"
#include "MCActiveSyncOperations.h"
#include "MCActiveSyncSession.h"
#include "MCOperationQueueCallback.h"

using namespace mailcore;

namespace mailcore {

    class ActiveSyncOperationQueueCallback : public Object, public OperationQueueCallback {
    public:
        ActiveSyncOperationQueueCallback(ActiveSyncAsyncSession * session)
        {
            mSession = session;
        }

        virtual ~ActiveSyncOperationQueueCallback()
        {
        }

        virtual void queueStartRunning()
        {
            mSession->retain();
            if (mSession->operationQueueCallback() != NULL) {
                mSession->operationQueueCallback()->queueStartRunning();
            }
        }

        virtual void queueStoppedRunning()
        {
            if (mSession->operationQueueCallback() != NULL) {
                mSession->operationQueueCallback()->queueStoppedRunning();
            }
            mSession->release();
        }

    private:
        ActiveSyncAsyncSession * mSession;
    };

}

ActiveSyncAsyncSession::ActiveSyncAsyncSession()
{
    mSession = new ActiveSyncSession();
    mQueue = new OperationQueue();
    mQueueCallback = new ActiveSyncOperationQueueCallback(this);
    mQueue->setCallback(mQueueCallback);
    mOperationQueueCallback = NULL;
}

ActiveSyncAsyncSession::~ActiveSyncAsyncSession()
{
    MC_SAFE_RELEASE(mQueueCallback);
    MC_SAFE_RELEASE(mQueue);
    MC_SAFE_RELEASE(mSession);
}

void ActiveSyncAsyncSession::setServerURL(String * serverURL)
{
    mSession->setServerURL(serverURL);
}

String * ActiveSyncAsyncSession::serverURL()
{
    return mSession->serverURL();
}

void ActiveSyncAsyncSession::setUsername(String * username)
{
    mSession->setUsername(username);
}

String * ActiveSyncAsyncSession::username()
{
    return mSession->username();
}

void ActiveSyncAsyncSession::setPassword(String * password)
{
    mSession->setPassword(password);
}

String * ActiveSyncAsyncSession::password()
{
    return mSession->password();
}

void ActiveSyncAsyncSession::setOAuth2Token(String * token)
{
    mSession->setOAuth2Token(token);
}

String * ActiveSyncAsyncSession::OAuth2Token()
{
    return mSession->OAuth2Token();
}

void ActiveSyncAsyncSession::setDeviceID(String * deviceID)
{
    mSession->setDeviceID(deviceID);
}

String * ActiveSyncAsyncSession::deviceID()
{
    return mSession->deviceID();
}

String * ActiveSyncAsyncSession::lastRedirectURL()
{
    return mSession->lastRedirectURL();
}

String * ActiveSyncAsyncSession::lastAuthenticateHeader()
{
    return mSession->lastAuthenticateHeader();
}

#if __APPLE__
void ActiveSyncAsyncSession::setDispatchQueue(dispatch_queue_t dispatchQueue)
{
    mQueue->setDispatchQueue(dispatchQueue);
}

dispatch_queue_t ActiveSyncAsyncSession::dispatchQueue()
{
    return mQueue->dispatchQueue();
}
#endif

void ActiveSyncAsyncSession::setOperationQueueCallback(OperationQueueCallback * callback)
{
    mOperationQueueCallback = callback;
}

OperationQueueCallback * ActiveSyncAsyncSession::operationQueueCallback()
{
    return mOperationQueueCallback;
}

bool ActiveSyncAsyncSession::isOperationQueueRunning()
{
    return mQueue->count() > 0;
}

void ActiveSyncAsyncSession::cancelAllOperations()
{
    mQueue->cancelAllOperations();
}

void ActiveSyncAsyncSession::runOperation(ActiveSyncOperation * operation)
{
    mQueue->addOperation(operation);
}

ActiveSyncSession * ActiveSyncAsyncSession::session()
{
    return mSession;
}

ActiveSyncConnectOperation * ActiveSyncAsyncSession::connectOperation()
{
    ActiveSyncConnectOperation * op = new ActiveSyncConnectOperation();
    op->setSession(this);
    return (ActiveSyncConnectOperation *) op->autorelease();
}

ActiveSyncLoginOperation * ActiveSyncAsyncSession::loginOperation()
{
    ActiveSyncLoginOperation * op = new ActiveSyncLoginOperation();
    op->setSession(this);
    return (ActiveSyncLoginOperation *) op->autorelease();
}

ActiveSyncLoginOAuth2Operation * ActiveSyncAsyncSession::loginOAuth2Operation()
{
    ActiveSyncLoginOAuth2Operation * op = new ActiveSyncLoginOAuth2Operation();
    op->setSession(this);
    return (ActiveSyncLoginOAuth2Operation *) op->autorelease();
}

ActiveSyncSetOAuth2TokenOnConnectionOperation * ActiveSyncAsyncSession::setOAuth2TokenOnConnectionOperation()
{
    ActiveSyncSetOAuth2TokenOnConnectionOperation * op = new ActiveSyncSetOAuth2TokenOnConnectionOperation();
    op->setSession(this);
    return (ActiveSyncSetOAuth2TokenOnConnectionOperation *) op->autorelease();
}

ActiveSyncOptionsOperation * ActiveSyncAsyncSession::optionsOperation()
{
    ActiveSyncOptionsOperation * op = new ActiveSyncOptionsOperation();
    op->setSession(this);
    return (ActiveSyncOptionsOperation *) op->autorelease();
}

ActiveSyncFolderSyncOperation * ActiveSyncAsyncSession::folderSyncOperation(String * syncKey)
{
    ActiveSyncFolderSyncOperation * op = new ActiveSyncFolderSyncOperation();
    op->setSession(this);
    op->setSyncKey(syncKey);
    return (ActiveSyncFolderSyncOperation *) op->autorelease();
}

ActiveSyncFolderResyncOperation * ActiveSyncAsyncSession::folderResyncOperation()
{
    ActiveSyncFolderResyncOperation * op = new ActiveSyncFolderResyncOperation();
    op->setSession(this);
    return (ActiveSyncFolderResyncOperation *) op->autorelease();
}

ActiveSyncProvisionOperation * ActiveSyncAsyncSession::provisionOperation()
{
    ActiveSyncProvisionOperation * op = new ActiveSyncProvisionOperation();
    op->setSession(this);
    return (ActiveSyncProvisionOperation *) op->autorelease();
}

ActiveSyncItemEstimateOperation * ActiveSyncAsyncSession::itemEstimateOperation(String * collectionID, String * syncKey)
{
    ActiveSyncItemEstimateOperation * op = new ActiveSyncItemEstimateOperation();
    op->setSession(this);
    op->setCollectionID(collectionID);
    op->setSyncKey(syncKey);
    return (ActiveSyncItemEstimateOperation *) op->autorelease();
}

ActiveSyncFolderCreateOperation * ActiveSyncAsyncSession::folderCreateOperation(String * syncKey, String * parentID, String * displayName)
{
    ActiveSyncFolderCreateOperation * op = new ActiveSyncFolderCreateOperation();
    op->setSession(this);
    op->setSyncKey(syncKey);
    op->setParentID(parentID);
    op->setDisplayName(displayName);
    return (ActiveSyncFolderCreateOperation *) op->autorelease();
}

ActiveSyncFolderUpdateOperation * ActiveSyncAsyncSession::folderUpdateOperation(String * syncKey, String * folderID, String * parentID, String * displayName)
{
    ActiveSyncFolderUpdateOperation * op = new ActiveSyncFolderUpdateOperation();
    op->setSession(this);
    op->setSyncKey(syncKey);
    op->setFolderID(folderID);
    op->setParentID(parentID);
    op->setDisplayName(displayName);
    return (ActiveSyncFolderUpdateOperation *) op->autorelease();
}

ActiveSyncFolderDeleteOperation * ActiveSyncAsyncSession::folderDeleteOperation(String * syncKey, String * folderID)
{
    ActiveSyncFolderDeleteOperation * op = new ActiveSyncFolderDeleteOperation();
    op->setSession(this);
    op->setSyncKey(syncKey);
    op->setFolderID(folderID);
    return (ActiveSyncFolderDeleteOperation *) op->autorelease();
}

ActiveSyncSyncOperation * ActiveSyncAsyncSession::syncOperation(ActiveSyncSyncRequest * request)
{
    ActiveSyncSyncOperation * op = new ActiveSyncSyncOperation();
    op->setSession(this);
    op->setRequest(request);
    return (ActiveSyncSyncOperation *) op->autorelease();
}

ActiveSyncSyncMessagesOperation * ActiveSyncAsyncSession::syncMessagesOperation(String * folderID, String * syncKey)
{
    ActiveSyncSyncMessagesOperation * op = new ActiveSyncSyncMessagesOperation();
    op->setSession(this);
    op->setFolderID(folderID);
    op->setSyncKey(syncKey);
    return (ActiveSyncSyncMessagesOperation *) op->autorelease();
}

ActiveSyncMarkMessagesReadOperation * ActiveSyncAsyncSession::markMessagesReadOperation(String * folderID, String * syncKey, Array * messageIDs, bool read)
{
    ActiveSyncMarkMessagesReadOperation * op = new ActiveSyncMarkMessagesReadOperation();
    op->setSession(this);
    op->setFolderID(folderID);
    op->setSyncKey(syncKey);
    op->setMessageIDs(messageIDs);
    op->setValue(read);
    return (ActiveSyncMarkMessagesReadOperation *) op->autorelease();
}

ActiveSyncSetMessagesFlaggedOperation * ActiveSyncAsyncSession::setMessagesFlaggedOperation(String * folderID, String * syncKey, Array * messageIDs, bool flagged)
{
    ActiveSyncSetMessagesFlaggedOperation * op = new ActiveSyncSetMessagesFlaggedOperation();
    op->setSession(this);
    op->setFolderID(folderID);
    op->setSyncKey(syncKey);
    op->setMessageIDs(messageIDs);
    op->setValue(flagged);
    return (ActiveSyncSetMessagesFlaggedOperation *) op->autorelease();
}

ActiveSyncDeleteMessagesOperation * ActiveSyncAsyncSession::deleteMessagesOperation(String * folderID, String * syncKey, Array * messageIDs, bool deletesAsMoves)
{
    ActiveSyncDeleteMessagesOperation * op = new ActiveSyncDeleteMessagesOperation();
    op->setSession(this);
    op->setFolderID(folderID);
    op->setSyncKey(syncKey);
    op->setMessageIDs(messageIDs);
    op->setDeletesAsMoves(deletesAsMoves);
    return (ActiveSyncDeleteMessagesOperation *) op->autorelease();
}

ActiveSyncMarkMessageReadOperation * ActiveSyncAsyncSession::markMessageReadOperation(String * folderID, String * syncKey, String * messageID, bool read)
{
    ActiveSyncMarkMessageReadOperation * op = new ActiveSyncMarkMessageReadOperation();
    op->setSession(this);
    op->setFolderID(folderID);
    op->setSyncKey(syncKey);
    op->setMessageID(messageID);
    op->setValue(read);
    return (ActiveSyncMarkMessageReadOperation *) op->autorelease();
}

ActiveSyncSetMessageFlaggedOperation * ActiveSyncAsyncSession::setMessageFlaggedOperation(String * folderID, String * syncKey, String * messageID, bool flagged)
{
    ActiveSyncSetMessageFlaggedOperation * op = new ActiveSyncSetMessageFlaggedOperation();
    op->setSession(this);
    op->setFolderID(folderID);
    op->setSyncKey(syncKey);
    op->setMessageID(messageID);
    op->setValue(flagged);
    return (ActiveSyncSetMessageFlaggedOperation *) op->autorelease();
}

ActiveSyncDeleteMessageOperation * ActiveSyncAsyncSession::deleteMessageOperation(String * folderID, String * syncKey, String * messageID, bool deletesAsMoves)
{
    ActiveSyncDeleteMessageOperation * op = new ActiveSyncDeleteMessageOperation();
    op->setSession(this);
    op->setFolderID(folderID);
    op->setSyncKey(syncKey);
    op->setMessageID(messageID);
    op->setDeletesAsMoves(deletesAsMoves);
    return (ActiveSyncDeleteMessageOperation *) op->autorelease();
}

ActiveSyncMoveMessagesOperation * ActiveSyncAsyncSession::moveMessagesOperation(Array * moves)
{
    ActiveSyncMoveMessagesOperation * op = new ActiveSyncMoveMessagesOperation();
    op->setSession(this);
    op->setMoves(moves);
    return (ActiveSyncMoveMessagesOperation *) op->autorelease();
}

ActiveSyncFetchMessageOperation * ActiveSyncAsyncSession::fetchMessageOperation(String * folderID, String * messageID)
{
    ActiveSyncFetchMessageOperation * op = new ActiveSyncFetchMessageOperation();
    op->setSession(this);
    op->setFolderID(folderID);
    op->setMessageID(messageID);
    return (ActiveSyncFetchMessageOperation *) op->autorelease();
}

ActiveSyncFetchMessageBodyPartOperation * ActiveSyncAsyncSession::fetchMessageBodyPartOperation(String * folderID, String * messageID, ActiveSyncBodyType bodyType, uint32_t truncationSize)
{
    ActiveSyncFetchMessageBodyPartOperation * op = new ActiveSyncFetchMessageBodyPartOperation();
    op->setSession(this);
    op->setFolderID(folderID);
    op->setMessageID(messageID);
    op->setBodyType(bodyType);
    op->setTruncationSize(truncationSize);
    return (ActiveSyncFetchMessageBodyPartOperation *) op->autorelease();
}

ActiveSyncFetchAttachmentOperation * ActiveSyncAsyncSession::fetchAttachmentOperation(String * fileReference, String * range)
{
    ActiveSyncFetchAttachmentOperation * op = new ActiveSyncFetchAttachmentOperation();
    op->setSession(this);
    op->setFileReference(fileReference);
    op->setRange(range);
    return (ActiveSyncFetchAttachmentOperation *) op->autorelease();
}

ActiveSyncSendMessageOperation * ActiveSyncAsyncSession::sendMessageOperation(Data * messageData, bool saveInSent)
{
    ActiveSyncSendMessageOperation * op = new ActiveSyncSendMessageOperation();
    op->setSession(this);
    op->setMessageData(messageData);
    op->setSaveInSent(saveInSent);
    return (ActiveSyncSendMessageOperation *) op->autorelease();
}

ActiveSyncSmartReplyOperation * ActiveSyncAsyncSession::smartReplyOperation(String * folderID, String * messageID, Data * messageData, bool saveInSent)
{
    ActiveSyncSmartReplyOperation * op = new ActiveSyncSmartReplyOperation();
    op->setSession(this);
    op->setFolderID(folderID);
    op->setMessageID(messageID);
    op->setMessageData(messageData);
    op->setSaveInSent(saveInSent);
    return (ActiveSyncSmartReplyOperation *) op->autorelease();
}

ActiveSyncSmartForwardOperation * ActiveSyncAsyncSession::smartForwardOperation(String * folderID, String * messageID, Data * messageData, bool saveInSent)
{
    ActiveSyncSmartForwardOperation * op = new ActiveSyncSmartForwardOperation();
    op->setSession(this);
    op->setFolderID(folderID);
    op->setMessageID(messageID);
    op->setMessageData(messageData);
    op->setSaveInSent(saveInSent);
    return (ActiveSyncSmartForwardOperation *) op->autorelease();
}

ActiveSyncPingOperation * ActiveSyncAsyncSession::pingOperation(Array * collectionIDs, uint32_t heartbeatInterval)
{
    ActiveSyncPingOperation * op = new ActiveSyncPingOperation();
    op->setSession(this);
    op->setCollectionIDs(collectionIDs);
    op->setHeartbeatInterval(heartbeatInterval);
    return (ActiveSyncPingOperation *) op->autorelease();
}
