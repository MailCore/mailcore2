#include "MCGmailMessagePart.h"

using namespace mailcore;

void GmailMessagePart::init()
{
    mPartID = NULL;
}

GmailMessagePart::GmailMessagePart()
{
    init();
}

GmailMessagePart::~GmailMessagePart()
{
    MC_SAFE_RELEASE(mPartID);
}

String * GmailMessagePart::partID()
{
    return mPartID;
}

void GmailMessagePart::setPartID(String * partID)
{
    MC_SAFE_REPLACE_COPY(String, mPartID, partID);
}
