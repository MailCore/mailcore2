#include "MCGmailPartDataOperation.h"

#include "MCGmailAsyncSession.h"
#include "MCGmail.h"

using namespace mailcore;

GmailPartDataOperation::GmailPartDataOperation()
{
    mMessageID = NULL;
    mPart = NULL;
    mData = NULL;
}

GmailPartDataOperation::~GmailPartDataOperation()
{
    MC_SAFE_RELEASE(mMessageID);
    MC_SAFE_RELEASE(mPart);
    MC_SAFE_RELEASE(mData);
}

void GmailPartDataOperation::setMessageID(String * messageID)
{
    MC_SAFE_REPLACE_COPY(String, mMessageID, messageID);
}

String * GmailPartDataOperation::messageID()
{
    return mMessageID;
}

void GmailPartDataOperation::setPart(AbstractPart * part)
{
    MC_SAFE_REPLACE_RETAIN(AbstractPart, mPart, part);
}

AbstractPart * GmailPartDataOperation::part()
{
    return mPart;
}

Data * GmailPartDataOperation::data()
{
    return mData;
}

void GmailPartDataOperation::main()
{
    ErrorCode error;
    Data * data = session()->session()->dataForPart(mMessageID, mPart, &error);
    MC_SAFE_REPLACE_RETAIN(Data, mData, data);
    setError(error);
}
