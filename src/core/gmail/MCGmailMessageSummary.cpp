#include "MCGmailMessageSummary.h"

using namespace mailcore;

void GmailMessageSummary::init()
{
    mIdentifier = NULL;
    mThreadID = NULL;
}

GmailMessageSummary::GmailMessageSummary()
{
    init();
}

GmailMessageSummary::~GmailMessageSummary()
{
    MC_SAFE_RELEASE(mIdentifier);
    MC_SAFE_RELEASE(mThreadID);
}

String * GmailMessageSummary::identifier()
{
    return mIdentifier;
}

void GmailMessageSummary::setIdentifier(String * identifier)
{
    MC_SAFE_REPLACE_COPY(String, mIdentifier, identifier);
}

String * GmailMessageSummary::threadID()
{
    return mThreadID;
}

void GmailMessageSummary::setThreadID(String * threadID)
{
    MC_SAFE_REPLACE_COPY(String, mThreadID, threadID);
}
