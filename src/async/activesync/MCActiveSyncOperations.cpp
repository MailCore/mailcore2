#include "MCActiveSyncOperations.h"

#include "MCActiveSyncAsyncSession.h"
#include "MCActiveSyncSession.h"

using namespace mailcore;

void ActiveSyncConnectOperation::main()
{
    ErrorCode error;
    session()->session()->connect(&error);
    setError(error);
}

void ActiveSyncLoginOperation::main()
{
    ErrorCode error;
    session()->session()->login(&error);
    setError(error);
}

void ActiveSyncLoginOAuth2Operation::main()
{
    ErrorCode error;
    session()->session()->loginOAuth2(&error);
    setError(error);
}

void ActiveSyncSetOAuth2TokenOnConnectionOperation::main()
{
    ErrorCode error;
    session()->session()->setOAuth2TokenOnConnection(&error);
    setError(error);
}

ActiveSyncOptionsOperation::ActiveSyncOptionsOperation()
{
    mOptions = NULL;
}

ActiveSyncOptionsOperation::~ActiveSyncOptionsOperation()
{
    MC_SAFE_RELEASE(mOptions);
}

ActiveSyncOptions * ActiveSyncOptionsOperation::options()
{
    return mOptions;
}

void ActiveSyncOptionsOperation::main()
{
    ErrorCode error;
    mOptions = session()->session()->options(&error);
    MC_SAFE_RETAIN(mOptions);
    setError(error);
}

ActiveSyncFolderSyncOperation::ActiveSyncFolderSyncOperation()
{
    mSyncKey = NULL;
    mResult = NULL;
}

ActiveSyncFolderSyncOperation::~ActiveSyncFolderSyncOperation()
{
    MC_SAFE_RELEASE(mSyncKey);
    MC_SAFE_RELEASE(mResult);
}

void ActiveSyncFolderSyncOperation::setSyncKey(String * syncKey)
{
    MC_SAFE_REPLACE_COPY(String, mSyncKey, syncKey);
}

String * ActiveSyncFolderSyncOperation::syncKey()
{
    return mSyncKey;
}

ActiveSyncFolderSyncResult * ActiveSyncFolderSyncOperation::result()
{
    return mResult;
}

void ActiveSyncFolderSyncOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->folderSync(mSyncKey, &error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

ActiveSyncFolderResyncOperation::ActiveSyncFolderResyncOperation()
{
    mResult = NULL;
}

ActiveSyncFolderResyncOperation::~ActiveSyncFolderResyncOperation()
{
    MC_SAFE_RELEASE(mResult);
}

ActiveSyncFolderSyncResult * ActiveSyncFolderResyncOperation::result()
{
    return mResult;
}

void ActiveSyncFolderResyncOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->folderResync(&error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

ActiveSyncProvisionOperation::ActiveSyncProvisionOperation()
{
    mResult = NULL;
}

ActiveSyncProvisionOperation::~ActiveSyncProvisionOperation()
{
    MC_SAFE_RELEASE(mResult);
}

ActiveSyncProvisionResult * ActiveSyncProvisionOperation::result()
{
    return mResult;
}

void ActiveSyncProvisionOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->provision(&error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

ActiveSyncItemEstimateOperation::ActiveSyncItemEstimateOperation()
{
    mCollectionID = NULL;
    mSyncKey = NULL;
    mResult = NULL;
}

ActiveSyncItemEstimateOperation::~ActiveSyncItemEstimateOperation()
{
    MC_SAFE_RELEASE(mCollectionID);
    MC_SAFE_RELEASE(mSyncKey);
    MC_SAFE_RELEASE(mResult);
}

void ActiveSyncItemEstimateOperation::setCollectionID(String * collectionID)
{
    MC_SAFE_REPLACE_COPY(String, mCollectionID, collectionID);
}

String * ActiveSyncItemEstimateOperation::collectionID()
{
    return mCollectionID;
}

void ActiveSyncItemEstimateOperation::setSyncKey(String * syncKey)
{
    MC_SAFE_REPLACE_COPY(String, mSyncKey, syncKey);
}

String * ActiveSyncItemEstimateOperation::syncKey()
{
    return mSyncKey;
}

ActiveSyncItemEstimateResult * ActiveSyncItemEstimateOperation::result()
{
    return mResult;
}

void ActiveSyncItemEstimateOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->itemEstimate(mCollectionID, mSyncKey, &error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

ActiveSyncFolderCreateOperation::ActiveSyncFolderCreateOperation()
{
    mSyncKey = NULL;
    mParentID = NULL;
    mDisplayName = NULL;
    mResult = NULL;
}

ActiveSyncFolderCreateOperation::~ActiveSyncFolderCreateOperation()
{
    MC_SAFE_RELEASE(mSyncKey);
    MC_SAFE_RELEASE(mParentID);
    MC_SAFE_RELEASE(mDisplayName);
    MC_SAFE_RELEASE(mResult);
}

void ActiveSyncFolderCreateOperation::setSyncKey(String * syncKey)
{
    MC_SAFE_REPLACE_COPY(String, mSyncKey, syncKey);
}

void ActiveSyncFolderCreateOperation::setParentID(String * parentID)
{
    MC_SAFE_REPLACE_COPY(String, mParentID, parentID);
}

void ActiveSyncFolderCreateOperation::setDisplayName(String * displayName)
{
    MC_SAFE_REPLACE_COPY(String, mDisplayName, displayName);
}

ActiveSyncFolderMutationResult * ActiveSyncFolderCreateOperation::result()
{
    return mResult;
}

void ActiveSyncFolderCreateOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->folderCreate(mSyncKey, mParentID, mDisplayName, &error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

ActiveSyncFolderUpdateOperation::ActiveSyncFolderUpdateOperation()
{
    mSyncKey = NULL;
    mFolderID = NULL;
    mParentID = NULL;
    mDisplayName = NULL;
    mResult = NULL;
}

ActiveSyncFolderUpdateOperation::~ActiveSyncFolderUpdateOperation()
{
    MC_SAFE_RELEASE(mSyncKey);
    MC_SAFE_RELEASE(mFolderID);
    MC_SAFE_RELEASE(mParentID);
    MC_SAFE_RELEASE(mDisplayName);
    MC_SAFE_RELEASE(mResult);
}

void ActiveSyncFolderUpdateOperation::setSyncKey(String * syncKey)
{
    MC_SAFE_REPLACE_COPY(String, mSyncKey, syncKey);
}

void ActiveSyncFolderUpdateOperation::setFolderID(String * folderID)
{
    MC_SAFE_REPLACE_COPY(String, mFolderID, folderID);
}

void ActiveSyncFolderUpdateOperation::setParentID(String * parentID)
{
    MC_SAFE_REPLACE_COPY(String, mParentID, parentID);
}

void ActiveSyncFolderUpdateOperation::setDisplayName(String * displayName)
{
    MC_SAFE_REPLACE_COPY(String, mDisplayName, displayName);
}

ActiveSyncFolderMutationResult * ActiveSyncFolderUpdateOperation::result()
{
    return mResult;
}

void ActiveSyncFolderUpdateOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->folderUpdate(mSyncKey, mFolderID, mParentID, mDisplayName, &error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

ActiveSyncFolderDeleteOperation::ActiveSyncFolderDeleteOperation()
{
    mSyncKey = NULL;
    mFolderID = NULL;
    mResult = NULL;
}

ActiveSyncFolderDeleteOperation::~ActiveSyncFolderDeleteOperation()
{
    MC_SAFE_RELEASE(mSyncKey);
    MC_SAFE_RELEASE(mFolderID);
    MC_SAFE_RELEASE(mResult);
}

void ActiveSyncFolderDeleteOperation::setSyncKey(String * syncKey)
{
    MC_SAFE_REPLACE_COPY(String, mSyncKey, syncKey);
}

void ActiveSyncFolderDeleteOperation::setFolderID(String * folderID)
{
    MC_SAFE_REPLACE_COPY(String, mFolderID, folderID);
}

ActiveSyncFolderMutationResult * ActiveSyncFolderDeleteOperation::result()
{
    return mResult;
}

void ActiveSyncFolderDeleteOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->folderDelete(mSyncKey, mFolderID, &error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

ActiveSyncSyncMessagesOperation::ActiveSyncSyncMessagesOperation()
{
    mFolderID = NULL;
    mSyncKey = NULL;
    mResult = NULL;
}

ActiveSyncSyncMessagesOperation::~ActiveSyncSyncMessagesOperation()
{
    MC_SAFE_RELEASE(mFolderID);
    MC_SAFE_RELEASE(mSyncKey);
    MC_SAFE_RELEASE(mResult);
}

void ActiveSyncSyncMessagesOperation::setFolderID(String * folderID)
{
    MC_SAFE_REPLACE_COPY(String, mFolderID, folderID);
}

void ActiveSyncSyncMessagesOperation::setSyncKey(String * syncKey)
{
    MC_SAFE_REPLACE_COPY(String, mSyncKey, syncKey);
}

ActiveSyncSyncResult * ActiveSyncSyncMessagesOperation::result()
{
    return mResult;
}

void ActiveSyncSyncMessagesOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->syncMessages(mFolderID, mSyncKey, &error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

ActiveSyncMessageMutationOperation::ActiveSyncMessageMutationOperation()
{
    mFolderID = NULL;
    mSyncKey = NULL;
    mMessageIDs = NULL;
    mMessageID = NULL;
    mValue = false;
    mDeletesAsMoves = false;
    mResult = NULL;
}

ActiveSyncMessageMutationOperation::~ActiveSyncMessageMutationOperation()
{
    MC_SAFE_RELEASE(mFolderID);
    MC_SAFE_RELEASE(mSyncKey);
    MC_SAFE_RELEASE(mMessageIDs);
    MC_SAFE_RELEASE(mMessageID);
    MC_SAFE_RELEASE(mResult);
}

void ActiveSyncMessageMutationOperation::setFolderID(String * folderID)
{
    MC_SAFE_REPLACE_COPY(String, mFolderID, folderID);
}

void ActiveSyncMessageMutationOperation::setSyncKey(String * syncKey)
{
    MC_SAFE_REPLACE_COPY(String, mSyncKey, syncKey);
}

void ActiveSyncMessageMutationOperation::setMessageIDs(Array * messageIDs)
{
    MC_SAFE_REPLACE_COPY(Array, mMessageIDs, messageIDs);
}

void ActiveSyncMessageMutationOperation::setMessageID(String * messageID)
{
    MC_SAFE_REPLACE_COPY(String, mMessageID, messageID);
}

void ActiveSyncMessageMutationOperation::setValue(bool value)
{
    mValue = value;
}

void ActiveSyncMessageMutationOperation::setDeletesAsMoves(bool deletesAsMoves)
{
    mDeletesAsMoves = deletesAsMoves;
}

ActiveSyncSyncResult * ActiveSyncMessageMutationOperation::result()
{
    return mResult;
}

void ActiveSyncMarkMessagesReadOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->markMessagesRead(mFolderID, mSyncKey, mMessageIDs, mValue, &error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

void ActiveSyncSetMessagesFlaggedOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->setMessagesFlagged(mFolderID, mSyncKey, mMessageIDs, mValue, &error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

void ActiveSyncDeleteMessagesOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->deleteMessages(mFolderID, mSyncKey, mMessageIDs, mDeletesAsMoves, &error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

void ActiveSyncMarkMessageReadOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->markMessageRead(mFolderID, mSyncKey, mMessageID, mValue, &error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

void ActiveSyncSetMessageFlaggedOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->setMessageFlagged(mFolderID, mSyncKey, mMessageID, mValue, &error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

void ActiveSyncDeleteMessageOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->deleteMessage(mFolderID, mSyncKey, mMessageID, mDeletesAsMoves, &error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

ActiveSyncMoveMessagesOperation::ActiveSyncMoveMessagesOperation()
{
    mMoves = NULL;
    mResult = NULL;
}

ActiveSyncMoveMessagesOperation::~ActiveSyncMoveMessagesOperation()
{
    MC_SAFE_RELEASE(mMoves);
    MC_SAFE_RELEASE(mResult);
}

void ActiveSyncMoveMessagesOperation::setMoves(Array * moves)
{
    MC_SAFE_REPLACE_COPY(Array, mMoves, moves);
}

Array * ActiveSyncMoveMessagesOperation::moves()
{
    return mMoves;
}

ActiveSyncMoveResult * ActiveSyncMoveMessagesOperation::result()
{
    return mResult;
}

void ActiveSyncMoveMessagesOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->moveMessages(mMoves, &error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}

ActiveSyncFetchMessageOperation::ActiveSyncFetchMessageOperation()
{
    mFolderID = NULL;
    mMessageID = NULL;
    mMessage = NULL;
}

ActiveSyncFetchMessageOperation::~ActiveSyncFetchMessageOperation()
{
    MC_SAFE_RELEASE(mFolderID);
    MC_SAFE_RELEASE(mMessageID);
    MC_SAFE_RELEASE(mMessage);
}

void ActiveSyncFetchMessageOperation::setFolderID(String * folderID)
{
    MC_SAFE_REPLACE_COPY(String, mFolderID, folderID);
}

void ActiveSyncFetchMessageOperation::setMessageID(String * messageID)
{
    MC_SAFE_REPLACE_COPY(String, mMessageID, messageID);
}

ActiveSyncMessage * ActiveSyncFetchMessageOperation::message()
{
    return mMessage;
}

void ActiveSyncFetchMessageOperation::main()
{
    ErrorCode error;
    mMessage = session()->session()->fetchMessage(mFolderID, mMessageID, &error);
    MC_SAFE_RETAIN(mMessage);
    setError(error);
}

ActiveSyncFetchMessageBodyPartOperation::ActiveSyncFetchMessageBodyPartOperation()
{
    mFolderID = NULL;
    mMessageID = NULL;
    mBodyType = ActiveSyncBodyTypePlainText;
    mTruncationSize = 0;
    mMessage = NULL;
}

ActiveSyncFetchMessageBodyPartOperation::~ActiveSyncFetchMessageBodyPartOperation()
{
    MC_SAFE_RELEASE(mFolderID);
    MC_SAFE_RELEASE(mMessageID);
    MC_SAFE_RELEASE(mMessage);
}

void ActiveSyncFetchMessageBodyPartOperation::setFolderID(String * folderID)
{
    MC_SAFE_REPLACE_COPY(String, mFolderID, folderID);
}

void ActiveSyncFetchMessageBodyPartOperation::setMessageID(String * messageID)
{
    MC_SAFE_REPLACE_COPY(String, mMessageID, messageID);
}

void ActiveSyncFetchMessageBodyPartOperation::setBodyType(ActiveSyncBodyType bodyType)
{
    mBodyType = bodyType;
}

void ActiveSyncFetchMessageBodyPartOperation::setTruncationSize(uint32_t truncationSize)
{
    mTruncationSize = truncationSize;
}

ActiveSyncMessage * ActiveSyncFetchMessageBodyPartOperation::message()
{
    return mMessage;
}

void ActiveSyncFetchMessageBodyPartOperation::main()
{
    ErrorCode error;
    mMessage = session()->session()->fetchMessageBodyPart(mFolderID, mMessageID, mBodyType, mTruncationSize, &error);
    MC_SAFE_RETAIN(mMessage);
    setError(error);
}

ActiveSyncFetchAttachmentOperation::ActiveSyncFetchAttachmentOperation()
{
    mFileReference = NULL;
    mRange = NULL;
    mAttachmentData = NULL;
}

ActiveSyncFetchAttachmentOperation::~ActiveSyncFetchAttachmentOperation()
{
    MC_SAFE_RELEASE(mFileReference);
    MC_SAFE_RELEASE(mRange);
    MC_SAFE_RELEASE(mAttachmentData);
}

void ActiveSyncFetchAttachmentOperation::setFileReference(String * fileReference)
{
    MC_SAFE_REPLACE_COPY(String, mFileReference, fileReference);
}

void ActiveSyncFetchAttachmentOperation::setRange(String * range)
{
    MC_SAFE_REPLACE_COPY(String, mRange, range);
}

ActiveSyncAttachmentData * ActiveSyncFetchAttachmentOperation::attachmentData()
{
    return mAttachmentData;
}

void ActiveSyncFetchAttachmentOperation::main()
{
    ErrorCode error;
    mAttachmentData = session()->session()->fetchAttachment(mFileReference, mRange, &error);
    MC_SAFE_RETAIN(mAttachmentData);
    setError(error);
}

ActiveSyncSendOperation::ActiveSyncSendOperation()
{
    mFolderID = NULL;
    mMessageID = NULL;
    mMessageData = NULL;
    mSaveInSent = false;
}

ActiveSyncSendOperation::~ActiveSyncSendOperation()
{
    MC_SAFE_RELEASE(mFolderID);
    MC_SAFE_RELEASE(mMessageID);
    MC_SAFE_RELEASE(mMessageData);
}

void ActiveSyncSendOperation::setFolderID(String * folderID)
{
    MC_SAFE_REPLACE_COPY(String, mFolderID, folderID);
}

void ActiveSyncSendOperation::setMessageID(String * messageID)
{
    MC_SAFE_REPLACE_COPY(String, mMessageID, messageID);
}

void ActiveSyncSendOperation::setMessageData(Data * messageData)
{
    MC_SAFE_REPLACE_RETAIN(Data, mMessageData, messageData);
}

void ActiveSyncSendOperation::setSaveInSent(bool saveInSent)
{
    mSaveInSent = saveInSent;
}

void ActiveSyncSendMessageOperation::main()
{
    ErrorCode error;
    session()->session()->sendMessage(mMessageData, mSaveInSent, &error);
    setError(error);
}

void ActiveSyncSmartReplyOperation::main()
{
    ErrorCode error;
    session()->session()->smartReply(mFolderID, mMessageID, mMessageData, mSaveInSent, &error);
    setError(error);
}

void ActiveSyncSmartForwardOperation::main()
{
    ErrorCode error;
    session()->session()->smartForward(mFolderID, mMessageID, mMessageData, mSaveInSent, &error);
    setError(error);
}

ActiveSyncPingOperation::ActiveSyncPingOperation()
{
    mCollectionIDs = NULL;
    mHeartbeatInterval = 0;
    mResult = NULL;
}

ActiveSyncPingOperation::~ActiveSyncPingOperation()
{
    MC_SAFE_RELEASE(mCollectionIDs);
    MC_SAFE_RELEASE(mResult);
}

void ActiveSyncPingOperation::setCollectionIDs(Array * collectionIDs)
{
    MC_SAFE_REPLACE_COPY(Array, mCollectionIDs, collectionIDs);
}

void ActiveSyncPingOperation::setHeartbeatInterval(uint32_t heartbeatInterval)
{
    mHeartbeatInterval = heartbeatInterval;
}

ActiveSyncPingResult * ActiveSyncPingOperation::result()
{
    return mResult;
}

void ActiveSyncPingOperation::main()
{
    ErrorCode error;
    mResult = session()->session()->ping(mCollectionIDs, mHeartbeatInterval, &error);
    MC_SAFE_RETAIN(mResult);
    setError(error);
}
