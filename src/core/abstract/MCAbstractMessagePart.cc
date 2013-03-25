#include "MCAbstractMessagePart.h"

#include "MCMessageHeader.h"

using namespace mailcore;

void AbstractMessagePart::init()
{
    mMainPart = NULL;
    mHeader = NULL;
}

AbstractMessagePart::AbstractMessagePart()
{
    init();
}

AbstractMessagePart::AbstractMessagePart(AbstractMessagePart * other)
{
    init();
    if (other->mainPart() != NULL) {
        setMainPart((AbstractPart *) other->mainPart()->copy()->autorelease());
    }
    if (other->mHeader != NULL) {
        setHeader((MessageHeader *) other->header()->copy()->autorelease());
    }
}

AbstractMessagePart::~AbstractMessagePart()
{
    MC_SAFE_RELEASE(mMainPart);
    MC_SAFE_RELEASE(mHeader);
}

String * AbstractMessagePart::description()
{
    String * result = String::string();
    result->appendUTF8Format("<%s:%p %s>", className(), this, mMainPart->description());
    return result;
}

Object * AbstractMessagePart::copy()
{
    return new AbstractMessagePart(this);
}

MessageHeader * AbstractMessagePart::header()
{
    if (mHeader == NULL) {
        mHeader = new MessageHeader();
    }
    return mHeader;
}

void AbstractMessagePart::setHeader(MessageHeader * header)
{
    MC_SAFE_REPLACE_RETAIN(MessageHeader, mHeader, header);
}

AbstractPart * AbstractMessagePart::mainPart()
{
    return mMainPart;
}

void AbstractMessagePart::setMainPart(AbstractPart * mainPart)
{
    MC_SAFE_REPLACE_RETAIN(AbstractPart, mMainPart, mainPart);
    applyMessage();
}

void AbstractMessagePart::applyMessage()
{
    if (mMainPart == NULL)
        return;
    
    mMainPart->setMessage(message());
}

void AbstractMessagePart::setMessage(AbstractMessage * message)
{
    AbstractPart::setMessage(message);
    applyMessage();
}

AbstractPart * AbstractMessagePart::partForContentID(String * contentID)
{
    return mainPart()->partForContentID(contentID);
}

AbstractPart * AbstractMessagePart::partForUniqueID(String * contentID)
{
    return mainPart()->partForContentID(contentID);
}
