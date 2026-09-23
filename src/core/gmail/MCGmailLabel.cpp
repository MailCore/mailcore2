#include "MCGmailLabel.h"

using namespace mailcore;

void GmailLabel::init()
{
    mIdentifier = NULL;
    mName = NULL;
    mType = NULL;
    mMessageListVisibility = NULL;
    mLabelListVisibility = NULL;
    mMessagesTotal = 0;
    mMessagesUnread = 0;
    mThreadsTotal = 0;
    mThreadsUnread = 0;
}

GmailLabel::GmailLabel()
{
    init();
}

GmailLabel::~GmailLabel()
{
    MC_SAFE_RELEASE(mIdentifier);
    MC_SAFE_RELEASE(mName);
    MC_SAFE_RELEASE(mType);
    MC_SAFE_RELEASE(mMessageListVisibility);
    MC_SAFE_RELEASE(mLabelListVisibility);
}

String * GmailLabel::identifier()
{
    return mIdentifier;
}

void GmailLabel::setIdentifier(String * identifier)
{
    MC_SAFE_REPLACE_COPY(String, mIdentifier, identifier);
}

String * GmailLabel::name()
{
    return mName;
}

void GmailLabel::setName(String * name)
{
    MC_SAFE_REPLACE_COPY(String, mName, name);
}

String * GmailLabel::type()
{
    return mType;
}

void GmailLabel::setType(String * type)
{
    MC_SAFE_REPLACE_COPY(String, mType, type);
}

String * GmailLabel::messageListVisibility()
{
    return mMessageListVisibility;
}

void GmailLabel::setMessageListVisibility(String * visibility)
{
    MC_SAFE_REPLACE_COPY(String, mMessageListVisibility, visibility);
}

String * GmailLabel::labelListVisibility()
{
    return mLabelListVisibility;
}

void GmailLabel::setLabelListVisibility(String * visibility)
{
    MC_SAFE_REPLACE_COPY(String, mLabelListVisibility, visibility);
}

uint32_t GmailLabel::messagesTotal()
{
    return mMessagesTotal;
}

void GmailLabel::setMessagesTotal(uint32_t messagesTotal)
{
    mMessagesTotal = messagesTotal;
}

uint32_t GmailLabel::messagesUnread()
{
    return mMessagesUnread;
}

void GmailLabel::setMessagesUnread(uint32_t messagesUnread)
{
    mMessagesUnread = messagesUnread;
}

uint32_t GmailLabel::threadsTotal()
{
    return mThreadsTotal;
}

void GmailLabel::setThreadsTotal(uint32_t threadsTotal)
{
    mThreadsTotal = threadsTotal;
}

uint32_t GmailLabel::threadsUnread()
{
    return mThreadsUnread;
}

void GmailLabel::setThreadsUnread(uint32_t threadsUnread)
{
    mThreadsUnread = threadsUnread;
}

String * GmailLabel::description()
{
    String * result = String::string();
    result->appendUTF8Format("<%s:%p %s>", className()->UTF8Characters(), this,
                             MCUTF8(mIdentifier));
    return result;
}
