#include "MCGmailMessagePart.h"

using namespace mailcore;

void GmailMessagePart::init()
{
    mPartID = NULL;
    mHeaders = (Array *) Array::array()->retain();
    mAttachmentID = NULL;
    mSize = 0;
    mData = NULL;
    mParts = (Array *) Array::array()->retain();
}

GmailMessagePart::GmailMessagePart()
{
    init();
}

GmailMessagePart::~GmailMessagePart()
{
    MC_SAFE_RELEASE(mPartID);
    MC_SAFE_RELEASE(mHeaders);
    MC_SAFE_RELEASE(mAttachmentID);
    MC_SAFE_RELEASE(mData);
    MC_SAFE_RELEASE(mParts);
}

String * GmailMessagePart::partID()
{
    return mPartID;
}

void GmailMessagePart::setPartID(String * partID)
{
    MC_SAFE_REPLACE_COPY(String, mPartID, partID);
}

Array * GmailMessagePart::headers()
{
    return mHeaders;
}

void GmailMessagePart::setHeaders(Array * headers)
{
    MC_SAFE_REPLACE_COPY(Array, mHeaders, headers);
}

String * GmailMessagePart::attachmentID()
{
    return mAttachmentID;
}

void GmailMessagePart::setAttachmentID(String * attachmentID)
{
    MC_SAFE_REPLACE_COPY(String, mAttachmentID, attachmentID);
}

uint32_t GmailMessagePart::size()
{
    return mSize;
}

void GmailMessagePart::setSize(uint32_t size)
{
    mSize = size;
}

Data * GmailMessagePart::data()
{
    return mData;
}

void GmailMessagePart::setData(Data * data)
{
    MC_SAFE_REPLACE_RETAIN(Data, mData, data);
}

Array * GmailMessagePart::parts()
{
    return mParts;
}

void GmailMessagePart::setParts(Array * parts)
{
    MC_SAFE_REPLACE_COPY(Array, mParts, parts);
}
