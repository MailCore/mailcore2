#include "MCGmailMessagePartDataOperation.h"

#include "MCGmailAsyncSession.h"
#include "MCGmail.h"

using namespace mailcore;

GmailMessagePartDataOperation::GmailMessagePartDataOperation()
{
    mMessageID = NULL;
    mPart = NULL;
    mData = NULL;
}

GmailMessagePartDataOperation::~GmailMessagePartDataOperation()
{
    MC_SAFE_RELEASE(mMessageID);
    MC_SAFE_RELEASE(mPart);
    MC_SAFE_RELEASE(mData);
}

void GmailMessagePartDataOperation::setMessageID(String * messageID)
{
    MC_SAFE_REPLACE_COPY(String, mMessageID, messageID);
}

String * GmailMessagePartDataOperation::messageID()
{
    return mMessageID;
}

void GmailMessagePartDataOperation::setPart(GmailMessagePart * part)
{
    MC_SAFE_REPLACE_RETAIN(GmailMessagePart, mPart, part);
}

GmailMessagePart * GmailMessagePartDataOperation::part()
{
    return mPart;
}

Data * GmailMessagePartDataOperation::data()
{
    return mData;
}

void GmailMessagePartDataOperation::main()
{
    ErrorCode error;
    Data * data = session()->session()->dataForMessagePart(mMessageID, mPart, &error);
    MC_SAFE_REPLACE_RETAIN(Data, mData, data);
    setError(error);
}
