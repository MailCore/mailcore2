#include "MCJMAPOperation.h"

#include "MCJMAPAsyncSession.h"
#include "MCJMAPSession.h"

using namespace mailcore;

JMAPOperation::JMAPOperation()
{
    mSession = NULL;
    mKind = JMAPOperationKindNone;
    mMessageID = NULL;
    mMailboxIDs = NULL;
    mKeywords = NULL;
    mError = ErrorNone;
}

JMAPOperation::~JMAPOperation()
{
    MC_SAFE_RELEASE(mSession);
    MC_SAFE_RELEASE(mMessageID);
    MC_SAFE_RELEASE(mMailboxIDs);
    MC_SAFE_RELEASE(mKeywords);
}

void JMAPOperation::setSession(JMAPAsyncSession * session)
{
    MC_SAFE_REPLACE_RETAIN(JMAPAsyncSession, mSession, session);
#if __APPLE__
    setCallbackDispatchQueue(session != NULL ? session->dispatchQueue() : dispatch_get_main_queue());
#endif
}

JMAPAsyncSession * JMAPOperation::session() { return mSession; }
void JMAPOperation::setKind(JMAPOperationKind kind) { mKind = kind; }
JMAPOperationKind JMAPOperation::kind() { return mKind; }
void JMAPOperation::setMessageID(String * messageID) { MC_SAFE_REPLACE_COPY(String, mMessageID, messageID); }
String * JMAPOperation::messageID() { return mMessageID; }
void JMAPOperation::setMailboxIDs(Array * mailboxIDs) { MC_SAFE_REPLACE_COPY(Array, mMailboxIDs, mailboxIDs); }
Array * JMAPOperation::mailboxIDs() { return mMailboxIDs; }
void JMAPOperation::setKeywords(Array * keywords) { MC_SAFE_REPLACE_COPY(Array, mKeywords, keywords); }
Array * JMAPOperation::keywords() { return mKeywords; }
void JMAPOperation::setError(ErrorCode error) { mError = error; }
ErrorCode JMAPOperation::error() { return mError; }

void JMAPOperation::start()
{
    mSession->runOperation(this);
}

void JMAPOperation::main()
{
    ErrorCode error = ErrorNone;
    switch (mKind) {
        case JMAPOperationKindConnect:
            session()->session()->connect(&error);
            break;
        case JMAPOperationKindDiscover:
            session()->session()->discover(&error);
            break;
        case JMAPOperationKindUpdateDraft:
            session()->session()->updateDraft(mMessageID, mMailboxIDs, mKeywords, &error);
            break;
        case JMAPOperationKindDeleteDraft:
            session()->session()->deleteDraft(mMessageID, &error);
            break;
        case JMAPOperationKindNone:
        default:
            break;
    }
    setError(error);
}
