#include "MCGmailLabelsOperation.h"

#include "MCGmailAsyncSession.h"
#include "MCGmail.h"

using namespace mailcore;

GmailLabelsOperation::GmailLabelsOperation()
{
    mLabels = NULL;
}

GmailLabelsOperation::~GmailLabelsOperation()
{
    MC_SAFE_RELEASE(mLabels);
}

Array * GmailLabelsOperation::labels()
{
    return mLabels;
}

void GmailLabelsOperation::main()
{
    ErrorCode error;
    Array * labels = session()->session()->labels(&error);
    MC_SAFE_REPLACE_RETAIN(Array, mLabels, labels);
    setError(error);
}
