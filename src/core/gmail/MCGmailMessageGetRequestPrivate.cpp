#include "MCGmailMessageGetRequestPrivate.h"

using namespace mailcore;

GmailMessageGetRequest::GmailMessageGetRequest()
{
    format = GmailMessageFormatFull;
    metadataHeaders = NULL;
}

GmailMessageGetRequest::~GmailMessageGetRequest()
{
    MC_SAFE_RELEASE(metadataHeaders);
}
