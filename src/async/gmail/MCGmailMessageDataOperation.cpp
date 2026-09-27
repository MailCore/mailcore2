#include "MCGmailMessageDataOperation.h"

#include "MCGmailAsyncSession.h"
#include "MCGmail.h"

using namespace mailcore;

GmailMessageDataOperation::GmailMessageDataOperation()
{
    mMessageID = NULL;
    mData = NULL;
}

GmailMessageDataOperation::~GmailMessageDataOperation()
{
    MC_SAFE_RELEASE(mMessageID);
    MC_SAFE_RELEASE(mData);
}

void GmailMessageDataOperation::setMessageID(String * messageID)
{
    MC_SAFE_REPLACE_COPY(String, mMessageID, messageID);
}

String * GmailMessageDataOperation::messageID()
{
    return mMessageID;
}

Data * GmailMessageDataOperation::data()
{
    return mData;
}

void GmailMessageDataOperation::main()
{
    ErrorCode error;
    Data * data = syncSession()->messageData(mMessageID, &error);
    MC_SAFE_REPLACE_RETAIN(Data, mData, data);
    setError(error);
}
