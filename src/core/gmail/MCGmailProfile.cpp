#include "MCGmailProfile.h"

using namespace mailcore;

void GmailProfile::init()
{
    mEmailAddress = NULL;
    mMessagesTotal = 0;
    mThreadsTotal = 0;
    mHistoryID = NULL;
}

GmailProfile::GmailProfile()
{
    init();
}

GmailProfile::~GmailProfile()
{
    MC_SAFE_RELEASE(mEmailAddress);
    MC_SAFE_RELEASE(mHistoryID);
}

String * GmailProfile::emailAddress()
{
    return mEmailAddress;
}

void GmailProfile::setEmailAddress(String * emailAddress)
{
    MC_SAFE_REPLACE_COPY(String, mEmailAddress, emailAddress);
}

uint32_t GmailProfile::messagesTotal()
{
    return mMessagesTotal;
}

void GmailProfile::setMessagesTotal(uint32_t messagesTotal)
{
    mMessagesTotal = messagesTotal;
}

uint32_t GmailProfile::threadsTotal()
{
    return mThreadsTotal;
}

void GmailProfile::setThreadsTotal(uint32_t threadsTotal)
{
    mThreadsTotal = threadsTotal;
}

String * GmailProfile::historyID()
{
    return mHistoryID;
}

void GmailProfile::setHistoryID(String * historyID)
{
    MC_SAFE_REPLACE_COPY(String, mHistoryID, historyID);
}

String * GmailProfile::description()
{
    String * result = String::string();
    result->appendUTF8Format("<%s:%p %s>", className()->UTF8Characters(), this,
                             MCUTF8(mEmailAddress));
    return result;
}
