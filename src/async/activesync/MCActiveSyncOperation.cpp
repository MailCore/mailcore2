#include "MCActiveSyncOperation.h"

#include "MCActiveSyncAsyncSession.h"

using namespace mailcore;

ActiveSyncOperation::ActiveSyncOperation()
{
    mSession = NULL;
    mError = ErrorNone;
}

ActiveSyncOperation::~ActiveSyncOperation()
{
    MC_SAFE_RELEASE(mSession);
}

void ActiveSyncOperation::setSession(ActiveSyncAsyncSession * session)
{
    MC_SAFE_REPLACE_RETAIN(ActiveSyncAsyncSession, mSession, session);
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

ActiveSyncAsyncSession * ActiveSyncOperation::session()
{
    return mSession;
}

void ActiveSyncOperation::setError(ErrorCode error)
{
    mError = error;
}

ErrorCode ActiveSyncOperation::error()
{
    return mError;
}

void ActiveSyncOperation::start()
{
    mSession->runOperation(this);
}
