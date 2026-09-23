#include "MCGmailProfileOperation.h"

#include "MCGmailAsyncSession.h"
#include "MCGmail.h"

using namespace mailcore;

GmailProfileOperation::GmailProfileOperation()
{
    mProfile = NULL;
}

GmailProfileOperation::~GmailProfileOperation()
{
    MC_SAFE_RELEASE(mProfile);
}

GmailProfile * GmailProfileOperation::profile()
{
    return mProfile;
}

void GmailProfileOperation::main()
{
    ErrorCode error;
    GmailProfile * profile = session()->session()->profile(&error);
    MC_SAFE_REPLACE_RETAIN(GmailProfile, mProfile, profile);
    setError(error);
}
