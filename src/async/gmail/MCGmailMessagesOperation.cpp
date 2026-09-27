#include "MCGmailMessagesOperation.h"

#include "MCGmailAsyncSession.h"
#include "MCGmail.h"

using namespace mailcore;

GmailMessagesOperation::GmailMessagesOperation()
{
    mKind = GmailMessagesOperationKindDefault;
    mQuery = NULL;
    mLabelID = NULL;
    mMessages = NULL;
}

GmailMessagesOperation::~GmailMessagesOperation()
{
    MC_SAFE_RELEASE(mQuery);
    MC_SAFE_RELEASE(mLabelID);
    MC_SAFE_RELEASE(mMessages);
}

void GmailMessagesOperation::setKind(GmailMessagesOperationKind kind)
{
    mKind = kind;
}

GmailMessagesOperation::GmailMessagesOperationKind GmailMessagesOperation::kind()
{
    return mKind;
}

void GmailMessagesOperation::setQuery(String * query)
{
    MC_SAFE_REPLACE_COPY(String, mQuery, query);
}

String * GmailMessagesOperation::query()
{
    return mQuery;
}

void GmailMessagesOperation::setLabelID(String * labelID)
{
    MC_SAFE_REPLACE_COPY(String, mLabelID, labelID);
}

String * GmailMessagesOperation::labelID()
{
    return mLabelID;
}

GmailMessageList * GmailMessagesOperation::messages()
{
    return mMessages;
}

void GmailMessagesOperation::main()
{
    ErrorCode error;
    GmailMessageList * messages = NULL;

    switch (mKind) {
        case GmailMessagesOperationKindDefault:
            messages = syncSession()->messages(&error);
            break;
        case GmailMessagesOperationKindQuery:
            messages = syncSession()->messagesWithQuery(mQuery, &error);
            break;
        case GmailMessagesOperationKindLabel:
            messages = syncSession()->messagesWithLabel(mLabelID, &error);
            break;
    }

    MC_SAFE_REPLACE_RETAIN(GmailMessageList, mMessages, messages);
    setError(error);
}
