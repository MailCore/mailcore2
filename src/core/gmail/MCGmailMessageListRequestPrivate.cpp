#include "MCGmailMessageListRequestPrivate.h"

using namespace mailcore;

GmailMessageListRequest::GmailMessageListRequest()
{
    query = NULL;
    labelIDs = NULL;
    maxResults = 0;
    pageToken = NULL;
    includeSpamTrash = false;
}

GmailMessageListRequest::~GmailMessageListRequest()
{
    MC_SAFE_RELEASE(query);
    MC_SAFE_RELEASE(labelIDs);
    MC_SAFE_RELEASE(pageToken);
}
