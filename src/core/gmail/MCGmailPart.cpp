#include "MCGmailPart.h"

using namespace mailcore;

void GmailPart::init()
{
    mPartID = NULL;
    mAttachmentID = NULL;
    mSize = 0;
    mData = NULL;
}

GmailPart::GmailPart()
{
    init();
}

GmailPart::~GmailPart()
{
    MC_SAFE_RELEASE(mPartID);
    MC_SAFE_RELEASE(mAttachmentID);
    MC_SAFE_RELEASE(mData);
}

String * GmailPart::partID()
{
    return mPartID;
}

void GmailPart::setPartID(String * partID)
{
    MC_SAFE_REPLACE_COPY(String, mPartID, partID);
}

String * GmailPart::attachmentID()
{
    return mAttachmentID;
}

void GmailPart::setAttachmentID(String * attachmentID)
{
    MC_SAFE_REPLACE_COPY(String, mAttachmentID, attachmentID);
}

uint32_t GmailPart::size()
{
    return mSize;
}

void GmailPart::setSize(uint32_t size)
{
    mSize = size;
}

Data * GmailPart::data()
{
    return mData;
}

void GmailPart::setData(Data * data)
{
    MC_SAFE_REPLACE_RETAIN(Data, mData, data);
}
