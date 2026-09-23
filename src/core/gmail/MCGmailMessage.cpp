#include "MCGmailMessage.h"

#include "MCGmailPart.h"
#include "MCGmailMultipart.h"
#include "MCGmailMessagePart.h"
#include "MCMessageParser.h"

using namespace mailcore;

void GmailMessage::init()
{
    mIdentifier = NULL;
    mThreadID = NULL;
    mLabelIDs = (Array *) Array::array()->retain();
    mSnippet = NULL;
    mHistoryID = NULL;
    mInternalDate = NULL;
    mSizeEstimate = 0;
    mRFC822Data = NULL;
    mPayload = NULL;
}

GmailMessage::GmailMessage()
{
    init();
}

GmailMessage::~GmailMessage()
{
    MC_SAFE_RELEASE(mIdentifier);
    MC_SAFE_RELEASE(mThreadID);
    MC_SAFE_RELEASE(mLabelIDs);
    MC_SAFE_RELEASE(mSnippet);
    MC_SAFE_RELEASE(mHistoryID);
    MC_SAFE_RELEASE(mInternalDate);
    MC_SAFE_RELEASE(mRFC822Data);
    MC_SAFE_RELEASE(mPayload);
}

String * GmailMessage::identifier()
{
    return mIdentifier;
}

void GmailMessage::setIdentifier(String * identifier)
{
    MC_SAFE_REPLACE_COPY(String, mIdentifier, identifier);
}

String * GmailMessage::threadID()
{
    return mThreadID;
}

void GmailMessage::setThreadID(String * threadID)
{
    MC_SAFE_REPLACE_COPY(String, mThreadID, threadID);
}

Array * GmailMessage::labelIDs()
{
    return mLabelIDs;
}

void GmailMessage::setLabelIDs(Array * labelIDs)
{
    MC_SAFE_REPLACE_COPY(Array, mLabelIDs, labelIDs);
}

String * GmailMessage::snippet()
{
    return mSnippet;
}

void GmailMessage::setSnippet(String * snippet)
{
    MC_SAFE_REPLACE_COPY(String, mSnippet, snippet);
}

String * GmailMessage::historyID()
{
    return mHistoryID;
}

void GmailMessage::setHistoryID(String * historyID)
{
    MC_SAFE_REPLACE_COPY(String, mHistoryID, historyID);
}

String * GmailMessage::internalDate()
{
    return mInternalDate;
}

void GmailMessage::setInternalDate(String * internalDate)
{
    MC_SAFE_REPLACE_COPY(String, mInternalDate, internalDate);
}

uint32_t GmailMessage::sizeEstimate()
{
    return mSizeEstimate;
}

void GmailMessage::setSizeEstimate(uint32_t sizeEstimate)
{
    mSizeEstimate = sizeEstimate;
}

Data * GmailMessage::RFC822Data()
{
    return mRFC822Data;
}

void GmailMessage::setRFC822Data(Data * RFC822Data)
{
    MC_SAFE_REPLACE_RETAIN(Data, mRFC822Data, RFC822Data);
}

AbstractPart * GmailMessage::payload()
{
    return mPayload;
}

void GmailMessage::setPayload(AbstractPart * payload)
{
    MC_SAFE_REPLACE_RETAIN(AbstractPart, mPayload, payload);
}

AbstractMessage * GmailMessage::parsedMessage(ErrorCode * pError)
{
    if (mRFC822Data == NULL) {
        * pError = ErrorFetch;
        return NULL;
    }

    * pError = ErrorNone;
    return MessageParser::messageParserWithData(mRFC822Data);
}

static AbstractPart * partForPartIDInPart(AbstractPart * part, String * partID)
{
    if (part == NULL)
        return NULL;

    GmailPart * gmailPart = dynamic_cast<GmailPart *>(part);
    if ((gmailPart != NULL) && (gmailPart->partID() != NULL) && gmailPart->partID()->isEqual(partID)) {
        return gmailPart;
    }

    GmailMultipart * gmailMultipart = dynamic_cast<GmailMultipart *>(part);
    if (gmailMultipart != NULL) {
        if ((gmailMultipart->partID() != NULL) && gmailMultipart->partID()->isEqual(partID)) {
            return gmailMultipart;
        }
        if (gmailMultipart->parts() != NULL) {
            for (unsigned int i = 0; i < gmailMultipart->parts()->count(); i ++) {
                AbstractPart * result = partForPartIDInPart((AbstractPart *) gmailMultipart->parts()->objectAtIndex(i),
                                                            partID);
                if (result != NULL)
                    return result;
            }
        }
    }

    GmailMessagePart * gmailMessagePart = dynamic_cast<GmailMessagePart *>(part);
    if (gmailMessagePart != NULL) {
        if ((gmailMessagePart->partID() != NULL) && gmailMessagePart->partID()->isEqual(partID)) {
            return gmailMessagePart;
        }
        return partForPartIDInPart(gmailMessagePart->mainPart(), partID);
    }

    return NULL;
}

AbstractPart * GmailMessage::partForPartID(String * partID)
{
    return partForPartIDInPart(mPayload, partID);
}

AbstractPart * GmailMessage::partForContentID(String * contentID)
{
    if (mPayload == NULL)
        return NULL;

    return mPayload->partForContentID(contentID);
}

AbstractPart * GmailMessage::partForUniqueID(String * uniqueID)
{
    if (mPayload == NULL)
        return NULL;

    return mPayload->partForUniqueID(uniqueID);
}
