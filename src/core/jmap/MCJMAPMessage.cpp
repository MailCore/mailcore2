#include "MCJMAPMessage.h"

#include "MCJMAPPart.h"
#include "MCJMAPMultipart.h"
#include "MCJMAPMessagePart.h"

using namespace mailcore;

void JMAPMessage::init()
{
    mIdentifier = NULL;
    mBlobID = NULL;
    mThreadID = NULL;
    mMailboxIDs = (Array *) Array::array()->retain();
    mKeywords = (Array *) Array::array()->retain();
    mSize = 0;
    mPreview = NULL;
    mMainPart = NULL;
    mTextBody = (Array *) Array::array()->retain();
    mHTMLBody = (Array *) Array::array()->retain();
    mAttachments = (Array *) Array::array()->retain();
}

JMAPMessage::JMAPMessage()
{
    init();
}

JMAPMessage::~JMAPMessage()
{
    MC_SAFE_RELEASE(mIdentifier);
    MC_SAFE_RELEASE(mBlobID);
    MC_SAFE_RELEASE(mThreadID);
    MC_SAFE_RELEASE(mMailboxIDs);
    MC_SAFE_RELEASE(mKeywords);
    MC_SAFE_RELEASE(mPreview);
    MC_SAFE_RELEASE(mMainPart);
    MC_SAFE_RELEASE(mTextBody);
    MC_SAFE_RELEASE(mHTMLBody);
    MC_SAFE_RELEASE(mAttachments);
}

String * JMAPMessage::identifier() { return mIdentifier; }
void JMAPMessage::setIdentifier(String * identifier) { MC_SAFE_REPLACE_COPY(String, mIdentifier, identifier); }
String * JMAPMessage::blobID() { return mBlobID; }
void JMAPMessage::setBlobID(String * blobID) { MC_SAFE_REPLACE_COPY(String, mBlobID, blobID); }
String * JMAPMessage::threadID() { return mThreadID; }
void JMAPMessage::setThreadID(String * threadID) { MC_SAFE_REPLACE_COPY(String, mThreadID, threadID); }
Array * JMAPMessage::mailboxIDs() { return mMailboxIDs; }
void JMAPMessage::setMailboxIDs(Array * mailboxIDs) { MC_SAFE_REPLACE_COPY(Array, mMailboxIDs, mailboxIDs); }
Array * JMAPMessage::keywords() { return mKeywords; }
void JMAPMessage::setKeywords(Array * keywords) { MC_SAFE_REPLACE_COPY(Array, mKeywords, keywords); }
unsigned int JMAPMessage::size() { return mSize; }
void JMAPMessage::setSize(unsigned int size) { mSize = size; }
String * JMAPMessage::preview() { return mPreview; }
void JMAPMessage::setPreview(String * preview) { MC_SAFE_REPLACE_COPY(String, mPreview, preview); }
AbstractPart * JMAPMessage::mainPart() { return mMainPart; }
void JMAPMessage::setMainPart(AbstractPart * mainPart) { MC_SAFE_REPLACE_RETAIN(AbstractPart, mMainPart, mainPart); }
Array * JMAPMessage::textBody() { return mTextBody; }
void JMAPMessage::setTextBody(Array * textBody) { MC_SAFE_REPLACE_COPY(Array, mTextBody, textBody); }
Array * JMAPMessage::htmlBody() { return mHTMLBody; }
void JMAPMessage::setHTMLBody(Array * htmlBody) { MC_SAFE_REPLACE_COPY(Array, mHTMLBody, htmlBody); }
Array * JMAPMessage::attachments() { return mAttachments; }
void JMAPMessage::setAttachments(Array * attachments) { MC_SAFE_REPLACE_COPY(Array, mAttachments, attachments); }

static AbstractPart * jmapPartForPartID(AbstractPart * part, String * partID)
{
    if (part == NULL)
        return NULL;

    JMAPPart * single = dynamic_cast<JMAPPart *>(part);
    if ((single != NULL) && (single->partID() != NULL) && single->partID()->isEqual(partID))
        return single;

    JMAPMultipart * multipart = dynamic_cast<JMAPMultipart *>(part);
    if (multipart != NULL) {
        if ((multipart->partID() != NULL) && multipart->partID()->isEqual(partID))
            return multipart;
        if (multipart->parts() != NULL) {
            for (unsigned int i = 0; i < multipart->parts()->count(); i ++) {
                AbstractPart * result = jmapPartForPartID((AbstractPart *) multipart->parts()->objectAtIndex(i), partID);
                if (result != NULL)
                    return result;
            }
        }
    }

    JMAPMessagePart * messagePart = dynamic_cast<JMAPMessagePart *>(part);
    if (messagePart != NULL) {
        if ((messagePart->partID() != NULL) && messagePart->partID()->isEqual(partID))
            return messagePart;
        return jmapPartForPartID(messagePart->mainPart(), partID);
    }

    return NULL;
}

AbstractPart * JMAPMessage::partForPartID(String * partID)
{
    return jmapPartForPartID(mMainPart, partID);
}

AbstractPart * JMAPMessage::partForContentID(String * contentID)
{
    if (mMainPart == NULL)
        return NULL;
    return mMainPart->partForContentID(contentID);
}

AbstractPart * JMAPMessage::partForUniqueID(String * uniqueID)
{
    if (mMainPart == NULL)
        return NULL;
    return mMainPart->partForUniqueID(uniqueID);
}
