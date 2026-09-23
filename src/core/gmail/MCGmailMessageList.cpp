#include "MCGmailMessageList.h"

using namespace mailcore;

void GmailMessageList::init()
{
    mMessages = (Array *) Array::array()->retain();
    mNextPageToken = NULL;
    mResultSizeEstimate = 0;
}

GmailMessageList::GmailMessageList()
{
    init();
}

GmailMessageList::~GmailMessageList()
{
    MC_SAFE_RELEASE(mMessages);
    MC_SAFE_RELEASE(mNextPageToken);
}

Array * GmailMessageList::messages()
{
    return mMessages;
}

void GmailMessageList::setMessages(Array * messages)
{
    MC_SAFE_REPLACE_COPY(Array, mMessages, messages);
}

String * GmailMessageList::nextPageToken()
{
    return mNextPageToken;
}

void GmailMessageList::setNextPageToken(String * nextPageToken)
{
    MC_SAFE_REPLACE_COPY(String, mNextPageToken, nextPageToken);
}

uint32_t GmailMessageList::resultSizeEstimate()
{
    return mResultSizeEstimate;
}

void GmailMessageList::setResultSizeEstimate(uint32_t resultSizeEstimate)
{
    mResultSizeEstimate = resultSizeEstimate;
}
