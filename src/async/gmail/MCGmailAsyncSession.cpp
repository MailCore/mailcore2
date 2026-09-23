#include "MCGmailAsyncSession.h"

#include "MCGmail.h"
#include "MCGmailOperation.h"
#include "MCGmailOperations.h"
#include "MCOperationQueueCallback.h"

using namespace mailcore;

namespace mailcore {

    class GmailOperationQueueCallback : public Object, public OperationQueueCallback {
    public:
        GmailOperationQueueCallback(GmailAsyncSession * session)
        {
            mSession = session;
        }

        virtual ~GmailOperationQueueCallback()
        {
        }

        virtual void queueStartRunning()
        {
            mSession->retain();
            if (mSession->operationQueueCallback() != NULL) {
                mSession->operationQueueCallback()->queueStartRunning();
            }
        }

        virtual void queueStoppedRunning()
        {
            if (mSession->operationQueueCallback() != NULL) {
                mSession->operationQueueCallback()->queueStoppedRunning();
            }
            mSession->release();
        }

    private:
        GmailAsyncSession * mSession;
    };

}

GmailAsyncSession::GmailAsyncSession()
{
    mSession = new GmailSession();
    mQueue = new OperationQueue();
    mQueueCallback = new GmailOperationQueueCallback(this);
    mQueue->setCallback(mQueueCallback);
    mOperationQueueCallback = NULL;
}

GmailAsyncSession::~GmailAsyncSession()
{
    MC_SAFE_RELEASE(mQueueCallback);
    MC_SAFE_RELEASE(mQueue);
    MC_SAFE_RELEASE(mSession);
}

void GmailAsyncSession::setUserID(String * userID)
{
    mSession->setUserID(userID);
}

String * GmailAsyncSession::userID()
{
    return mSession->userID();
}

void GmailAsyncSession::setOAuth2Token(String * token)
{
    mSession->setOAuth2Token(token);
}

String * GmailAsyncSession::OAuth2Token()
{
    return mSession->OAuth2Token();
}

void GmailAsyncSession::setUserAgent(String * userAgent)
{
    mSession->setUserAgent(userAgent);
}

String * GmailAsyncSession::userAgent()
{
    return mSession->userAgent();
}

void GmailAsyncSession::setTimeout(time_t timeout)
{
    mSession->setTimeout(timeout);
}

time_t GmailAsyncSession::timeout()
{
    return mSession->timeout();
}

int GmailAsyncSession::lastHTTPStatus()
{
    return mSession->lastHTTPStatus();
}

String * GmailAsyncSession::lastErrorMessage()
{
    return mSession->lastErrorMessage();
}

#if __APPLE__
void GmailAsyncSession::setDispatchQueue(dispatch_queue_t dispatchQueue)
{
    mQueue->setDispatchQueue(dispatchQueue);
}

dispatch_queue_t GmailAsyncSession::dispatchQueue()
{
    return mQueue->dispatchQueue();
}
#endif

void GmailAsyncSession::setOperationQueueCallback(OperationQueueCallback * callback)
{
    mOperationQueueCallback = callback;
}

OperationQueueCallback * GmailAsyncSession::operationQueueCallback()
{
    return mOperationQueueCallback;
}

bool GmailAsyncSession::isOperationQueueRunning()
{
    return mQueue->count() > 0;
}

void GmailAsyncSession::cancelAllOperations()
{
    mQueue->cancelAllOperations();
}

void GmailAsyncSession::runOperation(GmailOperation * operation)
{
    mQueue->addOperation(operation);
}

GmailSession * GmailAsyncSession::session()
{
    return mSession;
}

GmailProfileOperation * GmailAsyncSession::profileOperation()
{
    GmailProfileOperation * op = new GmailProfileOperation();
    op->setSession(this);
    return (GmailProfileOperation *) op->autorelease();
}

GmailLabelsOperation * GmailAsyncSession::labelsOperation()
{
    GmailLabelsOperation * op = new GmailLabelsOperation();
    op->setSession(this);
    return (GmailLabelsOperation *) op->autorelease();
}

GmailLabelOperation * GmailAsyncSession::labelOperation(String * labelID)
{
    GmailLabelOperation * op = new GmailLabelOperation();
    op->setSession(this);
    op->setLabelID(labelID);
    return (GmailLabelOperation *) op->autorelease();
}

GmailMessagesOperation * GmailAsyncSession::messagesOperation()
{
    GmailMessagesOperation * op = new GmailMessagesOperation();
    op->setSession(this);
    op->setKind(GmailMessagesOperationKindDefault);
    return (GmailMessagesOperation *) op->autorelease();
}

GmailMessagesOperation * GmailAsyncSession::messagesWithQueryOperation(String * query)
{
    GmailMessagesOperation * op = new GmailMessagesOperation();
    op->setSession(this);
    op->setKind(GmailMessagesOperationKindQuery);
    op->setQuery(query);
    return (GmailMessagesOperation *) op->autorelease();
}

GmailMessagesOperation * GmailAsyncSession::messagesWithLabelOperation(String * labelID)
{
    GmailMessagesOperation * op = new GmailMessagesOperation();
    op->setSession(this);
    op->setKind(GmailMessagesOperationKindLabel);
    op->setLabelID(labelID);
    return (GmailMessagesOperation *) op->autorelease();
}

GmailMessageOperation * GmailAsyncSession::messageOperation(String * messageID)
{
    GmailMessageOperation * op = new GmailMessageOperation();
    op->setSession(this);
    op->setKind(GmailMessageOperationKindDefault);
    op->setMessageID(messageID);
    return (GmailMessageOperation *) op->autorelease();
}

GmailMessageOperation * GmailAsyncSession::messageWithFormatOperation(String * messageID,
                                                                      GmailMessageFormat format)
{
    GmailMessageOperation * op = new GmailMessageOperation();
    op->setSession(this);
    op->setKind(GmailMessageOperationKindFormat);
    op->setMessageID(messageID);
    op->setFormat(format);
    return (GmailMessageOperation *) op->autorelease();
}

GmailMessageOperation * GmailAsyncSession::messageWithMetadataHeadersOperation(String * messageID,
                                                                               Array * headers)
{
    GmailMessageOperation * op = new GmailMessageOperation();
    op->setSession(this);
    op->setKind(GmailMessageOperationKindMetadataHeaders);
    op->setMessageID(messageID);
    op->setMetadataHeaders(headers);
    return (GmailMessageOperation *) op->autorelease();
}

GmailMessageDataOperation * GmailAsyncSession::messageDataOperation(String * messageID)
{
    GmailMessageDataOperation * op = new GmailMessageDataOperation();
    op->setSession(this);
    op->setMessageID(messageID);
    return (GmailMessageDataOperation *) op->autorelease();
}

GmailAttachmentDataOperation * GmailAsyncSession::attachmentDataOperation(String * messageID,
                                                                          String * attachmentID)
{
    GmailAttachmentDataOperation * op = new GmailAttachmentDataOperation();
    op->setSession(this);
    op->setMessageID(messageID);
    op->setAttachmentID(attachmentID);
    return (GmailAttachmentDataOperation *) op->autorelease();
}

GmailMessagePartDataOperation * GmailAsyncSession::dataForMessagePartOperation(String * messageID,
                                                                               GmailMessagePart * part)
{
    GmailMessagePartDataOperation * op = new GmailMessagePartDataOperation();
    op->setSession(this);
    op->setMessageID(messageID);
    op->setPart(part);
    return (GmailMessagePartDataOperation *) op->autorelease();
}
