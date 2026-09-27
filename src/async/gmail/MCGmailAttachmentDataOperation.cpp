#include "MCGmailAttachmentDataOperation.h"

#include "MCGmailAsyncSession.h"
#include "MCGmail.h"

using namespace mailcore;

GmailAttachmentDataOperation::GmailAttachmentDataOperation()
{
    mMessageID = NULL;
    mAttachmentID = NULL;
    mData = NULL;
}

GmailAttachmentDataOperation::~GmailAttachmentDataOperation()
{
    MC_SAFE_RELEASE(mMessageID);
    MC_SAFE_RELEASE(mAttachmentID);
    MC_SAFE_RELEASE(mData);
}

void GmailAttachmentDataOperation::setMessageID(String * messageID)
{
    MC_SAFE_REPLACE_COPY(String, mMessageID, messageID);
}

String * GmailAttachmentDataOperation::messageID()
{
    return mMessageID;
}

void GmailAttachmentDataOperation::setAttachmentID(String * attachmentID)
{
    MC_SAFE_REPLACE_COPY(String, mAttachmentID, attachmentID);
}

String * GmailAttachmentDataOperation::attachmentID()
{
    return mAttachmentID;
}

Data * GmailAttachmentDataOperation::data()
{
    return mData;
}

void GmailAttachmentDataOperation::main()
{
    ErrorCode error;
    Data * data = syncSession()->attachmentData(mMessageID, mAttachmentID, &error);
    MC_SAFE_REPLACE_RETAIN(Data, mData, data);
    setError(error);
}
