#ifndef MAILCORE_MCACTIVESYNCASYNCSESSION_H

#define MAILCORE_MCACTIVESYNCASYNCSESSION_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class ActiveSyncSession;
    class ActiveSyncOperation;
    class ActiveSyncConnectOperation;
    class ActiveSyncLoginOperation;
    class ActiveSyncLoginOAuth2Operation;
    class ActiveSyncSetOAuth2TokenOnConnectionOperation;
    class ActiveSyncOptionsOperation;
    class ActiveSyncFolderSyncOperation;
    class ActiveSyncFolderResyncOperation;
    class ActiveSyncProvisionOperation;
    class ActiveSyncItemEstimateOperation;
    class ActiveSyncFolderCreateOperation;
    class ActiveSyncFolderUpdateOperation;
    class ActiveSyncFolderDeleteOperation;
    class ActiveSyncSyncMessagesOperation;
    class ActiveSyncMarkMessagesReadOperation;
    class ActiveSyncSetMessagesFlaggedOperation;
    class ActiveSyncDeleteMessagesOperation;
    class ActiveSyncMarkMessageReadOperation;
    class ActiveSyncSetMessageFlaggedOperation;
    class ActiveSyncDeleteMessageOperation;
    class ActiveSyncMoveMessagesOperation;
    class ActiveSyncFetchMessageOperation;
    class ActiveSyncFetchMessageBodyPartOperation;
    class ActiveSyncFetchAttachmentOperation;
    class ActiveSyncSendMessageOperation;
    class ActiveSyncSmartReplyOperation;
    class ActiveSyncSmartForwardOperation;
    class ActiveSyncPingOperation;
    class ActiveSyncOperationQueueCallback;
    class Array;
    class Data;

    class MAILCORE_EXPORT ActiveSyncAsyncSession : public Object {
    public:
        ActiveSyncAsyncSession();
        virtual ~ActiveSyncAsyncSession();

        virtual void setServerURL(String * serverURL);
        virtual String * serverURL();
        virtual void setUsername(String * username);
        virtual String * username();
        virtual void setPassword(String * password);
        virtual String * password();
        virtual void setOAuth2Token(String * token);
        virtual String * OAuth2Token();
        virtual void setDeviceID(String * deviceID);
        virtual String * deviceID();

        virtual String * lastRedirectURL();
        virtual String * lastAuthenticateHeader();

#ifdef __APPLE__
        virtual void setDispatchQueue(dispatch_queue_t dispatchQueue);
        virtual dispatch_queue_t dispatchQueue();
#endif

        virtual void setOperationQueueCallback(OperationQueueCallback * callback);
        virtual OperationQueueCallback * operationQueueCallback();
        virtual bool isOperationQueueRunning();
        virtual void cancelAllOperations();

        virtual ActiveSyncConnectOperation * connectOperation();
        virtual ActiveSyncLoginOperation * loginOperation();
        virtual ActiveSyncLoginOAuth2Operation * loginOAuth2Operation();
        virtual ActiveSyncSetOAuth2TokenOnConnectionOperation * setOAuth2TokenOnConnectionOperation();
        virtual ActiveSyncOptionsOperation * optionsOperation();
        virtual ActiveSyncFolderSyncOperation * folderSyncOperation(String * syncKey);
        virtual ActiveSyncFolderResyncOperation * folderResyncOperation();
        virtual ActiveSyncProvisionOperation * provisionOperation();
        virtual ActiveSyncItemEstimateOperation * itemEstimateOperation(String * collectionID, String * syncKey);
        virtual ActiveSyncItemEstimateOperation * itemEstimateOperationForFolderID(String * folderID, String * syncKey);
        virtual ActiveSyncFolderCreateOperation * folderCreateOperation(String * syncKey, String * parentID, String * displayName);
        virtual ActiveSyncFolderUpdateOperation * folderUpdateOperation(String * syncKey, String * folderID, String * parentID, String * displayName);
        virtual ActiveSyncFolderDeleteOperation * folderDeleteOperation(String * syncKey, String * folderID);
        virtual ActiveSyncSyncMessagesOperation * syncMessagesOperation(String * folderID, String * syncKey);
        virtual ActiveSyncMarkMessagesReadOperation * markMessagesReadOperation(String * folderID, String * syncKey, Array * messageIDs, bool read);
        virtual ActiveSyncSetMessagesFlaggedOperation * setMessagesFlaggedOperation(String * folderID, String * syncKey, Array * messageIDs, bool flagged);
        virtual ActiveSyncDeleteMessagesOperation * deleteMessagesOperation(String * folderID, String * syncKey, Array * messageIDs, bool deletesAsMoves);
        virtual ActiveSyncMarkMessageReadOperation * markMessageReadOperation(String * folderID, String * syncKey, String * messageID, bool read);
        virtual ActiveSyncSetMessageFlaggedOperation * setMessageFlaggedOperation(String * folderID, String * syncKey, String * messageID, bool flagged);
        virtual ActiveSyncDeleteMessageOperation * deleteMessageOperation(String * folderID, String * syncKey, String * messageID, bool deletesAsMoves);
        virtual ActiveSyncMoveMessagesOperation * moveMessagesOperation(Array * moves);
        virtual ActiveSyncFetchMessageOperation * fetchMessageOperation(String * folderID, String * messageID);
        virtual ActiveSyncFetchMessageBodyPartOperation * fetchMessageBodyPartOperation(String * folderID, String * messageID, ActiveSyncBodyType bodyType, uint32_t truncationSize);
        virtual ActiveSyncFetchAttachmentOperation * fetchAttachmentOperation(String * fileReference, String * range);
        virtual ActiveSyncSendMessageOperation * sendMessageOperation(Data * messageData, bool saveInSent);
        virtual ActiveSyncSmartReplyOperation * smartReplyOperation(String * folderID, String * messageID, Data * messageData, bool saveInSent);
        virtual ActiveSyncSmartForwardOperation * smartForwardOperation(String * folderID, String * messageID, Data * messageData, bool saveInSent);
        virtual ActiveSyncPingOperation * pingOperation(Array * collectionIDs, uint32_t heartbeatInterval);
        virtual ActiveSyncPingOperation * pingOperationWithFolderIDs(Array * folderIDs, uint32_t heartbeatInterval);

    public:
        virtual void runOperation(ActiveSyncOperation * operation);
        virtual ActiveSyncSession * session();

    private:
        ActiveSyncSession * mSession;
        OperationQueue * mQueue;
        ActiveSyncOperationQueueCallback * mQueueCallback;
        OperationQueueCallback * mOperationQueueCallback;
    };

}

#endif

#endif
