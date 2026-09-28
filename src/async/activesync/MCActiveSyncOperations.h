#ifndef MAILCORE_MCACTIVESYNCOPERATIONS_H

#define MAILCORE_MCACTIVESYNCOPERATIONS_H

#include <MailCore/MCActiveSyncOperation.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class ActiveSyncOptions;
    class ActiveSyncFolderSyncResult;
    class ActiveSyncFolderMutationResult;
    class ActiveSyncSyncResult;
    class ActiveSyncProvisionResult;
    class ActiveSyncItemEstimateResult;
    class ActiveSyncMessage;
    class ActiveSyncMoveResult;
    class ActiveSyncAttachmentData;
    class ActiveSyncPingResult;
    class Array;
    class Data;
    class String;

    class MAILCORE_EXPORT ActiveSyncConnectOperation : public ActiveSyncOperation {
    public:
        virtual void main();
    };

    class MAILCORE_EXPORT ActiveSyncLoginOperation : public ActiveSyncOperation {
    public:
        virtual void main();
    };

    class MAILCORE_EXPORT ActiveSyncLoginOAuth2Operation : public ActiveSyncOperation {
    public:
        virtual void main();
    };

    class MAILCORE_EXPORT ActiveSyncSetOAuth2TokenOnConnectionOperation : public ActiveSyncOperation {
    public:
        virtual void main();
    };

    class MAILCORE_EXPORT ActiveSyncOptionsOperation : public ActiveSyncOperation {
    public:
        ActiveSyncOptionsOperation();
        virtual ~ActiveSyncOptionsOperation();

        virtual ActiveSyncOptions * options();

    public:
        virtual void main();

    private:
        ActiveSyncOptions * mOptions;
    };

    class MAILCORE_EXPORT ActiveSyncFolderSyncOperation : public ActiveSyncOperation {
    public:
        ActiveSyncFolderSyncOperation();
        virtual ~ActiveSyncFolderSyncOperation();

        virtual void setSyncKey(String * syncKey);
        virtual String * syncKey();
        virtual ActiveSyncFolderSyncResult * result();

    public:
        virtual void main();

    private:
        String * mSyncKey;
        ActiveSyncFolderSyncResult * mResult;
    };

    class MAILCORE_EXPORT ActiveSyncFolderResyncOperation : public ActiveSyncOperation {
    public:
        ActiveSyncFolderResyncOperation();
        virtual ~ActiveSyncFolderResyncOperation();

        virtual ActiveSyncFolderSyncResult * result();

    public:
        virtual void main();

    private:
        ActiveSyncFolderSyncResult * mResult;
    };

    class MAILCORE_EXPORT ActiveSyncProvisionOperation : public ActiveSyncOperation {
    public:
        ActiveSyncProvisionOperation();
        virtual ~ActiveSyncProvisionOperation();

        virtual ActiveSyncProvisionResult * result();

    public:
        virtual void main();

    private:
        ActiveSyncProvisionResult * mResult;
    };

    class MAILCORE_EXPORT ActiveSyncItemEstimateOperation : public ActiveSyncOperation {
    public:
        ActiveSyncItemEstimateOperation();
        virtual ~ActiveSyncItemEstimateOperation();

        virtual void setCollectionID(String * collectionID);
        virtual String * collectionID();
        virtual void setSyncKey(String * syncKey);
        virtual String * syncKey();
        virtual ActiveSyncItemEstimateResult * result();

    public:
        virtual void main();

    private:
        String * mCollectionID;
        String * mSyncKey;
        ActiveSyncItemEstimateResult * mResult;
    };

    class MAILCORE_EXPORT ActiveSyncFolderCreateOperation : public ActiveSyncOperation {
    public:
        ActiveSyncFolderCreateOperation();
        virtual ~ActiveSyncFolderCreateOperation();

        virtual void setSyncKey(String * syncKey);
        virtual void setParentID(String * parentID);
        virtual void setDisplayName(String * displayName);
        virtual ActiveSyncFolderMutationResult * result();

    public:
        virtual void main();

    private:
        String * mSyncKey;
        String * mParentID;
        String * mDisplayName;
        ActiveSyncFolderMutationResult * mResult;
    };

    class MAILCORE_EXPORT ActiveSyncFolderUpdateOperation : public ActiveSyncOperation {
    public:
        ActiveSyncFolderUpdateOperation();
        virtual ~ActiveSyncFolderUpdateOperation();

        virtual void setSyncKey(String * syncKey);
        virtual void setFolderID(String * folderID);
        virtual void setParentID(String * parentID);
        virtual void setDisplayName(String * displayName);
        virtual ActiveSyncFolderMutationResult * result();

    public:
        virtual void main();

    private:
        String * mSyncKey;
        String * mFolderID;
        String * mParentID;
        String * mDisplayName;
        ActiveSyncFolderMutationResult * mResult;
    };

    class MAILCORE_EXPORT ActiveSyncFolderDeleteOperation : public ActiveSyncOperation {
    public:
        ActiveSyncFolderDeleteOperation();
        virtual ~ActiveSyncFolderDeleteOperation();

        virtual void setSyncKey(String * syncKey);
        virtual void setFolderID(String * folderID);
        virtual ActiveSyncFolderMutationResult * result();

    public:
        virtual void main();

    private:
        String * mSyncKey;
        String * mFolderID;
        ActiveSyncFolderMutationResult * mResult;
    };

    class MAILCORE_EXPORT ActiveSyncSyncMessagesOperation : public ActiveSyncOperation {
    public:
        ActiveSyncSyncMessagesOperation();
        virtual ~ActiveSyncSyncMessagesOperation();

        virtual void setFolderID(String * folderID);
        virtual void setSyncKey(String * syncKey);
        virtual ActiveSyncSyncResult * result();

    public:
        virtual void main();

    private:
        String * mFolderID;
        String * mSyncKey;
        ActiveSyncSyncResult * mResult;
    };

    class MAILCORE_EXPORT ActiveSyncMessageMutationOperation : public ActiveSyncOperation {
    public:
        ActiveSyncMessageMutationOperation();
        virtual ~ActiveSyncMessageMutationOperation();

        virtual void setFolderID(String * folderID);
        virtual void setSyncKey(String * syncKey);
        virtual void setMessageIDs(Array * messageIDs);
        virtual void setMessageID(String * messageID);
        virtual void setValue(bool value);
        virtual void setDeletesAsMoves(bool deletesAsMoves);
        virtual ActiveSyncSyncResult * result();

    protected:
        String * mFolderID;
        String * mSyncKey;
        Array * mMessageIDs;
        String * mMessageID;
        bool mValue;
        bool mDeletesAsMoves;
        ActiveSyncSyncResult * mResult;
    };

    class MAILCORE_EXPORT ActiveSyncMarkMessagesReadOperation : public ActiveSyncMessageMutationOperation {
    public:
        virtual void main();
    };

    class MAILCORE_EXPORT ActiveSyncSetMessagesFlaggedOperation : public ActiveSyncMessageMutationOperation {
    public:
        virtual void main();
    };

    class MAILCORE_EXPORT ActiveSyncDeleteMessagesOperation : public ActiveSyncMessageMutationOperation {
    public:
        virtual void main();
    };

    class MAILCORE_EXPORT ActiveSyncMarkMessageReadOperation : public ActiveSyncMessageMutationOperation {
    public:
        virtual void main();
    };

    class MAILCORE_EXPORT ActiveSyncSetMessageFlaggedOperation : public ActiveSyncMessageMutationOperation {
    public:
        virtual void main();
    };

    class MAILCORE_EXPORT ActiveSyncDeleteMessageOperation : public ActiveSyncMessageMutationOperation {
    public:
        virtual void main();
    };

    class MAILCORE_EXPORT ActiveSyncMoveMessagesOperation : public ActiveSyncOperation {
    public:
        ActiveSyncMoveMessagesOperation();
        virtual ~ActiveSyncMoveMessagesOperation();

        virtual void setMoves(Array * moves);
        virtual Array * moves();
        virtual ActiveSyncMoveResult * result();

    public:
        virtual void main();

    private:
        Array * mMoves;
        ActiveSyncMoveResult * mResult;
    };

    class MAILCORE_EXPORT ActiveSyncFetchMessageOperation : public ActiveSyncOperation {
    public:
        ActiveSyncFetchMessageOperation();
        virtual ~ActiveSyncFetchMessageOperation();

        virtual void setFolderID(String * folderID);
        virtual void setMessageID(String * messageID);
        virtual ActiveSyncMessage * message();

    public:
        virtual void main();

    private:
        String * mFolderID;
        String * mMessageID;
        ActiveSyncMessage * mMessage;
    };

    class MAILCORE_EXPORT ActiveSyncFetchMessageBodyPartOperation : public ActiveSyncOperation {
    public:
        ActiveSyncFetchMessageBodyPartOperation();
        virtual ~ActiveSyncFetchMessageBodyPartOperation();

        virtual void setFolderID(String * folderID);
        virtual void setMessageID(String * messageID);
        virtual void setBodyType(ActiveSyncBodyType bodyType);
        virtual void setTruncationSize(uint32_t truncationSize);
        virtual ActiveSyncMessage * message();

    public:
        virtual void main();

    private:
        String * mFolderID;
        String * mMessageID;
        ActiveSyncBodyType mBodyType;
        uint32_t mTruncationSize;
        ActiveSyncMessage * mMessage;
    };

    class MAILCORE_EXPORT ActiveSyncFetchAttachmentOperation : public ActiveSyncOperation {
    public:
        ActiveSyncFetchAttachmentOperation();
        virtual ~ActiveSyncFetchAttachmentOperation();

        virtual void setFileReference(String * fileReference);
        virtual void setRange(String * range);
        virtual ActiveSyncAttachmentData * attachmentData();

    public:
        virtual void main();

    private:
        String * mFileReference;
        String * mRange;
        ActiveSyncAttachmentData * mAttachmentData;
    };

    class MAILCORE_EXPORT ActiveSyncSendOperation : public ActiveSyncOperation {
    public:
        ActiveSyncSendOperation();
        virtual ~ActiveSyncSendOperation();

        virtual void setFolderID(String * folderID);
        virtual void setMessageID(String * messageID);
        virtual void setMessageData(Data * messageData);
        virtual void setSaveInSent(bool saveInSent);

    protected:
        String * mFolderID;
        String * mMessageID;
        Data * mMessageData;
        bool mSaveInSent;
    };

    class MAILCORE_EXPORT ActiveSyncSendMessageOperation : public ActiveSyncSendOperation {
    public:
        virtual void main();
    };

    class MAILCORE_EXPORT ActiveSyncSmartReplyOperation : public ActiveSyncSendOperation {
    public:
        virtual void main();
    };

    class MAILCORE_EXPORT ActiveSyncSmartForwardOperation : public ActiveSyncSendOperation {
    public:
        virtual void main();
    };

    class MAILCORE_EXPORT ActiveSyncPingOperation : public ActiveSyncOperation {
    public:
        ActiveSyncPingOperation();
        virtual ~ActiveSyncPingOperation();

        virtual void setCollectionIDs(Array * collectionIDs);
        virtual void setHeartbeatInterval(uint32_t heartbeatInterval);
        virtual ActiveSyncPingResult * result();

    public:
        virtual void main();

    private:
        Array * mCollectionIDs;
        uint32_t mHeartbeatInterval;
        ActiveSyncPingResult * mResult;
    };

}

#endif

#endif
