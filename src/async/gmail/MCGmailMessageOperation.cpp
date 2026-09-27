#include "MCGmailMessageOperation.h"

#include "MCGmailAsyncSession.h"
#include "MCGmail.h"

using namespace mailcore;

GmailMessageOperation::GmailMessageOperation()
{
    mKind = GmailMessageOperationKindDefault;
    mMessageID = NULL;
    mFormat = GmailMessageFormatFull;
    mMetadataHeaders = NULL;
    mMessage = NULL;
}

GmailMessageOperation::~GmailMessageOperation()
{
    MC_SAFE_RELEASE(mMessageID);
    MC_SAFE_RELEASE(mMetadataHeaders);
    MC_SAFE_RELEASE(mMessage);
}

void GmailMessageOperation::setKind(GmailMessageOperationKind kind)
{
    mKind = kind;
}

GmailMessageOperation::GmailMessageOperationKind GmailMessageOperation::kind()
{
    return mKind;
}

void GmailMessageOperation::setMessageID(String * messageID)
{
    MC_SAFE_REPLACE_COPY(String, mMessageID, messageID);
}

String * GmailMessageOperation::messageID()
{
    return mMessageID;
}

void GmailMessageOperation::setFormat(GmailMessageFormat format)
{
    mFormat = format;
}

GmailMessageFormat GmailMessageOperation::format()
{
    return mFormat;
}

void GmailMessageOperation::setMetadataHeaders(Array * headers)
{
    MC_SAFE_REPLACE_COPY(Array, mMetadataHeaders, headers);
}

Array * GmailMessageOperation::metadataHeaders()
{
    return mMetadataHeaders;
}

GmailMessage * GmailMessageOperation::message()
{
    return mMessage;
}

void GmailMessageOperation::main()
{
    ErrorCode error;
    GmailMessage * message = NULL;

    switch (mKind) {
        case GmailMessageOperationKindDefault:
            message = syncSession()->message(mMessageID, &error);
            break;
        case GmailMessageOperationKindFormat:
            message = syncSession()->messageWithFormat(mMessageID, mFormat, &error);
            break;
        case GmailMessageOperationKindMetadataHeaders:
            message = syncSession()->messageWithMetadataHeaders(mMessageID, mMetadataHeaders, &error);
            break;
    }

    MC_SAFE_REPLACE_RETAIN(GmailMessage, mMessage, message);
    setError(error);
}
