#include "MCGmailOperation.h"

#include "MCGmailAsyncSession.h"

using namespace mailcore;

GmailOperation::GmailOperation()
{
    mSession = NULL;
    mError = ErrorNone;
}

GmailOperation::~GmailOperation()
{
    MC_SAFE_RELEASE(mSession);
}

void GmailOperation::setSession(GmailAsyncSession * session)
{
    MC_SAFE_REPLACE_RETAIN(GmailAsyncSession, mSession, session);
#if __APPLE__
    dispatch_queue_t queue;
    if (session != NULL) {
        queue = session->dispatchQueue();
    }
    else {
        queue = dispatch_get_main_queue();
    }
    setCallbackDispatchQueue(queue);
#endif
}

GmailAsyncSession * GmailOperation::session()
{
    return mSession;
}

void GmailOperation::setError(ErrorCode error)
{
    mError = error;
}

ErrorCode GmailOperation::error()
{
    return mError;
}

void GmailOperation::start()
{
    mSession->runOperation(this);
}
