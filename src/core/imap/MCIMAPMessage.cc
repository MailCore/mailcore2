#include "MCIMAPMessage.h"

#include "MCMessageHeader.h"

using namespace mailcore;

void IMAPMessage::init()
{
    mUid = NULL;
    mFlags = MessageFlagNone;
    mOriginalFlags = MessageFlagNone;
	mMainPart = NULL;
	mLabels = NULL;
}

IMAPMessage::IMAPMessage()
{
    init();
}

IMAPMessage::IMAPMessage(IMAPMessage * other)
{
    init();
    setUid(other->uid());
    setFlags(other->flags());
    setOriginalFlags(other->originalFlags());
    setMainPart((AbstractPart *) other->mainPart()->copy()->autorelease());
    setGmailLabels(other->gmailLabels());
}

IMAPMessage::~IMAPMessage()
{
    MC_SAFE_RELEASE(mMainPart);
    MC_SAFE_RELEASE(mLabels);
}

#if 0
String * IMAPMessage::className()
{
    return MCSTR("IMAPMessage");
}
#endif

Object * IMAPMessage::copy()
{
    return new IMAPMessage(this);
}

String * IMAPMessage::description()
{
    String * result = String::string();
    result->appendUTF8Format("<%s:%p %u\n", className()->UTF8Characters(), this, (unsigned int) uid());
    result->appendString(header()->description());
    if (mainPart() != NULL) {
        result->appendString(mainPart()->description());
        result->appendUTF8Characters("\n");
    }
    result->appendUTF8Characters(">");
    return result;
}

uint32_t IMAPMessage::uid()
{
    return mUid;
}

void IMAPMessage::setUid(uint32_t uid)
{
    mUid = uid;
}

void IMAPMessage::setFlags(MessageFlag flags)
{
    mFlags = flags;
}

MessageFlag IMAPMessage::flags()
{
    return mFlags;
}

void IMAPMessage::setOriginalFlags(MessageFlag flags)
{
    mOriginalFlags = flags;
}

MessageFlag IMAPMessage::originalFlags()
{
    return mOriginalFlags;
}

void IMAPMessage::setMainPart(AbstractPart * mainPart)
{
    MC_SAFE_REPLACE_RETAIN(AbstractPart, mMainPart, mainPart);
}

AbstractPart * IMAPMessage::mainPart()
{
    return mMainPart;
}

void IMAPMessage::setGmailLabels(Array * labels)
{
    MC_SAFE_REPLACE_COPY(Array, mLabels, labels);
}

Array * IMAPMessage::gmailLabels()
{
    return mLabels;
}

