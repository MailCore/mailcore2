#include "MCGmailMultipart.h"

using namespace mailcore;

void GmailMultipart::init()
{
    mPartID = NULL;
}

GmailMultipart::GmailMultipart()
{
    init();
}

GmailMultipart::~GmailMultipart()
{
    MC_SAFE_RELEASE(mPartID);
}

String * GmailMultipart::partID()
{
    return mPartID;
}

void GmailMultipart::setPartID(String * partID)
{
    MC_SAFE_REPLACE_COPY(String, mPartID, partID);
}
