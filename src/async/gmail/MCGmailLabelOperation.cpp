#include "MCGmailLabelOperation.h"

#include "MCGmailAsyncSession.h"
#include "MCGmail.h"

using namespace mailcore;

GmailLabelOperation::GmailLabelOperation()
{
    mLabelID = NULL;
    mLabel = NULL;
}

GmailLabelOperation::~GmailLabelOperation()
{
    MC_SAFE_RELEASE(mLabelID);
    MC_SAFE_RELEASE(mLabel);
}

void GmailLabelOperation::setLabelID(String * labelID)
{
    MC_SAFE_REPLACE_COPY(String, mLabelID, labelID);
}

String * GmailLabelOperation::labelID()
{
    return mLabelID;
}

GmailLabel * GmailLabelOperation::label()
{
    return mLabel;
}

void GmailLabelOperation::main()
{
    ErrorCode error;
    GmailLabel * label = syncSession()->label(mLabelID, &error);
    MC_SAFE_REPLACE_RETAIN(GmailLabel, mLabel, label);
    setError(error);
}
