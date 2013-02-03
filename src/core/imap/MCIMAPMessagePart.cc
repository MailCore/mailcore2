#include "MCIMAPMessagePart.h"

using namespace mailcore;

IMAPMessagePart::IMAPMessagePart()
{
    init();
}

IMAPMessagePart::IMAPMessagePart(IMAPMessagePart * other) : AbstractMessagePart(other)
{
    init();
    MC_SAFE_REPLACE_COPY(String, mPartID, other->mPartID);
}

IMAPMessagePart::~IMAPMessagePart()
{
    MC_SAFE_RELEASE(mPartID);
}

Object * IMAPMessagePart::copy()
{
    return new IMAPMessagePart(this);
}

void IMAPMessagePart::init()
{
    mPartID = NULL;
}

void IMAPMessagePart::setPartID(String * partID)
{
    MC_SAFE_REPLACE_COPY(String, mPartID, partID);
}

String * IMAPMessagePart::partID()
{
    return mPartID;
}
