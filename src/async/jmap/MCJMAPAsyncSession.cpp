#include "MCJMAPAsyncSession.h"

#include "MCJMAP.h"
#include "MCJMAPOperation.h"
#include "MCJMAPOperations.h"
#include "MCOperationQueueCallback.h"

using namespace mailcore;

namespace mailcore {

    class JMAPOperationQueueCallback : public Object, public OperationQueueCallback {
    public:
        JMAPOperationQueueCallback(JMAPAsyncSession * session) { mSession = session; }
        virtual ~JMAPOperationQueueCallback() {}
        virtual void queueStartRunning()
        {
            mSession->retain();
            if (mSession->operationQueueCallback() != NULL)
                mSession->operationQueueCallback()->queueStartRunning();
        }
        virtual void queueStoppedRunning()
        {
            if (mSession->operationQueueCallback() != NULL)
                mSession->operationQueueCallback()->queueStoppedRunning();
            mSession->release();
        }
    private:
        JMAPAsyncSession * mSession;
    };

}

JMAPAsyncSession::JMAPAsyncSession()
{
    mSession = new JMAPSession();
    mQueue = new OperationQueue();
    mQueueCallback = new JMAPOperationQueueCallback(this);
    mQueue->setCallback(mQueueCallback);
    mOperationQueueCallback = NULL;
}

JMAPAsyncSession::~JMAPAsyncSession()
{
    MC_SAFE_RELEASE(mQueueCallback);
    MC_SAFE_RELEASE(mQueue);
    MC_SAFE_RELEASE(mSession);
}

void JMAPAsyncSession::setSessionURL(String * sessionURL) { mSession->setSessionURL(sessionURL); }
String * JMAPAsyncSession::sessionURL() { return mSession->sessionURL(); }
void JMAPAsyncSession::setDomainOrEmail(String * domainOrEmail) { mSession->setDomainOrEmail(domainOrEmail); }
String * JMAPAsyncSession::domainOrEmail() { return mSession->domainOrEmail(); }
void JMAPAsyncSession::setAccountID(String * accountID) { mSession->setAccountID(accountID); }
String * JMAPAsyncSession::accountID() { return mSession->accountID(); }
void JMAPAsyncSession::setUsername(String * username) { mSession->setUsername(username); }
String * JMAPAsyncSession::username() { return mSession->username(); }
void JMAPAsyncSession::setOAuth2Token(String * token) { mSession->setOAuth2Token(token); }
String * JMAPAsyncSession::OAuth2Token() { return mSession->OAuth2Token(); }
void JMAPAsyncSession::setTimeout(time_t timeout) { mSession->setTimeout(timeout); }
time_t JMAPAsyncSession::timeout() { return mSession->timeout(); }
void JMAPAsyncSession::setCheckCertificateEnabled(bool enabled) { mSession->setCheckCertificateEnabled(enabled); }
bool JMAPAsyncSession::isCheckCertificateEnabled() { return mSession->isCheckCertificateEnabled(); }
int JMAPAsyncSession::lastHTTPStatus() { return mSession->lastHTTPStatus(); }
String * JMAPAsyncSession::lastErrorMessage() { return mSession->lastErrorMessage(); }

#if __APPLE__
void JMAPAsyncSession::setDispatchQueue(dispatch_queue_t dispatchQueue) { mQueue->setDispatchQueue(dispatchQueue); }
dispatch_queue_t JMAPAsyncSession::dispatchQueue() { return mQueue->dispatchQueue(); }
#endif

void JMAPAsyncSession::setOperationQueueCallback(OperationQueueCallback * callback) { mOperationQueueCallback = callback; }
OperationQueueCallback * JMAPAsyncSession::operationQueueCallback() { return mOperationQueueCallback; }
bool JMAPAsyncSession::isOperationQueueRunning() { return mQueue->count() > 0; }
void JMAPAsyncSession::cancelAllOperations() { mQueue->cancelAllOperations(); }
void JMAPAsyncSession::runOperation(JMAPOperation * operation) { mQueue->addOperation(operation); }
JMAPSession * JMAPAsyncSession::session() { return mSession; }

JMAPOperation * JMAPAsyncSession::connectOperation()
{
    JMAPOperation * op = new JMAPOperation();
    op->setSession(this);
    op->setKind(JMAPOperationKindConnect);
    return (JMAPOperation *) op->autorelease();
}

JMAPOperation * JMAPAsyncSession::discoverOperation()
{
    JMAPOperation * op = new JMAPOperation();
    op->setSession(this);
    op->setKind(JMAPOperationKindDiscover);
    return (JMAPOperation *) op->autorelease();
}

JMAPFetchMailboxesOperation * JMAPAsyncSession::fetchMailboxesOperation()
{
    JMAPFetchMailboxesOperation * op = new JMAPFetchMailboxesOperation();
    op->setSession(this);
    return (JMAPFetchMailboxesOperation *) op->autorelease();
}

JMAPQueryMessagesOperation * JMAPAsyncSession::queryMessagesWithTextOperation(String * text, unsigned int position, unsigned int limit)
{
    JMAPQueryMessagesOperation * op = new JMAPQueryMessagesOperation();
    op->setSession(this);
    op->setText(text);
    op->setPosition(position);
    op->setLimit(limit);
    return (JMAPQueryMessagesOperation *) op->autorelease();
}

JMAPQueryMessagesOperation * JMAPAsyncSession::queryMessagesInMailboxOperation(String * mailboxID, unsigned int position, unsigned int limit)
{
    JMAPQueryMessagesOperation * op = new JMAPQueryMessagesOperation();
    op->setSession(this);
    op->setMailboxID(mailboxID);
    op->setPosition(position);
    op->setLimit(limit);
    return (JMAPQueryMessagesOperation *) op->autorelease();
}

JMAPFetchMessagesOperation * JMAPAsyncSession::fetchMessagesOperation(Array * messageIDs, Array * properties)
{
    JMAPFetchMessagesOperation * op = new JMAPFetchMessagesOperation();
    op->setSession(this);
    op->setMessageIDs(messageIDs);
    op->setProperties(properties);
    return (JMAPFetchMessagesOperation *) op->autorelease();
}

JMAPUploadOperation * JMAPAsyncSession::uploadOperation(Data * data, String * contentType, String * accountID)
{
    JMAPUploadOperation * op = new JMAPUploadOperation();
    op->setSession(this);
    op->setData(data);
    op->setContentType(contentType);
    op->setAccountID(accountID);
    return (JMAPUploadOperation *) op->autorelease();
}

JMAPDownloadOperation * JMAPAsyncSession::downloadOperation(String * blobID, String * name, String * accept, String * accountID)
{
    JMAPDownloadOperation * op = new JMAPDownloadOperation();
    op->setSession(this);
    op->setBlobID(blobID);
    op->setName(name);
    op->setAccept(accept);
    op->setAccountID(accountID);
    return (JMAPDownloadOperation *) op->autorelease();
}

JMAPCreateDraftOperation * JMAPAsyncSession::createDraftOperation(Data * data, String * mailboxID, Array * keywords)
{
    JMAPCreateDraftOperation * op = new JMAPCreateDraftOperation();
    op->setSession(this);
    op->setData(data);
    op->setMailboxID(mailboxID);
    op->setKeywords(keywords);
    return (JMAPCreateDraftOperation *) op->autorelease();
}

JMAPOperation * JMAPAsyncSession::updateDraftOperation(String * messageID, Array * mailboxIDs, Array * keywords)
{
    JMAPOperation * op = new JMAPOperation();
    op->setSession(this);
    op->setKind(JMAPOperationKindUpdateDraft);
    op->setMessageID(messageID);
    op->setMailboxIDs(mailboxIDs);
    op->setKeywords(keywords);
    return (JMAPOperation *) op->autorelease();
}

JMAPOperation * JMAPAsyncSession::deleteDraftOperation(String * messageID)
{
    JMAPOperation * op = new JMAPOperation();
    op->setSession(this);
    op->setKind(JMAPOperationKindDeleteDraft);
    op->setMessageID(messageID);
    return (JMAPOperation *) op->autorelease();
}

JMAPSendOperation * JMAPAsyncSession::sendMessageOperation(String * messageID, String * identityID)
{
    JMAPSendOperation * op = new JMAPSendOperation();
    op->setSession(this);
    op->setMessageID(messageID);
    op->setIdentityID(identityID);
    return (JMAPSendOperation *) op->autorelease();
}

JMAPSendOperation * JMAPAsyncSession::sendDataOperation(Data * data, String * identityID)
{
    JMAPSendOperation * op = new JMAPSendOperation();
    op->setSession(this);
    op->setData(data);
    op->setIdentityID(identityID);
    return (JMAPSendOperation *) op->autorelease();
}
