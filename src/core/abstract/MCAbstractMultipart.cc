#include "MCAbstractMultipart.h"

using namespace mailcore;

AbstractMultipart::AbstractMultipart()
{
    init();
}

AbstractMultipart::AbstractMultipart(AbstractMultipart * other) : AbstractPart(other)
{
    init();
    
    setPartType(other->partType());
    Array * parts = Array::array();
    for(unsigned int i = 0 ; i < other->parts()->count() ; i ++) {
        AbstractPart * part = (AbstractPart *) other->parts()->objectAtIndex(i);
        parts->addObject(part->copy()->autorelease());
    }
    setParts(parts);
}

void AbstractMultipart::init()
{
    mParts = NULL;
    setPartType(PartTypeMultipartMixed);
}

AbstractMultipart::~AbstractMultipart()
{
    MC_SAFE_RELEASE(mParts);
}

Array * AbstractMultipart::parts()
{
    return mParts;
}

void AbstractMultipart::setParts(Array * parts)
{
    MC_SAFE_REPLACE_COPY(Array, mParts, parts);
    applyMessage();
}

String * AbstractMultipart::description()
{
    String * result = String::string();
    
    const char * partTypeName = NULL;
    switch (partType()) {
        default:
        case PartTypeMultipartMixed:
        partTypeName = "mixed";
        break;
        case PartTypeMultipartRelated:
        partTypeName = "related";
        break;
        case PartTypeMultipartAlternative:
        partTypeName = "alternative";
        break;
    }
    
    result->appendUTF8Format("<%s:%p %s %s>",
        MCUTF8(className()), this, partTypeName, MCUTF8(mParts->description()));
    return result;
}

Object * AbstractMultipart::copy()
{
    return new AbstractMultipart(this);
}

void AbstractMultipart::applyMessage()
{
    if (mParts == NULL)
        return;
    
    for(unsigned int i = 0 ; i < mParts->count() ; i ++) {
        AbstractPart * part = (AbstractPart *) mParts->objectAtIndex(i);
        part->setMessage(message());
    }
}

void AbstractMultipart::setMessage(AbstractMessage * message)
{
    AbstractPart::setMessage(message);
    applyMessage();
}

AbstractPart * AbstractMultipart::partForContentID(String * contentID)
{
    for(unsigned int i = 0 ; i < parts()->count() ; i ++) {
        mailcore::AbstractPart * subpart = (mailcore::AbstractPart *) parts()->objectAtIndex(i);
        mailcore::AbstractPart * result = subpart->partForContentID(contentID);
        if (result != NULL)
            return result;
    }
    return NULL;
}


AbstractPart * AbstractMultipart::partForUniqueID(String * uniqueID)
{
    for(unsigned int i = 0 ; i < parts()->count() ; i ++) {
        mailcore::AbstractPart * subpart = (mailcore::AbstractPart *) parts()->objectAtIndex(i);
        mailcore::AbstractPart * result = subpart->partForUniqueID(uniqueID);
        if (result != NULL)
            return result;
    }
    return NULL;
}

