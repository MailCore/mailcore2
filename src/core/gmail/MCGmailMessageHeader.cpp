#include "MCGmailMessageHeader.h"

using namespace mailcore;

void GmailMessageHeader::init()
{
    mName = NULL;
    mValue = NULL;
}

GmailMessageHeader::GmailMessageHeader()
{
    init();
}

GmailMessageHeader::~GmailMessageHeader()
{
    MC_SAFE_RELEASE(mName);
    MC_SAFE_RELEASE(mValue);
}

String * GmailMessageHeader::name()
{
    return mName;
}

void GmailMessageHeader::setName(String * name)
{
    MC_SAFE_REPLACE_COPY(String, mName, name);
}

String * GmailMessageHeader::value()
{
    return mValue;
}

void GmailMessageHeader::setValue(String * value)
{
    MC_SAFE_REPLACE_COPY(String, mValue, value);
}
