#include "MCActiveSyncMessage.h"

#include "MCActiveSyncPrivate.h"
#include "MCAbstractPart.h"
#include "MCMessageHeader.h"

using namespace mailcore;

void ActiveSyncMessage::init()
{
    mServerID = NULL;
    mMessageClass = NULL;
    mEstimatedSize = 0;
    mRead = false;
    mFlagged = false;
    mMIMEData = NULL;
    mBody = NULL;
}

ActiveSyncMessage::ActiveSyncMessage()
{
    init();
}

ActiveSyncMessage::~ActiveSyncMessage()
{
    MC_SAFE_RELEASE(mServerID);
    MC_SAFE_RELEASE(mMessageClass);
    MC_SAFE_RELEASE(mMIMEData);
    MC_SAFE_RELEASE(mBody);
}

void ActiveSyncMessage::setServerID(String * value)
{
    MC_SET_STRING_FIELD(mServerID, value);
}

String * ActiveSyncMessage::serverID()
{
    MC_GET_STRING_FIELD(mServerID);
}

void ActiveSyncMessage::setMessageClass(String * value)
{
    MC_SET_STRING_FIELD(mMessageClass, value);
}

String * ActiveSyncMessage::messageClass()
{
    MC_GET_STRING_FIELD(mMessageClass);
}

void ActiveSyncMessage::setEstimatedSize(uint32_t value)
{
    mEstimatedSize = value;
}

uint32_t ActiveSyncMessage::estimatedSize()
{
    return mEstimatedSize;
}

void ActiveSyncMessage::setRead(bool value)
{
    mRead = value;
}

bool ActiveSyncMessage::isRead()
{
    return mRead;
}

void ActiveSyncMessage::setFlagged(bool value)
{
    mFlagged = value;
}

bool ActiveSyncMessage::isFlagged()
{
    return mFlagged;
}

void ActiveSyncMessage::setMIMEData(Data * value)
{
    MC_SET_OBJECT_FIELD(Data, mMIMEData, value);
}

Data * ActiveSyncMessage::MIMEData()
{
    MC_GET_OBJECT_FIELD(mMIMEData);
}

void ActiveSyncMessage::setBody(ActiveSyncBody * value)
{
    MC_SET_OBJECT_FIELD(ActiveSyncBody, mBody, value);
}

ActiveSyncBody * ActiveSyncMessage::body()
{
    MC_GET_OBJECT_FIELD(mBody);
}

Array * /* AbstractPart */ ActiveSyncMessage::attachments()
{
    Array * /* AbstractPart */ result = Array::array();
    if (mBody == NULL || mBody->attachments() == NULL)
        return result;

    for (unsigned int i = 0; i < mBody->attachments()->count(); i++) {
        AbstractPart * part = (AbstractPart *) mBody->attachments()->objectAtIndex(i);
        if (part->isAttachment())
            result->addObject(part);
    }
    return result;
}

Array * /* AbstractPart */ ActiveSyncMessage::htmlInlineAttachments()
{
    Array * /* AbstractPart */ result = Array::array();
    if (mBody == NULL || mBody->attachments() == NULL)
        return result;

    for (unsigned int i = 0; i < mBody->attachments()->count(); i++) {
        AbstractPart * part = (AbstractPart *) mBody->attachments()->objectAtIndex(i);
        if (part->isInlineAttachment())
            result->addObject(part);
    }
    return result;
}

AbstractPart * ActiveSyncMessage::partForContentID(String * contentID)
{
    if (mBody == NULL || mBody->attachments() == NULL)
        return NULL;

    for (unsigned int i = 0; i < mBody->attachments()->count(); i++) {
        AbstractPart * part = (AbstractPart *) mBody->attachments()->objectAtIndex(i);
        AbstractPart * result = part->partForContentID(contentID);
        if (result != NULL)
            return result;
    }
    return NULL;
}

AbstractPart * ActiveSyncMessage::partForUniqueID(String * uniqueID)
{
    if (mBody == NULL || mBody->attachments() == NULL)
        return NULL;

    for (unsigned int i = 0; i < mBody->attachments()->count(); i++) {
        AbstractPart * part = (AbstractPart *) mBody->attachments()->objectAtIndex(i);
        AbstractPart * result = part->partForUniqueID(uniqueID);
        if (result != NULL)
            return result;
    }
    return NULL;
}

Object * ActiveSyncMessage::copy()
{
    ActiveSyncMessage * result = new ActiveSyncMessage();
    MessageHeader * copiedHeader = (MessageHeader *) header()->copy();
    result->setHeader(copiedHeader);
    MC_SAFE_RELEASE(copiedHeader);
    result->setServerID(serverID());
    result->setMessageClass(messageClass());
    result->setEstimatedSize(estimatedSize());
    result->setRead(isRead());
    result->setFlagged(isFlagged());
    result->setMIMEData(MIMEData());
    result->setBody(body());
    return result;
}

String * ActiveSyncMessage::description()
{
    return String::stringWithUTF8Format("<%s:%p %s>", className()->UTF8Characters(), this, MCUTF8(mServerID));
}
