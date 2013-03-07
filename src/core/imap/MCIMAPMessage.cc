#include "MCIMAPMessage.h"

#include "MCMessageHeader.h"
#include "MCIMAPPart.h"
#include "MCIMAPMessagePart.h"
#include "MCIMAPMultipart.h"
#include "MCHTMLRenderer.h"

using namespace mailcore;

static AbstractPart * partForPartIDInPart(AbstractPart * part, String * partID);
static AbstractPart * partForPartIDInMultipart(AbstractMultipart * part, String * partID);
static AbstractPart * partForPartIDInMessagePart(AbstractMessagePart * part, String * partID);

void IMAPMessage::init()
{
    mUid = 0;
    mFlags = MessageFlagNone;
    mOriginalFlags = MessageFlagNone;
	mMainPart = NULL;
	mLabels = NULL;
    mModSeqValue = 0;
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

void IMAPMessage::setModSeqValue(uint64_t uid)
{
    mModSeqValue = uid;
}

uint64_t IMAPMessage::modSeqValue()
{
    return mModSeqValue;
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

AbstractPart * IMAPMessage::partForPartID(String * partID)
{
    return partForPartIDInPart(mainPart(), partID);
}

static AbstractPart * partForPartIDInPart(AbstractPart * part, String * partID)
{
    switch (part->partType()) {
        case PartTypeSingle:
            if (partID->isEqual(((IMAPPart *) part)->partID())) {
                return part;
            }
            return NULL;
        case mailcore::PartTypeMultipartMixed:
        case mailcore::PartTypeMultipartRelated:
        case mailcore::PartTypeMultipartAlternative:
            if (partID->isEqual(((IMAPMultipart *) part)->partID())) {
                return part;
            }
            return partForPartIDInMultipart((AbstractMultipart *) part, partID);
        case mailcore::PartTypeMessage:
            if (partID->isEqual(((IMAPMessagePart *) part)->partID())) {
                return part;
            }
            return partForPartIDInMessagePart((AbstractMessagePart *) part, partID);
        default:
            return NULL;
    }
}

static AbstractPart * partForPartIDInMessagePart(AbstractMessagePart * part, String * partID)
{
    return partForPartIDInPart(part->mainPart(), partID);
}

static AbstractPart * partForPartIDInMultipart(AbstractMultipart * part, String * partID)
{
    for(unsigned int i = 0 ; i < part->parts()->count() ; i ++) {
        mailcore::AbstractPart * subpart = (mailcore::AbstractPart *) part->parts()->objectAtIndex(i);
        mailcore::AbstractPart * result = partForPartIDInPart(subpart, partID);
        if (result != NULL)
            return result;
    }
    return NULL;
}

AbstractPart * IMAPMessage::partForContentID(String * contentID)
{
    return mainPart()->partForContentID(contentID);
}

AbstractPart * IMAPMessage::partForUniqueID(String * uniqueID)
{
    return mainPart()->partForUniqueID(uniqueID);
}

String * IMAPMessage::htmlRendering(String * folder,
                                    HTMLRendererIMAPCallback * dataCallback,
                                    HTMLRendererTemplateCallback * htmlCallback)
{
    return HTMLRenderer::htmlForIMAPMessage(folder, this, dataCallback, htmlCallback);
}

